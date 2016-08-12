#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if(p) { delete p; p = NULL;}
#endif //SAFE_RELEASE

#include "switchwidget.h"
#include "lyricform.h"
#include "assestutil.h"
#include "rolllabel.h"
#include "playlist.h"
#include "logindialog.h"
#include "regdialog.h"
#include "userdatebase.h"
#include "tdialog.h"

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QListView>
#include <QLayout>
#include <QGroupBox>
#include <QMediaPlayer>
#include <QMultimedia>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QTimer>
#include <windows.h>
#include <QMouseEvent>
#include <QPalette>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <QMediaContent>
#include <QTime>
#include <QStringList>
#include <QTableWidget>
#include <QMenu>
#include <ctime>
#include <QMessageBox>
#include <QApplication>
#include <QSystemTrayIcon>

namespace Ui {
class PlayerWidget;
}

class PlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = 0);
    ~PlayerWidget();
    void updateLrc();
    void setConnection();
    void init_window();
    void init_play_list();
    void init_inner_lyric();
    void init_player();
    void init_song_info();
    void init_outter_lyric();
    void createSystemTrayIcon();
    void not_found_lyric();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_firstTrackButton_clicked();

    void on_rewindButton_clicked();

    void on_playPauseButton_clicked();

    void on_forwardButton_clicked();

    void on_lastTrackButton_clicked();

    void on_volumeUpButton_clicked();

    void on_volumeDownButton_clicked();

    void on_timeSlider_valueChanged(int position);

    void on_volumeSlider_valueChanged(int position);

    void handleOpenLocalMedia();//打开本地视频资源

    void playProgressUpdate();//进度条同步歌曲播放

    void metaDataUpdate();

    void pictureUpdate();

    void bgUpdate(bool);

    void readLrc(QMediaContent);

    void on_singleButton_clicked();

    void on_randomButton_clicked();

    void on_orderButton_clicked();

    void openLyric(bool);

    void dcPlay(QModelIndex);

    void empty_list();

    void remove_current_music(int);

    void handleLogin();

    void changeLoginStatus(QString);

    void exitLogin();

    void registerAccount();

    void trayiconActivated(QSystemTrayIcon::ActivationReason);

    void setTimer();

    void handleStopPlaying();

    void on_exitButton_clicked();

    void add_collection(int);

private:
//    QString mainColor;//主色调
    Ui::PlayerWidget *ui;
    QPoint offset;
    PlayList *localList,*collectList;
    QVBoxLayout *localLayout,*collectLayout;
    QGroupBox *localGroup,*collectGroup;
    QMediaPlayer *mediaPlayer;
    QMediaPlaylist *localMediaPlayList;
    QTimer *progressTimer;
    QTimer *pictureTimer;
    QAction *openFile, *loginMenu, *registerMenu, *exitLoginMenu, *timeMenu;
    CSwitchWidget *lyricSwitch, *backgroundSwitch;
    int count;

    userDatebase dbUtil;

    //歌词
    RollLabel *lrclabel[15];
    QStringList fileNameandUrlList;
    QString strTime;
    QStringList lrcLineList;
    LyricForm *lyricForm;
    bool isLeftButton;

    //歌曲详情
    RollLabel *specialLabel, *artistLabel, *titleLabel;

    LoginDialog *logindialog;
    TDialog *timeClock;

    QSystemTrayIcon *trayIcon;
    QAction *quitAction;
    QMenu *trayContextMenu;
    QAction *restoreAction;
};

#endif // PLAYERWIDGET_H
