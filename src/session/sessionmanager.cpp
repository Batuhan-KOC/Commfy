#include "sessionmanager.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

SessionManager::SessionManager(QObject *parent)
    : QObject{parent}
{}

QStringList SessionManager::ReadSessionsFile()
{
    // Open the session file
    QFile sessionFile(SESSION_FILE_NAME);

    // Create one if does not exists
    if (!sessionFile.exists()) {
        sessionFile.open(QIODevice::WriteOnly);
        sessionFile.close();
    }

    sessionFile.open(QIODevice::ReadOnly);

    QStringList sessionsList;

    // Read sessions line by line
    QTextStream sessionFileTextStreamIn(&sessionFile);
    while (!sessionFileTextStreamIn.atEnd()) {
        QString sessionLine = sessionFileTextStreamIn.readLine();
        sessionsList << sessionLine;
    }

    // Close session file
    sessionFile.close();

    // Check the session files recorded whether they exist or not
    auto sessionFileExistControl = [](const QString &str) -> bool{
        QFileInfo sessionFileInfo(str);

        return !sessionFileInfo.exists();
    };

    auto sessionFileIterator = std::remove_if(sessionsList.begin(), sessionsList.end(), sessionFileExistControl);

    // Erase the removed elements from the list
    sessionsList.erase(sessionFileIterator, sessionsList.end());

    // Only first MAXIMUM_RECENT_FILE_LIMIT amount of recent file is allowed
    sessionsList = sessionsList.mid(0, MAXIMUM_RECENT_FILE_LIMIT);

    // Write only the existing session files to the session record but delete everything before opening it again
    sessionFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);

    // Open the session text stream
    QTextStream sessionFileTextStreamOut(&sessionFile);

    for(const QString &session: sessionsList){
        sessionFileTextStreamOut << session << "\n";
    }

    sessionFile.close();

    return sessionsList;
}
