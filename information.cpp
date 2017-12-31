#include "information.h"

Information::Information(QObject* parent) : Protocol(parent)
{
    setType(Protocol::INFORMATION_PROTOCOL);
    END_LOCATION = PROTOCOL_SIZE - 2;
    CHECK_LOCATION = 2;
    CHECK_LENGTH = 32;
    SOUND_SPEED_LOCATION = 2;
    TEMPERATURE_LOCATION = 6;
    PRESSURE_LOCATION = 10;
    QUATERNION_1_LOCATION = 14;
    QUATERNION_2_LOCATION = 18;
    QUATERNION_3_LOCATION = 22;
    QUATERNION_4_LOCATION = 26;
    VERTICAL_SPEED_LOCATION = 30;
    CRC_LOCATION = 34;
    setStartFrame(START_FRAME);
    setEndFrame(END_FRAME);
}
