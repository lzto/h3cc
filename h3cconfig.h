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

#ifndef H3CCONFIG_H
#define H3CCONFIG_H

#include <QtCore/QObject>
#include <QFile>

#define CFG_FILE ".h3c"

class h3cConfig : public QObject
{
    Q_OBJECT
    public:
	h3cConfig();
	virtual ~h3cConfig();
	QString getUsername();
	QString getPassword();
	QString getNic();
	QString getCastMethod();
	bool getDHCP();
	void writeBack();
	void setUsername(QString);
	void setPassword(QString);
	void setNic(QString);
	void setCastMethod(QString);
	void setDHCP(bool);
    private:
	QString username;
	QString password;
	QString nic;
	QString castMethod;
	bool dhcp;
	
};

#endif // H3CCONFIG_H
