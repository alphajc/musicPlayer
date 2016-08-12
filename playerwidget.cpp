#include "playerwidget.h"
#include "ui_playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerWidget)
{
    ui->setupUi(this);

    init_window();
    init_song_info();
    init_player();
    init_play_list();
    init_inner_lyric();
    init_outter_lyric();

    setConnection();
}

void PlayerWidget::init_window()
{
    Sleep(1000);
    setWindowIcon(QIcon(":/image/icon/music_24px_580162_easyicon.net.png"));
    setWindowTitle("clld player");
    /*背景图片*/
    setAutoFillBackground(true);
    QPixmap pixmap(":/image/background/background2.jpg");
    resize(pixmap.size());
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    setPalette(palette);

    /*设置窗口透明度和框架*/
    setWindowOpacity(1);
    setWindowFlags(Qt::FramelessWindowHint);

    /*设置菜单*/
    QMenu *menu = new QMenu;
    openFile = menu->addAction(QIcon(":/image/icon/pin_local_24px_1075811_easyicon.net.png"),"添加本地歌曲");
    timeMenu = menu->addAction(QIcon(":/image/icon/setting_16px_1143813_easyicon.net.png"), "设置...");
    registerMenu = menu->addAction(QIcon(":/image/icon/edit_21.541353383459px_1153738_easyicon.net.png"), "注册...");
    loginMenu = menu->addAction(QIcon(":/image/icon/login_22.179310344828px_1170492_easyicon.net.png"), "登录");
    exitLoginMenu = menu->addAction(QIcon(":/image/icon/logout_24px_1137897_easyicon.net.png"), "登出");
    ui->moreButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->moreButton->setPopupMode(QToolButton::InstantPopup);
    ui->moreButton->setMenu(menu);

    /*添加自定义开关：歌词外挂、背景变换*/
    lyricSwitch = new CSwitchWidget(ui->bottomFrame);
    backgroundSwitch = new CSwitchWidget(ui->bottomFrame);
    lyricSwitch->setGeometry(420, 10, 50, 20);
    backgroundSwitch->setGeometry(150, 10, 50, 20);

    //初始化参数
    logindialog->status = false;//登录状态
    isLeftButton = false;//消除bug

    //设置昵称对齐方式
    ui->nickName->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    //初始化定时器
    timeClock = new TDialog;
    //创建系统托盘
    createSystemTrayIcon();
}

void PlayerWidget::init_play_list()
{

    localGroup = new QGroupBox(ui->listCategory);
    localLayout = new QVBoxLayout(localGroup);
    localList = new PlayList(0);//本地歌曲列表
    localLayout->addWidget(localList);
    collectGroup = new QGroupBox(ui->listCategory);
    collectLayout = new QVBoxLayout(collectGroup);
    collectList = new PlayList(1);//收藏歌曲列表
    collectLayout->addWidget(collectList);

    /*设置列表*/
    ui->listCategory->removeItem(0);
    ui->listCategory->removeItem(0);
    ui->listCategory->addItem(localGroup, QIcon(":/image/icon/song_21.718631178707px_1146829_easyicon.net.png"), "本地列表");
    ui->listCategory->addItem(collectGroup, QIcon(":/image/icon/user_collection_24px_1174210_easyicon.net.png"), "我的收藏");

    QStringList fileNameList = dbUtil.selectMusicPathList();
    localList->setList(fileNameList);
    QStringList collection_list = dbUtil.selectLoveMusicPathList();
    collectList->setList(collection_list);
    //存储歌词
    for(int j=0;j<fileNameList.size();j++){
        QString liststr = fileNameList[j].split('.').front();//此处有bug。。。需改为最后一个“.”
        liststr.append(".lrc");
        fileNameandUrlList.append(liststr);
    }
    //初始化播放列表
    if(!fileNameList.isEmpty())
    {
        foreach (const QString&fileName , fileNameList)
        {
           QMediaContent media = QMediaContent(QUrl::fromLocalFile(fileName));
           this->localMediaPlayList->addMedia(media);
        }
    }
}

