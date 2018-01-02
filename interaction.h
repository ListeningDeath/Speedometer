#ifndef INTERACTION_H
#define INTERACTION_H
#define INTERACTION_PROTOCOL_SIZE   46

#include "protocol.h"

class Interaction : public Protocol
{
public:
    static const int PROTOCOL_SIZE = INTERACTION_PROTOCOL_SIZE;
    static const BYTE2 START_FRAME = 0xE00E;
    static const BYTE2 END_FRAME = 0x0EE0;
    static const BYTE2 WRITE_COMMAND = 0x0002;
    static const BYTE2 READ_COMMAND = 0x0002;
    Interaction(QObject *parent = nullptr);
};

#endif // INTERACTION_H
