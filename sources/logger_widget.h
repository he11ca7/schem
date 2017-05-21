#pragma once

#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include "globals.h"
#include "protocol.h"
#include "duino.h"

/*!
 * \brief Виджет логера
 */
class LoggerWidget : public QListWidget
{
  Q_OBJECT

private:

  QMenu *menu;

  virtual void mousePressEvent(
      QMouseEvent *event);

public:

  LoggerWidget(
      QWidget *parent = nullptr);

  enum LoggerCodes
  {
    CODE_REGULAR,
    CODE_ERROR,
    CODE_CRITICAL
  };

  void addItem(
      QString text,
      LoggerCodes code = CODE_REGULAR);

private slots:

  void slotSerialGet(
      CommandsDuino command,
      Pins pin);
  void slotSerialConnected();
  void slotSerialDisconnected();
  void slotSerialError(
      Errors error);
  void slotSerialErrorInternal();
};