void PlayerWidget::init_inner_lyric()
{
    //歌词label初始化
    for(int i=0; i < 15; i++){
        lrclabel[i] = new RollLabel(ui->lrcFrame);
        lrclabel[i]->setGeometry(0,i*35,620,35);
        lrclabel[i]->setAlignment(Qt::AlignCenter);
    }

    lrclabel[7]->setStyleSheet("font-size:20pt; color:yellow");
}
//初始化播放器
void PlayerWidget::init_player()
{
    this->mediaPlayer = new QMediaPlayer(this);
    this->localMediaPlayList = new QMediaPlaylist(this);
    localMediaPlayList->setPlaybackMode(QMediaPlaylist::Loop);
    this->mediaPlayer->setPlaylist(this->localMediaPlayList);
    this->mediaPlayer->setVolume(50);
    ui->volumeSlider->setValue(50);
    this->progressTimer = new QTimer(this);
    this->progressTimer->setInterval(1000);//1秒
    this->progressTimer->start();
}

void PlayerWidget::init_song_info()
{
    specialLabel = new RollLabel(ui->leftFrame);
    artistLabel = new RollLabel(ui->leftFrame);
    titleLabel = new RollLabel(ui->leftFrame);

    specialLabel->setGeometry(130, 20, 90, 16);
    artistLabel->setGeometry(130, 60, 90, 16);
    titleLabel->setGeometry(130, 100, 90, 16);

    specialLabel->setText("专辑名");
    artistLabel->setText("歌手名");
    titleLabel->setText("歌曲名");
}

void PlayerWidget::init_outter_lyric()
{
    lyricForm = new LyricForm;
    lyricForm->lyric1->setLyric("懂你的");
    lyricForm->lyric2->setLyric("只有我");
}

void PlayerWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        offset = event->globalPos() - frameGeometry().topLeft();
        isLeftButton = true;
        event->accept();
    }
}

void PlayerWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(isLeftButton)
    {
        move(event->globalPos() - offset);
        event->accept();
    }
}
//解决窗口跳动的bug
void PlayerWidget::mouseReleaseEvent(QMouseEvent *)
{
    isLeftButton = false;
}

void PlayerWidget::setConnection()
{
    /*退出*/
    connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(on_exitButton_clicked()));
    /*最小化*/
    connect(ui->minimizeButton, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));
    /*打开本地视频*/
    connect(openFile,SIGNAL(triggered(bool)),this,SLOT(handleOpenLocalMedia()));

    connect(progressTimer,SIGNAL(timeout()),this,SLOT(playProgressUpdate()));

    connect(mediaPlayer,SIGNAL(metaDataChanged()),this,SLOT(metaDataUpdate()));
    //图片开关
    connect(backgroundSwitch,SIGNAL(clicked(bool)),this,SLOT(bgUpdate(bool)));
    //打开外挂歌词，KTV模式
    connect(lyricSwitch, SIGNAL(clicked(bool)), this, SLOT(openLyric(bool)));
    //读取歌词
    connect(mediaPlayer,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(readLrc(QMediaContent)));
    //双击播放
    connect(localList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(dcPlay(QModelIndex)));
    //清空列表
    connect(localList, SIGNAL(empty_list()), this, SLOT(empty_list()));
    //移除当前曲目
    connect(localList, SIGNAL(remove_current_music(int)), this, SLOT(remove_current_music(int)));
    //登录
    connect(loginMenu, SIGNAL(triggered(bool)), this, SLOT(handleLogin()));
    //登出
    connect(exitLoginMenu, SIGNAL(triggered(bool)), this, SLOT(exitLogin()));
    //注册
    connect(registerMenu, SIGNAL(triggered(bool)), this, SLOT(registerAccount()));
    //定时
    connect(timeMenu, SIGNAL(triggered(bool)), this ,SLOT(setTimer()));
    //收藏
    connect(localList, SIGNAL(collect(int)), this, SLOT(add_collection(int)));
    //播放收藏列表
    connect(collectList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(dcPlay(QModelIndex)));
    //清空列表
    connect(collectList, SIGNAL(empty_list()), this, SLOT(empty_list()));
    //移除当前曲目
    connect(collectList, SIGNAL(remove_current_music(int)), this, SLOT(remove_current_music(int)));
}

