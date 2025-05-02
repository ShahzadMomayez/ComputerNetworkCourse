#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>

class DataGenerator : public QObject
{
    Q_OBJECT

public:
    DataGenerator(QObject *parent = nullptr);
    int generateNumberOfPacket();

private:
    int iteration = 0;
};

#endif 
