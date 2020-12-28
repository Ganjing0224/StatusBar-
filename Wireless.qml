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
将后端无线网络相关的实时数据以Component的形式展现
    \qml
        Loader {
            id: wirelessLoader

            width: 100
            height: 100
            source: "Wireless.qml"
        }
    \endqml
*/

import QtQuick 2.0

Item {
    id: wirelessComponent

    Rectangle {
        id: wirelessRect

        width: parent.width
        height: parent.height
        /*! 网络连接动画过程中，当前显示的帧图片在imagelist中的索引 */
        property int currentIndex: 0
        /*! 网络连接动画需要播放的帧集合 */
        property var imagelist: ["/image/network-wireless-signal-low-symbolic-dark.svg"
                                ,"/image/network-wireless-signal-medium-symbolic-dark.svg"
                                ,"/image/network-wireless-signal-high-symbolic-dark.svg"
                                ,"/image/network-wireless-signal-full-symbolic-dark.svg"];
        /*! 网络当前状态 */
        property int state: 0
        /*! 当前wifi图标url链接 */
        property string iconUrl: ""

        Image {
            id: wireless

            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            source: wirelessRect.iconUrl
        }

        Timer {
            id: imageSwitch

            interval: 250
            repeat: true
            // 通过定时器，1秒播放完imagelist里的4帧图片，来实现WIFI连接的动画效果
            onTriggered: {
                wirelessRect.currentIndex = (wirelessRect.currentIndex + 1) % 4
                wirelessRect.iconUrl = wirelessRect.imagelist[wirelessRect.currentIndex]
            }
        }

        onStateChanged: {
            /*
                处于网络断开（#define NM_DEVICE_STATE_ACTIVATED 100）
                和网络可用（#define NM_DEVICE_STATE_DISCONNECTED 30）之间的状态，
                都认为是在连接网络中
            */
            if (wirelessRect.state < 100 && wirelessRect.state > 30) {
                if (!imageSwitch.running)
                    imageSwitch.start()
            } else {
                imageSwitch.stop()
            }
        }

        Component.onCompleted: {
            wirelessRect.state = CWireless.getState()
            wirelessRect.iconUrl = CWireless.getIconUrl()
        }

        Connections {
            target: CWireless

            onStateChanged: wirelessRect.state = state
            onIconUrlChanged: wirelessRect.iconUrl = iconUrl
        }
    }
}
