#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>

#define MAXIMUM_RECENT_FILE_LIMIT 20

#define SESSION_FILE_NAME "sessions.commfy"

class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);

    // Read a session file. If not exist, create one
    QStringList ReadSessionsFile();

signals:
};

#endif // SESSIONMANAGER_H
