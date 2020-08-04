#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <openssl/err.h>
#include <openssl/ssl.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/tls/SocketServer.h"
#include "timer/SingleshotTimer.h"

namespace tls {

    SocketServer::SocketServer(const std::function<void(tls::SocketConnection* cs)>& onConnect,
                               const std::function<void(tls::SocketConnection* cs)>& onDisconnect,
                               const std::function<void(tls::SocketConnection* cs, const char* junk, ssize_t n)>& onRead,
                               const std::function<void(tls::SocketConnection* cs, int errnum)>& onReadError,
                               const std::function<void(tls::SocketConnection* cs, int errnum)>& onWriteError)
        : ::SocketServer<tls::SocketConnection>(
              [this, onConnect](tls::SocketConnection* cs) -> void {
                  class TLSAccept
                      : public ReadEvent
                      , public WriteEvent
                      , public Socket {
                  public:
                      TLSAccept(tls::SocketConnection* cs, SSL_CTX* ctx, const std::function<void(tls::SocketConnection* cs)>& onConnect)
                          : Descriptor(true)
                          , cs(cs)
                          , ssl(cs->startSSL(ctx))
                          , onConnect(onConnect)
                          , timeOut(Timer::singleshotTimer(
                                [this]([[maybe_unused]] const void* arg) -> void {
                                    this->::ReadEvent::stop();
                                    this->::WriteEvent::stop();
                                    this->cs->stopSSL();
                                    delete this->cs;
                                },
                                (struct timeval){10, 0}, nullptr)) {
                          this->attachFd(cs->getFd());

                          int err = SSL_accept(ssl);
                          int sslErr = SSL_get_error(ssl, err);

                          if (sslErr == SSL_ERROR_WANT_READ) {
                              ::ReadEvent::start();
                          } else if (sslErr == SSL_ERROR_WANT_WRITE) {
                              ::WriteEvent::start();
                          } else {
                              if (sslErr == SSL_ERROR_NONE) {
                                  onConnect(cs);
                              }
                              timeOut.cancel();
                              delete this;
                          }
                      }

                      void readEvent() override {
                          int err = SSL_accept(ssl);
                          int sslErr = SSL_get_error(ssl, err);

                          if (sslErr != SSL_ERROR_WANT_READ) {
                              if (sslErr == SSL_ERROR_WANT_WRITE) {
                                  ::ReadEvent::stop();
                                  ::WriteEvent::start();
                              } else {
                                  timeOut.cancel();
                                  ::ReadEvent::stop();
                                  if (sslErr == SSL_ERROR_NONE) {
                                      cs->ReadEvent::start();
                                      this->onConnect(cs);
                                  } else {
                                      cs->stopSSL();
                                      delete cs;
                                  }
                              }
                          }
                      }

                      void writeEvent() override {
                          int err = SSL_accept(ssl);
                          int sslErr = SSL_get_error(ssl, err);

                          if (sslErr != SSL_ERROR_WANT_WRITE) {
                              if (sslErr == SSL_ERROR_WANT_READ) {
                                  ::WriteEvent::stop();
                                  ::ReadEvent::start();
                              } else {
                                  timeOut.cancel();
                                  ::WriteEvent::stop();
                                  if (sslErr == SSL_ERROR_NONE) {
                                      cs->ReadEvent::start();
                                      this->onConnect(cs);
                                  } else {
                                      cs->stopSSL();
                                      delete cs;
                                  }
                              }
                          }
                      }

                      void unmanaged() override {
                          delete this;
                      }

                  private:
                      tls::SocketConnection* cs = nullptr;
                      SSL* ssl = nullptr;
                      std::function<void(tls::SocketConnection* cs)> onConnect;
                      Timer& timeOut;
                  };

                  new TLSAccept(cs, ctx, onConnect);
                  /*
                  X509* client_cert = SSL_get_peer_certificate(ssl);
                  if (client_cert != NULL) {
                      printf("Client certificate:\n");

                      char* str = X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0);
                      printf("\t subject: %s\n", str);
                      OPENSSL_free(str);

                      str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0);
                      printf("\t issuer: %s\n", str);
                      OPENSSL_free(str);

                      // We could do all sorts of certificate verification stuff here before deallocating the certificate.

                      X509_free(client_cert);
                  } else {
                      printf("Client does not have certificate.\n");
                  }
                  */
              },
              [onDisconnect](tls::SocketConnection* cs) -> void {
                  cs->stopSSL();
                  onDisconnect(cs);
              },
              onRead, onReadError, onWriteError)
        , ctx(nullptr) {
    }

    SocketServer::~SocketServer() {
        if (ctx) {
            SSL_CTX_free(ctx);
            ctx = nullptr;
        }
    }

    void SocketServer::listen(in_port_t port, int backlog, const std::string& certChain, const std::string& keyPEM,
                              const std::string& password, const std::function<void(int err)>& onError) {
        ::SocketServer<tls::SocketConnection>::listen(port, backlog, [this, &certChain, &keyPEM, &password, onError](int err) -> void {
            if (!err) {
                ctx = SSL_CTX_new(TLS_server_method());
                unsigned long sslErr = 0;
                if (!ctx) {
                    ERR_print_errors_fp(stderr);
                    sslErr = ERR_get_error();
                } else {
                    SSL_CTX_set_default_passwd_cb(ctx, SocketServer::passwordCallback);
                    SSL_CTX_set_default_passwd_cb_userdata(ctx, ::strdup(password.c_str()));
                    if (SSL_CTX_use_certificate_chain_file(ctx, certChain.c_str()) <= 0) {
                        ERR_print_errors_fp(stderr);
                        sslErr = ERR_get_error();
                    } else if (SSL_CTX_use_PrivateKey_file(ctx, keyPEM.c_str(), SSL_FILETYPE_PEM) <= 0) {
                        ERR_print_errors_fp(stderr);
                        sslErr = ERR_get_error();
                    } else if (!SSL_CTX_check_private_key(ctx)) {
                        sslErr = ERR_get_error();
                    }
                }
                onError(-ERR_GET_REASON(sslErr));
            } else {
                onError(err);
            }
        });
    }

    int SocketServer::passwordCallback(char* buf, int size, [[maybe_unused]] int rwflag, void* u) {
        strncpy(buf, static_cast<char*>(u), size);
        buf[size - 1] = '\0';

        memset(u, 0, ::strlen(static_cast<char*>(u))); // garble password
        free(u);

        return ::strlen(buf);
    }

}; // namespace tls