/*第一曲*/
void PlayerWidget::on_firstTrackButton_clicked()
{
    qDebug() << "on_firstTrackButton_clicked()" << endl;
    if(!mediaPlayer->playlist()->isEmpty())
    {
            mediaPlayer->playlist()->setCurrentIndex(0);
    }
}
/*上一曲*/
void PlayerWidget::on_rewindButton_clicked()
{
    qDebug() << "on_rewindButton_clicked()" << endl;
    if(!mediaPlayer->playlist()->isEmpty())
    {
        if(mediaPlayer->playlist()->currentIndex() == 0)
            on_lastTrackButton_clicked();
        else
            this->mediaPlayer->playlist()->previous();

    }
}
/*播放与暂停*/
void PlayerWidget::on_playPauseButton_clicked()
{
    qDebug() << "on_playPauseButton_clicked()" << endl;
    if(!mediaPlayer->playlist()->isEmpty())
    {
        if(this->mediaPlayer->state() == QMediaPlayer::PlayingState)
        {
            this->mediaPlayer->pause();
            ui->playPauseButton->setStyleSheet("background:url(:/image/icon/play_48px_1180266_easyicon.net.png);border-radius:24px");
        }else{
            this->mediaPlayer->setVolume(this->ui->volumeSlider->value());
            this->mediaPlayer->play();
            ui->playPauseButton->setStyleSheet("background:url(:/image/icon/pause_48px_1180265_easyicon.net.png);border-radius:24px");
        }
    }
}
/*下一曲*/
void PlayerWidget::on_forwardButton_clicked()
{
    qDebug() << "on_forwardButton_clicked()" << endl;
    if(!mediaPlayer->playlist()->isEmpty())
    {
        if(mediaPlayer->playlist()->currentIndex() == mediaPlayer->playlist()->mediaCount() - 1)
            on_firstTrackButton_clicked();
        else
            this->mediaPlayer->playlist()->next();
    }
}
/*最后一曲*/
void PlayerWidget::on_lastTrackButton_clicked()
{
    qDebug() << "on_lastTrackButton_clicked()" << endl;
    if(!mediaPlayer->playlist()->isEmpty())
    {
        mediaPlayer->playlist()->setCurrentIndex(mediaPlayer->playlist()->mediaCount() - 1);
    }
}
/*音量增加*/
void PlayerWidget::on_volumeUpButton_clicked()
{
    qDebug() << "on_volumeUpButton_clicked()" << endl;
    if(ui->volumeSlider->value() < 100)
        ui->volumeSlider->setValue(ui->volumeSlider->value() + 5);
}
/*音量减小*/
void PlayerWidget::on_volumeDownButton_clicked()
{
    qDebug() << "on_volumeDownButton_clicked()" << endl;
    if(ui->volumeSlider->value() > 0)
        ui->volumeSlider->setValue(ui->volumeSlider->value() - 5);

}
/*播放进度改变*/
void PlayerWidget::on_timeSlider_valueChanged(int position)
{
        float percent = (position*1.0)/100;
        long value = this->mediaPlayer->duration()*percent;
        this->mediaPlayer->setPosition(value);
}
/*音量改变*/
void PlayerWidget::on_volumeSlider_valueChanged(int position)
{
    qDebug() << position << endl;
    this->mediaPlayer->setVolume(position);
    if(mediaPlayer->volume() == 0)
    {
        ui->volumeDownButton->setStyleSheet("background:url(:/image/icon/audio_off_24px_1180261_easyicon.net.png);border-radius:12px");
    }
    else if(mediaPlayer->volume() == 99)
    {
        ui->volumeUpButton->setStyleSheet("background:url(:/image/icon/audio_24px_1180260_easyicon.net.png);border-radius:12px");
    }
    else
    {
        ui->volumeDownButton->setStyleSheet("background:url(:/image/icon/Volume_Down_24px_1178995_easyicon.net.png);border-radius:12px");
        ui->volumeUpButton->setStyleSheet("background:url(:/image/icon/Volume_Up_24px_1178996_easyicon.net.png);border-radius:12px");
    }
}
/*读取本地歌曲*/
void PlayerWidget::handleOpenLocalMedia()
{
    QStringList fileNameList;
    fileNameList = QFileDialog::getOpenFileNames(this,"select local files","%USERPROFILE%/Music","Mp3/Mp4/wma Files(*.mp3 *.wma *.m4a)");

    /*歌词路径存储*/
    foreach (QString file, fileNameList) {
        QFileInfo _file(file);
        QString liststr = _file.path() + _file.baseName() + ".lrc";

        fileNameandUrlList.append(liststr);
    }

    /*添加列表*/
    localList->setList(fileNameList);

    if(!fileNameList.isEmpty())
    {
        foreach (const QString&fileName , fileNameList)
        {
           QMediaContent media = QMediaContent(QUrl::fromLocalFile(fileName));
           this->localMediaPlayList->addMedia(media);
           dbUtil.insertMusicPath(fileName);
        }
    }

    return;
}

