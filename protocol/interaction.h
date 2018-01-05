#ifndef INTERACTION_H
#define INTERACTION_H
#define INTERACTION_PROTOCOL_SIZE   46

#include "protocol.h"

class Interaction : public Protocol
{
public:
    static const int ProtocolSize = INTERACTION_PROTOCOL_SIZE;
    static const BYTE2 StartFrame = 0xE00E;
    static const BYTE2 EndFrame = 0x0EE0;
    static const BYTE2 WriteCommand = 0x0001;
    static const BYTE2 ReadCommand = 0x0002;
    Interaction(QObject *parent = nullptr);
};

#endif // INTERACTION_H
