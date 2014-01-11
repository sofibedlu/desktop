/*
 * Copyright (C) by Duncan Mac-Vicar P. <duncan@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#ifndef MIRALL_FOLDERWATCHER_H
#define MIRALL_FOLDERWATCHER_H

#include "config.h"

#include "mirall/folder.h"

#if defined(Q_OS_WIN)
#include "mirall/folderwatcher_win.h"
#elif defined(Q_OS_MAC)
#include "mirall/folderwatcher_mac.h"
#elif defined(USE_INOTIFY)
#include "mirall/folderwatcher_qt.h"
#endif

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QHash>
#include <QScopedPointer>

class QTimer;

namespace Mirall {

class FolderWatcherPrivate;

/*
 * Folder Watcher monitors a directory and its sub directories
 * for changes in the local file system. Changes are signalled
 * through the folderChanged() signal.
 */

class FolderWatcher : public QObject
{
    Q_OBJECT
public:
    /**
     * @param root Path of the root of the folder
     */
    FolderWatcher(const QString &root, QObject *parent = 0L);

    /**
      * Set a file name to load a file with ignore patterns.
      *
      * Valid entries do not start with a hash sign (#)
      * and may contain wildcards
      */
    void addIgnoreListFile( const QString& );

    QStringList ignores() const;

    /**
     * Not all backends are recursive by default.
     * Those need to be notified when a directory is added or removed while the watcher is disabled.
     * This is a no-op for backend that are recursive
     */
    void addPath(const QString&);
    void removePath(const QString&);

signals:
    /** Emitted when one of the paths is changed */
    void folderChanged(const QString &path);

    /** Emitted if an error occurs */
    void error(const QString& error);

protected slots:
    // called from the implementations to indicate a change in path
    void changeDetected( const QString& path);

protected:
    QHash<QString, int> _pendingPathes;

private:
    QScopedPointer<FolderWatcherPrivate> _d;
    QString _root;
    QStringList _ignores;

    friend class FolderWatcherPrivate;
};

}

#endif
