#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonPlay->setEnabled(false);
    ui->pushButtonRecord->setEnabled(false);
    refreshSerialPorts();
    timeIntervalDialog = new TimeIntervalDialog(this);
    serial = new QSerialPort(this);
    timer = new QTimer(this);
    timer->setInterval(timeIntervalDialog->getTimeInterval());

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(serialError(QSerialPort::SerialPortError)));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendPeriodicallySerialData()));
}

MainWindow::~MainWindow()
{
    if(serial->isOpen())
        serial->close();

    delete timer;
    delete serial;
    delete timeIntervalDialog;
    delete ui;
}

void MainWindow::refreshSerialPorts()
{
    ui->comboBoxSerialSelect->clear();
    auto availablePorts = QSerialPortInfo::availablePorts();
    for(auto port : availablePorts)
        ui->comboBoxSerialSelect->addItem(port.portName());
}

void MainWindow::initSerialConnection()
{
    recv_data.clear();
    serial->setBaudRate(BAUDRATE);
    serial->setDataBits(QSerialPort::Data8);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    auto portName = ui->comboBoxSerialSelect->currentText();
    serial->setPortName(portName);

    if(serial->open(QIODevice::ReadWrite))
    {
        statePushButtonConnect();
        isConnected = true;
    }
    else
        statusBarMsg(tr("Could not open the serial port..."), 2000);
}

void MainWindow::serialError(QSerialPort::SerialPortError error)
{
    if(error != QSerialPort::NoError)
    {
        statusBarMsg(serial->errorString(), 5000);
        if(serial->isOpen())
            serial->close();
        statePushButtonDisconnect();
    }
}

void MainWindow::readyRead()
{
    if(serial->canReadLine())
        recv_data.append(serial->readLine());
}

void MainWindow::statusBarMsg(const QString &msg, int timeout)
{
    this->statusBar()->showMessage(msg, timeout);
}

void MainWindow::statePushButtonConnect()
{
    statusBarMsg(CONNECT_STATUS_ACTIVE, 2000);;
    ui->pushButtonSerialConnect->setText(CONNECT_BTN_ACTIVE);
    ui->pushButtonPlay->setText(PLAY_BTN_IDLE);
    ui->pushButtonRecord->setText(RECORD_BTN_RECORD_IDLE);
    ui->pushButtonPlay->setEnabled(true);
    ui->pushButtonRecord->setEnabled(true);
}

void MainWindow::statePushButtonDisconnect()
{
    ui->pushButtonSerialConnect->setText(CONNECT_BTN_IDLE);
    ui->pushButtonPlay->setText(PLAY_BTN_IDLE);
    ui->pushButtonRecord->setText(RECORD_BTN_RECORD_IDLE);
    ui->pushButtonPlay->setEnabled(false);
    ui->pushButtonRecord->setEnabled(false);
}

void MainWindow::openDataFile()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Open data...");
    if(fileName.isEmpty())
    {
        statusBarMsg("Discarded...");
        return;
    }
    QFile openFile(fileName);
    if(openFile.open(QIODevice::ReadOnly))
    {
        while(!openFile.atEnd())
        {
            auto bytes = openFile.readLine();
            load_data.append(bytes);
        }
        openFile.close();
        statusBarMsg("File loaded", 2000);
    }
    else
         QMessageBox::critical(this, "Open error", "Could not open a file");
}

void MainWindow::saveDataFile()
{
    auto dir = QFileDialog::getSaveFileName(this, tr("Save data..."));
    if(dir.isEmpty())
    {
        statusBarMsg("Discarded...");
        return;
    }
    QFile saveFile(dir);
    if(saveFile.open(QIODevice::WriteOnly))
    {
        for(const auto &byteLine : recv_data)
            saveFile.write(byteLine);
        saveFile.flush();
        saveFile.close();
        statusBarMsg(tr("File saved"), 2000);
    }
    else
        QMessageBox::critical(this, "Save error", "Could not save a file");
}

void MainWindow::sendPeriodicallySerialData()
{
    if(serial->isOpen() && !load_data.isEmpty())
    {
        auto bytes = load_data.takeFirst();
        serial->write(bytes);
    }
    else
    {
        timer->stop();
        statusBarMsg(PLAY_STATUS_FINISHED);
        ui->pushButtonPlay->setText(PLAY_BTN_IDLE);
        isPlaying = false;
    }
}

void MainWindow::on_pushButtonSerialRefresh_clicked()
{
    refreshSerialPorts();
}

void MainWindow::on_pushButtonSerialConnect_clicked()
{
    if(!isConnected)
    {
        initSerialConnection();
    }
    else
    {
        statusBarMsg("May try save data...");
        statePushButtonDisconnect();
        if(serial->isOpen())
            serial->close();
        isConnected = false;
    }
}

void MainWindow::on_pushButtonRecord_clicked()
{
    if(!isRecording)
    {
        connect(serial, SIGNAL(readyRead()), this, SLOT(readyRead()));
        ui->pushButtonRecord->setText(RECORD_BTN_RECORD_ACTIVE);
        statusBarMsg(RECORD_STATUS_RECORD_ACTIVE);
        isRecording = true;
    }
    else
    {
        disconnect(serial, SIGNAL(readyRead()), this, SLOT(readyRead()));
        ui->pushButtonRecord->setText(RECORD_BTN_RECORD_IDLE);
        statusBarMsg("");
        isRecording = false;
    }
}

void MainWindow::on_pushButtonPlay_clicked()
{
    if(!isPlaying)
    {
        if(load_data.isEmpty())
            openDataFile();
        if(load_data.isEmpty())
            return;

        statusBarMsg(PLAY_STATUS_ACTIVE);
        ui->pushButtonPlay->setText(PLAY_BTN_ACTIVE);
        timer->start();
        isPlaying = true;
    }
    else
    {
        ui->pushButtonPlay->setText(PLAY_BTN_IDLE);
        statusBarMsg(PLAY_STATUS_STOP_ACTIVE);
        timer->stop();
        isPlaying = false;
    }
}

void MainWindow::on_action_Open_data_triggered()
{
    openDataFile();
}

void MainWindow::on_action_Save_data_triggered()
{
    if(!isRecording)
    {
        if(recv_data.isEmpty())
            statusBarMsg("No data recorded...");
        else
            saveDataFile();
    }
    else
        statusBarMsg("Can't save while recording...");
}

void MainWindow::on_action_Exit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionClear_play_data_triggered()
{
    load_data.clear();
}

void MainWindow::on_actionClear_record_data_triggered()
{
    recv_data.clear();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox::about(this, "Author", "<p><center>Krzysztof Kruk</center></p>"
                                       "<p><strong>Website: </strong> https://github.com/rCorvidae/</p>");
}

void MainWindow::on_actionSet_play_interval_triggered()
{
    timeIntervalDialog->show();
    timer->setInterval(timeIntervalDialog->getTimeInterval());
}
