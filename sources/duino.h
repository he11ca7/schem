#pragma once

#include <QObject>
#include <QQueue>
#include <QThread>
#include <QTimer>
#include <QSerialPort>
#include <QString>
#include <QDebug>
#include "pin.h"
#include "chip.h"
#include "protocol.h"

#define TIMEOUT_READYREAD 100  // Таймаут ожидания недостающих данных
#define TIMEOUT_PARSE 32       // Таймаут отправки сообщений в Duino

/*!
 * \brief Интерфейс обмена Duino <--> PC
 */
class Duino : public QSerialPort
{
  Q_OBJECT

private:

  struct DuinoMessage
  {
    CommandsPC command;
    Pins pin;

    DuinoMessage(CommandsPC _command, Pins _pin) : command(_command), pin(_pin)
    {}
  };
  QQueue<DuinoMessage> _queueMessagesOut;

  void parseSlots(
      CommandsDuino command,
      Pins pin);

private slots:

  void serialParseIn();
  void serialParseOut();

public:

  Duino();

public slots:

  void slotConfigureChip(
      Chip *chip);

  bool serialConnect(
      QString port);
  void serialDisconnect();

  //---------
  // Отправка
  //---------

  void serialPost(
      CommandsPC command,
      Pins pin);

  void slotSetValue(
      Pins pin,
      bool value);
  void slotSetState(
      Pins pin,
      States state);

signals:

  void signalConnected();
  void signalDisconnected();
  void signalError(
      Errors error);
  void signalErrorInternal();

  //------
  // Приём
  //------

  void serialGet(
      CommandsDuino command,
      Pins pin);

  void signalSetValue(
      Pins pin,
      bool value);
  void signalSetState(
      Pins pin,
      States state);
};