void PlayerWidget::playProgressUpdate() //进度条同步歌曲播放
{
    long pos = this->mediaPlayer->position();  //当前播放位置
    long duration = this->mediaPlayer->duration();  //当前播放总时间长度
    int value = 100*(1.0*pos)/duration;     //换算成进度条的
    ui->timeSlider->disconnect(SIGNAL(valueChanged(int)));
    this->ui->timeSlider->setValue(value);
    connect(ui->timeSlider, SIGNAL(valueChanged(int)), this, SLOT(on_timeSlider_valueChanged(int)));

    QTime time(0,0,0);
    QTime ff = time.addMSecs(pos);
    QTime tt = time.addMSecs(duration);

    float sec = pos*1.0/1000;
    QString str = QString::number(sec, 'f', 2);

    strTime = (ff.toString("mm:ss")+ "." + str.split(".").last()).split('.').front();

    updateLrc();

    this->ui->finishTime->setText(ff.toString("mm:ss"));
    this->ui->totalTime->setText(tt.toString("mm:ss"));

}

void PlayerWidget::metaDataUpdate()
{
    QString title,albumTitle,albumArtist;
    QImage coverArtImage;
    QPixmap coverPixmap;

    title = this->mediaPlayer->metaData("Title").toString();
    albumTitle = this->mediaPlayer->metaData("AlbumTitle").toString();
    albumArtist = this->mediaPlayer->metaData("Author").toString();
    if(albumArtist == "" || albumArtist == NULL)
        albumArtist = this->mediaPlayer->metaData("AlbumArtist").toString();
    coverArtImage = this->mediaPlayer->metaData("ThumbnailImage").value<QImage>();
    if(coverArtImage.isNull())
        coverArtImage = this->mediaPlayer->metaData("CoverArtImage").value<QImage>();
    if(coverArtImage.isNull())
    {
        coverPixmap = QPixmap(":/image/example.jpg");
    }
    else
    {
        coverPixmap.convertFromImage(coverArtImage);

    }
    if(albumArtist == "" || albumArtist == NULL)
        artistLabel->setText("未知艺术家");
    else
        AssestUtil::horse_race_lamp(artistLabel, albumArtist);

    if(title == "" || title == NULL)
        titleLabel->setText("未知歌曲名");
    else
        AssestUtil::horse_race_lamp(titleLabel, title);

    if(albumTitle == "" || albumTitle == NULL)
        specialLabel->setText("未知专辑");
    else
        AssestUtil::horse_race_lamp(specialLabel, albumTitle);

    this->ui->coverImage->setPixmap(coverPixmap.scaled(this->ui->coverImage->size()));

}

