#include "sessionmanager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDebug>

#include "../util/utilities.h"

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
        if(!sessionLine.isEmpty()){
            sessionsList << sessionLine;
        }
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

bool SessionManager::CreateNewSession(QString name, QString location)
{
    // Convert location to a QDir
    QDir dir(location);

    // Form the path to the new session folder
    QString sessionFolderPath = dir.filePath(name);

    // Delete the existing session folder if it exists
    if (dir.exists(name))
    {
        QDir sessionDir(sessionFolderPath);
        if (!sessionDir.removeRecursively())
        {
            qDebug() << "Failed to remove existing directory:" << sessionFolderPath;
            return false;
        }
    }

    // Create the new session folder
    if (!dir.mkdir(name))
    {
        qDebug() << "Failed to create directory:" << name;
        return false;
    }

    dir.cd(name);

    // Create the <name>.commfy file
    QString commfyFilePath = sessionFolderPath + "/" + name + ".commfy";
    QFile commfyFile(commfyFilePath);
    if (!commfyFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to create .commfy file:" << commfyFilePath;
        return false;
    }
    commfyFile.close();

    AddNewSessionToTheSessionsFile(commfyFilePath);

    // If there is an existing session, save it, close it, delete it
    if(activeSession != nullptr){
        SaveActiveSession();
        CloseActiveSession();
        DeleteActiveSession();
    }

    InstantiateActiveSession(name, commfyFilePath, sessionFolderPath);

    return true;
}

bool SessionManager::OpenSession(QString location)
{
    // If there is an existing session save it, close it, delete it
    if(activeSession != nullptr){
        SaveActiveSession();
        CloseActiveSession();
        DeleteActiveSession();
    }

    activeSession = new SessionType();

    // Parse session elements from file location
    QFileInfo fileInfo(location);

    QString sessionName = fileInfo.baseName();
    QString sessionFileLocation = location;
    QString sessionFolderLocation = fileInfo.absolutePath();

    activeSession->InstantiateSession(sessionName, sessionFileLocation, sessionFolderLocation);

    activeSession->UpdateSessionParametersFromSessionFile();

    emit UpdateSettingsWidgetsOnUI();

    return true;
}

SessionSettings SessionManager::GetActiveSettings(bool &ok)
{
    SessionSettings settings;

    if(activeSession == nullptr){
        ok = false;
        return settings;
    }

    ok = true;

    return activeSession->GetSettings();
}

void SessionManager::StartSession()
{
    if(activeSession != nullptr){
        activeSession->StartSession();
    }
}

void SessionManager::StopSession()
{
    if(activeSession != nullptr){
        activeSession->StopSession();
    }
}

void SessionManager::AddNewSessionToTheSessionsFile(QString sessionFilePath)
{
    // Open the session file
    QFile sessionFile(SESSION_FILE_NAME);

    // Create one if does not exists
    if (!sessionFile.exists()) {
        sessionFile.open(QIODevice::WriteOnly);
        sessionFile.close();
    }

    sessionFile.open(QIODevice::WriteOnly | QIODevice::Append);

    // Open the session text stream
    QTextStream sessionFileTextStreamOut(&sessionFile);

    sessionFileTextStreamOut << sessionFilePath << "\n";

    sessionFile.close();
}

void SessionManager::InstantiateActiveSession(QString sessionName, QString sessionFileLocation, QString sessionFolderLocation)
{
    activeSession = new SessionType();

    activeSession->InstantiateSession(sessionName, sessionFileLocation, sessionFolderLocation);
}

void SessionManager::SaveActiveSession()
{
    if(activeSession != nullptr){
        activeSession->SaveSession();
    }
}

void SessionManager::CloseActiveSession()
{
    if(activeSession != nullptr){
        activeSession->CloseSession();
    }
}

void SessionManager::DeleteActiveSession()
{
    if(activeSession != nullptr){
        delete activeSession;
    }

    activeSession = nullptr;
}

void SessionManager::SessionSettingsValueChanged(SessionSettingsType type, QVariant value)
{
    if(activeSession != nullptr){
        activeSession->ChangeSettingsValue(type, value);
    }
}

