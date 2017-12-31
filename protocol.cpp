#include "protocol.h"

Protocol::Protocol(QObject *parent) : QObject(parent)
{
    START_LOCATION = 0;
}

BYTE Protocol::getByte(int index) const
{
    return protocolData[index];
}

BYTE2 Protocol::CRC16_XMODEM(BYTE *puchMsg, BYTE4 usDataLen)
{
  unsigned short wCRCin = 0x0000;
  unsigned short wCPoly = 0x1021;
  unsigned char wChar = 0;

  while (usDataLen--)
  {
        wChar = *(puchMsg++);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  return (wCRCin) ;
}

void Protocol::setType(int Type)
{
    protocolType = Type;
}

int Protocol::getType()
{
    return this->protocolType;
}

void Protocol::setData(QByteArray &data)
{
    protocolData = QByteArray(data);
}

QByteArray &Protocol::getQByteArray()
{
    return protocolData;
}

int Protocol::dataFrame(BYTE* dataFramePtr) const
{
    for(int i = 0; i < CHECK_LENGTH; i++)
    {
        dataFramePtr[i] = protocolData[CHECK_LOCATION + i];
    }
    return CHECK_LENGTH;
}

void Protocol::setStartFrame(BYTE2 value)
{
    protocolData[START_LOCATION] = value & 0xFF;
    protocolData[START_LOCATION + 1] = value >> 8;
}

BYTE2 Protocol::getStartFrame() const
{
    return (BYTE2)((protocolData[START_LOCATION + 1] << 8) |
                    protocolData[START_LOCATION]);
}

void Protocol::setCommandFrame(BYTE2 value)
{
    protocolData[COMMAND_LOCATION] = value & 0xFF;
    protocolData[COMMAND_LOCATION + 1] = value >> 8;
}

BYTE2 Protocol::getCommandFrame() const
{
    return (BYTE2)((protocolData[COMMAND_LOCATION + 1] << 8) |
                    protocolData[COMMAND_LOCATION]);
}

void Protocol::setStateFrame(BYTE2 value)
{
    protocolData[START_LOCATION] = value & 0xFF;
    protocolData[START_LOCATION + 1] = value >> 8;
}

BYTE2 Protocol::getStateFrame() const
{
    return (BYTE2)((protocolData[STATE_LOCATION + 1] << 8) |
                    protocolData[STATE_LOCATION]);
}

void Protocol::setSoundSpeedFrame(BYTE4 value)
{
    protocolData[SOUND_SPEED_LOCATION] = value & 0xFF;
    protocolData[SOUND_SPEED_LOCATION + 1] = (value >> 8) & 0xFF;
    protocolData[SOUND_SPEED_LOCATION + 2] = (value >> 16) & 0xFF;
    protocolData[SOUND_SPEED_LOCATION + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getSoundSpeedFrame() const
{
    return (BYTE4)((protocolData[SOUND_SPEED_LOCATION + 3] << 24) |
                   (protocolData[SOUND_SPEED_LOCATION + 2] << 16) |
                   (protocolData[SOUND_SPEED_LOCATION + 1] << 8)  |
                    protocolData[SOUND_SPEED_LOCATION]);
}

void Protocol::setTemperatureFrame(BYTE4 value)
{
    protocolData[TEMPERATURE_LOCATION] = value & 0xFF;
    protocolData[TEMPERATURE_LOCATION + 1] = (value >> 8) & 0xFF;
    protocolData[TEMPERATURE_LOCATION + 2] = (value >> 16) & 0xFF;
    protocolData[TEMPERATURE_LOCATION + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getTemperatureFrame() const
{
    return (BYTE4)((protocolData[TEMPERATURE_LOCATION + 3] << 24) |
                   (protocolData[TEMPERATURE_LOCATION + 2] << 16) |
                   (protocolData[TEMPERATURE_LOCATION + 1] << 8)  |
                    protocolData[TEMPERATURE_LOCATION]);
}

void Protocol::setPressureFrame(BYTE4 value)
{
    protocolData[PRESSURE_LOCATION] = value & 0xFF;
    protocolData[PRESSURE_LOCATION + 1] = (value >> 8) & 0xFF;
    protocolData[PRESSURE_LOCATION + 2] = (value >> 16) & 0xFF;
    protocolData[PRESSURE_LOCATION + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getPressureFrame() const
{
    return (BYTE4)((protocolData[PRESSURE_LOCATION + 3] << 24) |
                   (protocolData[PRESSURE_LOCATION + 2] << 16) |
                   (protocolData[PRESSURE_LOCATION + 1] << 8)  |
                    protocolData[PRESSURE_LOCATION]);
}

void Protocol::setQuat1Frame(BYTE4 value)
{
    protocolData[QUATERNION_1_LOCATION] = value & 0xFF;
    protocolData[QUATERNION_1_LOCATION + 1] = (value >> 8) & 0xFF;
    protocolData[QUATERNION_1_LOCATION + 2] = (value >> 16) & 0xFF;
    protocolData[QUATERNION_1_LOCATION + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getQuat1Frame() const
{
    return (BYTE4)((protocolData[QUATERNION_1_LOCATION + 3] << 24) |
                   (protocolData[QUATERNION_1_LOCATION + 2] << 16) |
                   (protocolData[QUATERNION_1_LOCATION + 1] << 8)  |
                    protocolData[QUATERNION_1_LOCATION]);
}

void Protocol::setQuat2Frame(BYTE4 value)
{
    protocolData[QUATERNION_2_LOCATION] = value & 0xFF;
    protocolData[QUATERNION_2_LOCATION + 1] = (value >> 8) & 0xFF;
    protocolData[QUATERNION_2_LOCATION + 2] = (value >> 16) & 0xFF;
    protocolData[QUATERNION_2_LOCATION + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getQuat2Frame() const
{
    return (BYTE4)((protocolData[QUATERNION_2_LOCATION + 3] << 24) |
                   (protocolData[QUATERNION_2_LOCATION + 2] << 16) |
                   (protocolData[QUATERNION_2_LOCATION + 1] << 8)  |
                    protocolData[QUATERNION_2_LOCATION]);
}

void Protocol::setQuat3Frame(BYTE4 value)
{
    protocolData[QUATERNION_3_LOCATION] = value & 0xFF;
    protocolData[QUATERNION_3_LOCATION + 1] = (value >> 8) & 0xFF;
    protocolData[QUATERNION_3_LOCATION + 2] = (value >> 16) & 0xFF;
    protocolData[QUATERNION_3_LOCATION + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getQuat3Frame() const
{
    return (BYTE4)((protocolData[QUATERNION_3_LOCATION + 3] << 24) |
                   (protocolData[QUATERNION_3_LOCATION + 2] << 16) |
                   (protocolData[QUATERNION_3_LOCATION + 1] << 8)  |
                    protocolData[QUATERNION_3_LOCATION]);
}

void Protocol::setQuat4Frame(BYTE4 value)
{
    protocolData[QUATERNION_4_LOCATION] = value & 0xFF;
    protocolData[QUATERNION_4_LOCATION + 1] = (value >> 8) & 0xFF;
    protocolData[QUATERNION_4_LOCATION + 2] = (value >> 16) & 0xFF;
    protocolData[QUATERNION_4_LOCATION + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getQuat4Frame() const
{
    return (BYTE4)((protocolData[QUATERNION_4_LOCATION + 3] << 24) |
                   (protocolData[QUATERNION_4_LOCATION + 2] << 16) |
                   (protocolData[QUATERNION_4_LOCATION + 1] << 8)  |
                    protocolData[QUATERNION_4_LOCATION]);
}

void Protocol::setVerticalSpeedFrame(BYTE4 value)
{
    protocolData[VERTICAL_SPEED_LOCATION] = value & 0xFF;
    protocolData[VERTICAL_SPEED_LOCATION + 1] = (value >> 8) & 0xFF;
    protocolData[VERTICAL_SPEED_LOCATION + 2] = (value >> 16) & 0xFF;
    protocolData[VERTICAL_SPEED_LOCATION + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getVerticalSpeedFrame() const
{
    return (BYTE4)((protocolData[VERTICAL_SPEED_LOCATION + 3] << 24) |
                   (protocolData[VERTICAL_SPEED_LOCATION + 2] << 16) |
                   (protocolData[VERTICAL_SPEED_LOCATION + 1] << 8)  |
                    protocolData[VERTICAL_SPEED_LOCATION]);
}

void Protocol::setVoltageFrame(BYTE4 value)
{
    protocolData[VOLTAGE_LOCATION] = value & 0xFF;
    protocolData[VOLTAGE_LOCATION + 1] = (value >> 8) & 0xFF;
    protocolData[VOLTAGE_LOCATION + 2] = (value >> 16) & 0xFF;
    protocolData[VOLTAGE_LOCATION + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getVoltageFrame() const
{
    return (BYTE4)((protocolData[VOLTAGE_LOCATION + 3] << 24) |
                   (protocolData[VOLTAGE_LOCATION + 2] << 16) |
                   (protocolData[VOLTAGE_LOCATION + 1] << 8)  |
                    protocolData[VOLTAGE_LOCATION]);
}

void Protocol::setCRCFrame(BYTE2 value)
{
    protocolData[CRC_LOCATION] = value & 0xFF;
    protocolData[CRC_LOCATION + 1] = value >> 8;
}

BYTE2 Protocol::getCRCFrame() const
{
    return (BYTE2)((protocolData[CRC_LOCATION + 1] << 8) |
                    protocolData[CRC_LOCATION]);
}

bool Protocol::CheckCRC() const
{
    BYTE checkBytes[CHECK_LENGTH];
    dataFrame(checkBytes);
    BYTE2 CRCValue = Protocol::CRC16_XMODEM(checkBytes, CHECK_LENGTH);
    return CRCValue == getCRCFrame();
}

void Protocol::setEndFrame(BYTE2 value)
{
    protocolData[START_LOCATION] = value & 0xFF;
    protocolData[START_LOCATION + 1] = value >> 8;
}

BYTE2 Protocol::getEndFrame() const
{
    return (BYTE2)((getByte(START_LOCATION + 1) << 8) |
                    getByte(START_LOCATION));
}