void PlayerWidget::bgUpdate(bool checked)
{
    if(checked)
    {
        /*图片变换*/
        pictureTimer = new QTimer(this);
        this->pictureTimer->start(3000);
        qsrand(time(NULL));
        connect(pictureTimer, SIGNAL(timeout()), this, SLOT(pictureUpdate()));
    }
    else
    {
        delete pictureTimer;
    }
}
//背景变换
void PlayerWidget::pictureUpdate()
{
    QString picName(":/image/background/background");
    count = qrand() % 4;
    //如此，循环的背景图片不可超过十张
    qDebug() << count << picName+ (count + '0') +".jpg";
    QPixmap pixmap(picName+ (count + '0') +".jpg");

    resize(pixmap.size());
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    setPalette(palette);
}
//歌词滚动
void PlayerWidget::updateLrc()
{
    QString timedelxx;
   for(int i=0;i<lrcLineList.size();i++)
   {
        QString timestr = lrcLineList[i].split(']').front().split('[').last();
        timedelxx = timestr.split('.').front();
        QString thelrc = lrcLineList[i].split(']').last();
        if(strTime.compare(timedelxx) == 0){
            //前7句更新
            for(int j = i - 1, row = 6; row >= 0; j--, row--)
            {
                if(j < 0)
                    lrclabel[row]->setText("");
                else
                   AssestUtil::horse_race_lamp(lrclabel[row],lrcLineList[j].split(']').last());
            }
            qDebug() <<timestr <<"thelrc:"<<thelrc<<endl;
            AssestUtil::horse_race_lamp(lrclabel[7], thelrc);

            //设置遮罩时间间隔
            qint64 interval;
            QTime current = QTime::fromString(timestr+"0","mm:ss.zzz");
            QTime next;
            if(i+1 < lrcLineList.size())
            {
                next = QTime::fromString(lrcLineList[i+1].split(']').front().split('[').last()+"0","mm:ss.zzz");
                interval = current.msecsTo(next);
            }
            else    interval = current.msecsTo(next.addMSecs(mediaPlayer->duration()));

             //桌面歌词的更新，及其遮罩
            if(i%2 == 0)
            {
                lyricForm->lyric1->setLyric(lrcLineList[i].split(']').last());
                if(i+1 < lrcLineList.size())
                    lyricForm->lyric2->setLyric(lrcLineList[i+1].split(']').last());
                lyricForm->lyric1->start_lrc_mask(interval);
            }
            else
            {
                lyricForm->lyric2->start_lrc_mask(interval);
            }

            //后7句更新
            for(int j = i + 1, row = 8; row < 15; ++row, ++j)
            {
                if(j >= lrcLineList.size())
                    lrclabel[row]->setText("");
                else
                    AssestUtil::horse_race_lamp(lrclabel[row],lrcLineList[j].split(']').last());
            }
            break;
        }
    }
   if(strTime.compare(timedelxx) > 0 && (!lrcLineList.empty()))
   {
       qDebug() << "executed";
       int size = lrcLineList.size();
       lyricForm->lyric1->setLyric(lrcLineList[size-2].split(']').last());
       lyricForm->lyric2->setLyric(lrcLineList[size-1].split(']').last());
       for(int i = 0; i < 15; ++i)
       {
           AssestUtil::horse_race_lamp(lrclabel[i],lrcLineList[size-1-i].split(']').last());
       }
   }
}
//读取歌词
void PlayerWidget::readLrc(QMediaContent)
{
    int index;
    if(!mediaPlayer->playlist()->isEmpty())
    {
        index = mediaPlayer->playlist()->currentIndex();
        if(index < 0)
            return;
        QFile file(fileNameandUrlList[index]);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
         {
            not_found_lyric();
         }
        QTextStream lrc(&file);
        QString linelrc;
        lrc.setCodec("utf-8");
        lrcLineList.clear();
        while(!lrc.atEnd()){
            linelrc = lrc.readLine();
            qDebug() << linelrc << endl;
            lrcLineList.append(linelrc);
        }

        file.close();
    }
}
//单曲循环
void PlayerWidget::on_singleButton_clicked()
{
    mediaPlayer->playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    ui->singleButton->setStyleSheet("background:rgba(0, 0, 0, 0) url(:/image/icon/selected_one_16px_1175852_easyicon.net.png) no-repeat");
    ui->randomButton->setStyleSheet("background:rgba(0, 0, 0, 0) url(:/image/icon/random_16px_1175848_easyicon.net.png) no-repeat");
    ui->orderButton->setStyleSheet("background:rgba(0, 0, 0, 0) url(:/image/icon/repeat_16px_1175852_easyicon.net.png) no-repeat");
}
//随机播放
void PlayerWidget::on_randomButton_clicked()
{
    mediaPlayer->playlist()->setPlaybackMode(QMediaPlaylist::Random);
    ui->singleButton->setStyleSheet("background:rgba(0, 0, 0, 0) url(:/image/icon/repeat_one_16px_1175852_easyicon.net.png) no-repeat");
    ui->randomButton->setStyleSheet("background:rgba(0, 0, 0, 0) url(:/image/icon/selected_radom_16px_1175852_easyicon.net.png) no-repeat");
    ui->orderButton->setStyleSheet("background:rgba(0, 0, 0, 0) url(:/image/icon/repeat_16px_1175852_easyicon.net.png) no-repeat");
}
//循环播放
void PlayerWidget::on_orderButton_clicked()
{
    mediaPlayer->playlist()->setPlaybackMode(QMediaPlaylist::Loop);
    ui->singleButton->setStyleSheet("background:rgba(0, 0, 0, 0) url(:/image/icon/repeat_one_16px_1175852_easyicon.net.png) no-repeat");
    ui->randomButton->setStyleSheet("background:rgba(0, 0, 0, 0) url(:/image/icon/random_16px_1175848_easyicon.net.png) no-repeat");
    ui->orderButton->setStyleSheet("background:rgba(0, 0, 0, 0) url(:/image/icon/selected_repeat_16px_1175852_easyicon.net.png) no-repeat");
}
//打开外挂歌词
void PlayerWidget::openLyric(bool checked)
{
    if(checked)
        lyricForm->show();
    else
        lyricForm->hide();
}

