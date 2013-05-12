/*
    Copyright (C) 2010 lovewilliam<lovewilliam@gmail.com>

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

#ifndef H3CMAINWINDOW_H
#define H3CMAINWINDOW_H

#include <QMainWindow>

#include "h3cgui.h"

class h3cMainWindow : public QMainWindow
{
    Q_OBJECT
    public:
	h3cMainWindow();
	~h3cMainWindow();
    signals:
	void signal_log();
	
    private:
	QSystemTrayIcon *tray;
	h3cgui *ah3cgui;
    private slots:
	void slot_exit();
	void slot_log();
	void slot_connect();
	void slot_disconnect();
	void slot_tray();
	void slot_tray(QSystemTrayIcon::ActivationReason);
	void slot_about();
	void slot_gotMsg(QString);
};

#endif // H3CMAINWINDOW_H
