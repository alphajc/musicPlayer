#include "logindialog.h"
#include "ui_logindialog.h"
#include "QMessageBox.h"
#include "QTextCodec.h"
#include "regdialog.h"
#include "playerwidget.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
     ui->setupUi(this);
     ui->pwdlineEdit->setEchoMode(QLineEdit::Password);

     setAttribute(Qt::WA_TranslucentBackground, true);
     setWindowFlags(Qt::Window | Qt::FramelessWindowHint
                        | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
                        | Qt::WindowMaximizeButtonHint);

     setAutoFillBackground(true);
     QPixmap pixmap(":/image/background/login.jpg");
     resize(pixmap.size());
     QPalette palette;
     palette.setBrush(QPalette::Background,QBrush(pixmap));
     setPalette(palette);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        windowPos = pos();
        mousePos = event->globalPos();
        dPos = mousePos - windowPos;
        event->accept();
    }
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event){
    //if(event->button() == Qt::LeftButton){
        move(event->globalPos() - dPos);
        event->accept();
        //}
}

void LoginDialog::paintEvent(QPaintEvent *)
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


void LoginDialog::on_loginInButton_clicked()
{
    userDatebase *userdb = new userDatebase();
    QString name = this->ui->loginlineEdit->text();
    QString psd = this->ui->pwdlineEdit->text();
    int index = userdb->selectUserPsd(name,psd);
    if(index==0)
    {
        status = true;
        QString nickname = userdb->selectNickname(name);
        qDebug() <<nickname<<endl;
        emit login(nickname);
        this->ui->loginlineEdit->clear();
        this->ui->pwdlineEdit->clear();
        this->hide();
        qDebug() <<"登录成功"<<endl;
    }
    else if(index == -1)
    {
        qDebug() <<"密码错误！"<<endl;
        this->ui->errorPwd->setText("密码错误！");
    }
    else if(index == 1)
    {
        qDebug() <<"用户不存在！"<<endl;
        this->ui->errorLabel->setText("用户不存在！");
    }
}

void LoginDialog::on_registerInButton_clicked()
{
    RegDialog *reg = new RegDialog;
    this->close();
    reg->show();
}


void LoginDialog::on_loginExitButton_clicked()
{ 
    this->close();
}
