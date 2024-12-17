#include "utilities.h"

#include <QDir>
#include <QFile>

bool CopyDirectory(const QString &source, const QString &destination)
{
    QDir sourceDir(source);
    if (!sourceDir.exists())
    {
        return false;
    }

    QDir destDir(destination);
    if (!destDir.exists())
    {
        destDir.mkpath(".");
    }

    foreach (QString fileName, sourceDir.entryList(QDir::Files))
    {
        QString srcFilePath = sourceDir.filePath(fileName);
        QString destFilePath = destDir.filePath(fileName);
        if (!QFile::copy(srcFilePath, destFilePath))
        {
            return false;
        }
    }

    foreach (QString subDirName, sourceDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString srcSubDirPath = sourceDir.filePath(subDirName);
        QString destSubDirPath = destDir.filePath(subDirName);
        if (!CopyDirectory(srcSubDirPath, destSubDirPath))
        {
            return false;
        }
    }

    return true;
}
