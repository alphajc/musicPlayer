#ifndef LYRICLABEL_H
#define LYRICLABEL_H

#define TEXT_SPEED 500

#include "rolllabel.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>

class LyricLabel : public RollLabel
{
    Q_OBJECT
public:
    LyricLabel(QWidget *parent = 0);
    void setLyric(const QString &context);
    void start_lrc_mask(qint64 intervaltime);
    void stop_lrc_mask();
    void set_size(int size);

protected:
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void hover();

private slots:
    void time_out();

private:
    QLinearGradient linearGradient;
    QLinearGradient maskLinearGradient;
    QFont lrcFont;
    QTimer *lrcTimer;
    qreal lrcMaskWidth, lrcMaskWidthInterval;
    QPoint offSet;
};

#endif // LYRICLABEL_H
