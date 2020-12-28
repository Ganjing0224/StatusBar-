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

#include <QDBusInterface>
#include <QJsonObject>

#include "type.h"
#include "wireless.h"

const QString nmService = "org.freedesktop.NetworkManager";
const QString nmInterface = "/org/freedesktop/NetworkManager";
const QString nmAccessPoint = "org.freedesktop.NetworkManager.AccessPoint";
const QString nmDevice = "org.freedesktop.NetworkManager.Device";
const QString nmDeviceWireless = "org.freedesktop.NetworkManager.Device.Wireless";
const QString interfaceDBusProps = "org.freedesktop.DBus.Properties";
const QString memberPropsChanged = "PropertiesChanged";

const QString homePath = "/image/";
const QString urlPrefix = "network-wireless-signal-";
const QString urlSuffix = "-symbolic-dark.svg";

CWireless::CWireless(QObject *parent)
    : QObject(parent)
    , m_networkInter(new Network("com.deepin.daemon.Network", "/com/deepin/daemon/Network", QDBusConnection::sessionBus(), this))
    , m_enable(false)
    , m_state(0)
    , m_devices("")
    , m_devPath("")
    , m_activeAp("")
    , m_strength(0)
    , m_iconUrl("")
    , m_reinit(false)
{
    //初始化属性值
    setDevices(m_networkInter->devices());
    init();
    connectToDeviceAdd(m_devices);
    connectToDeviceRemove();
}

CWireless:: ~ CWireless()
{
}

void CWireless::init()
{
    parseDevicesJsonInfo("Path");
    parseDevicesJsonInfo("ActiveAp");

    //在平板上可能是多余逻辑判断：默认平板上是会有一个无线网卡的
    if (m_devPath == "") {
        m_reinit = true;
        setEnable(false);
    } else {
        setEnable(m_networkInter->IsDeviceEnabled(QDBusObjectPath(m_devPath)));
        setStrength(getActiveAccessPointStrength(m_activeAp));
        setState(getDeviceState(m_devPath));
        generateIconUrl();

        connect(m_networkInter, &Network::DeviceEnabled, this, [=](QString devPath, bool enabled) {
            if (devPath == m_devPath) {
                setEnable(enabled);
            }
        });

        connectToActiveAccessPoint(m_devPath);
        connectToDeviceState(m_devPath);
    }
}

void CWireless::parseDevicesJsonInfo(QString key)
{
    const QJsonDocument doc = QJsonDocument::fromJson(m_devices.toLocal8Bit());
    QJsonObject obj = doc.object();
    QJsonArray array = obj["wireless"].toArray();

    QJsonValue value = array[0];
    obj = value.toObject();

    //后期可继续补充需要解析的字段
    if (key == "Path") {
        setDevPath(obj["Path"].toString());
    } else if (key == "ActiveAp") {
        setActiveAp(obj["ActiveAp"].toString());
    }
}

void CWireless::setEnable(bool enable)
{
    if (!enable != !m_enable) {
        qDebug() << "Enable changed" << enable;
        m_enable = enable;

        emit enableChanged(m_enable);
    }
}

void CWireless::setState(quint32 state)
{
    if (state != m_state) {
        qDebug() << "State changed" << state;
        m_state = state;

        emit stateChanged(m_state);
    }
}

void CWireless::setDevices(QString devices)
{
    if (devices != m_devices) {
        qDebug() << "Devices changed" << devices;
        m_devices = devices;

        emit devicesChanged(m_devices);
    }
}

void CWireless::setDevPath(QString devPath)
{
    if (devPath != m_devPath) {
        qDebug() << "DevPath changed" << devPath;
        m_devPath = devPath;

        emit devPathChanged(m_devPath);
    }
}

void CWireless::setActiveAp(QString activeAP)
{
    if (activeAP != m_activeAp) {
        qDebug() << "ActiveAp changed" << activeAP;
        m_activeAp = activeAP;
        //监听WIFI信号强度变化
        QDBusConnection::systemBus().connect(nmService, m_activeAp, interfaceDBusProps,
                                             memberPropsChanged, "sa{sv}as", this, SLOT(handleStrengthChangedSignal(QDBusMessage)));

        emit activeApChanged(m_activeAp);
    }
}

void CWireless::setStrength(quint32 strength)
{
    if (strength != m_strength) {
        qDebug() << "Strength changed" << strength;
        m_strength = strength;

        emit strengthChanged(m_strength);
    }
}

void CWireless::setIconUrl(QString iconUrl)
{
    if (iconUrl != m_iconUrl) {
        m_iconUrl = iconUrl;

        emit iconUrlChanged(m_iconUrl);
    }
}

quint32 CWireless::getActiveAccessPointStrength(QString activeAP)
{
    if (activeAP == "") {
        return 0;
    } else {
        QDBusInterface inter(nmService, activeAP, nmAccessPoint, QDBusConnection::systemBus(), this);
        return inter.property("Strength").toUInt();
    }
}

quint32 CWireless::getDeviceState(QString devPath)
{
    if (devPath == "") {
        return 0;
    } else {
        QDBusInterface inter(nmService, devPath, nmDevice, QDBusConnection::systemBus(), this);
        return inter.property("State").toUInt();
    }
}

