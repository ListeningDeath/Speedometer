#include "protocol.h"

Protocol::Protocol(QObject *parent) : QObject(parent)
{
    START_LOCATION = 0;
}

void Protocol::setByte(int index, BYTE value)
{
    protocolData[index] = value;
}

BYTE Protocol::getByte(int index) const
{
    return protocolData[index];
}

void Protocol::setByte2(int index, BYTE2 value)
{
    protocolData[index] = value & 0xFF;
    protocolData[index + 1] = value >> 8;
}

BYTE2 Protocol::getByte2(int index) const
{
    return (BYTE2)((protocolData[index + 1] << 8) |
                   (protocolData[index] & 0xFF));
}

void Protocol::setByte4(int index, BYTE4 value)
{
    protocolData[index] = value & 0xFF;
    protocolData[index + 1] = (value >> 8) & 0xFF;
    protocolData[index + 2] = (value >> 16) & 0xFF;
    protocolData[index + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::getByte4(int index) const
{
    return (BYTE4)((protocolData[index + 3] << 24) |
            ((protocolData[index + 2] & 0xFF) << 16) |
            ((protocolData[index + 1] & 0xFF) << 8) |
             (protocolData[index] & 0xFF));
}

void Protocol::setFloat4(int index, float value)
{
    BYTE4 *p = (BYTE4*)&value;
    protocolData[index] = *p & 0xFF;
    protocolData[index + 1] = (*p >> 8) & 0xFF;
    protocolData[index + 2] = (*p >> 16) & 0xFF;
    protocolData[index + 3] = (*p >> 24) & 0xFF;
}

float Protocol::getFloat4(int index) const
{
    float value;
    BYTE4 *p = (BYTE4*)&value;
    *p = (protocolData[index + 3] << 24) |
         ((protocolData[index + 2] & 0xFF) << 16) |
         ((protocolData[index + 1] & 0xFF) << 8) |
          (protocolData[index] & 0xFF);
    return value;
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
    setByte2(START_LOCATION, value);
}

BYTE2 Protocol::getStartFrame() const
{
    return getByte2(START_LOCATION);
}

void Protocol::setCommandFrame(BYTE2 value)
{
    setByte2(COMMAND_LOCATION, value);
}

BYTE2 Protocol::getCommandFrame() const
{
    return getByte2(COMMAND_LOCATION);
}

void Protocol::setStateFrame(BYTE2 value)
{
    setByte2(STATE_LOCATION, value);
}

BYTE2 Protocol::getStateFrame() const
{
    return getByte2(STATE_LOCATION);
}

void Protocol::setSoundSpeedFrame(float value)
{
    setFloat4(SOUND_SPEED_LOCATION, value);
}

float Protocol::getSoundSpeedFrame() const
{
    return getFloat4(SOUND_SPEED_LOCATION);
}

void Protocol::setTemperatureFrame(float value)
{
    setFloat4(TEMPERATURE_LOCATION, value);
}

float Protocol::getTemperatureFrame() const
{
    return getFloat4(TEMPERATURE_LOCATION);
}

void Protocol::setPressureFrame(float value)
{
    setFloat4(PRESSURE_LOCATION, value);
}

float Protocol::getPressureFrame() const
{
    return getFloat4(PRESSURE_LOCATION);
}

void Protocol::setQuat1Frame(float value)
{
    setFloat4(QUATERNION_1_LOCATION, value);
}

float Protocol::getQuat1Frame() const
{
    return getFloat4(QUATERNION_1_LOCATION);
}

void Protocol::setQuat2Frame(float value)
{
    setFloat4(QUATERNION_2_LOCATION, value);
}

float Protocol::getQuat2Frame() const
{
    return getFloat4(QUATERNION_2_LOCATION);
}

void Protocol::setQuat3Frame(float value)
{
    setFloat4(QUATERNION_3_LOCATION, value);
}

float Protocol::getQuat3Frame() const
{
    return getFloat4(QUATERNION_3_LOCATION);
}

void Protocol::setQuat4Frame(float value)
{
    setFloat4(QUATERNION_4_LOCATION, value);
}

float Protocol::getQuat4Frame() const
{
    return getFloat4(QUATERNION_4_LOCATION);
}

void Protocol::setVerticalSpeedFrame(float value)
{
    setFloat4(VERTICAL_SPEED_LOCATION, value);
}

float Protocol::getVerticalSpeedFrame() const
{
    return getFloat4(VERTICAL_SPEED_LOCATION);
}

void Protocol::setVoltageFrame(float value)
{
    setFloat4(VOLTAGE_LOCATION, value);
}

float Protocol::getVoltageFrame() const
{
    return getFloat4(VOLTAGE_LOCATION);
}

void Protocol::setCRCFrame(BYTE2 value)
{
    if(value != 0)
    {
        setByte2(CRC_LOCATION, value);
    }
    else
    {
        BYTE checkBytes[CHECK_LENGTH];
        dataFrame(checkBytes);
        BYTE2 CRCValue = Protocol::CRC16_XMODEM(checkBytes, CHECK_LENGTH);
        setCRCFrame(CRCValue);
    }
}

BYTE2 Protocol::getCRCFrame() const
{
    return getByte2(CRC_LOCATION);
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
    setByte2(END_LOCATION, value);
}

BYTE2 Protocol::getEndFrame() const
{
    return getByte2(END_LOCATION);
}
