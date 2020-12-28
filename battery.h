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

#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusMessage>

class CBattery : public QObject
{
    Q_OBJECT
public:
    CBattery(QObject *parent = nullptr);
    ~ CBattery();
    void setStatus(quint32 status);
    void setBatteryPercentage(double percentage);
    void setOnBattery(bool onBattery);
    void setMode(QString mode);
    void setTimeToFull(quint32 time);
    void setIconUrl(QString iconUrl);

    //供QML调用的接口函数,主要是在组件创建时初始化属性值
    Q_INVOKABLE quint32 getStatus();
    Q_INVOKABLE double getBatteryPercentage();
    Q_INVOKABLE bool getOnBattery();
    Q_INVOKABLE QString getMode();
    Q_INVOKABLE quint32 getTimeToFull();
    Q_INVOKABLE QString getIconUrl();

private:
    void connectToStatus();
    void connectToBatteryPercentage();
    void connectToOnBattery();
    void connectToMode();
    void connectToTimeToFull();
    quint32 getStatusProp();
    double getBatteryPercentageProp();
    bool getOnBatteryProp();
    QString getModeProp();
    quint32 getTimeToFullProp();
    void generateIconUrl();

signals:
    void statusChanged(quint32 status);
    void batteryPercentageChanged(double batteryPercentage);
    void onBatteryChanged(bool onBattery);
    void modeChanged(QString mode);
    void timeToFullChanged(quint32 time);
    void iconUrlChanged(QString iconUrl);

public slots:
    void handleStatusChangedSignal(const QDBusMessage &msg);
    void handleBatteryPercentageChangedSignal(const QDBusMessage &msg);
    void handleOnBatteryChangedSignal(const QDBusMessage &msg);
    void handleModeChangedSignal(const QDBusMessage &msg);
    void handleTimeToFullChangedSignal(const QDBusMessage &msg);

private:
    quint32 m_status; //当前电池的状态：1表示充电，2表示放电，4表示已充满
    double m_batteryPercentage; //当前电池电量百分比
    bool m_onBattery; //是否正在使用电池
    QString m_mode; //当前电池性能模式：平衡模式，高性能模式，节能模式
    quint32 m_timeToFull; //剩余充电时间
    QString m_iconUrl; //剩余充电时间
};

#endif // BATTERY_H
