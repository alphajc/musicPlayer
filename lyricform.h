#ifndef LYRICFORM_H
#define LYRICFORM_H

#include <QWidget>
#include <QDebug>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QDesktopWidget>

#include "lyriclabel.h"

namespace Ui {
class LyricForm;
}

class LyricForm : public QWidget
{
    Q_OBJECT

public:
    explicit LyricForm(QWidget *parent = 0);
    LyricLabel *lyric1, *lyric2;
    ~LyricForm();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

private slots:
    void on_exitButton_clicked();
    void show_frame();

private:
    Ui::LyricForm *ui;
    QPoint pos;
};

#endif // LYRICFORM_H
