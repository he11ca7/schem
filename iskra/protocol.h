#pragma once

// TODO Ввести пространства имён

// Типы пинов
enum States
{
  STATES_BEGIN = 0,

  STATE_UNUSED = STATES_BEGIN,  // Запрещённое состояние
  STATE_V,                      // Питание
  STATE_GND,                    // Земля
  STATE_IN,                     // Входной пин
  STATE_OUT,                    // Выходной пин

  STATES_END = STATE_OUT
};

// Идентификаторы пинов
enum Pins
{
  PINS_BEGIN = 0b0000,

  PIN_00 = PINS_BEGIN,
  PIN_01 = 0b0001,
  PIN_02 = 0b0010,
  PIN_03 = 0b0011,
  PIN_04 = 0b0100,
  PIN_05 = 0b0101,
  PIN_06 = 0b0110,
  PIN_07 = 0b0111,

  PIN_08 = 0b1000,
  PIN_09 = 0b1001,
  PIN_10 = 0b1010,
  PIN_11 = 0b1011,
  PIN_12 = 0b1100,
  PIN_13 = 0b1101,
  PIN_14 = 0b1110,
  PIN_15 = 0b1111,

  PINS_END = PIN_15
};

// TODO
// Поделить на группы команд
// Сделать возможным отправку однобайтной команды (Пример -- Test)

// Команды pc --> duino
enum CommandsPC
{
  COMMANDSPC_BEGIN = 0,

  COM_SET_0 = COMMANDSPC_BEGIN,  // Установить 0
  COM_SET_1,                     // Установить 1

  COM_DEF_UNUSED,                // Задать незадействованным
  COM_DEF_V,                     // Задать питание
  COM_DEF_GND,                   // Задать землю
  COM_DEF_IN,                    // Задать вход
  COM_DEF_OUT,                   // Задать выход

  COM_SET,                       // Запросить значение
  COM_DEF,                       // Запросить тип

  COMMANDSPC_END = COM_DEF
};

// Команды duino --> pc
enum CommandsDuino
{
  COMMANDS_DUINO_BEGIN = 0,

  COM_VAL_0 = COMMANDS_DUINO_BEGIN, // В ответ на COM_SET
  COM_VAL_1,

  COM_GET_UNUSED, // В ответ на COM_DEF
  COM_GET_V,
  COM_GET_GND,
  COM_GET_IN,
  COM_GET_OUT,

  COM_ERROR, // Ошибка

  COMMANDS_DUINO_END = COM_ERROR
};

// Коды ошибок
enum Errors
{
  ERRORS_BEGIN = 0,

  ERROR_SUCCESS = ERRORS_BEGIN,

  ERROR_UNKNOWN_COMMAND,  // Неизвестная команда
  ERROR_UNKNOWN_PIN,      // Неизвестный пин

  ERROR_SERIAL_DEFICIT,   // Данных недостаточно
  ERROR_SERIAL_INTERNAL,  // Внутренняя ошибка

  ERROR_ILLEGAL_COMMAND,  // Некорректная попытка чтения/записи

  ERRORS_END = ERROR_ILLEGAL_COMMAND
};

// TODO Ввести команду, инициирующую оповещение Duino о состоянии всех пинов
