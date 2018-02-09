#ifndef INFORMATION_H
#define INFORMATION_H
#define INFORMATION_PROTOCOL_SIZE   11

#include "protocol.h"

class Information : public Protocol
{
public:
    static const int ProtocolSize = INFORMATION_PROTOCOL_SIZE;
    static const BYTE2 StartFrame = 0x4040;
    static const BYTE2 EndFrame = 0x2424;
    Information(QObject *parent = nullptr);
};

#endif // INFORMATION_H
