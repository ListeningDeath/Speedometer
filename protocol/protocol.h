#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QByteArray>

typedef unsigned char BYTE;
typedef unsigned short BYTE2;
typedef unsigned int BYTE4;

class Protocol : public QObject
{
    Q_OBJECT
protected:
    static BYTE2 CRC16_XMODEM(BYTE*, BYTE4);
    void SetType(int);

    // 协议初始化
    int nStartLocation = 0;
    int nCheckLocation = 0;
    int nCheckLength = 0;
    int nCommandLocation = 0;
    int nStateLocation = 0;
    int nSoundSpeedLocation = 0;
    int nTemperatureLocation = 0;
    int nPressureLocation = 0;
    int nQuaternion1Location = 0;
    int nQuaternion2Location = 0;
    int nQuaternion3Location = 0;
    int nQuaternion4Location = 0;
    int nVerticalSpeedLocation = 0;
    int nVoltageLocation = 0;
    int nCRCLocation = 0;
    int nEndLocation = 0;

public:
    explicit Protocol(QObject *parent = nullptr);
    // 协议类型
    static const int InteractionProtocol = 0;
    static const int InformationProtocol = 1;
    int GetType();
    void SetData(QByteArray&);

    // 接口
    QByteArray &GetQByteArray();
    int DataFrame(BYTE*) const;
    void SetStartFrame(BYTE2);
    BYTE2 GetStartFrame() const;
    void SetCommandFrame(BYTE2);
    BYTE2 GetCommandFrame() const;
    void SetStateFrame(BYTE2);
    BYTE2 GetStateFrame() const;
    void SetSoundSpeedFrame(float);
    float GetSoundSpeedFrame() const;
    void SetTemperatureFrame(float);
    float GetTemperatureFrame() const;
    void SetPressureFrame(float);
    float GetPressureFrame() const;
    void SetQuat1Frame(float);
    float GetQuat1Frame() const;
    void SetQuat2Frame(float);
    float GetQuat2Frame() const;
    void SetQuat3Frame(float);
    float GetQuat3Frame() const;
    void SetQuat4Frame(float);
    float GetQuat4Frame() const;
    void SetVerticalSpeedFrame(float);
    float GetVerticalSpeedFrame() const;
    void SetVoltageFrame(float);
    float GetVoltageFrame() const;
    void SetCRCFrame(BYTE2 value = 0);
    BYTE2 GetCRCFrame() const;
    void SetEndFrame(BYTE2);
    BYTE2 GetEndFrame() const;
    bool CheckCRC() const;

    QString PrintDebug();   // only for debug

private:
    QByteArray protocolData;
    int protocolType;
    void SetByte(int, BYTE);
    BYTE GetByte(int) const;
    void SetByte2(int, BYTE2);
    BYTE2 GetByte2(int) const;
    void SetByte4(int, BYTE4);
    BYTE4 GetByte4(int) const;
    void SetFloat4(int, float);
    float GetFloat4(int) const;
};

#endif // PROTOCOL_H
