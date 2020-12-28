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

#include "timedate.h"

CTime::CTime(QObject *parent)
    : QObject(parent)
    , m_timedateInter(new Timedate("com.deepin.daemon.Timedate", "/com/deepin/daemon/Timedate", QDBusConnection::sessionBus(), this))
    , m_timeZone("")
    , m_is24HourSystem(false)
    , m_shortTimeFormat(0)
{
    m_timedateInter->setSync(false);
    setTimeZone(m_timedateInter->timezone());
    setIs24HourSystem(m_timedateInter->use24HourFormat());
    setShortTimeFormat(m_timedateInter->shortTimeFormat());

    connect(m_timedateInter, &Timedate::TimezoneChanged, this, [=](QString zone) {
        setTimeZone(zone);
    });

    connect(m_timedateInter, &Timedate::Use24HourFormatChanged, this, [=](bool is24HourSystem) {
        setIs24HourSystem(is24HourSystem);
    });

    connect(m_timedateInter, &Timedate::ShortTimeFormatChanged, this, [=](int shortTimeFormat) {
        setShortTimeFormat(shortTimeFormat);
    });
}

CTime:: ~ CTime()
{
}

void CTime::setTimeZone(QString zone)
{
    if (zone != m_timeZone) {
        m_timeZone = zone;

        emit timeZoneChanged(m_timeZone);
    }
}

void CTime::setIs24HourSystem(bool flag)
{
    if (!flag != !m_is24HourSystem) {
        m_is24HourSystem = flag;

        emit is24HourSystemChanged(m_is24HourSystem);
    }
}

void CTime::setShortTimeFormat(qint32 format)
{
    if (format != m_shortTimeFormat) {
        m_shortTimeFormat = format;

        emit shortTimeFormatChanged(m_shortTimeFormat);
    }
}

bool CTime::getIs24HourSystem()
{
    return m_is24HourSystem;
}

qint32 CTime::getShortTimeFormat()
{
    return m_shortTimeFormat;
}
