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

#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include "filewatcher.h"

FileWatcher* FileWatcher::m_pInstance = nullptr;

FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent)
{
}

// 监控目录
void FileWatcher::addWatchPath(QString path)
{
    qDebug() << QString("Add to watch: %1").arg(path);

    if (m_pInstance == nullptr) {
        m_pInstance = new FileWatcher();
        m_pInstance->m_pSystemWatcher = new QFileSystemWatcher();

        // 连接QFileSystemWatcher的directoryChanged和fileChanged信号到相应的槽
        connect(m_pInstance->m_pSystemWatcher, SIGNAL(directoryChanged(QString)),
                m_pInstance, SLOT(directoryUpdated(QString)));

        connect(m_pInstance->m_pSystemWatcher, SIGNAL(fileChanged(QString)),
                m_pInstance, SLOT(fileUpdated(QString)));
    }

    // 添加监控路径
    m_pInstance->m_pSystemWatcher->addPath(path);

    // 如果添加路径是一个目录，保存当前内容列表
    QFileInfo file(path);
    if (file.isDir()) {
        const QDir dirw(path);
        m_pInstance->m_currentContentsMap[path] = dirw.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    }
}

FileWatcher * FileWatcher::getInstance()
{
    return m_pInstance;
}

// 监控目录更新（添加、删除、重命名）
void FileWatcher::directoryUpdated(const QString &path)
{
    qDebug() << QString("Directory updated: %1").arg(path);

    // 比较最新的内容和保存的内容找出区别
    QStringList currEntryList = m_currentContentsMap[path];
    const QDir dir(path);

    QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

    QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);
    QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList);

    // 添加了文件
    QSet<QString> newFiles = newDirSet - currentDirSet;
    QStringList newFile = newFiles.toList();

    // 文件已被移除
    QSet<QString> deletedFiles = currentDirSet - newDirSet;
    QStringList deleteFile = deletedFiles.toList();

    // 更新当前设置
    m_currentContentsMap[path] = newEntryList;

    if (!newFile.isEmpty() && !deleteFile.isEmpty()) {
        // 文件重命名
        if ((newFile.count() == 1) && (deleteFile.count() == 1)) {
            qDebug() << QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
        }
    } else {
        // 添加新文件至Dir
        if (!newFile.isEmpty()) {
            qDebug() << "New File added: " << newFile;

            foreach (QString file, newFile) {
                emit fileAdded(file);
            }
        }

        // 从Dir中删除文件
        if (!deleteFile.isEmpty()) {
            qDebug() << "Dirs deleted: " << deleteFile;

            foreach(QString file, deleteFile) {
                emit fileDeleted(file);
            }
        }
    }
}

// 监控文件修改
void FileWatcher::fileUpdated(const QString &path)
{
    QFileInfo file(path);
    QString strPath = file.absolutePath();
    QString strName = file.fileName();

    qDebug() << QString("The file %1 at path %2 is updated").arg(strName).arg(strPath);
}
