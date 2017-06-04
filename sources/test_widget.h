#pragma once

#include <QStringList>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QLayout>
#include <QDebug>
#include "pin.h"
#include "protocol.h"
#include "globals.h"

/*!
 * \brief Виджет тестирования
 */
class TestWidget : public QGroupBox
{
  Q_OBJECT

private:

  int level;
  bool flagTestOver;
  QMap<int, bool> mapLastValues;

  QComboBox *comboTests;
  QPushButton *buttonSelect;
  QTableWidget *table;

  void createUI();
  void resetUI();

private slots:

  void slotComboTestsIndexChanged(
      int index);
  void slotButtonSelectClicked();

public:

  TestWidget(QWidget *parent = nullptr);

  void updateUI();

public slots:

  void slotSetValue(
      Pins pin,
      bool value);

signals:

  void signalLog(QString message);
};
