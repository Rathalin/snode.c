#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "ManagedDescriptor.h"

class OutOfBandEvent : public ManagedDescriptor {
public:
    ~OutOfBandEvent() override = default;

    virtual void exceptionEvent() = 0;

    void enable() override;
    void disable() override;
};

#endif // EXCEPTION_H
