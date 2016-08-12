#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QTableWidget>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QAction>

class PlayList : public QTableWidget
{
    Q_OBJECT
public:
    PlayList(int);
    int flag;
    void setList(QStringList);
    void init_table();
    void init_menu();

signals:
    void remove_current_music(int);
    void empty_list();
    void collect(int);
private:
    QMenu *contextMenuLess, *contextMenuMore;
    QAction *addMusic, *removeCurr, *removeAll;
    int currentIndex;

public slots:
    void collect_music();
    void remove_current_music();
    void clear_list();
    void playlisttable_menu_requested(const QPoint);
};

#endif // PLAYLIST_H
