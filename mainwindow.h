#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTimer>
#include "timeintervaldialog.h"

#define RECORD_BTN_RECORD_ACTIVE tr("Stop &recording")
#define RECORD_STATUS_RECORD_ACTIVE tr("Recording...")
#define RECORD_BTN_RECORD_IDLE tr("&Record")
#define CONNECT_BTN_IDLE tr("&Connect")
#define CONNECT_BTN_ACTIVE tr("Dis&connect")
#define CONNECT_STATUS_ACTIVE tr("Serial opened...")
#define PLAY_BTN_ACTIVE tr("Stop &playing")
#define PLAY_BTN_IDLE tr("&Play")
#define PLAY_STATUS_ACTIVE tr("Playing...")
#define PLAY_STATUS_FINISHED tr("Playing finished!")
#define PLAY_STATUS_STOP_ACTIVE tr("Stoped playing...")


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    const QSerialPort::BaudRate BAUDRATE = QSerialPort::Baud115200;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void refreshSerialPorts();
    void initSerialConnection();

    void serialError(QSerialPort::SerialPortError error);
    void readyRead();

    void statusBarMsg(const QString &msg, int timeout = 0);
    void statePushButtonConnect();
    void statePushButtonDisconnect();
    void openDataFile();
    void saveDataFile();
    void sendPeriodicallySerialData();

    void on_pushButtonSerialRefresh_clicked();
    void on_pushButtonSerialConnect_clicked();
    void on_pushButtonRecord_clicked();
    void on_pushButtonPlay_clicked();
    void on_action_Open_data_triggered();
    void on_action_Save_data_triggered();
    void on_action_Exit_triggered();
    void on_actionClear_play_data_triggered();
    void on_actionClear_record_data_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionAuthor_triggered();
    void on_actionSet_play_interval_triggered();

private:
    Ui::MainWindow *ui;
    TimeIntervalDialog *timeIntervalDialog;
    QSerialPort *serial;
    QTimer *timer;
    QList<QByteArray> recv_data;
    QList<QByteArray> load_data;
    bool isConnected {false};
    bool isRecording {false};
    bool isPlaying {false};
};

#endif // MAINWINDOW_H
