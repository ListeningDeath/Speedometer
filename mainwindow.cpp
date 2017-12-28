#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

Q_DECLARE_METATYPE(QSerialPortInfo)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags((this->windowFlags()&~Qt::WindowMinMaxButtonsHint)|Qt::WindowMinimizeButtonHint);
    setFixedSize(width(), height());

    //读取串口信息
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QVariant var;
        var.setValue(info);
        ui->cbSerialPortName->addItem(info.portName(), var);
    }

    //初始化
    spHelper = nullptr;
    initCombobox();
    changeState(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeState(bool isConnected)
{
    ui->cbSerialPortName->setEnabled(!isConnected);
    ui->cbSerialPortBaudRate->setEnabled(!isConnected);
    ui->cbSerialPortDataBits->setEnabled(!isConnected);
    ui->cbSerialPortStopBits->setEnabled(!isConnected);
    ui->cbSerialPortParity->setEnabled(!isConnected);
    ui->cbSerialPortFlowControl->setEnabled(!isConnected);
    ui->lblSerialPortState->setText(QString(isConnected ? "已连接" : "未连接"));
    ui->btnSerialPortConnect->setText(QString(isConnected ? "断开(&D)" : "连接(&C)"));
    ui->txtMessageReceived->setEnabled(isConnected);
    ui->txtMessageSend->setEnabled(isConnected);
    ui->btnMessageSend->setEnabled(isConnected);
}

void MainWindow::initCombobox()
{
    ui->cbSerialPortBaudRate->addItem(QString("1200"), QSerialPort::Baud1200);
    ui->cbSerialPortBaudRate->addItem(QString("2400"), QSerialPort::Baud2400);
    ui->cbSerialPortBaudRate->addItem(QString("4800"), QSerialPort::Baud4800);
    ui->cbSerialPortBaudRate->addItem(QString("9600"), QSerialPort::Baud9600);
    ui->cbSerialPortBaudRate->addItem(QString("19200"), QSerialPort::Baud19200);
    ui->cbSerialPortBaudRate->addItem(QString("38400"), QSerialPort::Baud38400);
    ui->cbSerialPortBaudRate->addItem(QString("57600"), QSerialPort::Baud57600);
    ui->cbSerialPortBaudRate->addItem(QString("115200"), QSerialPort::Baud115200);
    ui->cbSerialPortBaudRate->setCurrentText("2400");

    ui->cbSerialPortDataBits->addItem(QString("5"), QSerialPort::Data5);
    ui->cbSerialPortDataBits->addItem(QString("6"), QSerialPort::Data6);
    ui->cbSerialPortDataBits->addItem(QString("7"), QSerialPort::Data7);
    ui->cbSerialPortDataBits->addItem(QString("8"), QSerialPort::Data8);
    ui->cbSerialPortDataBits->setCurrentText("8");

    ui->cbSerialPortStopBits->addItem(QString("1"), QSerialPort::OneStop);
    ui->cbSerialPortStopBits->addItem(QString("1.5"), QSerialPort::OneAndHalfStop);
    ui->cbSerialPortStopBits->addItem(QString("2"), QSerialPort::TwoStop);

    ui->cbSerialPortParity->addItem(QString("None"), QSerialPort::NoParity);
    ui->cbSerialPortParity->addItem(QString("Even"), QSerialPort::EvenParity);
    ui->cbSerialPortParity->addItem(QString("Odd"), QSerialPort::OddParity);
    ui->cbSerialPortParity->addItem(QString("Space"), QSerialPort::SpaceParity);
    ui->cbSerialPortParity->addItem(QString("Mark"), QSerialPort::MarkParity);

    ui->cbSerialPortFlowControl->addItem(QString("None"), QSerialPort::NoFlowControl);
    ui->cbSerialPortFlowControl->addItem(QString("Hardware"), QSerialPort::HardwareControl);
    ui->cbSerialPortFlowControl->addItem(QString("Software"), QSerialPort::SoftwareControl);
}

void MainWindow::on_btnSerialPortConnect_clicked()
{
    if(spHelper == nullptr)
    {
        if(ui->cbSerialPortName->currentIndex() == -1)
        {
            QMessageBox::critical(NULL, "错误", "未选择任何串口");
            return;
        }
        //打开串口
        QSerialPortInfo info = ui->cbSerialPortName->currentData().value<QSerialPortInfo>();
        spHelper = new SerialPortHelper(info);
        spHelper->setBaudRate(QSerialPort::BaudRate(ui->cbSerialPortBaudRate->currentData().toInt()));
        spHelper->setDataBits(QSerialPort::DataBits(ui->cbSerialPortDataBits->currentData().toInt()));
        spHelper->setStopBits(QSerialPort::StopBits(ui->cbSerialPortStopBits->currentData().toInt()));
        spHelper->setParity(QSerialPort::Parity(ui->cbSerialPortParity->currentData().toInt()));
        spHelper->setFlowControl(QSerialPort::FlowControl(ui->cbSerialPortFlowControl->currentData().toInt()));
        spHelper->open();
        //绑定数据接收槽
        QObject::connect(spHelper, SIGNAL(receiveMessage(QString&)), this, SLOT(on_message_received(QString&)));
        changeState(true);
    }
    else
    {
        spHelper->close();
        delete spHelper;
        spHelper = nullptr;
        changeState(false);
        return;
    }
    if(spHelper != nullptr && !spHelper->isConnected())
    {
        spHelper->close();
        delete spHelper;
        spHelper = nullptr;
        changeState(false);
    }
}

void MainWindow::on_message_received(QString &message)
{
    ui->txtMessageReceived->append(message);
}

void MainWindow::on_btnMessageSend_clicked()
{
    QString message = ui->txtMessageSend->text();
    spHelper->send(message);
    ui->txtMessageSend->clear();
}

void MainWindow::on_btnMessageEmpty_clicked()
{
    ui->txtMessageReceived->clear();
}
