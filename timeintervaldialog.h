#ifndef TIMEINTERVALDIALOG_H
#define TIMEINTERVALDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class TimeIntervalDialog;
}

class TimeIntervalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeIntervalDialog(QWidget *parent = 0);
    ~TimeIntervalDialog();

    int getTimeInterval() const;
    void setTimeInterval(int value);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::TimeIntervalDialog *ui;
    int timeInterval {1};
};

#endif // TIMEINTERVALDIALOG_H
