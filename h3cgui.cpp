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

#include "h3cgui.h"

#include <pcap.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QDebug>

h3cgui::h3cgui()
{
    dot1xthread = new dot1xThread();
    h3ccfg = new h3cConfig;
    dot1x = dot1xthread->dot1x;
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);
    QGridLayout *layout1 = new QGridLayout();
    layout->addLayout(layout1);
    QLabel *labelUser = new QLabel(tr("Username"));
    leditUsername = new QLineEdit(h3ccfg->getUsername());
    layout1->addWidget(labelUser,0,0);
    layout1->addWidget(leditUsername,0,1);
    
    QLabel *labelPassword = new QLabel(tr("Password"));
    leditPassword = new QLineEdit(h3ccfg->getPassword());
    leditPassword->setEchoMode(QLineEdit::Password);
    layout1->addWidget(labelPassword,1,0);
    layout1->addWidget(leditPassword,1,1);
    
    QLabel *labelNic = new QLabel(tr("NIC"));
    boxNic = new QComboBox();
    pcap_if_t* allDev = dot1x->getAllNic();
    while(allDev)
    {
	if(QString(allDev->name)=="lo"||
	    QString(allDev->name)=="any")
	{
	}else
	{
	    boxNic->addItem(allDev->name);
	}
	allDev = allDev->next;
    }
    layout1->addWidget(labelNic,2,0);
    layout1->addWidget(boxNic,2,1);
    
    QLabel *labelCastMethod = new QLabel(tr("Cast Method"));
    boxCast = new QComboBox();
    boxCast->addItem(tr("Multicast"));
    //boxCast->addItem(tr("Singlecast"));
    boxCast->setCurrentIndex(0);
    layout1->addWidget(labelCastMethod,3,0);
    layout1->addWidget(boxCast,3,1);
    
    cboxRenewIP = new QCheckBox(tr("Renew IP"));
    if(h3ccfg->getDHCP())
	cboxRenewIP->setCheckState(Qt::Checked);
    else
	cboxRenewIP->setCheckState(Qt::Unchecked);
    
    layout1->addWidget(cboxRenewIP,4,0,1,2,Qt::AlignLeft);
    
    btnConnect = new QPushButton(tr("Connect"));
    layout1->addWidget(btnConnect,5,1);
    
    connect(btnConnect,SIGNAL(clicked(bool)),this,SLOT(slot_connect8021x()));
    
    btnDisconnect = new QPushButton(tr("Disconnect"));
    layout1->addWidget(btnDisconnect,5,0);
    connect(btnDisconnect,SIGNAL(clicked(bool)),this,SLOT(slot_disconnect8021x()));
    
    logwidget = new logWidget();
    dot1x->setLog(logwidget->getLogPoint());
    layout->addWidget(logwidget);
    connect(logwidget,SIGNAL(signal_msg(QString)),this,SIGNAL(signal_gotMsg(QString)));
}

h3cgui::~h3cgui()
{
}

void h3cgui::slot_Log()
{
    if(logwidget->isVisible())
    {
	logwidget->hide();
    }else
    {
	logwidget->show();
    }
}

void h3cgui::slot_connect8021x()
{
    if(dot1xthread->isRunning())
    {
	return;
    }
    if(leditUsername->text().isEmpty()||
	leditUsername->text().isNull())
    {
	QMessageBox msgBox;
	msgBox.setText(tr("Username is Empty!"));
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setWindowTitle(tr("Empty username!"));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setDefaultButton(QMessageBox::Ok);
	msgBox.exec();
	return;
    }
    if(leditPassword->text().isEmpty()||
	leditPassword->text().isNull())
    {
	QMessageBox msgBox;
	msgBox.setText(tr("Password is Empty!"));
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setWindowTitle(tr("Empty password!"));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setDefaultButton(QMessageBox::Ok);
	msgBox.exec();
	return;
    }
    h3ccfg->setUsername(leditUsername->text());
    h3ccfg->setPassword(leditPassword->text());
    h3ccfg->setNic(boxNic->currentText());
    h3ccfg->setDHCP(cboxRenewIP->isChecked());
    h3ccfg->writeBack();
    
    QString temp;
    if(cboxRenewIP->isChecked())
    {
	temp.append("dhcpcd ");
	temp.append(boxNic->currentText());
    }
    
    dot1x->setUserName(leditUsername->text().toStdString());
    dot1x->setPassword(leditPassword->text().toStdString());
    dot1x->setDefaultNic(boxNic->currentText().toStdString());
    dot1x->setDHCPcmd(temp.toStdString());
    dot1x->setRun(TRUE);
    qDebug()<<"Calling start";
    dot1xthread->start();
}

void h3cgui::slot_disconnect8021x()
{
    if(dot1xthread->isRunning())
	dot1x->logoff();
}

#include "h3cgui.moc"
