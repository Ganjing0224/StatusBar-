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
将后端时间相关的实时数据以Component的形式展现
    \qml
        Loader {
            id: timeLoader

            width: 100
            height: 100
            source: "Timedate.qml"
        }
    \endqml
*/

import QtQuick 2.0

Item {
    id: timeComponent

    Rectangle {
        id: timeRect

        width: parent.width
        height: parent.height
        /*! 是否使用24小时制 */
        property bool is24HourSytem: true
        /*! 是否使用24小时制 */
        /*! shortTimeFormat = 0 时间格式: 9:40 */
        /*! shortTimeFormat = 1 时间格式: 09:40 */
        property int shortTimeFormat: 0

        Text {
            id: timeText

            anchors.centerIn: parent
        }

        Timer {
            id: timer

            interval: 1
            repeat: true
            onTriggered: {
                timeText.text = timeRect.currentDateTime()
            }
        }

        /*! 根据是否是24小时制,以及长短时间格式显示当前系统时间 */
        function currentDateTime() {
            if (timeRect.is24HourSytem) {
                if (timeRect.shortTimeFormat === 0) {
                    return Qt.formatDateTime(new Date(), "h:mm")
                } else if (timeRect.shortTimeFormat === 1) {
                    return Qt.formatDateTime(new Date(), "hh:mm")
                } else {
                    return "error"
                }
            } else {
                if (timeRect.shortTimeFormat === 0) {
                    return Qt.formatDateTime(new Date(), "h:mm ap")
                } else if (timeRect.shortTimeFormat === 1) {
                    return Qt.formatDateTime(new Date(), "hh:mm ap")
                } else {
                    return "error"
                }
            }
        }
    }

    Component.onCompleted: {
        timeRect.is24HourSytem = CTime.getIs24HourSystem()
        timeRect.shortTimeFormat = CTime.getShortTimeFormat()
        timeText.text = timeRect.currentDateTime()
        timer.start()
    }

    Connections {
        target: CTime

        onIs24HourSystemChanged: timeRect.is24HourSytem = flag
        onShortTimeFormatChanged: timeRect.shortTimeFormat = format
    }
}
