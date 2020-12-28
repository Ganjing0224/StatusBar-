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

#ifndef WIRELESS_H
#define WIRELESS_H

#include <QObject>
#include <com_deepin_daemon_network.h>

using Network=com::deepin::daemon::Network;

class CWireless : public QObject
{
    Q_OBJECT
public:
    CWireless(QObject *parent = nullptr);
    ~ CWireless();
    void setEnable(bool enable);
    void setState(quint32 state);
    void setDevices(QString devices);
    void setDevPath(QString devPath);
    void setActiveAp(QString activeAP);
    void setStrength(quint32 strength);
    void setIconUrl(QString iconUrl);

    //供QML调用的接口函数,主要是在组件创建时初始化属性值
    Q_INVOKABLE bool getEnabled();
    Q_INVOKABLE quint32 getActiveAPStrength();
    Q_INVOKABLE quint32 getState();
    Q_INVOKABLE QString getIconUrl();

private:
    void parseDevicesJsonInfo(QString key);
    quint32 getActiveAccessPointStrength(QString activeAP);
    quint32 getDeviceState(QString devPath);
    void connectToDeviceState(QString devPath);
    void connectToActiveAccessPoint(QString devPath);
    void connectToDeviceAdd(QString devices);
    void connectToDeviceRemove();
    void generateIconUrl();
    void init();

signals:
    void enableChanged(bool enabled);
    void stateChanged(quint32 state);
    void devicesChanged(QString devices);
    void devPathChanged(QString devpath);
    void activeApChanged(QString activeAp);
    void strengthChanged(quint32 strength);
    void iconUrlChanged(QString iconUrl);

public slots:
    void handleStrengthChangedSignal(const QDBusMessage &msg);
    void handleStateChangedSignal(const QDBusMessage &msg);
    void handleActiveAccessPointChangedSignal(const QDBusMessage &msg);
    void handleDeviceAddedSignal(const QDBusObjectPath &path);
    void handleDeviceRemovedSignal(const QDBusObjectPath &path);

private:
    Network *m_networkInter;
    bool m_enable; //WIFI功能是否开启
    quint32 m_state; //WIFI当前连接状态
    QString m_devices;
    QString m_devPath; //wireless device path
    QString m_activeAp; //WIFI当前接入的AccessPoint
    quint32 m_strength; //WIFI信号强度
    QString m_iconUrl; //当前wifi图标url链接
    bool m_reinit; //是否需要重新初始化
};

#endif // WIRELESS_H
