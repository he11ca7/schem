#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QDebug>
#include "globals.h"
#include "duino.h"

/*!
 * \brief Виджет управления Duino
 */
class DuinoWidget : public QWidget
{
  Q_OBJECT

private:

  QComboBox
  *comboConnections,
  *comboCommands,
  *comboPins;
  QPushButton
  *buttonRefresh,
  *buttonConnect,
  *buttonDisconnect,
  *buttonExtra,
  *buttonSend;

  void createUI();

private slots:

  void comboConnectionsChanged(
      int index);
  void buttonRefreshClicked();
  void buttonConnectClicked();
  void buttonSendClicked();

public:

  DuinoWidget(
      QWidget *parent = nullptr);

public slots:

  void slotSerialConnected();
  void slotSerialDisconnected();

signals:

  void signalSerialConnect(
      QString name);
  void signalSerialDisconnect();
  void signalSerialPost(
      CommandsPC command,
      Pins pin);
};
