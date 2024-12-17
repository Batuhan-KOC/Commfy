#include "sessiontype.h"

#include <QTextStream>
#include <QFile>

#define SESSION_PROTOCOL_TYPE_KEYWORD       "PROTOCOL"
#define SESSION_TO_IP_KEYWORD               "TO_IP"
#define SESSION_TO_PORT_KEYWORD             "TO_PORT"
#define SESSION_FROM_IP_KEYWORD             "FROM_IP"
#define SESSION_FROM_PORT_KEYWORD           "FROM_PORT"
#define SESSION_TX_ENABLED_KEYWORD          "TX_ENABLED"
#define SESSION_RX_ENABLED_KEYWORD          "RX_ENABLED"
#define SESSION_SERIAL_DEVICE_NAME_KEYWORD  "SERIAL_DEVICE_NAME"
#define SESSION_BAUDRATE_KEYWORD            "BAUDRATE"
#define SESSION_FLOW_CONTROL_KEYWORD        "FLOW_CONTROL"
#define SESSION_PARITY_KEYWORD              "PARITY"
#define SESSION_STOP_BITS_KEYWORD           "STOP_BITS"
#define SESSION_BYTE_SIZE_KEYWORD           "BYTE_SIZE"

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
    UpdateSessionSettings();
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

void SessionType::UpdateSessionParametersFromSessionFile()
{
    QFile sessionFile(sessionFileLocation);
    if (!sessionFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open the file.");
        return;
    }

    QByteArray fileData = sessionFile.readAll();
    sessionFile.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(fileData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return;
    }

    QJsonObject settingsJsonObject = doc.object();

    settings.protocol = static_cast<SessionProtocolType>(settingsJsonObject[SESSION_PROTOCOL_TYPE_KEYWORD].toInt());
    settings.toIp = settingsJsonObject[SESSION_TO_IP_KEYWORD].toString();
    settings.fromIp = settingsJsonObject[SESSION_FROM_IP_KEYWORD].toString();
    settings.toPort = settingsJsonObject[SESSION_TO_PORT_KEYWORD].toInt();
    settings.fromPort = settingsJsonObject[SESSION_FROM_PORT_KEYWORD].toInt();
    settings.txEnabled = settingsJsonObject[SESSION_TX_ENABLED_KEYWORD].toBool();
    settings.rxEnabled = settingsJsonObject[SESSION_RX_ENABLED_KEYWORD].toBool();
    settings.serialDevice = settingsJsonObject[SESSION_SERIAL_DEVICE_NAME_KEYWORD].toString();
    settings.baudrate = static_cast<SessionBaudrate>(settingsJsonObject[SESSION_BAUDRATE_KEYWORD].toInt());
    settings.flowControl = static_cast<SessionFlowControl>(settingsJsonObject[SESSION_FLOW_CONTROL_KEYWORD].toInt());
    settings.parity = static_cast<SessionParity>(settingsJsonObject[SESSION_PARITY_KEYWORD].toInt());
    settings.stopBits = static_cast<SessionStopBits>(settingsJsonObject[SESSION_STOP_BITS_KEYWORD].toInt());
    settings.byteSize = static_cast<SessionByteSize>(settingsJsonObject[SESSION_BYTE_SIZE_KEYWORD].toInt());
}

void SessionType::ChangeSettingsValue(SessionSettingsType type, QVariant value)
{
    switch (type) {
    case SessionSettingsType::SESSION_SETTINGS_PROTOCOL_TYPE:
        settings.protocol = value.value<SessionProtocolType>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_TO_IP:
        settings.toIp = value.value<QString>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_FROM_IP:
        settings.fromIp = value.value<QString>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_TO_PORT:
        settings.toPort = value.value<int>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_FROM_PORT:
        settings.fromPort = value.value<int>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_TX_ENABLED:
        settings.txEnabled = value.value<bool>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_RX_ENABLED:
        settings.rxEnabled = value.value<bool>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_SERIAL_DEVICE:
        settings.serialDevice = value.value<QString>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_BAUDRATE:
        settings.baudrate = value.value<SessionBaudrate>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_FLOW_CONTROL:
        settings.flowControl = value.value<SessionFlowControl>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_PARITY:
        settings.parity = value.value<SessionParity>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_STOP_BITS:
        settings.stopBits = value.value<SessionStopBits>();
        break;
    case SessionSettingsType::SESSION_SETTINGS_BYTE_SIZE:
        settings.byteSize = value.value<SessionByteSize>();
        break;
    default:
        break;
    }
}

SessionSettings SessionType::GetSettings()
{
    return settings;
}

QJsonObject SessionType::SessionSettingsToJson()
{
    QJsonObject json;

    json[SESSION_PROTOCOL_TYPE_KEYWORD]         = settings.protocol;
    json[SESSION_TO_IP_KEYWORD]                 = settings.toIp;
    json[SESSION_FROM_IP_KEYWORD]               = settings.fromIp;
    json[SESSION_TO_PORT_KEYWORD]               = settings.toPort;
    json[SESSION_FROM_PORT_KEYWORD]             = settings.fromPort;
    json[SESSION_TX_ENABLED_KEYWORD]            = settings.txEnabled;
    json[SESSION_RX_ENABLED_KEYWORD]            = settings.rxEnabled;
    json[SESSION_SERIAL_DEVICE_NAME_KEYWORD]    = settings.serialDevice;
    json[SESSION_BAUDRATE_KEYWORD]              = settings.baudrate;
    json[SESSION_FLOW_CONTROL_KEYWORD]          = settings.flowControl;
    json[SESSION_PARITY_KEYWORD]                = settings.parity;
    json[SESSION_STOP_BITS_KEYWORD]             = settings.stopBits;
    json[SESSION_BYTE_SIZE_KEYWORD]             = settings.byteSize;

    return json;
}

void SessionType::UpdateSessionSettings()
{
    QFile sessionFile(sessionFileLocation);

    // If file is missing, create it again
    if(!sessionFile.exists()){
        sessionFile.open(QIODevice::WriteOnly);
        sessionFile.close();
    }

    sessionFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QJsonObject json = SessionSettingsToJson();
    QJsonDocument doc(json);

    sessionFile.write(doc.toJson());
    sessionFile.close();
}
