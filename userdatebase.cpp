#include "userdatebase.h"

userDatebase::userDatebase()
{

}

//匹配用户名与密码 正确返回0 密码错误返回-1 用户不存在返回1
int userDatebase::selectUserPsd(QString name,QString psd)
{
    initDatebase();

    QSqlQuery sql_query;
    QString select_user = "select name,password from user";
    QStringList usernamelist,userpsdlist;

    sql_query.prepare(select_user);
    if(sql_query.exec())
    {
        qDebug() <<"select user OK!"<<endl;

        while(sql_query.next()){
            QString username = sql_query.value("name").toString();
            QString userpsd = sql_query.value("password").toString();
            qDebug() <<username<<endl;
            usernamelist.append(username);
            userpsdlist.append(userpsd);
        }

        for(int i=0;i<usernamelist.size();i++)
        {
            if(usernamelist[i]==name)
            {
                if(userpsdlist[i]==psd)
                {
                    return 0;
                }
                else
                {
                    return -1;
                }
            }
        }
        db.close();
        return 1;
    }
    else
    {
        qDebug() <<"select user error!"<<endl;
        return -2;
    }

}

//初始化音乐路径表
void userDatebase::initMusicDatebase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("userInfor.db");
    db.setUserName("root");
    db.setPassword("123456");

    if(!db.open())
    {
        qDebug()  <<"open db error!"<<endl;
    }
    else
    {

        QSqlQuery sql_query;
        QString create_table = "create table if not exists musiclist(musicid integer primary key autoincrement not null,musicpath text not null)";
        if(sql_query.exec(create_table))
        {
            qDebug() <<"create table musiclist success!"<<endl;
        }
        else
        {
            qDebug() <<"create table musiclist falut!"<<endl;
        }
    }
}

