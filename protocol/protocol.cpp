#include "protocol.h"

Protocol::Protocol(QObject *parent) : QObject(parent)
{
    nStartLocation = 0;
}

void Protocol::SetByte(int nIndex, BYTE ucValue)
{
    protocolData[nIndex] = ucValue;
}

BYTE Protocol::GetByte(int nIndex) const
{
    return protocolData[nIndex];
}

void Protocol::SetByte2(int nIndex, BYTE2 usValue)
{
    protocolData[nIndex] = usValue & 0xFF;
    protocolData[nIndex + 1] = usValue >> 8;
}

BYTE2 Protocol::GetByte2(int nIndex) const
{
    return (BYTE2)((protocolData[nIndex + 1] << 8) |
                   (protocolData[nIndex] & 0xFF));
}

void Protocol::SetByte4(int nIndex, BYTE4 unValue)
{
    protocolData[nIndex] = unValue & 0xFF;
    protocolData[nIndex + 1] = (unValue >> 8) & 0xFF;
    protocolData[nIndex + 2] = (unValue >> 16) & 0xFF;
    protocolData[nIndex + 3] = (unValue >> 24) & 0xFF;
}

BYTE4 Protocol::GetByte4(int nIndex) const
{
    return (BYTE4)((protocolData[nIndex + 3] << 24) |
            ((protocolData[nIndex + 2] & 0xFF) << 16) |
            ((protocolData[nIndex + 1] & 0xFF) << 8) |
             (protocolData[nIndex] & 0xFF));
}

void Protocol::SetFloat4(int nIndex, float nValue)
{
    BYTE4 *p = (BYTE4*)&nValue;
    protocolData[nIndex] = *p & 0xFF;
    protocolData[nIndex + 1] = (*p >> 8) & 0xFF;
    protocolData[nIndex + 2] = (*p >> 16) & 0xFF;
    protocolData[nIndex + 3] = (*p >> 24) & 0xFF;
}

float Protocol::GetFloat4(int nIndex) const
{
    float fValue;
    BYTE4 *p = (BYTE4*)&fValue;
    *p = (protocolData[nIndex + 3] << 24) |
         ((protocolData[nIndex + 2] & 0xFF) << 16) |
         ((protocolData[nIndex + 1] & 0xFF) << 8) |
          (protocolData[nIndex] & 0xFF);
    return fValue;
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

void Protocol::SetType(int nType)
{
    protocolType = nType;
}

int Protocol::GetType()
{
    return this->protocolType;
}

void Protocol::SetData(QByteArray &rgData)
{
    protocolData = QByteArray(rgData);
}

QByteArray &Protocol::GetQByteArray()
{
    return protocolData;
}

int Protocol::DataFrame(BYTE* pucDataFrame) const
{
    for(int i = 0; i < nCheckLength; i++)
    {
        pucDataFrame[i] = protocolData[nCheckLocation + i];
    }
    return nCheckLength;
}

void Protocol::SetStartFrame(BYTE2 usValue)
{
    SetByte2(nStartLocation, usValue);
}

BYTE2 Protocol::GetStartFrame() const
{
    return GetByte2(nStartLocation);
}

void Protocol::SetCommandFrame(BYTE2 usValue)
{
    SetByte2(nCommandLocation, usValue);
}

BYTE2 Protocol::GetCommandFrame() const
{
    return GetByte2(nCommandLocation);
}

void Protocol::SetStateFrame(BYTE2 usValue)
{
    SetByte2(nStateLocation, usValue);
}

BYTE2 Protocol::GetStateFrame() const
{
    return GetByte2(nStateLocation);
}

void Protocol::SetSoundSpeedFrame(float fValue)
{
    SetFloat4(nSoundSpeedLocation, fValue);
}

float Protocol::GetSoundSpeedFrame() const
{
    return GetFloat4(nSoundSpeedLocation);
}

void Protocol::SetTemperatureFrame(float fValue)
{
    SetFloat4(nTemperatureLocation, fValue);
}

float Protocol::GetTemperatureFrame() const
{
    return GetFloat4(nTemperatureLocation);
}

void Protocol::SetPressureFrame(float fValue)
{
    SetFloat4(nPressureLocation, fValue);
}

float Protocol::GetPressureFrame() const
{
    return GetFloat4(nPressureLocation);
}

void Protocol::SetQuat1Frame(float fValue)
{
    SetFloat4(nQuaternion1Location, fValue);
}

float Protocol::GetQuat1Frame() const
{
    return GetFloat4(nQuaternion1Location);
}

void Protocol::SetQuat2Frame(float fValue)
{
    SetFloat4(nQuaternion2Location, fValue);
}

float Protocol::GetQuat2Frame() const
{
    return GetFloat4(nQuaternion2Location);
}

void Protocol::SetQuat3Frame(float fValue)
{
    SetFloat4(nQuaternion3Location, fValue);
}

float Protocol::GetQuat3Frame() const
{
    return GetFloat4(nQuaternion3Location);
}

void Protocol::SetQuat4Frame(float fValue)
{
    SetFloat4(nQuaternion4Location, fValue);
}

float Protocol::GetQuat4Frame() const
{
    return GetFloat4(nQuaternion4Location);
}

void Protocol::SetVerticalSpeedFrame(float fValue)
{
    SetFloat4(nVerticalSpeedLocation, fValue);
}

float Protocol::GetVerticalSpeedFrame() const
{
    return GetFloat4(nVerticalSpeedLocation);
}

void Protocol::SetVoltageFrame(float fValue)
{
    SetFloat4(nVoltageLocation, fValue);
}

float Protocol::GetVoltageFrame() const
{
    return GetFloat4(nVoltageLocation);
}

void Protocol::SetCRCFrame(BYTE2 usValue)
{
    if(usValue != 0)
    {
        SetByte2(nCRCLocation, usValue);
    }
    else
    {
        BYTE gucCheckBytes[nCheckLength];
        DataFrame(gucCheckBytes);
        BYTE2 usCRCValue = Protocol::CRC16_XMODEM(gucCheckBytes, nCheckLength);
        SetCRCFrame(usCRCValue);
    }
}

BYTE2 Protocol::GetCRCFrame() const
{
    return GetByte2(nCRCLocation);
}

void Protocol::SetEndFrame(BYTE2 usValue)
{
    SetByte2(nEndLocation, usValue);
}

BYTE2 Protocol::GetEndFrame() const
{
    return GetByte2(nEndLocation);
}

bool Protocol::CheckCRC() const
{
    BYTE gucCheckBytes[nCheckLength];
    DataFrame(gucCheckBytes);
    BYTE2 usCRCValue = Protocol::CRC16_XMODEM(gucCheckBytes, nCheckLength);
    return usCRCValue == GetCRCFrame();
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
