#ifndef REGDIALOG_H
#define REGDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include "windows.h"
#include "userdatebase.h"

namespace Ui {
class RegDialog;
}

class RegDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegDialog(QWidget *parent = 0);
    ~RegDialog();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::RegDialog *ui;
    QPoint windowPos,mousePos,dPos;

private slots:
    void on_regokButton_clicked();
    void on_regnotokButton_clicked();
    void on_regExitButton_clicked();
};

#endif // REGDIALOG_H
