#ifndef SESSIONTYPE_H
#define SESSIONTYPE_H

#include <QObject>

class SessionType : public QObject
{
    Q_OBJECT
public:
    explicit SessionType(QObject *parent = nullptr);

    void InstantiateSession(QString sessionName, QString sessionFileLocation, QString sessionFolderLocation);

    void SaveSession();

    void CloseSession();

private:
    QString sessionName;
    QString sessionFileLocation;
    QString sessionFolderLocation;

signals:
};

#endif // SESSIONTYPE_H
