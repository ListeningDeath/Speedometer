#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QIntValidator>
#include <QList>
#include <QMUtex>
#include "chart/view.h"
#include "protocol/protocol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort *m_pSerialPort;
    QIntValidator *m_intValidator;
    QByteArray m_gReceiver;
    QByteArray m_gProtocol;
    QMutex m_iReceiverMutex;
    QList<QPointF> m_points;
    void SetState(bool);
    void SerialPortConfigInit();
    void ChartInit();
    void PrintCaliText(Protocol*);
    void PrintDataText(Protocol*);
    void PrintPoint(Protocol*);
    void ReadCali();
    void WriteCali(float, float, float, float, float, float, float, float, float);
    int FindFrameOf(bool, int*);
    void ReadProtocol();
    void SendProtocol(Protocol*);

private slots:
    void ProtocolDeal(int);
    void on_btnSerialPortConnect_clicked();
    void on_btnCaliVerify_clicked();
    void on_btnCaliReset_clicked();
    void on_btnCaliRefresh_clicked();

    void on_cbSerialPortBaudRate_currentIndexChanged(int index);

signals:
    void GetProtocol(int);
};

#endif // MAINWINDOW_H
