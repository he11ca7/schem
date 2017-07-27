#include "globals.h"

QString G_PATH = QString();

QString CommandName(
    CommandsDuino command)
{
  switch (command)
    {
    case COM_VAL_0:      return "0";
    case COM_VAL_1:      return "1";
    case COM_GET_UNUSED: return "Не задействован";
    case COM_GET_V:      return "Питание";
    case COM_GET_GND:    return "Земля";
    case COM_GET_IN:     return "Вход";
    case COM_GET_OUT:    return "Выход";
    case COM_ERROR:      return "Ошибка";
    default: return "";
    }
}

QString CommandName(
    CommandsPC command)
{
  switch (command)
    {
    case COM_SET_0:      return "Установить 0";
    case COM_SET_1:      return "Установить 1";
    case COM_DEF_UNUSED: return "Задать неиспользуемый";
    case COM_DEF_V:      return "Задать питание";
    case COM_DEF_GND:    return "Задать землю";
    case COM_DEF_IN:     return "Задать вход";
    case COM_DEF_OUT:    return "Задать выход";
    case COM_SET:        return "Запросить значение";
    case COM_DEF:        return "Запросить тип";
    default: return "";
    }
}

QString ErrorName(
    Errors code)
{
  switch (code)
    {
    case ERROR_SUCCESS:         return "Успех";
    case ERROR_UNKNOWN_COMMAND: return "Неизвестная команда";
    case ERROR_UNKNOWN_PIN:     return "Неизвестный пин";
    case ERROR_SERIAL_DEFICIT:  return "Данных недостаточно";
    case ERROR_SERIAL_INTERNAL: return "Внутренняя ошибка";
    case ERROR_ILLEGAL_COMMAND: return "Некорректная попытка чтения/записи";
    default: return "";
    }
}

QString StateName(
    int state)
{
  switch (state)
    {
    case STATE_UNUSED: return "STATE_UNUSED";
    case STATE_V:      return "STATE_V";
    case STATE_GND:    return "STATE_GND";
    case STATE_IN:     return "STATE_IN";
    case STATE_OUT:    return "STATE_OUT";
    default: return "";
    }
}

int findEnum(
    QString text,
    QString f(int),
    int begin,
    int end)
{
  for(int i = begin; i <= end; ++i)
    if(text == f(i))
      return i;
  return -1;
}
