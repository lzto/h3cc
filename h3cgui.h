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

#ifndef H3CGUI_H
#define H3CGUI_H

#include <QtGui/QWidget>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSystemTrayIcon>

#include "h3c8021x.h"
#include "dot1xthread.h"
#include "logwidget.h"
#include "h3cconfig.h"

class h3cgui : public QWidget
{
    Q_OBJECT
    public:
	h3cgui();
	virtual ~h3cgui();
    public slots:
	void slot_connect8021x();
	void slot_disconnect8021x();
	void slot_Log();
    signals:
	void signal_connect8021x();
	void signal_disconnect8021x();
	void signal_gotMsg(QString);
    private slots:
	
    private:
	h3c8021x *dot1x;
	dot1xThread *dot1xthread;
	h3cConfig *h3ccfg;
	logWidget *logwidget;
	QLineEdit *leditUsername;
	QLineEdit *leditPassword;
	QPushButton *btnConnect;
	QPushButton *btnDisconnect;
	QComboBox *boxNic;
	QComboBox *boxCast;
	QCheckBox *cboxRenewIP;
};

#endif // H3CGUI_H
