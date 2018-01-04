#include "interaction.h"

Interaction::Interaction(QObject *parent) : Protocol(parent)
{
    SetType(Protocol::InteractionProtocol);
    sn_end_location = ProtocolSize - 2;
    sn_check_location = 2;
    sn_check_length = 40;
    sn_command_location = 2;
    sn_state_location = 4;
    sn_sound_speed_location = 6;
    sn_temperature_location = 10;
    sn_pressure_location = 14;
    sn_quaternion_1_location = 18;
    sn_quaternion_2_location = 22;
    sn_quaternion_3_location = 26;
    sn_quaternion_4_location = 30;
    sn_vertical_speed_location = 34;
    sn_voltage_location = 38;
    sn_crc_location = 42;
    SetStartFrame(StartFrame);
    SetEndFrame(EndFrame);
}