//根据字符串参数插入数据到音乐路径表
void userDatebase::insertMusicPath(QString musicpath)
{
    initMusicDatebase();
    QSqlQuery sql_query;
    QString insert_Infor = "insert into musiclist(musicpath) values(?)";

    sql_query.prepare(insert_Infor);
    sql_query.addBindValue(musicpath);

    if(sql_query.exec())
    {
        qDebug() <<"insert musicpath OK!"<<endl;
    }
    else
    {
        qDebug() <<"insert musicpath falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    db.close();
}

void userDatebase::insertLoveMusicPath(QString lovemusicpath)
{
    initLoveMusicDatebase();

    QSqlQuery sql_query;
    QString insert_Infor = "insert into lovemusiclist(lovemusicpath) values(?)";

    sql_query.prepare(insert_Infor);
    sql_query.addBindValue(lovemusicpath);

    if(sql_query.exec())
    {
        qDebug() <<"insert lovemusicpath OK!"<<endl;
    }
    else
    {
        qDebug() <<"insert lovemusicpath falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    db.close();
}

void userDatebase::deleteMusic(int index)
{

    initMusicDatebase();
    index = index+1;

    qDebug() <<"rowid:"<<index<<endl;
    QSqlQuery sql_query;
    QString delete_Infor = "delete from musiclist where musicid=?";
    QString update_infor = "update musiclist set musicid = musicid - 1 where musicid > ?";

    sql_query.prepare(delete_Infor);
    sql_query.addBindValue(index);

    if(sql_query.exec())
    {
        qDebug() <<"delete music OK!"<<endl;
    }
    else
    {
        qDebug() <<"delete music falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    sql_query.prepare(update_infor);
    sql_query.addBindValue(index);

    if(sql_query.exec())
    {
        qDebug() <<"update music OK!"<<endl;
    }
    else
    {
        qDebug() <<"update music falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    db.close();

}

void userDatebase::deleteLoveMusic(int index)
{
    initLoveMusicDatebase();

    index = index+1;

    QSqlQuery sql_query;
    QString delete_Infor = "delete from lovemusiclist where lovemusicid=?";
    QString update_infor = "update lovemusiclist set lovemusicid = lovemusicid - 1 where lovemusicid > ?";

    sql_query.prepare(delete_Infor);
    sql_query.addBindValue(index);

    if(sql_query.exec())
    {
        qDebug() <<"delete lovemusic OK!"<<endl;
    }
    else
    {
        qDebug() <<"delete lovemusic falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    sql_query.prepare(update_infor);
    sql_query.addBindValue(index);

    if(sql_query.exec())
    {
        qDebug() <<"update lovemusic OK!"<<endl;
    }
    else
    {
        qDebug() <<"update lovemusic falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    db.close();
}

void userDatebase::removeAllMusic()
{
    initMusicDatebase();

    QSqlQuery sql_query;
    QString delete_Infor = "delete from musiclist";
    QString update_table = "update sqlite_sequence SET seq=0 where name='musiclist'";

    sql_query.prepare(delete_Infor);
    if(sql_query.exec())
    {
        qDebug() <<"delete musiclist OK!"<<endl;
    }
    else
    {
        qDebug() <<"delete musiclist falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    sql_query.prepare(update_table);
    if(sql_query.exec())
    {
        qDebug() <<"delete musiclist OK!"<<endl;
    }
    else
    {
        qDebug() <<"delete musiclist falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    db.close();
}

void userDatebase::removeAllLoveMusic()
{
    initLoveMusicDatebase();

    QSqlQuery sql_query;
    QString delete_Infor = "delete from lovemusiclist";
    QString update_table = "update sqlite_sequence SET seq=0 where name='lovemusiclist'";

    sql_query.prepare(delete_Infor);
    if(sql_query.exec())
    {
        qDebug() <<"delete lovemusiclist OK!"<<endl;
    }
    else
    {
        qDebug() <<"delete lovemusiclist falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    sql_query.prepare(update_table);
    if(sql_query.exec())
    {
        qDebug() <<"delete lovemusiclist OK!"<<endl;
    }
    else
    {
        qDebug() <<"delete lovemusiclist falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
    }

    db.close();
}

//返回所有音乐路径到QStringList
QStringList userDatebase::selectMusicPathList()
{
    initMusicDatebase();

    QSqlQuery sql_query;
    QString select_musicpath = "select musicpath from musiclist";
    QStringList musicpathlist;
    QString musicpath;

    sql_query.prepare(select_musicpath);
    if(sql_query.exec())
    {
        qDebug() <<"select musicpath OK!"<<endl;

        while(sql_query.next())
        {
            musicpath = sql_query.value("musicpath").toString();
            musicpathlist.append(musicpath);
        }

    }
    else
    {
        qDebug() <<sql_query.lastError()<<endl;
    }

    db.close();

    return musicpathlist;
}

QStringList userDatebase::selectLoveMusicPathList()
{
    initLoveMusicDatebase();

    QSqlQuery sql_query;
    QString select_lovemusicpath = "select lovemusicpath from lovemusiclist";
    QStringList lovemusicpathlist;
    QString musicpath;

    sql_query.prepare(select_lovemusicpath);
    if(sql_query.exec())
    {
        qDebug() <<"select lovemusicpath OK!"<<endl;

        while(sql_query.next())
        {
            musicpath = sql_query.value("lovemusicpath").toString();
            lovemusicpathlist.append(musicpath);
        }

    }
    else
    {
        qDebug() <<sql_query.lastError()<<endl;
    }

    db.close();

    return lovemusicpathlist;
}

//初始化用户表
void userDatebase::initDatebase()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("userInfor.db");
    db.setUserName("root");
    db.setPassword("123456");

    if(!db.open())
    {
        qDebug() <<"open db error!"<<endl;
    }
    else
    {

        QSqlQuery sql_query;
        QString create_table = "create table if not exists user(lovemusicid integer primary key autoincrement not null,name text UNIQUE not null,password text not null,nickname text not null)";
        if(sql_query.exec(create_table))
        {
            qDebug() <<"create table user success!"<<endl;
        }
        else
        {
             qDebug() <<sql_query.lastError()<<endl;
        }
    }

}

void userDatebase::initLoveMusicDatebase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("userInfor.db");
    db.setUserName("root");
    db.setPassword("123456");

    if(!db.open())
    {
        qDebug() <<"open db error!"<<endl;
    }
    else
    {

        QSqlQuery sql_query;
        QString create_table = "create table if not exists lovemusiclist(lovemusicid integer primary key autoincrement not null,lovemusicpath text not null)";
        if(sql_query.exec(create_table))
        {
            qDebug() <<"create table lovemusicpath success!"<<endl;
        }
        else
        {
             qDebug() <<sql_query.lastError()<<endl;
        }
    }

}

//根据用户名返回用户昵称
QString userDatebase::selectNickname(QString name)
{
    initDatebase();
    QSqlQuery sql_query;
    QString select_nickname = "select nickname from user where name=?";

    sql_query.prepare(select_nickname);
    sql_query.addBindValue(name);

    if(!sql_query.exec())
    {
       qDebug() <<sql_query.lastError()<<endl;
    }

    QString nickname;
    while(sql_query.next()){
       nickname = sql_query.value(0).toString();
    }

     db.close();
    return  nickname;
}

//插入新用户数据
int userDatebase::insertUser(QString name,QString psd,QString nickname)
{
    initDatebase();
    QSqlQuery sql_query;
    QString insert_Infor = "insert into user(name,password,nickname) values(?,?,?)";

    sql_query.prepare(insert_Infor);
    sql_query.addBindValue(name);
    sql_query.addBindValue(psd);
    sql_query.addBindValue(nickname);

    if(sql_query.exec())
    {
        qDebug() <<"insert OK!"<<endl;
        db.close();
        return 0;
    }
    else
    {
        qDebug() <<"insert falut!"<<endl;
        qDebug() <<sql_query.lastError()<<endl;
        db.close();
        return -1;
    }
}
