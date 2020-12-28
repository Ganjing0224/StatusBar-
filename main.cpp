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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "battery.h"
#include "bluetooth.h"
#include "filewatcher.h"
#include "timedate.h"
#include "wireless.h"

const QString dirPath = "/home/ganjing/work/items";

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    CWireless wireless;
    CTime time;
    CBluetooth bluetooth;
    CBattery battery;

    engine.rootContext()->setContextProperty("CWireless", &wireless);
    engine.rootContext()->setContextProperty("CTime", &time);
    engine.rootContext()->setContextProperty("CBluetooth", &bluetooth);
    engine.rootContext()->setContextProperty("CBattery", &battery);

    FileWatcher::addWatchPath(dirPath);
    engine.rootContext()->setContextProperty("FileWatcher", FileWatcher::getInstance());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
