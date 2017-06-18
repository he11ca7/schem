#pragma once

#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QFrame>
#include "chip.h"
#include "ugo.h"
#include "pin.h"
#include "pin_widget.h"

/*!
 * \brief Виджет УГО
 */
class UGOWidget : public QGroupBox
{
  Q_OBJECT

private:

  QMap<Pins, PinWidget *> _mapPinWidgets;

  QLabel *_label;
  QVBoxLayout
  *_layoutLeft,
  *_layoutRight;

  void createUI();

public:

  UGOWidget(
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
      Chip *chip,
      UGO *ugo);
};
