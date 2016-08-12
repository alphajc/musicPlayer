#include "regdialog.h"
#include "ui_regdialog.h"
#include "QMessageBox.h"
#include "QTextCodec.h"

RegDialog::RegDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegDialog)
{
    ui->setupUi(this);
    ui->newpwdlineEdit->setEchoMode(QLineEdit::Password);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint
                       | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
                       | Qt::WindowMaximizeButtonHint);

    setAutoFillBackground(true);
    QPixmap pixmap(":/image/background/register.jpg");
    resize(pixmap.size());
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    setPalette(palette);
}

RegDialog::~RegDialog()
{
    delete ui;
}

void RegDialog::paintEvent(QPaintEvent *)
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

void RegDialog::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        windowPos = pos();
        mousePos = event->globalPos();
        dPos = mousePos - windowPos;
        event->accept();
    }
}

void RegDialog::mouseMoveEvent(QMouseEvent *event){
    //if(event->button() == Qt::LeftButton){
        move(event->globalPos() - dPos);
        event->accept();
    //}
}


void RegDialog::on_regokButton_clicked()
{
    if(this->ui->newpwdlineEdit->text().trimmed() == this->ui->renewpwdlineEdit->text().trimmed())
     {

         QString name = this->ui->newnamelineEdit->text();
         QString psd = this->ui->newpwdlineEdit->text();
         QString nickname = this->ui->namelineEdit->text();
         userDatebase *userdb = new userDatebase();
         int index = userdb->insertUser(name,psd,nickname);
         if(index==0){
            this->close();
         }
         else if(index==-1)
         {
             this->ui->userLabel->setText("用户名已注册！");
             return;
         }
     }
     else
     {
         QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
         QMessageBox::warning(this,tr("警告"),tr("密码不一致，请重新输入!"),QMessageBox::Yes);
         this->ui->newnamelineEdit->clear();
         this->ui->newpwdlineEdit->clear();
         this->ui->renewpwdlineEdit->clear();
         this->ui->newnamelineEdit->setFocus();
     }
}

void RegDialog::on_regnotokButton_clicked()
{
    this->close();
}


void RegDialog::on_regExitButton_clicked()
{
    this->close();
}
