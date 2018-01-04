#include "information.h"

Information::Information(QObject* parent) : Protocol(parent)
{
    SetType(Protocol::InformationProtocol);
    sn_end_location = ProtocolSize - 2;
    sn_check_location = 2;
    sn_check_length = 32;
    sn_sound_speed_location = 2;
    sn_temperature_location = 6;
    sn_pressure_location = 10;
    sn_quaternion_1_location = 14;
    sn_quaternion_2_location = 18;
    sn_quaternion_3_location = 22;
    sn_quaternion_4_location = 26;
    sn_vertical_speed_location = 30;
    sn_crc_location = 34;
    SetStartFrame(StartFrame);
    SetEndFrame(EndFrame);
}
