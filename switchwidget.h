#ifndef CSwitchWidget_H
#define CSwitchWidget_H

#include <QWidget>

namespace Ui {
class CSwitchWidget;
}

class CSwitchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CSwitchWidget(QWidget *parent = 0);
    ~CSwitchWidget();
    void setChecked(const bool checked);
    bool isChecked()const{return checked;}
//    inline void setText(const QString &textON = "ON",const QString &textOFF = "OFF")
//    {
//        text_on = textON;
//        text_off = textOFF;
//    }
//    inline QString textON()const { return this->text_on;}
//    inline QString textOFF()const{return this->text_off;}

    //这是滑动速度 1表示瞬滑
    inline void setspeed(const quint16 speed){this->speed = speed;}

signals:
    void clicked();
    void clicked(bool checked);
protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void resizePainter();
    void resizeEvent(QResizeEvent *ev);

private:
    QTimer *timer;
    qreal rd,round,w,h,pieR;
    QLinearGradient linearON,linearOFF;
    QRadialGradient radialON,radialOFF;
    QRectF rect;
    QRect wRect;
    QPointF pieON,pieOFF;
    qreal  framed;
    bool mPress;
    bool hover;
    bool checked;
    QString text_on,text_off;
    quint16 speed;

private slots:
    void timeout();

};

#endif // CSwitchWidget_H
