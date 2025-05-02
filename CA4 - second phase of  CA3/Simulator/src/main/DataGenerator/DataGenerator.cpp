#include "DataGenerator.h"
#include <QRandomGenerator>
#include <cmath>

#define PARETO_X_M 1
#define PARETO_ALPHA 1
#define SEED 0

DataGenerator::DataGenerator(QObject *parent)
    : QObject(parent) {}

int DataGenerator::generateNumberOfPacket() {
    QRandomGenerator customRng(SEED + iteration++);
    double randomValue = customRng.generateDouble();

    return std::floor(PARETO_X_M * std::pow(1 - randomValue, -1.0 / PARETO_ALPHA));
}
