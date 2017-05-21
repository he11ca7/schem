#pragma once

#include <QGroupBox>
#include <QLayout>
#include <QMap>
#include "chip.h"
#include "duino.h"
#include "pin.h"
#include "pin_widget.h"

/*!
 * \brief Виджет МС
 */
class ChipWidget : public QGroupBox
{
  Q_OBJECT

private:

  QMap<Pins, PinWidget *> _mapPinWidgets;

  void createUI();

public:

  ChipWidget(
      QWidget *parent = nullptr);

signals:

  void signalSetValue(
      Pins pin,
      bool value);

public slots:

  void slotSetValue(
      Pins pin,
      bool value);
  void slotSetState(
      Pins pin,
      States state);

  void updateUI(
      Chip *chip);
};
