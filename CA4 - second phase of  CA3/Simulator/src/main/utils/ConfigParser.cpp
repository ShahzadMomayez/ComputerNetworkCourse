#include "ConfigParser.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <QString>
#include <QVector>
#include "TopologyTypeUtil.h"

ConfigParser::ConfigParser(QString path) {
    QFile configFile(path);

    if(!configFile.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("Failed to open config file");
    }

    QByteArray configArray = configFile.readAll();
    configFile.close();

    QJsonDocument configDoc = QJsonDocument::fromJson(configArray);

    if(configDoc.isNull())
    {
        throw std::runtime_error("Failed to create JSON doc for config");
    }

    this->config = configDoc.object();
}

ConfigParser *
ConfigParser::instance(QString path)
{
    if(!self)
    {
        self = new ConfigParser(path);
    }

    return self;
}

void
ConfigParser::release()
{
    if(self)
    {
        delete self;
        self = nullptr;
    }
}

int ConfigParser::getRouterBufferSize() {
    return config["router_buffer_size"].toInt();
}

int ConfigParser::getRouterPortCount() {
    return config["router_port_count"].toInt();
}

QJsonArray ConfigParser::getASsConfig() {
    return config["Autonomous_systems"].toArray();
}

QJsonObject ConfigParser::getASConfig(int index) {
    return getASsConfig().at(index).toObject();
}

int ConfigParser::getASId(int index) {
    QJsonObject ASConfig = getASConfig(index);
    return ASConfig["id"].toInt();
}

int ConfigParser::getASNodeCount(int index) {
    QJsonObject ASConfig = getASConfig(index);
    return ASConfig["node_count"].toInt();
}

int ConfigParser::getASDHCPServer(int index) {
    QJsonObject ASConfig = getASConfig(index);
    return ASConfig["dhcp_server"].toInt();
}

bool
ConfigParser::useChunkServer()
{
    return config["data_server"] == "chunk_server";
}

QList<qsizetype>
ConfigParser::getSenderPCsIDs()
{
    QList<qsizetype> senderPCsIDs;

    for(auto value : config["sender_pcs"].toArray())
    {
        senderPCsIDs.append(value.toInt());
    }

    return senderPCsIDs;
}

qsizetype
ConfigParser::getPacketSize()
{
    return config["packet_size"].toInt();
}

QString
ConfigParser::getStreamingContentPath()
{
    return config["streaming_content"].toString();
}

QList<qsizetype>
ConfigParser::getReceiverPCsIDs()
{
    QList<qsizetype> receiverPCsIDs;

    for(auto value : config["receiver_pcs"].toArray())
    {
        receiverPCsIDs.append(value.toInt());
    }

    return receiverPCsIDs;
}

qsizetype
ConfigParser::getTCPWindowSize()
{
    return config["tcp_window_size"].toInt();
}

std::string ConfigParser::getASNetworkPortion(int index) {
    QJsonObject ASConfig = getASConfig(index);
    return ASConfig["network_portion"].toString().toStdString();
}

int ConfigParser::getAsCount() {
    return getASsConfig().size();
}

UT::TopologyType ConfigParser::getASTopologyType(int index) {
    QJsonObject ASConfig = getASConfig(index);
    return TopologyTypeUtil::parseTopologyString(ASConfig["topology_type"].toString().toStdString());
}

QJsonArray ConfigParser::getGatewaysArray(int index) {
    QJsonObject ASConfig = getASConfig(index);
    return ASConfig["gateways"].toArray();
}

QJsonArray ConfigParser::getConnectToASsArray(int index) {
    QJsonObject ASConfig = getASConfig(index);
    return ASConfig["connect_to_as"].toArray();
}

QMap<int, QList<int>> ConfigParser::getGateways(int index) {
    QJsonArray gatewaysArray = getGatewaysArray(index);
    
    QMap<int, QList<int>> gatewaysMap;

    for (const QJsonValue &value : gatewaysArray) {
        QJsonObject gateway = value.toObject();
        int node = gateway["node"].toInt();
        QJsonArray usersArray = gateway["users"].toArray();

        QList<int> users;

        for (const QJsonValue &user : usersArray) {
            users.append(user.toInt());
        }

        gatewaysMap.insert(node, users);
    }
    return gatewaysMap;
}

QVector<QPair<int, QVector<QPair<int, int>>>> ConfigParser::getConnectToASs(int index) {
    QVector<QPair<int, QVector<QPair<int, int>>>> connectToASMap;
    QJsonArray connectToASsArray = getConnectToASsArray(index);

    for (const QJsonValue &value : connectToASsArray) {
        QJsonObject connectToAS = value.toObject();

        int id = connectToAS["id"].toInt();
        QJsonArray gatewaysPairs = connectToAS["gateway_pairs"].toArray();

        QVector<QPair<int, int>> gatewaysPairsVector = toVector(gatewaysPairs);

        connectToASMap.append(qMakePair(id, gatewaysPairsVector));
    }

    return connectToASMap;
}

QVector<QPair<int, int>> ConfigParser::toVector(QJsonArray gatewaysPairs) {
    QVector<QPair<int, int>> gatewaysPairsVector;

    for (const QJsonValue &value : gatewaysPairs) { 
        QJsonObject gatewaysPair = value.toObject();
        int gateway = gatewaysPair["gateway"].toInt();
        int connectTo = gatewaysPair["connect_to"].toInt();

        gatewaysPairsVector.append(qMakePair(gateway, connectTo));
    }

    return gatewaysPairsVector;
}

int ConfigParser::getCycleDuration() {
    QString cycleDuration = config["cycle_duration"].toString();
    return parseTime(cycleDuration);
}

int ConfigParser::getSimulationDuration() {
    QString simulationDuration = config["simulation_duration"].toString();
    return parseTime(simulationDuration);
}

int ConfigParser::parseTime(QString time) {
    if (time.right(2) == "ms") {
        return time.mid(0, time.length() - 2).toInt();
    }
    if (time.right(1) == "s") {
        return time.mid(0, time.length() - 1).toInt() * 1000;
    }

    return time.mid(0, time.length() - 2).toInt();
}

QVector<int> ConfigParser::getBrokenRouters(int index) {
    QJsonObject ASConfig = getASConfig(index);
    QVector<int> brokenRouters;
    for (auto value : ASConfig["broken_routers"].toArray()) {
        brokenRouters.append(value.toInt());
    }

    return brokenRouters;
}
