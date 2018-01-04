#include "protocol.h"

Protocol::Protocol(QObject *parent) : QObject(parent)
{
    sn_start_location = 0;
}

void Protocol::SetByte(int index, BYTE value)
{
    protocolData[index] = value;
}

BYTE Protocol::GetByte(int index) const
{
    return protocolData[index];
}

void Protocol::SetByte2(int index, BYTE2 value)
{
    protocolData[index] = value & 0xFF;
    protocolData[index + 1] = value >> 8;
}

BYTE2 Protocol::GetByte2(int index) const
{
    return (BYTE2)((protocolData[index + 1] << 8) |
                   (protocolData[index] & 0xFF));
}

void Protocol::SetByte4(int index, BYTE4 value)
{
    protocolData[index] = value & 0xFF;
    protocolData[index + 1] = (value >> 8) & 0xFF;
    protocolData[index + 2] = (value >> 16) & 0xFF;
    protocolData[index + 3] = (value >> 24) & 0xFF;
}

BYTE4 Protocol::GetByte4(int index) const
{
    return (BYTE4)((protocolData[index + 3] << 24) |
            ((protocolData[index + 2] & 0xFF) << 16) |
            ((protocolData[index + 1] & 0xFF) << 8) |
             (protocolData[index] & 0xFF));
}

void Protocol::SetFloat4(int index, float value)
{
    BYTE4 *p = (BYTE4*)&value;
    protocolData[index] = *p & 0xFF;
    protocolData[index + 1] = (*p >> 8) & 0xFF;
    protocolData[index + 2] = (*p >> 16) & 0xFF;
    protocolData[index + 3] = (*p >> 24) & 0xFF;
}

float Protocol::GetFloat4(int index) const
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

void Protocol::SetType(int type)
{
    protocolType = type;
}

int Protocol::GetType()
{
    return this->protocolType;
}

void Protocol::SetData(QByteArray &data)
{
    protocolData = QByteArray(data);
}

QByteArray &Protocol::GetQByteArray()
{
    return protocolData;
}

int Protocol::DataFrame(BYTE* dataFramePtr) const
{
    for(int i = 0; i < sn_check_length; i++)
    {
        dataFramePtr[i] = protocolData[sn_check_location + i];
    }
    return sn_check_length;
}

void Protocol::SetStartFrame(BYTE2 value)
{
    SetByte2(sn_start_location, value);
}

BYTE2 Protocol::GetStartFrame() const
{
    return GetByte2(sn_start_location);
}

void Protocol::SetCommandFrame(BYTE2 value)
{
    SetByte2(sn_command_location, value);
}

BYTE2 Protocol::GetCommandFrame() const
{
    return GetByte2(sn_command_location);
}

void Protocol::SetStateFrame(BYTE2 value)
{
    SetByte2(sn_state_location, value);
}

BYTE2 Protocol::GetStateFrame() const
{
    return GetByte2(sn_state_location);
}

void Protocol::SetSoundSpeedFrame(float value)
{
    SetFloat4(sn_sound_speed_location, value);
}

float Protocol::GetSoundSpeedFrame() const
{
    return GetFloat4(sn_sound_speed_location);
}

void Protocol::SetTemperatureFrame(float value)
{
    SetFloat4(sn_temperature_location, value);
}

float Protocol::GetTemperatureFrame() const
{
    return GetFloat4(sn_temperature_location);
}

void Protocol::SetPressureFrame(float value)
{
    SetFloat4(sn_pressure_location, value);
}

float Protocol::GetPressureFrame() const
{
    return GetFloat4(sn_pressure_location);
}

void Protocol::SetQuat1Frame(float value)
{
    SetFloat4(sn_quaternion_1_location, value);
}

float Protocol::GetQuat1Frame() const
{
    return GetFloat4(sn_quaternion_1_location);
}

void Protocol::SetQuat2Frame(float value)
{
    SetFloat4(sn_quaternion_2_location, value);
}

float Protocol::GetQuat2Frame() const
{
    return GetFloat4(sn_quaternion_2_location);
}

void Protocol::SetQuat3Frame(float value)
{
    SetFloat4(sn_quaternion_3_location, value);
}

float Protocol::GetQuat3Frame() const
{
    return GetFloat4(sn_quaternion_3_location);
}

void Protocol::SetQuat4Frame(float value)
{
    SetFloat4(sn_quaternion_4_location, value);
}

float Protocol::GetQuat4Frame() const
{
    return GetFloat4(sn_quaternion_4_location);
}

void Protocol::SetVerticalSpeedFrame(float value)
{
    SetFloat4(sn_vertical_speed_location, value);
}

float Protocol::GetVerticalSpeedFrame() const
{
    return GetFloat4(sn_vertical_speed_location);
}

void Protocol::SetVoltageFrame(float value)
{
    SetFloat4(sn_voltage_location, value);
}

float Protocol::GetVoltageFrame() const
{
    return GetFloat4(sn_voltage_location);
}

void Protocol::SetCRCFrame(BYTE2 value)
{
    if(value != 0)
    {
        SetByte2(sn_crc_location, value);
    }
    else
    {
        BYTE checkBytes[sn_check_length];
        DataFrame(checkBytes);
        BYTE2 CRCValue = Protocol::CRC16_XMODEM(checkBytes, sn_check_length);
        SetCRCFrame(CRCValue);
    }
}

BYTE2 Protocol::GetCRCFrame() const
{
    return GetByte2(sn_crc_location);
}

void Protocol::SetEndFrame(BYTE2 value)
{
    SetByte2(sn_end_location, value);
}

BYTE2 Protocol::GetEndFrame() const
{
    return GetByte2(sn_end_location);
}

bool Protocol::CheckCRC() const
{
    BYTE checkBytes[sn_check_length];
    DataFrame(checkBytes);
    BYTE2 CRCValue = Protocol::CRC16_XMODEM(checkBytes, sn_check_length);
    return CRCValue == GetCRCFrame();
}

QString Protocol::PrintDebug()
{
    QString str;
    foreach (BYTE byte, protocolData)
    {
        str.append(QString::number(byte, 16)).append(" ");
    }
    return str;
}
