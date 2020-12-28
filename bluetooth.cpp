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

#include "bluetooth.h"

const QString btDaemonService = "com.deepin.daemon.Bluetooth";
const QString btDaemonInterface = "/com/deepin/daemon/Bluetooth";
const QString bluetoothService = "org.bluez";
const QString bluetoothAdapter = "org.bluez.Adapter1";
const QString interfaceDBusProps = "org.freedesktop.DBus.Properties";
const QString memberPropsChanged = "PropertiesChanged";

const QString homePath = "/image/";
const QString urlPrefix = "bluetooth-active";
const QString urlSuffix = "-symbolic-dark.svg";

CBluetooth::CBluetooth(QObject *parent)
    : QObject(parent)
    , m_bluetoothInter(new DBusBluetooth(btDaemonService, btDaemonInterface, QDBusConnection::sessionBus(), this))
    , m_enabled(false)
    , m_adapter("")
    , m_iconUrl("")
{
    parseAdapterJson();
    generateIconUrl();
    //监听设备增加事件
    connectToAdapterAdd();
    //监听设备删除事件
    connectToAdapterRemove();
}

CBluetooth:: ~ CBluetooth()
{
}

 void CBluetooth::parseAdapterJson()
 {
     QDBusReply<QString> reply = m_bluetoothInter->GetAdapters();
     QJsonDocument doc = QJsonDocument::fromJson(reply.value().toUtf8());

     //没有检测到蓝牙适配器
     if (!doc.array().empty()) {
         //目前只考虑一个蓝牙适配器的情况
         QJsonValue value = doc.array()[0];
         QJsonObject obj = value.toObject();

         setAdapter(obj["Path"].toString());
         setEnabled(obj["Powered"].toBool());
     }
}

void CBluetooth::setAdapter(QString adapter)
{
    if (adapter != m_adapter) {
        qDebug() << "setAdapter" << adapter;
        m_adapter = adapter;
        connectToPowered(m_adapter);
    }
}

void CBluetooth::setEnabled(bool enabled)
{
    if (!enabled != !m_enabled) {
        qDebug() << "setEnabled" << enabled;
        m_enabled = enabled;

        emit enableChanged(m_enabled);
    }
}

void CBluetooth::setIconUrl(QString iconUrl)
{
    if (iconUrl != m_iconUrl) {
        m_iconUrl = iconUrl;

        emit iconUrlChanged(m_iconUrl);
    }
}

bool CBluetooth::getEnabled()
{
    return m_enabled;
}

void CBluetooth::connectToPowered(QString adapter)
{
    if (adapter == "") {
        return;
    } else {
        QDBusConnection::systemBus().connect(bluetoothService, adapter, interfaceDBusProps,
                                             memberPropsChanged, "sa{sv}as", this, SLOT(handlePoweredChangedSignal(QDBusMessage)));
    }
}

void CBluetooth::handlePoweredChangedSignal(const QDBusMessage &msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QString interfaceName = msg.arguments().at(0).toString();
    if (interfaceName != bluetoothAdapter)
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    QStringList keys = changedProps.keys();

    foreach (const QString &prop, keys) {
        if (prop == "Powered") {
            QVariant ret = changedProps[prop];
            setEnabled(changedProps[prop].toBool());
            break;
        }
    }
}

void CBluetooth::handleAdapterAddedSignal(const QString &adapter)
{
    QString adapterPath = (adapter.section(':', 1, 1).trimmed()).section('"', 1, 1);
    QDBusInterface inter(bluetoothService, adapterPath, bluetoothAdapter, QDBusConnection::systemBus(), this);
    bool powered = inter.property("Powered").toBool();
    qDebug() << "handleAdapterAddedSignal" << adapterPath << powered;

    setAdapter(adapterPath);
    setEnabled(powered);
}

void CBluetooth::handleAdapterRemovedSignal(const QString &adapter)
{
    QString adapterPath = (adapter.section(':', 1, 1).trimmed()).section('"', 1, 1);
    qDebug() << "handleAdapterRemovedSignal" << adapterPath << ":" << m_adapter;

    if (adapterPath == m_adapter) {
        setAdapter("");
    }
}

QString CBluetooth::getIconUrl()
{
    return m_iconUrl;
}

void  CBluetooth::generateIconUrl()
{
    QString iconUrl;
    iconUrl = homePath + urlPrefix + urlSuffix;
    setIconUrl(iconUrl);
}

void CBluetooth::connectToAdapterAdd()
{
    QDBusConnection::sessionBus().connect(btDaemonService, btDaemonInterface,
                                          btDaemonService, "AdapterAdded",
                                          this, SLOT(handleAdapterAddedSignal(const QString &)));
}

void CBluetooth::connectToAdapterRemove()
{
    QDBusConnection::sessionBus().connect(btDaemonService, btDaemonInterface,
                                          btDaemonService, "AdapterRemoved",
                                          this, SLOT(handleAdapterRemovedSignal(const QString &)));
}
