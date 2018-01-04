#include "information.h"

Information::Information(QObject* parent) : Protocol(parent)
{
    SetType(Protocol::InformationProtocol);
    nEndLocation = ProtocolSize - 2;
    nCheckLocation = 2;
    nCheckLength = 32;
    nSoundSpeedLocation = 2;
    nTemperatureLocation = 6;
    nPressureLocation = 10;
    nQuaternion1Location = 14;
    nQuaternion2Location = 18;
    nQuaternion3Location = 22;
    nQuaternion4Location = 26;
    nVerticalSpeedLocation = 30;
    nCRCLocation = 34;
    SetStartFrame(StartFrame);
    SetEndFrame(EndFrame);
}
