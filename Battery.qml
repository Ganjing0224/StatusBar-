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

/*!
将后端电池相关的实时数据以Component的形式展现
    \qml
        Loader {
            id: batteryLoader

            width: 100
            height: 100
            source: "Battery.qml"
        }
    \endqml
*/

import QtQuick 2.0

Item {
    id: batteryComponent

    Rectangle {
        id: batteryRect

        width: parent.width
        height: parent.height
        /*! 当前电池的状态：1表示充电，2表示放电，4表示已充满 */
        property int batteryStatus: 0
        /*! 当前电池电量百分比 */
        property double batteryPercentage: 0
        /*! 是否正在使用电池 */
        property bool onBattery: false
        /*! 当前电池性能模式：平衡模式，高性能模式，节能模式 */
        property string mode: ""
        /*! 剩余充电时间 */
        property int timeToFull: 0
        /*! 当前电池图标url链接 */
        property string iconUrl: ""

        Image {
            id: battery

            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            source: batteryRect.iconUrl
        }
    }

    Component.onCompleted: {
        batteryRect.batteryStatus = CBattery.getStatus()
        batteryRect.batteryPercentage = CBattery.getBatteryPercentage()
        batteryRect.onBattery = CBattery.getOnBattery()
        batteryRect.mode = CBattery.getMode()
        batteryRect.timeToFull = CBattery.getTimeToFull()
        batteryRect.iconUrl = CBattery.getIconUrl()
    }

    Connections {
        target: CBattery

        onStatusChanged: batteryRect.batteryStatus = status
        onBatteryPercentageChanged: batteryRect.batteryPercentage = batteryPercentage
        onOnBatteryChanged: batteryRect.onBattery = onBattery
        onModeChanged: batteryRect.mode = mode
        onTimeToFullChanged: batteryRect.timeToFull = time
        onIconUrlChanged: batteryRect.iconUrl = iconUrl
    }
}
