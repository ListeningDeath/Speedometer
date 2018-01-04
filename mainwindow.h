#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
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
    QSerialPort *sp;
    QByteArray receivedBytes;
    QByteArray m_protocol;
    QMutex receiverMutex;
//    QChart *myChart;
//    QLineSeries *myLine;
    QList<QPointF> pointList;
    void changeState(bool);
    void initCombobox();
    void initChart();
    void printCaliText(Protocol&);
    void printDataText(Protocol&);
    void printPoint(Protocol&);
    void readCali();
    void writeCali(float, float, float, float, float, float, float, float, float);
    void storeBytes();
    int findFrameOf(bool);
    void sendProtocol(Protocol*);

private slots:
    void protocolDeal();
    void on_btnSerialPortConnect_clicked();
    void on_btnCaliVerify_clicked();
    void on_btnCaliReset_clicked();
    void on_btnCaliRefresh_clicked();

signals:
    void getProtocol();
};

#endif // MAINWINDOW_H