void PlayerWidget::dcPlay(QModelIndex index)
{
    if(ui->listCategory->currentIndex() == 0)
    {
        localMediaPlayList->clear();
        QStringList musicList = dbUtil.selectMusicPathList();
        if(!musicList.isEmpty())
        {
            foreach (const QString&fileName , musicList)
            {
               QMediaContent media = QMediaContent(QUrl::fromLocalFile(fileName));
               this->localMediaPlayList->addMedia(media);
            }
        }
        mediaPlayer->playlist()->setCurrentIndex(index.row());
        mediaPlayer->play();
    }
    else if(ui->listCategory->currentIndex() == 1)
    {
        localMediaPlayList->clear();
        QStringList loveList = dbUtil.selectLoveMusicPathList();
        if(!loveList.isEmpty())
        {
            foreach (const QString&fileName , loveList)
            {
               QMediaContent media = QMediaContent(QUrl::fromLocalFile(fileName));
               this->localMediaPlayList->addMedia(media);
            }
        }
        mediaPlayer->playlist()->setCurrentIndex(index.row());
        mediaPlayer->play();
    }
    ui->playPauseButton->setStyleSheet("background:url(:/image/icon/pause_48px_1180265_easyicon.net.png);border-radius:24px");
}

void PlayerWidget::empty_list()
{
    mediaPlayer->stop();
    mediaPlayer->playlist()->clear();
    fileNameandUrlList.clear();

    specialLabel->setText("专辑名");
    artistLabel->setText("歌手名");
    titleLabel->setText("歌曲名");
    ui->finishTime->setText("00:00");
    ui->totalTime->setText("00:00");
    ui->timeSlider->setValue(0);

    if(ui->listCategory->currentIndex() == 0)
        dbUtil.removeAllMusic();
    else if(ui->listCategory->currentIndex() == 1)
        dbUtil.removeAllLoveMusic();

    not_found_lyric();
}

