#pragma once

#include <QToolButton>
#include "pin.h"

/*!
 * \brief Виджет пина
 */
class PinWidget : public QToolButton
{
  Q_OBJECT

private:

  Pins _pinN;
  States _state;

  void createUI();

private slots:

  void slotClicked(
      bool toggled);

public:

  PinWidget(
      Pins pinN,
      QWidget *parent = nullptr);

  void updateUI(
      QString name,
      QString description);

public slots:

  void slotSetValue(
      bool value);
  void slotSetState(
      States state);

signals:

  void signalSetValue(
      Pins pin,
      bool value);
};
