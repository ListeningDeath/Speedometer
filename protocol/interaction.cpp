#include "interaction.h"

Interaction::Interaction(QObject *parent) : Protocol(parent)
{
    SetType(Protocol::InteractionProtocol);
    nEndLocation = ProtocolSize - 2;
    nCheckLocation = 2;
    nCheckLength = 40;
    nCommandLocation = 2;
    nStateLocation = 4;
    nSoundSpeedLocation = 6;
    nTemperatureLocation = 10;
    nPressureLocation = 14;
    nQuaternion1Location = 18;
    nQuaternion2Location = 22;
    nQuaternion3Location = 26;
    nQuaternion4Location = 30;
    nVerticalSpeedLocation = 34;
    nVoltageLocation = 38;
    nCRCLocation = 42;
    SetStartFrame(StartFrame);
    SetEndFrame(EndFrame);
}
