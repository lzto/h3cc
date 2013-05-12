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

#include "h3cmainwindow.h"
#include "aboutwidget.h"

#include <QMenuBar>

h3cMainWindow::h3cMainWindow()
{
    this->setWindowIcon(QIcon("/usr/share/h3cc/icons/h3cc.png"));
    this->setWindowTitle(tr("H3C 802.1x Client"));
    ah3cgui = new h3cgui;
    setCentralWidget(ah3cgui);
    
    QMenu *menu = menuBar()->addMenu( tr("&Client") );
    QAction *actionConnect = new QAction(tr("&Connect"),this);
    menu->addAction(actionConnect);
    QAction *actionDisconnect = new QAction(tr("&Disconnect"),this);
    menu->addAction(actionDisconnect);
    menu->addSeparator();
    QAction *actionTray = new QAction(tr("&Tray"),this);
    menu->addAction(actionTray);
    menu->addSeparator();
    QAction *actionExit = new QAction(tr("E&xit"),this);
    menu->addAction(actionExit);
    
    QMenu *toolMenu = menuBar()->addMenu(tr("&Tool"));
    QAction *actionInfo = new QAction(tr("&Log"),this);
    toolMenu->addAction(actionInfo);
    
    QMenu *aboutMenu = menuBar()->addMenu(tr("&About"));
    QAction *actionAbout = new QAction(tr("&About"),this);
    aboutMenu->addAction(actionAbout);
    
    connect(actionConnect,SIGNAL(triggered(bool)),this,SLOT(slot_connect()));
    connect(actionDisconnect,SIGNAL(triggered(bool)),this,SLOT(slot_disconnect()));
    connect(actionExit,SIGNAL(triggered(bool)),this,SLOT(slot_exit()));
    connect(actionInfo,SIGNAL(triggered(bool)),this,SLOT(slot_log()));
    connect(actionTray,SIGNAL(triggered(bool)),this,SLOT(slot_tray()));
    connect(actionAbout,SIGNAL(triggered(bool)),this,SLOT(slot_about()));
    
    tray = new QSystemTrayIcon(ah3cgui);
    tray->setIcon(QIcon("/usr/share/h3cc/icons/h3cc.png"));
    QMenu *tmenu= new QMenu();
    QAction *ta_connect = new QAction(tr("&Connect"),this);;
    QAction *ta_disconnect = new QAction(tr("&Disconnect"),this);
    QAction *ta_exit = new QAction(tr("E&xit"),this);
    QAction *ta_about = new QAction(tr("&About"),this);

    tmenu->addAction(actionConnect);
    tmenu->addAction(actionDisconnect);
    tmenu->addSeparator();
    tmenu->addAction(actionAbout);
    tmenu->addSeparator();
    tmenu->addAction(actionExit);
    tray->setContextMenu(tmenu);
    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	    this,SLOT(slot_tray(QSystemTrayIcon::ActivationReason)));
    tray->show();
    
    connect(ah3cgui,SIGNAL(signal_gotMsg(QString)),this,SLOT(slot_gotMsg(QString)));
}

h3cMainWindow::~h3cMainWindow()
{

}

void h3cMainWindow::slot_connect()
{
    tray->showMessage("Connecting","Connecting...");
    ah3cgui->slot_connect8021x();
}

void h3cMainWindow::slot_disconnect()
{
    tray->showMessage("Disconnected","Disconnected.");
    ah3cgui->slot_disconnect8021x();
}

void h3cMainWindow::slot_log()
{
    ah3cgui->slot_Log();
}

void h3cMainWindow::slot_tray(QSystemTrayIcon::ActivationReason r)
{
    switch (r)
    {
	case(QSystemTrayIcon::Trigger):
	{
	    slot_tray();
	    break;
	}
	default:
	    break;
    }
}


void h3cMainWindow::slot_tray()
{
    if(this->isVisible())
    {
	hide();
    }
    else
    {
	show();
    }
}

void h3cMainWindow::slot_gotMsg(QString msg)
{
    tray->showMessage("Connection Info",msg);
}

void h3cMainWindow::slot_about()
{
    aboutWidget *aboutw = new aboutWidget;
    aboutw->show();
}


void h3cMainWindow::slot_exit()
{
    slot_disconnect();
    exit(0);
}

#include "h3cmainwindow.moc"
