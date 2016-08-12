#include "rolllabel.h"

RollLabel::RollLabel(QWidget *parent)
    :QLabel(parent)
{
    label_init();
}

RollLabel::~RollLabel()
{
    rollStop();
}

void RollLabel::rollText(const QString &context, const int &ms)
{
    mContext = context;
    mMs = ms;

    if (mMs != 0)
    {
        if (mTimer == NULL)
        {
            mTimer = new QTimer(this);

            if (mTimer)
            {
               connect(mTimer, SIGNAL(timeout()), this, SLOT(roll_begin()));
                mTimer->start(mMs);
            }
        }
    }
}

void RollLabel::rollStop()
{
    if (mTimer){
            if (mTimer->isActive())
            {
                mTimer->stop();
            }
            delete mTimer;
            mTimer = NULL;
        }
}

void RollLabel::roll_begin()
{
    QString str="";

    if (mPos > mContext.count())
    {
        mPos = 0;
    }

    str = mContext.mid(mPos);

    setText(str);
    mPos++;
}

void RollLabel::label_init(int ms)
{
    mTimer = NULL;
    mMs = ms;
    mContext = "";
    mPos = 0;
}
