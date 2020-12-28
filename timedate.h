/*
   * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     ganjing <ganjing@uniontech.com>
   *
   * Maintainer: ganjing <ganjing@uniontech.com>
   *
   * This program is free software: you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation, either version 3 of the License, or
   * any later version.
   *
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TIMEDATE_H
#define TIMEDATE_H

#include <QObject>
#include <com_deepin_daemon_timedate.h>

using Timedate = com::deepin::daemon::Timedate;

class CTime : public QObject
{
    Q_OBJECT
public:
    CTime(QObject *parent = nullptr);
    ~ CTime();
    void setTimeZone(QString zone);
    void setIs24HourSystem(bool flag);
    void setShortTimeFormat(qint32 format);

    //供QML调用的接口函数,主要是在组件创建时初始化属性值
    Q_INVOKABLE bool getIs24HourSystem();
    Q_INVOKABLE qint32 getShortTimeFormat();

signals:
    void timeZoneChanged(QString zone);
    void is24HourSystemChanged(bool flag);
    void shortTimeFormatChanged(qint32 format);

private:
    Timedate *m_timedateInter;
    QString m_timeZone; //当前时区
    bool m_is24HourSystem; //是否使用24小时制
    qint32 m_shortTimeFormat; //短时间显示格式
};

#endif // TIMEDATE_H
