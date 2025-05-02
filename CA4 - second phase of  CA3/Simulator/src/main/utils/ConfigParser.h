#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "../Globals/Globals.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

class ConfigParser
{
    ConfigParser(QString path);

public:
    static ConfigParser                          *instance(QString path = "");
    static void                                   release();

    int                                           getCycleDuration();
    int                                           getSimulationDuration();
    int                                           getRouterBufferSize();
    int                                           getRouterPortCount();
    int                                           getAsCount();
    int                                           getASId(int index);
    int                                           getASNodeCount(int index);
    int                                           getASDHCPServer(int index);

    bool                                          useChunkServer();
    QList<qsizetype>                              getSenderPCsIDs();
    qsizetype                                     getPacketSize();
    QString                                       getStreamingContentPath();
    QList<qsizetype>                              getReceiverPCsIDs();
    qsizetype                                     getTCPWindowSize();

    std::string                                   getASNetworkPortion(int index);
    UT::TopologyType                              getASTopologyType(int index);
    QMap<int, QList<int>>                         getGateways(int index);
    QVector<QPair<int, QVector<QPair<int, int>>>> getConnectToASs(int index);
    QVector<int>                                  getBrokenRouters(int index);


private:
    int                         parseTime(QString time);
    QJsonArray                  getASsConfig();
    QJsonObject                 getASConfig(int index);
    QJsonArray                  getGatewaysArray(int index);
    QJsonArray                  getConnectToASsArray(int index);

    QVector<QPair<int, int>> toVector(QJsonArray gatewaysPairs);

    QJsonObject                 config;

    inline static ConfigParser *self = nullptr;
};

#endif
