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
    explicit Protocol(QObject *parent = nullptr);
    static BYTE2 CRC16_XMODEM(BYTE*, BYTE4);
    void setType(int);

    // 协议初始化
    int START_LOCATION = 0;
    int CHECK_LOCATION = 0;
    int CHECK_LENGTH = 0;
    int COMMAND_LOCATION = 0;
    int STATE_LOCATION = 0;
    int SOUND_SPEED_LOCATION = 0;
    int TEMPERATURE_LOCATION = 0;
    int PRESSURE_LOCATION = 0;
    int QUATERNION_1_LOCATION = 0;
    int QUATERNION_2_LOCATION = 0;
    int QUATERNION_3_LOCATION = 0;
    int QUATERNION_4_LOCATION = 0;
    int VERTICAL_SPEED_LOCATION = 0;
    int VOLTAGE_LOCATION = 0;
    int CRC_LOCATION = 0;
    int END_LOCATION = 0;

public:
    // 协议类型
    static const int INTERACTION_PROTOCOL = 0;
    static const int INFORMATION_PROTOCOL = 1;
    int getType();
    void setData(QByteArray&);

    // 接口
    QByteArray &getQByteArray();
    int dataFrame(BYTE*) const;
    void setStartFrame(BYTE2);
    BYTE2 getStartFrame() const;
    void setCommandFrame(BYTE2);
    BYTE2 getCommandFrame() const;
    void setStateFrame(BYTE2);
    BYTE2 getStateFrame() const;
    void setSoundSpeedFrame(float);
    float getSoundSpeedFrame() const;
    void setTemperatureFrame(float);
    float getTemperatureFrame() const;
    void setPressureFrame(float);
    float getPressureFrame() const;
    void setQuat1Frame(float);
    float getQuat1Frame() const;
    void setQuat2Frame(float);
    float getQuat2Frame() const;
    void setQuat3Frame(float);
    float getQuat3Frame() const;
    void setQuat4Frame(float);
    float getQuat4Frame() const;
    void setVerticalSpeedFrame(float);
    float getVerticalSpeedFrame() const;
    void setVoltageFrame(float);
    float getVoltageFrame() const;
    void setCRCFrame(BYTE2 value = 0);
    BYTE2 getCRCFrame() const;
    void setEndFrame(BYTE2);
    BYTE2 getEndFrame() const;
    bool CheckCRC() const;

private:
    QByteArray protocolData;
    int protocolType;
    void setByte(int, BYTE);
    BYTE getByte(int) const;
    void setByte2(int, BYTE2);
    BYTE2 getByte2(int) const;
    void setByte4(int, BYTE4);
    BYTE4 getByte4(int) const;
    void setFloat4(int, float);
    float getFloat4(int) const;
};

#endif // PROTOCOL_H
