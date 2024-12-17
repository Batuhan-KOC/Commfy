#ifndef SESSIONTYPE_H
#define SESSIONTYPE_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>

enum SessionSettingsType{
    SESSION_SETTINGS_PROTOCOL_TYPE = 0,
    SESSION_SETTINGS_TO_IP,
    SESSION_SETTINGS_FROM_IP,
    SESSION_SETTINGS_TO_PORT,
    SESSION_SETTINGS_FROM_PORT,
    SESSION_SETTINGS_TX_ENABLED,
    SESSION_SETTINGS_RX_ENABLED,
    SESSION_SETTINGS_SERIAL_DEVICE,
    SESSION_SETTINGS_BAUDRATE,
    SESSION_SETTINGS_FLOW_CONTROL,
    SESSION_SETTINGS_PARITY,
    SESSION_SETTINGS_STOP_BITS,
    SESSION_SETTINGS_BYTE_SIZE,
    SESSION_SETTINGS_LAST_ELEMENT,
};
Q_DECLARE_METATYPE(SessionSettingsType)

enum SessionProtocolType{
    PROTOCOL_UDP,
    PROTOCOL_TCP,
    PROTOCOL_SERIAL
};
Q_DECLARE_METATYPE(SessionProtocolType)

enum SessionBaudrate{
    BAUD_50 = 0,
    BAUD_75,
    BAUD_110,
    BAUD_134,
    BAUD_150,
    BAUD_200,
    BAUD_300,
    BAUD_600,
    BAUD_1200,
    BAUD_1800,
    BAUD_2400,
    BAUD_4800,
    BAUD_9600,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200,
    BAUD_230400,
    BAUD_460800,
    BAUD_500000,
    BAUD_576000,
    BAUD_921600,
    BAUD_1000000,
    BAUD_1152000,
    BAUD_1500000,
    BAUD_2000000,
    BAUD_2500000,
    BAUD_3000000,
    BAUD_3500000,
    BAUD_4000000,
};
Q_DECLARE_METATYPE(SessionBaudrate)

enum SessionFlowControl{
    FLOW_CONTROL_NO_FLOW_CONTROL = 0,
    FLOW_CONTROL_SOFTWARE_FLOW_CONTROL,
    FLOW_CONTROL_RTC_CTS,
    FLOW_CONTROL_DSR_DTR,
};
Q_DECLARE_METATYPE(SessionFlowControl)

enum SessionParity{
    PARITY_NONE = 0,
    PARITY_EVEN,
    PARITY_ODD,
    PARITY_MARK,
    PARITY_SPACE,
};
Q_DECLARE_METATYPE(SessionParity)

enum SessionStopBits{
    STOPBITS_ONE = 0,
    STOPBITS_ONE_POINT_FIVE,
    STOPBITS_TWO,
};
Q_DECLARE_METATYPE(SessionStopBits)

enum SessionByteSize{
    BYTESIZE_FIVE_BITS = 0,
    BYTESIZE_SIX_BITS,
    BYTESIZE_SEVEN_BITS,
    BYTESIZE_EIGHT_BITS,
};
Q_DECLARE_METATYPE(SessionByteSize)

struct SessionSettings{
    SessionProtocolType protocol{SessionProtocolType::PROTOCOL_UDP};
    QString toIp{"127.0.0.1"};
    QString fromIp{"127.0.0.1"};
    int toPort{5555};
    int fromPort{5555};
    bool txEnabled{false};
    bool rxEnabled{false};
    QString serialDevice{""};
    SessionBaudrate baudrate{BAUD_9600};
    SessionFlowControl flowControl{FLOW_CONTROL_NO_FLOW_CONTROL};
    SessionParity parity{PARITY_NONE};
    SessionStopBits stopBits{STOPBITS_ONE};
    SessionByteSize byteSize{BYTESIZE_EIGHT_BITS};
};

class SessionType : public QObject
{
    Q_OBJECT
public:
    explicit SessionType(QObject *parent = nullptr);

    void InstantiateSession(QString sessionName, QString sessionFileLocation, QString sessionFolderLocation);

    // Save session parameters to session file
    void SaveSession();

    // Close the current session
    void CloseSession();

    // Start session execution
    void StartSession();

    // Stop session execution
    void StopSession();

    // When triggered, session object fetches its settings parameters from session file
    void UpdateSessionParametersFromSessionFile();

    // Change session settings value with the given value
    void ChangeSettingsValue(SessionSettingsType type, QVariant value);

    // Return the protocol settings
    SessionSettings GetSettings();

private:
    QString sessionName;
    QString sessionFileLocation;
    QString sessionFolderLocation;
    SessionSettings settings;

    // Convert the settings object to a json file string content
    QJsonObject SessionSettingsToJson();
    // Update the session file content with current values
    void UpdateSessionSettings();

signals:
};

#endif // SESSIONTYPE_H
