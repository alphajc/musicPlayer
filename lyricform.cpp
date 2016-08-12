#include "lyricform.h"
#include "ui_lyricform.h"

LyricForm::LyricForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LyricForm)
{
    ui->setupUi(this);

    //无边界的窗口,总在最上
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

//    QPixmap pixmap(":/image/background/lrcbackground.png");
//    QPalette palette;
//    palette.setBrush(backgroundRole(),QBrush(pixmap));
//    setPalette(palette);
//    resize( pixmap.size() );
//    setMask(pixmap.mask());
//    setAutoFillBackground(true);

    setFixedSize(1000, 140);

    move(QApplication::desktop()->availableGeometry().width()/2 - width()/2,
         QApplication::desktop()->availableGeometry().height() - height());

    lyric1 = new LyricLabel(this);
    lyric2 = new LyricLabel(this);

    lyric1->setGeometry(0, 40, 1000, 45);
    lyric2->setGeometry(0, 95, 1000, 45);

    lyric2->setAlignment(Qt::AlignRight);

//    connect(lyric1, SIGNAL(hover()), this, SLOT(show_frame()));
//    connect(lyric2, SIGNAL(hover()), this, SLOT(show_frame()));
}

LyricForm::~LyricForm()
{
    delete ui;
}

void LyricForm::on_exitButton_clicked()
{
    close();
}

void LyricForm::show_frame()
{
    qDebug() << "触发" << endl;
    setWindowOpacity(0.8);
}

void LyricForm::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        pos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void LyricForm::mouseMoveEvent(QMouseEvent *event)
{
        setCursor(Qt::PointingHandCursor);
        qDebug() << pos.x() << ',' << pos.y() << endl;
        qDebug() << event->globalPos().x() << ',' << event->globalPos().y() << endl;
        move(event->globalPos() - pos);
        event->accept();
}

void LyricForm::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
//    painter.setBrush(Qt::blue);
//    painter.drawRect(0, 0, width(), height());
    painter.drawPixmap(0, 0, 1000, 140, QPixmap(":/image/background/lrcbackground.png"));
}

