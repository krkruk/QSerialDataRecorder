#include "timeintervaldialog.h"
#include "ui_timeintervaldialog.h"

TimeIntervalDialog::TimeIntervalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeIntervalDialog)
{
    ui->setupUi(this);

    QSettings settings("Krzysztof Kruk", "AutonomousDroneCrashSystem");
    settings.beginGroup("TIME_INTERVAL");
    timeInterval = settings.value("SEND_TIME_INTERVAL", 1).toInt();
    ui->spinBoxTimeInterval->setValue(timeInterval);
    settings.endGroup();
}

TimeIntervalDialog::~TimeIntervalDialog()
{
    QSettings settings("Krzysztof Kruk", "AutonomousDroneCrashSystem");
    settings.beginGroup("TIME_INTERVAL");
    settings.setValue("SEND_TIME_INTERVAL", timeInterval);
    settings.endGroup();
    delete ui;
}

void TimeIntervalDialog::on_buttonBox_accepted()
{
    timeInterval = ui->spinBoxTimeInterval->value();
}

void TimeIntervalDialog::on_buttonBox_rejected()
{
    //do nothing
}

int TimeIntervalDialog::getTimeInterval() const
{
    return timeInterval;
}

void TimeIntervalDialog::setTimeInterval(int value)
{
    timeInterval = value;
}
