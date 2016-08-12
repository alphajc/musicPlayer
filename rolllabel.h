#ifndef ROLLLABEL_H
#define ROLLLABEL_H

#include <QLabel>
#include <QTimer>
#include <QDebug>

class RollLabel : public QLabel
{
    Q_OBJECT
public:
   explicit  RollLabel(QWidget *parent=0);
    ~RollLabel();
    //设置文本:
    //如果ms为0则文字不滚动(默认不滚动);
    //如果调用setText()文字将不滚动(与使用QLabel相同);
    void rollText(const QString &context=0, const int &ms=0);

    //停止滚动
    void rollStop();

private slots:

    void roll_begin();

private:

    void label_init(int ms = 0);

    QString mContext;
    QTimer *mTimer;

    int mMs;
    int mPos;
};

#endif // ROLLLABEL_H
