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
将后端蓝牙相关的实时数据以Component的形式展现
    \qml
        Loader {
            id: bluetoothLoader

            width: 100
            height: 100
            source: "Bluetooth.qml"
        }
    \endqml
*/

import QtQuick 2.0

Item {
    id: bluetoothComponent

    Rectangle {
        id: bluetoothRect

        width: parent.width
        height: parent.height
        /*! 当前蓝牙图标url链接 */
        property string iconUrl: ""

        Image {
            id: bluetooth

            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            source: bluetoothRect.iconUrl
        }

        Component.onCompleted: {
            bluetoothRect.iconUrl = CBluetooth.getIconUrl()
        }
    }
}
