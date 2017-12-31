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
    BYTE getByte(int) const;
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
    void setSoundSpeedFrame(BYTE4);
    BYTE4 getSoundSpeedFrame() const;
    void setTemperatureFrame(BYTE4);
    BYTE4 getTemperatureFrame() const;
    void setPressureFrame(BYTE4);
    BYTE4 getPressureFrame() const;
    void setQuat1Frame(BYTE4);
    BYTE4 getQuat1Frame() const;
    void setQuat2Frame(BYTE4);
    BYTE4 getQuat2Frame() const;
    void setQuat3Frame(BYTE4);
    BYTE4 getQuat3Frame() const;
    void setQuat4Frame(BYTE4);
    BYTE4 getQuat4Frame() const;
    void setVerticalSpeedFrame(BYTE4);
    BYTE4 getVerticalSpeedFrame() const;
    void setVoltageFrame(BYTE4);
    BYTE4 getVoltageFrame() const;
    void setCRCFrame(BYTE2);
    BYTE2 getCRCFrame() const;
    void setEndFrame(BYTE2);
    BYTE2 getEndFrame() const;
    bool CheckCRC() const;

private:
    QByteArray protocolData;
    int protocolType;
};

#endif // PROTOCOL_H
