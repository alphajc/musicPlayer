#ifndef TDIALOG_H
#define TDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QString>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <windows.h>


namespace Ui {
class TDialog;
}

class TDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TDialog(QWidget *parent = 0);
    ~TDialog();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

protected:
    void paintEvent(QPaintEvent *);

signals:
    void time_out();

private:
    Ui::TDialog *ui;
    int hourFinal;
    int minuteFinal;
    int msFinal;
    int minute;
    QString hour_final;
    QString minute_final;
    QString ms_final;
    QAction *quitAction;
    QMenu *trayContextMenu;
    QAction *restoreAction;

    QPoint windowPos,mousePos,dPos;
    QTimer *emitTimer, *finalTimer;

private slots:
    void timerUpdate();
    void on_exutTimeButton_clicked();
    void on_timeStartButton_clicked();
    void on_closeTimeButton_clicked();
    void timeOutClock();
    void finalTimeClock();
};

#endif // TDIALOG_H
