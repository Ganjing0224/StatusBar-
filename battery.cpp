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

#include "battery.h"
#include <math.h>
#include <qdbusargument.h>

const QString powerService = "com.deepin.system.Power";
const QString powerObjPath = "/com/deepin/system/Power";
const QString powerInterface = "com.deepin.system.Power";
const QString interfaceDBusProps = "org.freedesktop.DBus.Properties";
const QString memberPropsChanged = "PropertiesChanged";

const double EPS = 1e-6;
const QString homePath = "/image/";
const QString urlPrefix = "battery-";
const QString urlSuffix = "-symbolic-dark.svg";
const QString charging = "-plugged";
const QString fullCharged = "full-charged";

CBattery::CBattery(QObject *parent)
    : QObject(parent)
    , m_status(0)
    , m_batteryPercentage(0)
    , m_onBattery(false)
    , m_mode("")
    , m_timeToFull(0)
    , m_iconUrl("")
{
    setStatus(getStatusProp());
    setBatteryPercentage(getBatteryPercentageProp());
    setOnBattery(getOnBatteryProp());
    setMode(getModeProp());
    setTimeToFull(getTimeToFullProp());
    generateIconUrl();

    connectToStatus();
    connectToBatteryPercentage();
    connectToOnBattery();
    connectToMode();
    connectToTimeToFull();
}

CBattery:: ~ CBattery()
{
}

void CBattery::setStatus(quint32 status)
{
    if (status != m_status) {
        m_status = status;

        emit statusChanged(m_status);
    }
}

void CBattery::setBatteryPercentage(double percentage)
{
    if (fabs(percentage - m_batteryPercentage) > EPS) {
        m_batteryPercentage = percentage;

        emit batteryPercentageChanged(m_batteryPercentage);
    }
}

void CBattery::setOnBattery(bool onBattery)
{
    if (!onBattery != !m_onBattery) {
        m_onBattery = onBattery;

        emit onBatteryChanged(onBattery);
    }
}

void CBattery::setMode(QString mode)
{
    if (mode != m_mode) {
        m_mode = mode;

        emit modeChanged(m_mode);
    }
}

void CBattery::setTimeToFull(quint32 time)
{
    if (time != m_timeToFull) {
        m_timeToFull = time;

        emit timeToFullChanged(m_timeToFull);
    }
}

void CBattery::setIconUrl(QString iconUrl)
{
    if (iconUrl != m_iconUrl) {
        m_iconUrl = iconUrl;

        emit iconUrlChanged(m_iconUrl);
    }
}

quint32 CBattery::getStatusProp()
{
    QDBusInterface inter (powerService, powerObjPath, powerInterface, QDBusConnection::systemBus(), this);
    return inter.property("BatteryStatus").toUInt();
}

double CBattery::getBatteryPercentageProp()
{
    QDBusInterface inter(powerService, powerObjPath, powerInterface, QDBusConnection::systemBus(), this);
    return inter.property("BatteryPercentage").toDouble();
}

bool CBattery::getOnBatteryProp()
{
    QDBusInterface inter(powerService, powerObjPath, powerInterface, QDBusConnection::systemBus(), this);
    return inter.property("BatteryPercentage").toBool();
}

QString CBattery::getModeProp()
{
    QDBusInterface inter(powerService, powerObjPath, powerInterface, QDBusConnection::systemBus(), this);
    return inter.property("Mode").toString();
}

quint32 CBattery::getTimeToFullProp()
{
    QDBusInterface inter(powerService, powerObjPath, powerInterface, QDBusConnection::systemBus(), this);
    return inter.property("BatteryTimeToFUll").toUInt();
}

void CBattery::connectToStatus()
{
    QDBusConnection::systemBus().connect(powerService, powerObjPath, interfaceDBusProps,
                                         memberPropsChanged, "sa{sv}as", this, SLOT(handleStateChangedSignal(QDBusMessage)));
}

