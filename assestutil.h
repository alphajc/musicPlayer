#ifndef ASSESTUTIL_H
#define ASSESTUTIL_H

#define TEXT_SPEED 500

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "rolllabel.h"

class AssestUtil:public QObject
{
public:
    AssestUtil();
    static void horse_race_lamp(RollLabel *,QString &);

};

#endif // ASSESTUTIL_H
