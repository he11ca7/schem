#include "chip_widget.h"

ChipWidget::ChipWidget(
    QWidget *parent) :
  QGroupBox(parent)
{
  createUI();
}

void ChipWidget::slotSetValue(
    Pins pin,
    bool value)
{
  _mapPinWidgets[pin]->slotSetValue(value);
}

void ChipWidget::slotSetState(
    Pins pin,
    States state)
{
  _mapPinWidgets[pin]->slotSetState(state);
}

void ChipWidget::createUI()
{
  QGridLayout *layout = new QGridLayout;
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->setSpacing(0);

  for(int i = PINS_BEGIN; i <= PINS_END; ++i)
    {
      PinWidget *pinWidget = new PinWidget((Pins) i);
      connect(pinWidget,
              SIGNAL(signalSetValue(Pins,bool)),
              this,
              SIGNAL(signalSetValue(Pins,bool))
              );
      _mapPinWidgets.insert((Pins) i, pinWidget);

      if(i <= PIN_07)
        {
          // NOTE Способ сделать подписи слева от иконок
          // pinWidget->setLayoutDirection(Qt::RightToLeft);
          layout->addWidget(pinWidget, i, 0);
        }
      else
        layout->addWidget(pinWidget, 15 - i, 1);
    }

  setLayout(layout);
}

void ChipWidget::updateUI(
    Chip *chip)
{
  for(int i = PINS_BEGIN; i <= PINS_END; ++i)
    {
      Pin pin = chip->pin((Pins) i);
      _mapPinWidgets[(Pins) i]->updateUI(pin._name, pin._description);
    }

  setTitle(chip->_name);
  setToolTip(chip->_description);
}
