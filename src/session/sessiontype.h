#ifndef SESSIONTYPE_H
#define SESSIONTYPE_H

#include <QObject>

enum SessionProtocol{
    Udp,
    Tcp,
    Serial
};

class SessionType : public QObject
{
    Q_OBJECT
public:
    explicit SessionType(QObject *parent = nullptr);

    void InstantiateSession(QString sessionName, QString sessionFileLocation, QString sessionFolderLocation);

    void SaveSession();

    void CloseSession();

    // Start session execution
    void StartSession();

    // Stop session execution
    void StopSession();

private:
    QString sessionName;
    QString sessionFileLocation;
    QString sessionFolderLocation;

signals:
};

#endif // SESSIONTYPE_H
