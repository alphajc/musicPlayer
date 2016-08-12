#include "lyriclabel.h"

LyricLabel::LyricLabel(QWidget *parent):RollLabel(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    //设置定时器
    lrcTimer = new QTimer(this);
    connect(lrcTimer, SIGNAL(timeout()), this, SLOT(time_out()));
}

void LyricLabel::setLyric(const QString &context)
{
    if(width() > fontMetrics().width(context))
    {
        rollStop();
        setText(context);
    }
    else//跑马灯效果
    {
        rollText(context, TEXT_SPEED);
    }
    lrcTimer->stop();
    //歌词的线性渐变填充
        linearGradient.setStart(0, 0);//填充的起点坐标
        linearGradient.setFinalStop(fontMetrics().width(context), 0);//填充的终点坐标
        maskLinearGradient.setStart(0, 0);
        maskLinearGradient.setFinalStop(fontMetrics().width(context), 0);
    //第一个参数终点坐标，相对于我们上面的区域而言，按照比例进行计算
    linearGradient.setColorAt(0.1, QColor(14, 179, 255));
    linearGradient.setColorAt(0.5, QColor(114, 232, 255));
    linearGradient.setColorAt(0.9, QColor(14, 179, 255));
    linearGradient.setColorAt(0, QColor(14, 179, 230));

    // 遮罩的线性渐变填充
    maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
    maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
    maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));
    maskLinearGradient.setColorAt(0, QColor(222, 54, 36));
    // 设置字体
    lrcFont.setFamily(QString::fromLocal8Bit("微软雅黑"));
    lrcFont.setBold(true);
    lrcFont.setPointSize(25);

    // 设置定时器
    lrcMaskWidth = 0;
    lrcMaskWidthInterval = 0;
}

// 开启遮罩，需要指定当前歌词开始与结束之间的时间间隔
void LyricLabel::start_lrc_mask(qint64 intervaltime)
{
    // 这里设置每隔30毫秒更新一次遮罩的宽度，因为如果更新太频繁
    // 会增加CPU占用率，而如果时间间隔太大，则动画效果就不流畅了
    qreal count = intervaltime / 30;
    // 获取遮罩每次需要增加的宽度，这里的800是部件的固定宽度
    lrcMaskWidthInterval = fontMetrics().width(text()) / count;
    lrcMaskWidth = 0;
    lrcTimer->start(30);
}

void LyricLabel::stop_lrc_mask()
{
    lrcTimer->stop();
    lrcMaskWidth = 0;
    update();
}

void LyricLabel::paintEvent(QPaintEvent *)
{
//    qDebug() << text();
    QPainter painter(this);
    painter.setFont(lrcFont);

    if(text().size() < 2){
        setText("Music...");
    }
    QString tmp = text();
    tmp.toUtf8();
    // 先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(90, 105, 115, 200));
    if(alignment() == Qt::AlignRight)
        painter.drawText(1000-fontMetrics().width(text())+1, 1, fontMetrics().width(text()), 45, Qt::AlignLeft, tmp);
    else
        painter.drawText(1, 1, fontMetrics().width(text()), 45, Qt::AlignLeft, tmp);

    // 再在上面绘制文字
    painter.setPen(QPen(linearGradient, 0));
    if(alignment() == Qt::AlignRight)
        painter.drawText(1000-fontMetrics().width(text()), 0, fontMetrics().width(text()), 45, Qt::AlignLeft, tmp);
    else
        painter.drawText(0, 0, fontMetrics().width(text()), 45, Qt::AlignLeft, tmp);

     //设置歌词遮罩
    painter.setPen(QPen(maskLinearGradient, 0));
    if(alignment() == Qt::AlignRight)
        painter.drawText(1000-fontMetrics().width(text()), 0, lrcMaskWidth, 45, Qt::AlignLeft, tmp);
    else
        painter.drawText(0, 0, lrcMaskWidth, 45, Qt::AlignLeft, tmp);

}

// 左击操作
 void LyricLabel::mousePressEvent(QMouseEvent *event)
 {
    if (event->button() == Qt::LeftButton)
        offSet = event->globalPos() - frameGeometry().topLeft();
 }


 void LyricLabel::mouseMoveEvent(QMouseEvent *event)
 {
    //移动鼠标到歌词上时，会显示手型
    //event->buttons()返回鼠标点击的类型，分为左击，中击，右击
    //这里用与操作表示是左击
    if (event->buttons() & Qt::LeftButton) {
        setCursor(Qt::PointingHandCursor);
        //实现移动操作
        move(event->globalPos() - offSet);
    }
 }

//右击事件
void LyricLabel::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction("隐藏", this->parent(), SLOT(hide()));
    menu.exec(event->globalPos());//globalPos()为当前鼠标的位置坐标
}


void LyricLabel::time_out()
{

    //每隔一段固定的时间笼罩的长度就增加一点
    lrcMaskWidth += lrcMaskWidthInterval;
    update();//更新widget，但是并不立即重绘，而是安排一个Paint事件，当返回主循环时由系统来重绘
}


void LyricLabel::set_size(int size)
{
    lrcFont.setPointSize(size);
}
