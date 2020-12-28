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

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QObject>
#include <com_deepin_daemon_bluetooth.h>

using  DBusBluetooth = com::deepin::daemon::Bluetooth;

class CBluetooth : public QObject
{
    Q_OBJECT
public:
    CBluetooth(QObject *parent = nullptr);
    ~ CBluetooth();
    void setAdapter(QString adapter);
    void setEnabled(bool enabled);
    void setIconUrl(QString iconUrl);

    //供QML调用的接口函数,主要是在组件创建时初始化属性值
    Q_INVOKABLE bool getEnabled();
    Q_INVOKABLE QString getIconUrl();

private:
    void parseAdapterJson();
    void connectToPowered(QString adapter);
    void generateIconUrl();
    void connectToAdapterAdd();
    void connectToAdapterRemove();

signals:
    void enableChanged(bool enabled);
    void iconUrlChanged(QString iconUrl);

public slots:
    void handlePoweredChangedSignal(const QDBusMessage &msg);
    void handleAdapterAddedSignal(const QString &adapter);
    void handleAdapterRemovedSignal(const QString &adapter);

private:
    DBusBluetooth *m_bluetoothInter;
    bool m_enabled; //蓝牙是否开启
    QString m_adapter; //蓝牙适配器路径
    QString m_iconUrl; //当前蓝牙图标url链接
};

#endif // BLUETOOTH_H
