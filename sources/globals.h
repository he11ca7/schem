#pragma once

#include <QString>
#include <QMetaType>
#include "protocol.h"

extern QString G_PATH; // Каталог приложения
#define G_PATH_DATA "/data/"

// NOTE Выполняется здесь для защиты protocol.h
Q_DECLARE_METATYPE(States)
Q_DECLARE_METATYPE(Pins)
Q_DECLARE_METATYPE(CommandsPC)
Q_DECLARE_METATYPE(CommandsDuino)
Q_DECLARE_METATYPE(Errors)

QString CommandName(
    CommandsDuino command);
QString CommandName(
    CommandsPC command);
QString ErrorName(
    Errors code);
QString StateName(
    int state);

int findEnum(
    QString text,
    QString f(int),
    int begin,
    int end);
