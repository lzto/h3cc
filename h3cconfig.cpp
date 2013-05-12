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

#include "h3cconfig.h"

#include <QTextStream>
#include <QDir>

h3cConfig::h3cConfig()
{
    QFile file(QDir::homePath()+"/"+CFG_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	return;
    
    QTextStream in(&file);
    while (!in.atEnd()) {
	QString line = in.readLine();
	QStringList list = line.split("=");
	if(list.length()==2)
	{
	    QString temp = list.at(0).simplified();
		if(temp=="USERNAME")
		{
		    username = list.at(1).simplified();
		}else if(temp=="PASSWORD")
		{
		    password = list.at(1).simplified();
		}else if(temp=="NIC")
		{
		    nic = list.at(1).simplified();
		}else if(temp=="CASTMETHOD")
		{
		    castMethod = list.at(1).simplified();
		}else if(temp=="DHCP")
		{
		    if(list.at(1).simplified()=="TRUE")
		    {
			dhcp = TRUE;
		    }else
		    {
			dhcp = FALSE;
		    }
		}
	}
    }
    file.close();
}

h3cConfig::~h3cConfig()
{

}

QString h3cConfig::getUsername()
{
    return username;
}

QString h3cConfig::getPassword()
{
    return password;
}

QString h3cConfig::getNic()
{
    return nic;
}

QString h3cConfig::getCastMethod()
{
    return castMethod;
}

bool h3cConfig::getDHCP()
{
    return dhcp;
}

void h3cConfig::writeBack()
{
    QFile file(QDir::homePath()+"/"+CFG_FILE);
    file.remove();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	return;
    QTextStream out(&file);
    out<<"USERNAME="<<username<<endl;
    out<<"PASSWORD="<<password<<endl;
    out<<"NIC="<<nic<<endl;
    out<<"CASTMETHOD="<<castMethod<<endl;
    if(dhcp)
    {
	out<<"DHCP="<<"TRUE"<<endl;
    }else
    {
	out<<"DHCP="<<"FALSE"<<endl;
    }
    file.close();
}

void h3cConfig::setUsername(QString u)
{
    username = u;
}

void h3cConfig::setPassword(QString p)
{
    password = p;
}

void h3cConfig::setNic(QString n)
{
    nic = n;
}

void h3cConfig::setCastMethod(QString c)
{
    castMethod = c;
}

void h3cConfig::setDHCP(bool d)
{
    dhcp = d;
}



#include "h3cconfig.moc"
