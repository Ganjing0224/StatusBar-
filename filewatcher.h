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

#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QMap>
#include <QFileSystemWatcher>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    static void addWatchPath(QString path);
    static FileWatcher * getInstance();

signals:
    void fileAdded(QString fileName);
    void fileDeleted(QString fileName);

public slots:
    void directoryUpdated(const QString &path); //目录更新时调用，path是监控的路径
    void fileUpdated(const QString &path); //文件被修改时调用，path是监控的路径

private:
    explicit FileWatcher(QObject *parent = nullptr);

private:
    static FileWatcher *m_pInstance; //单例
    QFileSystemWatcher *m_pSystemWatcher; //QFileSystemWatcher变量
    QMap<QString, QStringList> m_currentContentsMap; //当前每个监控的内容目录列表
};

#endif // FILEWATCHER_H
