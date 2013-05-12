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

#ifndef DOT1XTHREAD_H
#define DOT1XTHREAD_H

#include <QThread>
#include "h3c8021x.h"

class dot1xThread:public QThread 
{
  public:
    dot1xThread();
    virtual ~dot1xThread();
    void run();
    h3c8021x *dot1x;
  private:
};

#endif // DOT1XTHREAD_H
