#ifndef INFORMATION_H
#define INFORMATION_H
#define INFORMATION_PROTOCOL_SIZE   38

#include "protocol.h"

class Information : public Protocol
{
public:
    static const int ProtocolSize = INFORMATION_PROTOCOL_SIZE;
    static const BYTE2 StartFrame = 0xF00F;
    static const BYTE2 EndFrame = 0x0FF0;
    Information(QObject *parent = nullptr);
};

#endif // INFORMATION_H
