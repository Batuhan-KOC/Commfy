#include "sessiontype.h"

SessionType::SessionType(QObject *parent)
    : QObject{parent}
{}

void SessionType::InstantiateSession(QString sessionName, QString sessionFileLocation, QString sessionFolderLocation)
{
    this->sessionName = sessionName;
    this->sessionFileLocation = sessionFileLocation;
    this->sessionFolderLocation = sessionFolderLocation;
}

void SessionType::SaveSession()
{

}

void SessionType::CloseSession()
{

}

void SessionType::StartSession()
{

}

void SessionType::StopSession()
{

}
