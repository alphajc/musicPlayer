#include "playlist.h"

PlayList::PlayList(int flag)
{
    this->flag = flag;
    init_table();
    init_menu();
}

void PlayList::setList(QStringList playList)
{
    for(int i = 0; i < playList.size(); ++i)
    {
        insertRow(rowCount());
        QString nameStr = playList.at(i).split("/").last().split(".").front();
        setItem(rowCount()-1, 0, new QTableWidgetItem("  " + nameStr));
    }
}

void PlayList::init_table()
{
    setColumnCount(1);
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
    setColumnWidth(0,300);
    setColumnWidth(1,45);
    //禁止编辑单元格内容
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //选中一行
    setSelectionBehavior(QAbstractItemView::SelectRows);
    //去掉水平滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //垂直滚动条按项移动
    setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    //去除水平表头
    horizontalHeader()->setFixedHeight(0);
    //去除垂直表头
    verticalHeader()->setFixedWidth(0);
    //设置无边框
    setFrameShape(QFrame::NoFrame);
    //设置不显示格子线
    setShowGrid(false);
    //设置右键菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    setStyleSheet("QScrollBar{background:transparent; width: 5px;}"
            "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
            "QScrollBar::handle:hover{background:gray;}"
            "QScrollBar::sub-line{background:transparent;}"
            "QScrollBar::add-line{background:transparent;}");
    setAcceptDrops(true);
    setAcceptDrops(true);
    resize(260, 315);
}

void PlayList::init_menu()
{
    contextMenuLess = new QMenu(this);
    contextMenuMore = new QMenu(this);
    if(flag == 0)
        addMusic = new QAction(QIcon(":/image/icon/love_18.295503211991px_1190642_easyicon.net.png") ,"加入收藏",this);
    removeCurr = new QAction("移除本曲",this);
    removeAll = new QAction("移除所有",this);
    if(flag ==0)
        connect(addMusic, SIGNAL(triggered()), this, SLOT(collect_music()));
    connect(removeCurr, SIGNAL(triggered()), this, SLOT(remove_current_music()));
    connect(removeAll, SIGNAL(triggered()), this, SLOT(clear_list()));
    connect(this,SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(playlisttable_menu_requested(const QPoint)));

    if(flag == 0)
    {
        contextMenuLess->addAction(addMusic);
        contextMenuLess->addSeparator();
        contextMenuMore->addAction(addMusic);
        contextMenuMore->addSeparator();
    }
    contextMenuLess->addAction(removeAll);

    contextMenuMore->addAction(removeCurr);
    contextMenuMore->addAction(removeAll);

    contextMenuLess->setStyleSheet(
        "QMenu{padding:5px;background:white;border:1px solid gray;}"
        "QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
        "QMenu::item:selected:enabled{background:#0096ff;color:white;}"
        "QMenu::item:selected:!enabled{background:transparent;}"
        "QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");

    contextMenuMore->setStyleSheet(
        "QMenu{padding:5px;background:white;border:1px solid gray;}"
        "QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
        "QMenu::item:selected:enabled{background:#0096ff;color:white;}"
        "QMenu::item:selected:!enabled{background:transparent;}"
        "QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");
}

void PlayList::collect_music()
{
    emit collect(currentIndex);
}

void PlayList::remove_current_music()
{
    removeRow(currentIndex);
    emit remove_current_music(currentIndex);
}

void PlayList::clear_list()
{
    clear();
    while(rowCount())
            removeRow(0);
    emit empty_list();
}

void PlayList::playlisttable_menu_requested(const QPoint pos)
{
    if(itemAt(pos))
        {
            currentIndex = rowAt(pos.y());
            contextMenuMore->exec(QCursor::pos());
        }
        else contextMenuLess->exec(QCursor::pos());
}

