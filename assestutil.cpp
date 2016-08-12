#include "assestutil.h"

AssestUtil::AssestUtil()
{

}

void AssestUtil::horse_race_lamp(RollLabel *label, QString &str)
{
    if(label->width() > label->fontMetrics().width(str))
    {
        label->rollStop();
        label->setText(str);
    }
    else//跑马灯效果
    {
        label->rollText(str, TEXT_SPEED);
    }
}
