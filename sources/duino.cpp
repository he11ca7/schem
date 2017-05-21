#include "duino.h"

Duino::Duino()
{
  // NOTE Выполняется здесь для защиты protocol.h
  qRegisterMetaType<States>("States");
  qRegisterMetaType<Pins>("Pins");
  qRegisterMetaType<CommandsPC>("CommandsPC");
  qRegisterMetaType<CommandsDuino>("CommandsDuino");
  qRegisterMetaType<Errors>("Errors");
  qRegisterMetaType<Chip>("Chip");

  connect(this,
          SIGNAL(readyRead()),
          this,
          SLOT(serialParseIn())
          );

  QTimer *timer = new QTimer(this);
  connect(timer,
          SIGNAL(timeout()),
          this,
          SLOT(serialParseOut())
          );
  timer->start(TIMEOUT_PARSE);
}

void Duino::serialPost(
    CommandsPC command,
    Pins pin)
{
  _queueMessagesOut.enqueue(DuinoMessage(command, pin));
}

void Duino::slotConfigureChip(
    Chip *chip)
{
  for(int i = PINS_BEGIN; i <= PINS_END; ++i)
    slotSetState((Pins) i, chip->pin((Pins) i)._type);
}

bool Duino::serialConnect(
    QString port)
{
  setPortName(port);
  if(open(QSerialPort::ReadWrite))
    {
      setDataTerminalReady(false);
      if(!setBaudRate(QSerialPort::Baud9600)) qDebug() << "ERROR setBaudRate()";
      if(!setDataBits(QSerialPort::Data8)) qDebug() << "ERROR setDataBits()";
      if(!setParity(QSerialPort::NoParity)) qDebug() << "ERROR setParity()";
      if(!setStopBits(QSerialPort::OneStop)) qDebug() << "ERROR setStopBits()";
      if(!setFlowControl(QSerialPort::NoFlowControl)) qDebug() << "ERROR setFlowControl()";
      setDataTerminalReady(true);
      emit signalConnected();
      return true;
    }
  else
    {
      emit signalErrorInternal();
      return false;
    }
}

void Duino::serialDisconnect()
{
  if(isOpen())
    {
      emit signalDisconnected();
      close();
    }
}

void Duino::slotSetValue(
    Pins pin,
    bool value)
{
  serialPost(value ? COM_SET_1 : COM_SET_0, pin);
}

void Duino::slotSetState(
    Pins pin,
    States state)
{
  switch (state)
    {
    case STATE_UNUSED: serialPost(COM_DEF_UNUSED, pin); break;
    case STATE_V: serialPost(COM_DEF_V, pin); break;
    case STATE_GND: serialPost(COM_DEF_GND, pin); break;
    case STATE_IN: serialPost(COM_DEF_IN, pin); break;
    case STATE_OUT: serialPost(COM_DEF_OUT, pin); break;
    }
}

void Duino::serialParseIn()
{
  CommandsDuino command;

  qint64 available = bytesAvailable();
  if(available >= 2)
    {
      char buffer[2];
      read(buffer, 2);

      char
          &char0 = buffer[0],
          &char1 = buffer[1];

      if (char0 >= COMMANDS_DUINO_BEGIN &&
          char0 <= COMMANDS_DUINO_END)
        command = (CommandsDuino) char0;
      else
        emit signalErrorInternal();

      if(command == COM_ERROR)
        {
          if (char1 >= ERRORS_BEGIN &&
              char1 <= ERRORS_END)
            emit signalError((Errors) char1);
          else
            emit signalErrorInternal();
        }
      else
        {
          if (char1 >= PINS_BEGIN &&
              char1 <= PINS_END)
            {
              emit serialGet(command, (Pins) char1);
              parseSlots(command, (Pins) char1);
            }
          else
            emit signalErrorInternal();
        }
      if(bytesAvailable())
        serialParseIn();
    }
  else
    if(available == 1)
      // Ждём
      {
        if(waitForReadyRead(TIMEOUT_READYREAD))
          serialParseIn();
        else
          emit signalErrorInternal();
      }
}

void Duino::serialParseOut()
{
  if(_queueMessagesOut.isEmpty())
    return;

  DuinoMessage message = _queueMessagesOut.dequeue();
  char *data = new char[2];
  data[0] = (char) message.command;
  data[1] = (char) message.pin;
  if(isWritable())
    if(QSerialPort::write(data, 2) == 2)
      {
        delete [] data;
        return;
      }

  delete [] data;
  emit signalErrorInternal();
}

void Duino::parseSlots(
    CommandsDuino command,
    Pins pin)
{
  switch (command)
    {
    case COM_VAL_0: emit signalSetValue(pin, false); break;
    case COM_VAL_1: emit signalSetValue(pin, true); break;

    case COM_GET_UNUSED: emit signalSetState(pin, STATE_UNUSED); break;
    case COM_GET_V: emit signalSetState(pin, STATE_V); break;
    case COM_GET_GND: emit signalSetState(pin, STATE_GND); break;
    case COM_GET_IN: emit signalSetState(pin, STATE_IN); break;
    case COM_GET_OUT: emit signalSetState(pin, STATE_OUT); break;
    default: break;
    }
}