void CBattery::connectToBatteryPercentage()
{
    QDBusConnection::systemBus().connect(powerService, powerObjPath, interfaceDBusProps,
                                         memberPropsChanged, "sa{sv}as", this, SLOT(handleBatteryPercentageChangedSignal(QDBusMessage)));
}

void CBattery::connectToOnBattery()
{
    QDBusConnection::systemBus().connect(powerService, powerObjPath, interfaceDBusProps,
                                         memberPropsChanged, "sa{sv}as", this, SLOT(handleOnBatteryChangedSignal(QDBusMessage)));
}

void CBattery::connectToMode()
{
    QDBusConnection::systemBus().connect(powerService, powerObjPath, interfaceDBusProps,
                                         memberPropsChanged, "sa{sv}as", this, SLOT(handleModeChangedSignal(QDBusMessage)));
}

void CBattery::connectToTimeToFull()
{
    QDBusConnection::systemBus().connect(powerService, powerObjPath, interfaceDBusProps,
                                         memberPropsChanged, "sa{sv}as", this, SLOT(handleTimeToFullChangedSignal(QDBusMessage)));
}

void CBattery::handleStatusChangedSignal(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != powerInterface)
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys = changedProps.keys();

    foreach (const QString &prop, keys) {
        if (prop == "BatteryStatus") {
            QVariant ret = changedProps[prop];
            setStatus(quint32(changedProps[prop].toInt()));
            generateIconUrl();
            break;
        }
    }
}

void CBattery::handleBatteryPercentageChangedSignal(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != powerInterface)
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys = changedProps.keys();

    foreach (const QString &prop, keys) {
        if (prop == "BatteryPercentage") {
            QVariant ret = changedProps[prop];
            setBatteryPercentage(changedProps[prop].toDouble());
            generateIconUrl();
            break;
        }
    }
}

void CBattery::handleOnBatteryChangedSignal(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != powerInterface)
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys = changedProps.keys();

    foreach (const QString &prop, keys) {
        if (prop == "OnBattery") {
            QVariant ret = changedProps[prop];
            setOnBattery(changedProps[prop].toBool());
            break;
        }
    }
}

void CBattery::handleModeChangedSignal(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != powerInterface)
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys = changedProps.keys();

    foreach (const QString &prop, keys) {
        if (prop == "Mode") {
            QVariant ret = changedProps[prop];
            setMode(changedProps[prop].toString());
            break;
        }
    }
}

void CBattery::handleTimeToFullChangedSignal(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != powerInterface)
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys = changedProps.keys();

    foreach (const QString &prop, keys) {
        if (prop == "BatteryTimeToFull") {
            QVariant ret = changedProps[prop];
            setTimeToFull(quint32(changedProps[prop].toInt()));
            break;
        }
    }
}

quint32 CBattery::getStatus()
{
    return m_status;
}

double CBattery::getBatteryPercentage()
{
    return m_batteryPercentage;
}

bool CBattery::getOnBattery()
{
    return m_onBattery;
}

QString CBattery::getMode()
{
    return m_mode;
}

quint32 CBattery::getTimeToFull()
{
    return m_timeToFull;
}

QString CBattery::getIconUrl()
{
    return m_iconUrl;
}

void  CBattery::generateIconUrl()
{
    QString iconUrl;

    QString batteryPercent = QString("%1").arg(int(m_batteryPercentage/10), 3, 10, QChar('0'));
    if (m_status == 1) {
        iconUrl = homePath + urlPrefix + batteryPercent + charging + urlSuffix;
        setIconUrl(iconUrl);
    } else if (m_status == 2) {
        iconUrl = homePath + urlPrefix + batteryPercent + urlSuffix;
        setIconUrl(iconUrl);
    } else if (m_status == 4) {
        iconUrl = homePath + urlPrefix + fullCharged + urlSuffix;
        setIconUrl(iconUrl);
    } else {
        iconUrl = "/image/battery-unknow-symbolic-dark.svg";
        setIconUrl(iconUrl);
    }
}






