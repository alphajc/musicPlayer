#ifndef DATEBASE_H
#define DATEBASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QStringList>

class userDatebase
{
public:
    userDatebase();
    //创建table
    void initMusicDatebase();
    void initDatebase();
    void initLoveMusicDatebase();

    //查询
    QString selectNickname(QString name);
    int selectUserPsd(QString name,QString psd);
    QStringList selectMusicPathList();
    QStringList selectLoveMusicPathList();

    //插入
    int insertUser(QString name,QString psd,QString nickname);
    void insertMusicPath(QString musicpath);
    void insertLoveMusicPath(QString lovemusicpath);

    //删除
    void deleteMusic(int index);
    void deleteLoveMusic(int index);
    void removeAllMusic();
    void removeAllLoveMusic();
    QSqlDatabase db;
};

#endif // DATEBASE_H
