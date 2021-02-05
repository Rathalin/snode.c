/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "Logger.h"
#include "SNodeC.h"
#include "client/Response.h"
#include "client/legacy/Client.h"
#include "client/tls/Client.h"
#include "config.h" // just for this example app

#include <cstring>
#include <iostream>
#include <openssl/x509v3.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

using namespace http::client;

int main(int argc, char* argv[]) {
    net::SNodeC::init(argc, argv);

    {
        tls::Client6<> tlsClient(
            [](tls::Client6<>::SocketConnection* socketConnection) -> void {
                VLOG(0) << "-- OnConnect";

                VLOG(0) << "\tServer: " + socketConnection->getRemoteAddress().toString();
                VLOG(0) << "\tClient: " + socketConnection->getLocalAddress().toString();

                X509* server_cert = SSL_get_peer_certificate(socketConnection->getSSL());
                if (server_cert != NULL) {
                    int verifyErr = SSL_get_verify_result(socketConnection->getSSL());

                    VLOG(0) << "     Server certificate: " + std::string(X509_verify_cert_error_string(verifyErr));

                    char* str = X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0);
                    VLOG(0) << "        Subject: " + std::string(str);
                    OPENSSL_free(str);

                    str = X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0);
                    VLOG(0) << "        Issuer: " + std::string(str);
                    OPENSSL_free(str);

                    // We could do all sorts of certificate verification stuff here before deallocating the certificate.

                    GENERAL_NAMES* subjectAltNames =
                        static_cast<GENERAL_NAMES*>(X509_get_ext_d2i(server_cert, NID_subject_alt_name, NULL, NULL));

                    int32_t altNameCount = sk_GENERAL_NAME_num(subjectAltNames);
                    VLOG(0) << "        Subject alternative name count: " << altNameCount;
                    for (int32_t i = 0; i < altNameCount; ++i) {
                        GENERAL_NAME* generalName = sk_GENERAL_NAME_value(subjectAltNames, i);
                        if (generalName->type == GEN_URI) {
                            std::string subjectAltName =
                                std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.uniformResourceIdentifier)),
                                            ASN1_STRING_length(generalName->d.uniformResourceIdentifier));
                            VLOG(0) << "           SAN (URI): '" + subjectAltName;
                        } else if (generalName->type == GEN_DNS) {
                            std::string subjectAltName =
                                std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.dNSName)),
                                            ASN1_STRING_length(generalName->d.dNSName));
                            VLOG(0) << "           SAN (DNS): '" + subjectAltName;
                        } else {
                            VLOG(0) << "           SAN (Type): '" + std::to_string(generalName->type);
                        }
                    }
                    sk_GENERAL_NAME_pop_free(subjectAltNames, GENERAL_NAME_free);

                    X509_free(server_cert);
                } else {
                    VLOG(0) << "     Server certificate: no certificate";
                }
            },
            [](Request& request) -> void {
                request.url = "/index.html";
                request.end();
            },
            [](Response& response) -> void {
                VLOG(0) << "-- OnResponse";
                VLOG(0) << "     Status:";
                VLOG(0) << "       " << response.httpVersion << " " << response.statusCode << " " << response.reason;

                VLOG(0) << "     Headers:";
                for (auto [field, value] : *response.headers) {
                    VLOG(0) << "       " << field + " = " + value;
                }

                VLOG(0) << "     Cookies:";
                for (auto [name, cookie] : *response.cookies) {
                    VLOG(0) << "       " + name + " = " + cookie.getValue();
                    for (auto [option, value] : cookie.getOptions()) {
                        VLOG(0) << "         " + option + " = " + value;
                    }
                }

                char* body = new char[response.contentLength + 1];
                memcpy(body, response.body, response.contentLength);
                body[response.contentLength] = 0;

                VLOG(1) << "     Body:\n----------- start body -----------\n" << body << "------------ end body ------------";

                delete[] body;
            },
            [](int status, const std::string& reason) -> void {
                VLOG(0) << "-- OnResponseError";
                VLOG(0) << "     Status: " << status;
                VLOG(0) << "     Reason: " << reason;
            },
            [](tls::Client6<>::SocketConnection* socketConnection) -> void {
                VLOG(0) << "-- OnDisconnect";

                VLOG(0) << "\tServer: " + socketConnection->getRemoteAddress().toString();
                VLOG(0) << "\tClient: " + socketConnection->getLocalAddress().toString();
            },
            {{"certChain", CLIENTCERTF}, {"keyPEM", CLIENTKEYF}, {"password", KEYFPASS}, {"caFile", SERVERCAFILE}});

        tlsClient.connect("localhost", 8088, [](int err) -> void {
            if (err != 0) {
                PLOG(ERROR) << "OnError: " << err;
            }
        }); // Connection:keep-alive\r\n\r\n"

        tlsClient.connect("localhost", 8088, [](int err) -> void {
            if (err != 0) {
                PLOG(ERROR) << "OnError: " << err;
            }
        }); // Connection:keep-alive\r\n\r\n"
    }

    return net::SNodeC::start();
}

struct A {
    int a;
    int b;
    int c = 4;
    std::string d;
};

struct B {
    std::string e;
    std::string f;
    A a;
};

void f([[maybe_unused]] B a) {
}

void g() {
    f({.e = "hihi", .f = "lkjlkj", .a = {.a = 3, .b = 4, .c = 5, .d = "hihi"}});
}
