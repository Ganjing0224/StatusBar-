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

#ifndef TYPE_H
#define TYPE_H

// device state
#define NM_DEVICE_STATE_UNKNOWN      0
#define NM_DEVICE_STATE_UNMANAGED    10
#define NM_DEVICE_STATE_UNAVAILABLE  20
#define NM_DEVICE_STATE_DISCONNECTED 30
#define NM_DEVICE_STATE_PREPARE      40
#define NM_DEVICE_STATE_CONFIG       50
#define NM_DEVICE_STATE_NEED_AUTH    60
#define NM_DEVICE_STATE_IP_CONFIG    70
#define NM_DEVICE_STATE_IP_CHECK     80
#define NM_DEVICE_STATE_SECONDARIES  90
#define NM_DEVICE_STATE_ACTIVATED    100
#define NM_DEVICE_STATE_DEACTIVATING 110
#define NM_DEVICE_STATE_FAILED       120

//battery state
#define StatusCharging 1
#define StatusDischarging 2
#define StatusNotCharging 3
#define StatusFull 4

#endif // TYPE_H
