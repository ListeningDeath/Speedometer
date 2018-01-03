#ifndef INFORMATION_H
#define INFORMATION_H
#define INFORMATION_PROTOCOL_SIZE   38

#include "protocol.h"

class Information : public Protocol
{
public:
    static const int PROTOCOL_SIZE = INFORMATION_PROTOCOL_SIZE;
    static const BYTE2 START_FRAME = 0xF00F;
    static const BYTE2 END_FRAME = 0x0FF0;
    Information(QObject *parent = nullptr);
};

#endif // INFORMATION_H
