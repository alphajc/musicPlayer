#include "tdialog.h"
#include "ui_tdialog.h"
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <qDebug>

TDialog::TDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint
                       | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
                       | Qt::WindowMaximizeButtonHint);

    setWindowTitle("音乐定时设置");
    ui->timeBox->setRange(0,100);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(100);
}

TDialog::~TDialog()
{
    delete ui;
}

void TDialog::mouseMoveEvent(QMouseEvent *event){
    //if(event->button() == Qt::LeftButton){
        move(event->globalPos() - dPos);
        event->accept();
    //}
}

void TDialog::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        windowPos = pos();
        mousePos = event->globalPos();
        dPos = mousePos - windowPos;
        event->accept();
    }
}

void TDialog::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QColor color(150, 205, 205, 255);
    p.setBrush(QBrush(color));
    p.setPen(color);
    p.drawRoundedRect(0, 0, width() - 1, height()/2, 5, 5);
    color.setRed(245);
    color.setGreen(245);
    color.setBlue(245);
    p.setPen(color);
    p.setBrush(QBrush(color));
    p.drawRect(0, 35, width() - 1, height()/2);
    p.drawRoundedRect(0, height()/2, width() - 1, height()/2 - 1, 5, 5);
}


void TDialog::timerUpdate()
{
    QDateTime timedate = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = timedate.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
    ui->nowTimeLabel->setText(str);
    update();
}


void TDialog::on_exutTimeButton_clicked()
{
    close();
}

void TDialog::on_timeStartButton_clicked()
{
    minute = ui->timeBox->value();
    emitTimer = new QTimer;
    emitTimer->start(minute*60*1000);
    connect(emitTimer,SIGNAL(timeout()),this,SLOT(timeOutClock()));

    finalTimer = new QTimer;
    finalTimer->start(1000);
    connect(finalTimer,SIGNAL(timeout()),this,SLOT(finalTimeClock()));

    hourFinal = minute/60;
    minuteFinal = minute%60;
    msFinal = 0;
    QString hour_final = QString::number(hourFinal);
    QString minute_final = QString::number(minuteFinal);
    QString ms_final = QString::number(msFinal);
    ui->theFinalLabel->setText(hour_final + ":" + minute_final + ":" + ms_final);

    this->hide();
}

void TDialog::on_closeTimeButton_clicked()
{
    delete emitTimer;
    delete finalTimer;
    ui->theFinalLabel->setText("");
    ui->timeBox->setValue(0);
}

void TDialog::timeOutClock()
{
    emit time_out();
}

void TDialog::finalTimeClock()
{
     if(msFinal == 0)
     {
         if(minuteFinal!=0)
         {
            minuteFinal = minuteFinal - 1;
            msFinal = 59;
         }
     }
     if(minuteFinal == 0)
     {
         if(hourFinal!=0)
         {
            hourFinal = hourFinal - 1;
            minuteFinal = 59;
         }
     }
     if(msFinal>0)
     {
          msFinal = msFinal - 1;
     }

     hour_final = QString::number(hourFinal);
     minute_final = QString::number(minuteFinal);
     ms_final = QString::number(msFinal);
     ui->theFinalLabel->setText(hour_final + ":" + minute_final + ":" + ms_final);

}

