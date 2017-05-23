#include "iskra.h"

void setup()
{
  Serial.begin(BAUD);
  while (!Serial) {;}
}

//--------------
// Основной цикл
//--------------

Iskra iskra;
CommandsPC command;
Pins pin;

void loop()
{
  // NOTE Как вариант поток можно обрабатывать посредством serialEvent()

  delay(LOOP_TIMEOUT);

  // TODO Сделать возможным вызывать части протокола внутри протокола

  Errors code = Iskra::serialGet(command, pin);
  switch (code)
    {
    case ERROR_SUCCESS:
      switch (command)
        {

        case COM_SET_0:
          if(iskra.setValue(pin, LOW))
            {
              Iskra::serialPost(COM_VAL_0, pin);
              iskra.serialPostOutputs();
            }
          else
            Iskra::serialPostError(ERROR_ILLEGAL_COMMAND);
          break;
        case COM_SET_1:
          if(iskra.setValue(pin, HIGH))
            {
              Iskra::serialPost(COM_VAL_1, pin);
              iskra.serialPostOutputs();
            }
          else
            Iskra::serialPostError(ERROR_ILLEGAL_COMMAND);
          break;

        case COM_DEF_UNUSED:
          iskra.setState(pin, STATE_UNUSED);
          Iskra::serialPost(COM_GET_UNUSED, pin);
          break;
        case COM_DEF_V:
          iskra.setState(pin, STATE_V);
          Iskra::serialPost(COM_GET_V, pin);
          break;
        case COM_DEF_GND:
          iskra.setState(pin, STATE_GND);
          Iskra::serialPost(COM_GET_GND, pin);
          break;
        case COM_DEF_IN:
          iskra.setState(pin, STATE_IN);
          Iskra::serialPost(COM_GET_IN, pin);
          break;
        case COM_DEF_OUT:
          iskra.setState(pin, STATE_OUT);
          Iskra::serialPost(COM_GET_OUT, pin);
          break;

        case COM_SET:
          bool value;
          if(iskra.getValue(pin, value))
            Iskra::serialPost(value ? COM_VAL_1 : COM_VAL_0, pin);
          else
            Iskra::serialPostError(ERROR_ILLEGAL_COMMAND);
          break;
        case COM_DEF:
          CommandsDuino commandDuino;
          switch (iskra.getState(pin))
            {
            case STATE_UNUSED: commandDuino = COM_GET_UNUSED; break;
            case STATE_V: commandDuino = COM_GET_V; break;
            case STATE_GND: commandDuino = COM_GET_GND; break;
            case STATE_IN: commandDuino = COM_GET_IN; break;
            case STATE_OUT: commandDuino = COM_GET_OUT; break;
            }
          Iskra::serialPost(commandDuino, pin);
          break;
        }
      break;
    case ERROR_SERIAL_DEFICIT:
      break;
    case ERROR_SERIAL_INTERNAL:
    case ERROR_ILLEGAL_COMMAND:
      Iskra::serialPostError(code);
      break;
    default: break;
    }
}
