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

import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

Window {
    id: win

    visible: true
    width: 600
    height: 680
    title: qsTr("DEMO")

    Rectangle {
        id: rect

        width: parent.width
        height: 40
        border.color: "blue"
        property int itemWidth: 50

        Loader {
            id: timeLoader

            width: rect.itemWidth
            height: rect.height
            anchors {
                left: parent.left
                top: parent.top
            }
            source: "Timedate.qml"
        }

        Loader {
            id: test

            width: rect.itemWidth
            height: rect.height
            anchors {
                right: wirelessLoader.left
                rightMargin: wirelessLoader.width
                top: parent.top
            }
        }

        Loader {
            id: wirelessLoader

            width: rect.itemWidth
            height: rect.height
            anchors {
                right: batteryLoader.left
                rightMargin: bluetoothLoader.width
                top: parent.top
            }
        }

        Loader {
            id: bluetoothLoader

            width: rect.itemWidth
            height: rect.height
            anchors {
                right: batteryLoader.left
                top: parent.top
            }
        }

        Loader {
            id: batteryLoader

            width: rect.itemWidth
            height: rect.height
            anchors {
                right: parent.right
                top: parent.top
            }
            source: "Battery.qml"
        }
    }

    Component.onCompleted: {
        wirelessLoader.source = CWireless.getEnabled() ? "Wireless.qml" : ""
        bluetoothLoader.source = CBluetooth.getEnabled() ? "Bluetooth.qml" : ""
    }

    Connections {
        target: CWireless

        onEnableChanged: enabled ? wirelessLoader.source = "Wireless.qml" : wirelessLoader.source = ""
    }

    Connections {
        target: CBluetooth

        onEnableChanged: {
            if (enabled) {
                bluetoothLoader.source = "Bluetooth.qml"
                bluetoothLoader.width = rect.itemWidth
            } else {
                bluetoothLoader.source = ""
                bluetoothLoader.width = 0
            }
        }
    }

    Connections {
        target: FileWatcher

        onFileAdded: test.source = "Bluetooth.qml"
        onFileDeleted: test.source = ""
    }
}