void CWireless::connectToDeviceState(QString devPath)
{
    if (devPath == "") {
        return;
    } else {
        QDBusConnection::systemBus().connect(nmService, devPath, interfaceDBusProps,
                                             memberPropsChanged, "sa{sv}as", this, SLOT(handleStateChangedSignal(QDBusMessage)));
    }
}

void CWireless::connectToActiveAccessPoint(QString devPath)
{
    if (devPath == "") {
        return;
    } else {
        QDBusConnection::systemBus().connect(nmService, devPath, interfaceDBusProps,
                                             memberPropsChanged, "sa{sv}as", this, SLOT(handleActiveAccessPointChangedSignal(QDBusMessage)));
    }
}

void CWireless::connectToDeviceAdd(QString devices)
{
    if (devices == "") {
        return;
    } else {
        QDBusConnection::systemBus().connect(nmService, nmInterface, nmService, "DeviceAdded", this, SLOT(handleDeviceAddedSignal(const QDBusObjectPath &)));
    }
}

void CWireless::connectToDeviceRemove()
{
    QDBusConnection::systemBus().connect(nmService, nmInterface, nmService, "DeviceRemoved", this, SLOT(handleDeviceRemovedSignal(const QDBusObjectPath &)));
}

void CWireless::handleStrengthChangedSignal(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != nmAccessPoint)
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys = changedProps.keys();

    foreach (const QString &prop, keys) {
        if (prop == "Strength") {
            QVariant ret = changedProps[prop];
            setStrength(quint32(changedProps[prop].toInt()));
            generateIconUrl();
            break;
        }
    }
}

void CWireless::handleDeviceAddedSignal(const QDBusObjectPath &path)
{
    QString newDevPath = path.path();
    QDBusInterface inter(nmService, newDevPath, nmDevice, QDBusConnection::systemBus(), this);
    quint32 devType = inter.property("DeviceType").toUInt();
    //wifi设备的type值是2
    if (devType == 2) {
        //重新初始化一遍
        setDevPath(newDevPath);
        setEnable(m_networkInter->IsDeviceEnabled(QDBusObjectPath(m_devPath)));
        connectToActiveAccessPoint(newDevPath);
    }
}

void CWireless::handleDeviceRemovedSignal(const QDBusObjectPath &path)
{
    QString newDevPath = path.path();
    //只考虑一个wifi设备的情况
    if (m_devPath == newDevPath) {
        m_reinit = true;
        setEnable(false);
        setState(NM_DEVICE_STATE_PREPARE);
        setDevPath("");
        setActiveAp("");
        setStrength(0);
        setIconUrl("");
    }
}

void CWireless::handleStateChangedSignal(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != nmDevice)
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys = changedProps.keys();
    foreach (const QString &prop, keys) {
        if (prop == "State") {
            generateIconUrl();
            QVariant ret = changedProps[prop];
            setState(quint32(changedProps[prop].toInt()));
            break;
        }
    }
}

void CWireless::handleActiveAccessPointChangedSignal(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != nmDeviceWireless)
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys = changedProps.keys();
    foreach (const QString &prop, keys) {
        if (prop == "ActiveAccessPoint") {
            QDBusObjectPath path = changedProps[prop].value<QDBusObjectPath>();
            if (path.path() != "/") {
                setActiveAp(path.path());
                if (m_reinit) {
                    setStrength(getActiveAccessPointStrength(m_activeAp));
                    setState(getDeviceState(m_devPath));
                    generateIconUrl();
                    connect(m_networkInter, &Network::DeviceEnabled, this, [=](QString devPath, bool enabled) {
                        setEnable(enabled);
                        qDebug() << "DeviceEnabled changed" << devPath << enabled;
                    });
                    connectToDeviceState(m_devPath);
                    m_reinit = false;
                }
            }
            break;
        }
    }
}

bool CWireless::getEnabled()
{
    return m_enable;
}

quint32 CWireless::getActiveAPStrength()
{
    return m_strength;
}

quint32 CWireless::getState()
{
    return m_state;
}

QString CWireless::getIconUrl()
{
    return m_iconUrl;
}

void  CWireless::generateIconUrl()
{
    QString strength;
    QString iconUrl;

    if (m_state == NM_DEVICE_STATE_ACTIVATED) {
        if (m_strength <= 20) {
            strength = "none";
        } else if (m_strength > 20 && m_strength <= 40) {
            strength = "low";
        } else if (m_strength > 40 && m_strength <= 60) {
            strength = "medium";
        } else if (m_strength > 60 && m_strength <= 80) {
            strength = "high";
        } else if (m_strength > 80 && m_strength <= 100) {
            strength = "full";
        }
        iconUrl = homePath + urlPrefix + strength + urlSuffix;
        setIconUrl(iconUrl);
    } else if (m_state == NM_DEVICE_STATE_UNAVAILABLE) {
        iconUrl = homePath + "wireless-disabled" + urlSuffix;
        setIconUrl(iconUrl);
    } else if (m_state == NM_DEVICE_STATE_DISCONNECTED) {
        iconUrl = homePath + urlPrefix + "none" + urlSuffix;
        setIconUrl(iconUrl);
    } else if (m_state < NM_DEVICE_STATE_ACTIVATED
               && m_state > NM_DEVICE_STATE_DISCONNECTED) {
        //TODO
    } else {
        iconUrl = homePath + urlPrefix + "none" + urlSuffix;
        setIconUrl(iconUrl);
    }
}
