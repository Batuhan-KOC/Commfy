#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>

#include "sessiontype.h"

#define MAXIMUM_RECENT_FILE_LIMIT 20

#define SESSION_PYTHON_FOLDER_NAME "python"
#define SOURCE_PYTHON_FOLDER_NAME "python"
#define SOURCE_PYTHON_FOLDER_RELATIVE_PATH_COMPLETER "/../"

#define SESSION_FILE_NAME "sessions.commfy"

class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);

    // Read a session file. If not exist, create one
    QStringList ReadSessionsFile();

    // Create a new session based on new session name and location
    void CreateNewSession(QString name, QString location);

    // Adding the session file path to the SESSION_FILE_NAME file
    void AddNewSessionToTheSessionsFile(QString sessionFilePath);

    // Initializes the active session based on the session name and location
    void InstantiateActiveSession(QString sessionName, QString sessionFileLocation, QString sessionFolderLocation);

    // Save the current state of the active session
    void SaveActiveSession();

    // Closes the current active session
    void CloseActiveSession();

    // Deletes the active session object and assign it to null
    void DeleteActiveSession();

private:
    SessionType* activeSession{nullptr};

signals:
};

#endif // SESSIONMANAGER_H
