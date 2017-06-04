#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QLayout>
#include <QPushButton>
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include "globals.h"
#include "protocol.h"
#include "duino.h"

/*!
 * \brief Виджет логера
 */
class LoggerWidget : public QGroupBox
{
  Q_OBJECT

private:

  QListWidget *_list;
  QPushButton *_buttonClear;

  void createUI();

public:

  LoggerWidget(
      QWidget *parent = nullptr);

  enum LoggerCodes
  {
    CODE_REGULAR,
    CODE_ERROR,
    CODE_CRITICAL
  };

public slots:

  void addItem(
      QString text);
  void addItem(
      QString text,
      LoggerCodes code);

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
