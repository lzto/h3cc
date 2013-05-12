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

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <string>

#include <QtGui/QWidget>
#include <QTextEdit>
#include <QTimer>
#include <QScrollBar>

using namespace std;

class logWidget : public QTextEdit
{
    Q_OBJECT
    public:
	logWidget();
	virtual ~logWidget();
	string* getLogPoint();
	string *log;
    signals:
	void signal_msg(QString);
    private slots:
	void slot_fetchLog();
    private:
	QString lastmsg;
	QTimer *timer;
	QScrollBar *sb;
};

#endif // LOGWIDGET_H
