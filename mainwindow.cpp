#include <QMessageBox>
#include <QDebug>
#include <QQueue>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "protocol/interaction.h"
#include "protocol/information.h"
#define     MAX_RECEIVER_BYTE_COUNT         64      // 最大存储字节数
#define     DEFAULT_BAUDRATE                "2400"
#define     DEFAULT_DATABITS                "8"
#define     DEFAULT_STOPBITS                "1"
#define     DEFAULT_PARITY                  "None"
#define     DEFAULT_FLOWCONTROL             "None"

Q_DECLARE_METATYPE(QSerialPortInfo)

QRegExp g_CaliberationValidatorRegExp("10|([0-9]{0,1}[\\.][0-9]{1,7})");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pSerialPort(new QSerialPort(this)),
    m_pCustomSerialPortBaudRate(new QIntValidator(0, 4000000, this)),
    m_pSoundSpeedCali(new QRegExpValidator(g_CaliberationValidatorRegExp, this)),
    m_pTemperatureCali(new QRegExpValidator(g_CaliberationValidatorRegExp, this)),
    m_pPressureCali(new QRegExpValidator(g_CaliberationValidatorRegExp, this)),
    m_pQuatern1Cali(new QRegExpValidator(g_CaliberationValidatorRegExp, this)),
    m_pQuatern2Cali(new QRegExpValidator(g_CaliberationValidatorRegExp, this)),
    m_pQuatern3Cali(new QRegExpValidator(g_CaliberationValidatorRegExp, this)),
    m_pQuatern4Cali(new QRegExpValidator(g_CaliberationValidatorRegExp, this)),
    m_pVerticalSpeedCali(new QRegExpValidator(g_CaliberationValidatorRegExp, this)),
    m_pVoltageCali(new QRegExpValidator(g_CaliberationValidatorRegExp, this))
{
    ui->setupUi(this);
    setWindowFlags((this->windowFlags()&~Qt::WindowMinMaxButtonsHint)|Qt::WindowMinimizeButtonHint);
    setFixedSize(width(), height());

//    connect(ui->cbSerialPortName, &ComboBoxWithRefresher::popupShow,
//            this, &MainWindow::on_cbSerialPortName_popupShow);
    connect(this, &MainWindow::GetProtocol, this, &MainWindow::ProtocolDeal);

    //初始化
    SerialPortInfoInit();
    SerialPortConfigInit();
    CalibrationInit();
    ChartInit();
    SetState(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetState(bool bIsConnected)
{
    ui->gbSerialPortInfo->setEnabled(!bIsConnected);
    ui->lblSerialPortState->setText(QString(bIsConnected ? "已连接" : "未连接"));
    ui->btnSerialPortConnect->setText(QString(bIsConnected ? "断开(&D)" : "连接(&C)"));
    ui->gbCalibration->setEnabled(bIsConnected);
    ui->gbDataCollection->setEnabled(bIsConnected);
}

void MainWindow::SerialPortInfoInit()
{
    ui->cbSerialPortName->clear();
    //读取串口信息
    foreach (const QSerialPortInfo &iInfo, QSerialPortInfo::availablePorts())
    {
        QVariant iVar;
        iVar.setValue(iInfo);
        ui->cbSerialPortName->addItem(iInfo.portName(), iVar);
    }
}

void MainWindow::SerialPortConfigInit()
{
    ui->cbSerialPortBaudRate->addItem(QString("1200"), QSerialPort::Baud1200);
    ui->cbSerialPortBaudRate->addItem(QString("2400"), QSerialPort::Baud2400);
    ui->cbSerialPortBaudRate->addItem(QString("4800"), QSerialPort::Baud4800);
    ui->cbSerialPortBaudRate->addItem(QString("9600"), QSerialPort::Baud9600);
    ui->cbSerialPortBaudRate->addItem(QString("19200"), QSerialPort::Baud19200);
    ui->cbSerialPortBaudRate->addItem(QString("38400"), QSerialPort::Baud38400);
    ui->cbSerialPortBaudRate->addItem(QString("57600"), QSerialPort::Baud57600);
    ui->cbSerialPortBaudRate->addItem(QString("115200"), QSerialPort::Baud115200);
    ui->cbSerialPortBaudRate->addItem(QString("Custom"));
    ui->cbSerialPortBaudRate->setCurrentText(DEFAULT_BAUDRATE);

    ui->cbSerialPortDataBits->addItem(QString("5"), QSerialPort::Data5);
    ui->cbSerialPortDataBits->addItem(QString("6"), QSerialPort::Data6);
    ui->cbSerialPortDataBits->addItem(QString("7"), QSerialPort::Data7);
    ui->cbSerialPortDataBits->addItem(QString("8"), QSerialPort::Data8);
    ui->cbSerialPortDataBits->setCurrentText(DEFAULT_DATABITS);

    ui->cbSerialPortStopBits->addItem(QString("1"), QSerialPort::OneStop);
    ui->cbSerialPortStopBits->addItem(QString("1.5"), QSerialPort::OneAndHalfStop);
    ui->cbSerialPortStopBits->addItem(QString("2"), QSerialPort::TwoStop);
    ui->cbSerialPortStopBits->setCurrentText(DEFAULT_STOPBITS);

    ui->cbSerialPortParity->addItem(QString("None"), QSerialPort::NoParity);
    ui->cbSerialPortParity->addItem(QString("Even"), QSerialPort::EvenParity);
    ui->cbSerialPortParity->addItem(QString("Odd"), QSerialPort::OddParity);
    ui->cbSerialPortParity->addItem(QString("Space"), QSerialPort::SpaceParity);
    ui->cbSerialPortParity->addItem(QString("Mark"), QSerialPort::MarkParity);
    ui->cbSerialPortParity->setCurrentText(DEFAULT_PARITY);

    ui->cbSerialPortFlowControl->addItem(QString("None"), QSerialPort::NoFlowControl);
    ui->cbSerialPortFlowControl->addItem(QString("Hardware"), QSerialPort::HardwareControl);
    ui->cbSerialPortFlowControl->addItem(QString("Software"), QSerialPort::SoftwareControl);
    ui->cbSerialPortFlowControl->setCurrentText(DEFAULT_FLOWCONTROL);
}

void MainWindow::CalibrationInit()
{
    ui->txtCaliSoundSpeed->setValidator(m_pSoundSpeedCali);
    ui->txtCaliTemperature->setValidator(m_pTemperatureCali);
    ui->txtCaliPressure->setValidator(m_pPressureCali);
    ui->txtCaliQuat1->setValidator(m_pQuatern1Cali);
    ui->txtCaliQuat2->setValidator(m_pQuatern2Cali);
    ui->txtCaliQuat3->setValidator(m_pQuatern3Cali);
    ui->txtCaliQuat4->setValidator(m_pQuatern4Cali);
    ui->txtCaliVertSpeed->setValidator(m_pVerticalSpeedCali);
    ui->txtCaliVoltage->setValidator(m_pVoltageCali);
}

void MainWindow::ChartInit()
{
//    // 测试用样点
//    *ui->chartView->series() << QPointF(1300, 8000);
//    *ui->chartView->series() << QPointF(1800, 9000);
//    *ui->chartView->series() << QPointF(1200, 14000);
//    *ui->chartView->series() << QPointF(1800, 18000);
}

void MainWindow::PrintCaliText(Protocol *pData)
{
    ui->txtCaliSoundSpeed->setText(QString::number(pData->GetSoundSpeedFrame()));
    ui->txtCaliTemperature->setText(QString::number(pData->GetTemperatureFrame()));
    ui->txtCaliPressure->setText(QString::number(pData->GetPressureFrame()));
    ui->txtCaliQuat1->setText(QString::number(pData->GetQuat1Frame()));
    ui->txtCaliQuat2->setText(QString::number(pData->GetQuat2Frame()));
    ui->txtCaliQuat3->setText(QString::number(pData->GetQuat3Frame()));
    ui->txtCaliQuat4->setText(QString::number(pData->GetQuat4Frame()));
    ui->txtCaliVertSpeed->setText(QString::number(pData->GetVerticalSpeedFrame()));
    ui->txtCaliVoltage->setText(QString::number(pData->GetVoltageFrame()));
}

void MainWindow::PrintDataText(Protocol *pData)
{
    ui->txtDataSoundSpeed->setText(QString::number(pData->GetSoundSpeedFrame()));
    ui->txtDataTemperature->setText(QString::number(pData->GetTemperatureFrame()));
    ui->txtDataPressure->setText(QString::number(pData->GetPressureFrame()));
    ui->txtDataVertSpeed->setText(QString::number(pData->GetVerticalSpeedFrame()));
    ui->txtDataQuat1->setText(QString::number(pData->GetQuat1Frame()));
    ui->txtDataQuat2->setText(QString::number(pData->GetQuat2Frame()));
    ui->txtDataQuat3->setText(QString::number(pData->GetQuat3Frame()));
    ui->txtDataQuat4->setText(QString::number(pData->GetQuat4Frame()));
}

void MainWindow::PrintPoint(Protocol *pData)
{
    QPointF point(pData->GetSoundSpeedFrame(), pData->GetPressureFrame());
    m_points << point;
    *ui->chartView->series() << point;
}

void MainWindow::ReadCali()
{
    Interaction *pData = new Interaction(this);
    pData->SetCommandFrame(Interaction::ReadCommand);
    pData->SetCRCFrame();
    SendProtocol(pData);
    delete pData;
}

void MainWindow::WriteCali(float soundSpeed,
                           float temperature,
                           float pressure,
                           float quat1,
                           float quat2,
                           float quat3,
                           float quat4,
                           float verticalSpeed,
                           float voltage)
{
    Interaction *pData = new Interaction(this);
    pData->SetCommandFrame(Interaction::WriteCommand);
    pData->SetSoundSpeedFrame(soundSpeed);
    pData->SetTemperatureFrame(temperature);
    pData->SetPressureFrame(pressure);
    pData->SetQuat1Frame(quat1);
    pData->SetQuat2Frame(quat2);
    pData->SetQuat3Frame(quat3);
    pData->SetQuat4Frame(quat4);
    pData->SetVerticalSpeedFrame(verticalSpeed);
    pData->SetVoltageFrame(voltage);
    pData->SetCRCFrame();
    SendProtocol(pData);
    delete pData;
}

void MainWindow::ReadProtocol()
{
    m_iReceiverMutex.lock();
    m_gReceiver.append(m_pSerialPort->readAll());
    int nAbandonLen = m_gReceiver.size() - MAX_RECEIVER_BYTE_COUNT;
    if(nAbandonLen > 0)
    {
        // 若超过最大存储字节数，丢弃头部多余的字节
        m_gReceiver.remove(0, nAbandonLen);
    }
    int nStartFrameLocation;
    int nEndFrameLocation;
    int nStartFrameProtocolType;
    int nEndFrameProtocolType;
    if((nStartFrameLocation = FindFrameOf(true, &nStartFrameProtocolType)) != -1)
    {
        m_gReceiver.remove(0, nStartFrameLocation);
    }
    if((nEndFrameLocation = FindFrameOf(false, &nEndFrameProtocolType)) != -1 && nStartFrameLocation != -1)
    {
        if(nStartFrameProtocolType != nEndFrameProtocolType)
        {
            // 若首尾类型不一致，丢弃
            m_gReceiver.remove(0, nEndFrameLocation);
        }
        m_gProtocol = m_gReceiver.mid(0, nEndFrameLocation - nStartFrameLocation + 1);
        m_gReceiver.remove(0, nEndFrameLocation);
//        qDebug() << "find end" << m_protocol;
        emit GetProtocol(nStartFrameProtocolType);
    }
    m_iReceiverMutex.unlock();
}

int MainWindow::FindFrameOf(bool bIsStart, int *pnProtocolType)
{
    int nIndex = -1;
    BYTE2 gcFeatures[2] =
    {bIsStart ? Interaction::StartFrame : Interaction::EndFrame,
     bIsStart ? Information::StartFrame : Information::EndFrame};
    for(int i = 0; i < m_gReceiver.size(); i++)
    {
        BYTE2 cCurFrame = (BYTE2)((m_gReceiver[i + 1] << 8) |
                          (m_gReceiver[i] & 0xFF));
        if(cCurFrame == gcFeatures[Protocol::InteractionProtocol])
        {
            nIndex = bIsStart ? i : i + 1;
            *pnProtocolType = Protocol::InteractionProtocol;
            break;
        }
        else if(cCurFrame == gcFeatures[Protocol::InformationProtocol])
        {
            nIndex = bIsStart ? i : i + 1;
            *pnProtocolType = Protocol::InformationProtocol;
            break;
        }
    }
    return nIndex;
}

void MainWindow::SendProtocol(Protocol *pData)
{
    m_pSerialPort->write(pData->GetQByteArray());
}

void MainWindow::ProtocolDeal(int nProtocolType)
{
    Protocol *pData;
    if(nProtocolType == Protocol::InteractionProtocol)
    {
        pData = new Interaction(this);
        pData->SetData(m_gProtocol);
        // 校验CRC
        if(pData->CheckCRC())
        {
            PrintCaliText(pData);
        }
    }
    else
    {
        pData = new Information(this);
        pData->SetData(m_gProtocol);
        // 校验CRC
        if(pData->CheckCRC())
        {
            PrintDataText(pData);
            PrintPoint(pData);
        }
    }
    delete pData;
}

void MainWindow::on_btnSerialPortConnect_clicked()
{
    if(m_pSerialPort->isOpen() == false)
    {
        if(ui->cbSerialPortName->currentIndex() == -1)
        {
            QMessageBox::critical(NULL, "错误", "未选择任何串口");
            return;
        }
        //打开串口
        QSerialPortInfo info = ui->cbSerialPortName->currentData().value<QSerialPortInfo>();
        m_pSerialPort->setPortName(info.portName());
        if(ui->cbSerialPortBaudRate->currentIndex() == 8)
        {
            m_pSerialPort->setBaudRate(ui->cbSerialPortBaudRate->currentText().toInt());
        }
        else
        {
            m_pSerialPort->setBaudRate(QSerialPort::BaudRate(ui->cbSerialPortBaudRate->currentData().toInt()));
        }
        m_pSerialPort->setDataBits(QSerialPort::DataBits(ui->cbSerialPortDataBits->currentData().toInt()));
        m_pSerialPort->setStopBits(QSerialPort::StopBits(ui->cbSerialPortStopBits->currentData().toInt()));
        m_pSerialPort->setParity(QSerialPort::Parity(ui->cbSerialPortParity->currentData().toInt()));
        m_pSerialPort->setFlowControl(QSerialPort::FlowControl(ui->cbSerialPortFlowControl->currentData().toInt()));
        if(!m_pSerialPort->open(QIODevice::ReadWrite))
        {
            QMessageBox::critical(NULL, "错误", "串口打开失败");
            return;
        }
        //绑定数据接收槽
        QObject::connect(m_pSerialPort, &QSerialPort::readyRead, this, &MainWindow::ReadProtocol);
        SetState(true);
        // 发送请求读取校准值协议
        ReadCali();
    }
    else
    {
        m_pSerialPort->close();
        SetState(false);
    }
}

void MainWindow::on_btnCaliVerify_clicked()
{
    WriteCali(ui->txtCaliSoundSpeed->text().toFloat(),
              ui->txtCaliTemperature->text().toFloat(),
              ui->txtCaliPressure->text().toFloat(),
              ui->txtCaliQuat1->text().toFloat(),
              ui->txtCaliQuat2->text().toFloat(),
              ui->txtCaliQuat3->text().toFloat(),
              ui->txtCaliQuat4->text().toFloat(),
              ui->txtCaliVertSpeed->text().toFloat(),
              ui->txtCaliVoltage->text().toFloat());
}

void MainWindow::on_btnCaliReset_clicked()
{
    WriteCali(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void MainWindow::on_btnCaliRefresh_clicked()
{
    ReadCali();
}

void MainWindow::on_cbSerialPortName_popupShow()
{
    SerialPortInfoInit();
}

void MainWindow::on_cbSerialPortBaudRate_currentIndexChanged(int index)
{
    if(!ui->cbSerialPortBaudRate->itemData(index).isValid())
    {
        ui->cbSerialPortBaudRate->setEditable(true);
        ui->cbSerialPortBaudRate->clearEditText();
        QLineEdit *pEdit = ui->cbSerialPortBaudRate->lineEdit();
        pEdit->setValidator(m_pCustomSerialPortBaudRate);
    }
    else
    {
        ui->cbSerialPortBaudRate->setEditable(false);
    }
}
