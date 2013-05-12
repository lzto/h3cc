/*
    Copyright (C) 2010 lovewilliam <lovewilliam@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "logwidget.h"
#include <QDebug>

logWidget::logWidget()
{
    log = new string("Log started");
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(slot_fetchLog()));
    timer->setSingleShot(FALSE);
    timer->start(360);
    this->setReadOnly(TRUE);
}

logWidget::~logWidget()
{

}

string* logWidget::getLogPoint()
{
    return log;
}


void logWidget::slot_fetchLog()
{
    QString qlog = QString::fromLocal8Bit(log->c_str());
    sb = this->verticalScrollBar();
    int value =sb->value();
    bool bottom = FALSE;
    if(value== sb->maximum())
    {
	bottom = TRUE;
    }
    QStringList list1 = qlog.split("]",QString::SkipEmptyParts);
    
    this->setText(qlog);
    if(bottom)
    {
	sb->setValue(sb->maximum());
    }else
    {
	sb->setValue(value);
    }
    if(list1.last().contains("*"))
    {
	if(list1.last() != lastmsg)
	{
	    lastmsg = list1.last();
	    QString temp = lastmsg.simplified();
	    temp.remove(0,1);
	    emit signal_msg(temp);
	}
    }
}


#include "logwidget.moc"
