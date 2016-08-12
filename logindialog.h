#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QPaintEvent>
#include "windows.h"
#include "userdatebase.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    bool status;
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

signals:
    void login(QString);

private slots:
    void on_loginInButton_clicked();

    void on_registerInButton_clicked();

    void on_loginExitButton_clicked();

private:
    Ui::LoginDialog *ui;
    QPoint windowPos,mousePos,dPos;
};

#endif // LOGINDIALOG_H
