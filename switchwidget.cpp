#include "switchwidget.h"
#include <QtGui>
CSwitchWidget::CSwitchWidget(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::Widget | Qt::SubWindow);
    setAttribute(Qt::WA_TranslucentBackground);

    checked = false;
    mPress = false;
    hover = false;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()),Qt::DirectConnection);
    //按钮尺寸自己调
    setFixedSize(40,20);
    framed = 8;//阴影框与按钮框的间距
    resizePainter();

   //字体设置和文字设置 自己DIY
//    QFont font1 = this->font();
//    font1.setBold(true);
//    font1.setPointSize(10);
//    setFont(font1);
//    setText();
    setspeed(50);//设置滑动速度

}

CSwitchWidget::~CSwitchWidget()
{
}
void CSwitchWidget::resizeEvent(QResizeEvent *ev)
{
    QWidget::resizeEvent(ev);
    resizePainter();

}
void CSwitchWidget::resizePainter()
{

    h = this->size().height() - framed;//开关矩形高度
    pieR = h/2;//开关两边半圆半径
    w = this->size().width() - framed - pieR*2;//开关矩形宽度
    if(checked)
        rd = 0;
    else
        rd = w;
    //外框矩形
    wRect = QRect(0,0,this->size().width(),this->size().height());
    round = wRect.height()/2;
    //开关矩形
    rect = QRect(framed/2 + pieR,framed/2,w,h);
    //开关半圆弧
    pieON = QPoint(rect.x() + w,rect.y() + pieR);
    pieOFF =  QPoint(rect.x(),rect.y() + pieR);

    //开启的渐变颜色  自己DIY
    linearON = QLinearGradient(rect.topLeft(),rect.bottomLeft());
    linearON.setColorAt(0,QColor(159, 55, 2));
    linearON.setColorAt(0.2,QColor(223, 89, 0));
    linearON.setColorAt(0.5,QColor(235, 97, 0));
    linearON.setColorAt(0.8,QColor(233, 89, 0));
    linearON.setColorAt(1,QColor(159, 55, 2));

    radialON = QRadialGradient(pieON,pieR);
    radialON.setColorAt(0,QColor(235, 97, 0));
    radialON.setColorAt(0.55,QColor(233, 89, 0));
    radialON.setColorAt(1,QColor(159, 55, 2));

    //关闭按钮的渐变颜色
    linearOFF = QLinearGradient(rect.topLeft(),rect.bottomLeft());
    linearOFF.setColorAt(0,QColor(56, 56, 56));
    linearOFF.setColorAt(0.2,QColor(85, 85, 85));
    linearOFF.setColorAt(0.5,QColor(93, 93, 93));
    linearOFF.setColorAt(0.8,QColor(85, 85, 85));
    linearOFF.setColorAt(1,QColor(56, 56, 56));

    radialOFF = QRadialGradient(pieOFF,pieR);
    radialOFF.setColorAt(0,QColor(93, 93, 93));
    radialOFF.setColorAt(0.6,QColor(85, 85, 85));
    radialOFF.setColorAt(1,QColor(56, 56, 56));

}
void CSwitchWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setWindow(wRect);


    //先画一个外框方框
    painter.setPen(Qt::NoPen);
 //   painter.setBrush(QColor(255,255,255,0));
  //  painter.drawRoundedRect(wRect,wRect.height()/2,wRect.height()/2);

    //画开启框左右圆弧
    painter.setBrush(radialON);
    painter.drawEllipse(pieON,this->pieR,this->pieR);
    //画开启框
    painter.setBrush(linearON);
    painter.drawRect(rect);


    //画开启文字
//    painter.setPen(QPen(Qt::white,1,Qt::SolidLine,
//                        Qt::RoundCap, Qt::RoundJoin));
//    painter.setFont(this->font());
//    painter.drawText(QRect(rect.x() + pieR,rect.y(),rect.width() - pieR ,rect.height()),
//                    Qt::AlignCenter,textON());
//    painter.setPen(Qt::NoPen);
    //画关闭圆弧
    painter.setBrush(radialOFF);
    painter.drawEllipse(pieOFF,pieR,pieR);
    //画关闭框
    QRect ONRect(rect.x(),rect.y(),rd,rect.height());
    painter.setBrush(linearOFF);
    painter.drawRect(ONRect);

    //画关闭文字
//    painter.setPen(QPen(Qt::white,1,Qt::SolidLine,
//                       Qt::RoundCap, Qt::RoundJoin));
//    painter.setFont(this->font());
//    painter.drawText(QRect(rect.x(),rect.y(),rd - pieR,rect.height()),
//                     Qt::AlignCenter,textOFF());

//    painter.setPen(Qt::NoPen);
    if(hover)
    {
        painter.setBrush(QColor(0, 0, 0, 50));
        painter.drawRoundedRect(wRect,round,round);
    }
    //画开关圆头
    QPointF pieONr(pieOFF.x() + rd,pieOFF.y());
    //圆头的渐变颜色
    QRadialGradient radial(pieONr,wRect.height() / 2 - 1);
    radial.setColorAt(0,QColor(124, 238, 255));
    radial.setColorAt(0.8,QColor(124, 238, 255));
    radial.setColorAt(1,QColor(0, 170, 255));
    painter.setBrush(radial);

    painter.drawEllipse(pieONr,wRect.height() / 2 - 1,wRect.height() / 2  - 1);
    ev->accept();

}


void CSwitchWidget::timeout()
{
    speed = (speed == 0) ? 1 : speed;
    if(!checked)
    {
        rd+=w/speed;
        if(rd >= w)
        {
            timer->stop();
            mPress = false;
            rd = w;
        }
    }
    else
    {
        rd-=w/speed;
        if(rd <= 0)
        {
            timer->stop();
            mPress = false;
            rd = 0;
        }
    }
    this->update();
}

void CSwitchWidget::mousePressEvent(QMouseEvent *ev)
{
    ev->accept();
//    if(!mPress && ev->button() == Qt::LeftButton && this->rect.contains(ev->pos()))
//    {
//        mPress = true;
//        rd = 0;
//        timer->start(2);
//    }


}

void CSwitchWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(!mPress && ev->button() == Qt::LeftButton && this->wRect.contains(ev->pos()))
    {
        mPress = true;
        checked = !checked;
        emit clicked();
        emit clicked(checked);
        qDebug()<<checked;
        timer->start(1);

    }
}

void CSwitchWidget::enterEvent(QEvent *)
{
    hover = true;
    this->update();
}

void CSwitchWidget::leaveEvent(QEvent *)
{
    hover = false;
    this->update();
}


void CSwitchWidget::setChecked(const bool checked)
{
    this->checked = checked;
    timer->start(1);
}