void PlayerWidget::remove_current_music(int index)
{
    mediaPlayer->playlist()->removeMedia(index);
    fileNameandUrlList.removeAt(index);

    if(ui->listCategory->currentIndex() == 0)
        dbUtil.deleteMusic(index);
    else if(ui->listCategory->currentIndex() == 1)
        dbUtil.deleteLoveMusic(index);

    readLrc(QMediaContent());
}

PlayerWidget::~PlayerWidget()
{
    SAFE_RELEASE(ui);
    SAFE_RELEASE(localList);
    SAFE_RELEASE(collectList);
    SAFE_RELEASE(localLayout);
    SAFE_RELEASE(collectLayout);
    SAFE_RELEASE(localGroup);
    SAFE_RELEASE(collectGroup);
    SAFE_RELEASE(openFile);
    SAFE_RELEASE(lyricSwitch);
    SAFE_RELEASE(backgroundSwitch);
    SAFE_RELEASE(localMediaPlayList);
    SAFE_RELEASE(mediaPlayer);
    SAFE_RELEASE(progressTimer);
}

void PlayerWidget::handleLogin()
{
    if(!logindialog->status)
    {
    logindialog = new LoginDialog(this);
    logindialog->show();
    connect(logindialog,SIGNAL(login(QString)),this,SLOT(changeLoginStatus(QString)));
    }
    else
    {
        QMessageBox::warning(this,"提示","请先退出账号！",QMessageBox::Yes);
    }
}

void PlayerWidget::changeLoginStatus(QString nickname)
{
    this->ui->nickName->setText("["+nickname+"]");
}

void PlayerWidget::exitLogin()
{

    if(logindialog->status){
        this->ui->nickName->setText("");
        logindialog->status = false;
    }
    else
    {
        QMessageBox::warning(this,"提示","尚未登录，请先登录！",QMessageBox::Yes);
    }
}

void PlayerWidget::registerAccount()
{
    RegDialog *reg = new RegDialog(this);
    reg->show();
}

void PlayerWidget::setTimer()
{
    timeClock->setFixedSize(370, 180);
    connect(timeClock,SIGNAL(time_out()),this,SLOT(handleStopPlaying()));
    timeClock->show();
}

void PlayerWidget::handleStopPlaying()
{
    this->mediaPlayer->pause();
    ui->playPauseButton->setStyleSheet("background:url(:/image/icon/play_48px_1180266_easyicon.net.png);border-radius:24px");
}


void PlayerWidget::on_exitButton_clicked()
{
    close();
}

void PlayerWidget::add_collection(int index)
{
    QStringList musiclist = dbUtil.selectMusicPathList();
    dbUtil.insertLoveMusicPath(musiclist[index]);

    QStringList collect_list;
    collect_list.append(musiclist[index]);
    collectList->setList(collect_list);
}

void PlayerWidget::createSystemTrayIcon()
{
    trayIcon=new QSystemTrayIcon(this);
    trayIcon->setToolTip(tr("clld's MusicPlayer"));
    QIcon icon = QIcon(":/image/icon/music_24px_580162_easyicon.net.png");
    trayIcon->setIcon(icon);

    restoreAction=new QAction(tr("打开主界面"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(show()));

    quitAction=new QAction(tr("退出"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayContextMenu=new QMenu(this);

    trayContextMenu->addAction(restoreAction);
    trayContextMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayContextMenu);

    trayIcon->show();
    //添加单/双击鼠标相应
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayiconActivated(QSystemTrayIcon::ActivationReason)));
}

void PlayerWidget::not_found_lyric()
{
    for(int i=0;i<7;i++){
        lrclabel[i]->setText("");
    }
    lrclabel[7]->setText("未找到歌词");
    lrclabel[7]->setStyleSheet("font-size:20pt; color:yellow");
    for(int i=8;i<15;i++){
        lrclabel[i]->setText("");
    }
}


void PlayerWidget::trayiconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        this->showNormal();
        this->raise();
        break;
    default:
        break;
    }
}
