#pragma once
#include <Arduino.h>
#include "protocol.h"

#define LOOP_TIMEOUT 16
#define TIMEOUT 8
#define BAUD 9600

/*!
 * \brief Класс работы с Duino
 */
class Iskra
{
private:

    // Состояние каждого из пинов
    States _states[PINS_END + 1];

  /*
   * Конфигурация связи панели с нулевым усилием и выводов Duino
   * Отсчёт ведётся от верхнего правого угла против часовой стрелки
   * ZIF_07 и ZIF_15 в действительности подключены к питанию
   */
    static const int _config[];

    // Преобразовать логический индекс в физический
    static int pin(Pins id) {return _config[id];}

    static Errors serialPost(
            byte command,
            byte operand);

public:

    Iskra();

    // Состояние

    States getState(Pins id) {return _states[id];}
    void setState(
            Pins id,
            States state);

    // Значение

    bool getValue(
            Pins id,
            bool &value);
    bool setValue(
            Pins id,
            bool value);

    // Обмен

    static Errors serialGet(
            CommandsPC &command,
            Pins &pin);
    static Errors serialPost(
            CommandsDuino command,
            Pins pin);
    static Errors serialPostError(
            Errors code);

    void serialPostOutputs();
};
