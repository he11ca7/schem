#include "ugo_widget.h"

void UGOWidget::createUI()
{
  for(int i = PINS_BEGIN; i <= PINS_END; ++i)
    {
      PinWidget *pinWidget = new PinWidget((Pins) i);
      connect(pinWidget,
              SIGNAL(signalSetValue(Pins,bool)),
              this,
              SIGNAL(signalSetValue(Pins,bool))
              );
      _mapPinWidgets.insert((Pins) i, pinWidget);
    }

  _layoutLeft = new QVBoxLayout;
  _layoutRight = new QVBoxLayout;

  _label = new QLabel;
  _label->setContentsMargins(16, 16, 16, 16);

  QFrame *limiter1 = new QFrame;
  limiter1->setFrameShape(QFrame::VLine);
  limiter1->setStyleSheet("color: #ccc");
  QFrame *limiter2 = new QFrame;
  limiter2->setFrameShape(QFrame::VLine);
  limiter2->setStyleSheet("color: #ccc");

  QHBoxLayout *layoutCenter = new QHBoxLayout;
  layoutCenter->addWidget(limiter1);
  layoutCenter->addWidget(_label);
  layoutCenter->addWidget(limiter2);

  QHBoxLayout *layout = new QHBoxLayout;
  layout->addLayout(_layoutLeft);
  layout->addLayout(layoutCenter);
  layout->addLayout(_layoutRight);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->setSpacing(0);

  setLayout(layout);

  hide();
}

UGOWidget::UGOWidget(
    QWidget *parent) :
  QGroupBox(parent)
{
  createUI();
}

void UGOWidget::slotSetValue(
    Pins pin,
    bool value)
{
  _mapPinWidgets[pin]->slotSetValue(value);
}

void UGOWidget::slotSetState(
    Pins pin,
    States state)
{
  _mapPinWidgets[pin]->slotSetState(state);
}

void UGOWidget::updateUI(
    Chip *chip,
    UGO *ugo)
{
  if(!ugo)
    {
      hide();
      return;
    }
  show();

  // Очистить виджет
  QLayoutItem *child;
  while ((child = _layoutLeft->takeAt(0)) != 0)
    _layoutLeft->removeItem(child);
  while ((child = _layoutRight->takeAt(0)) != 0)
    _layoutRight->removeItem(child);

  _label->setText(ugo->_name);
  setToolTip(chip->_description);

  for(int i = PINS_BEGIN; i <= PINS_END; ++i)
    {
      Pin pin = chip->pin((Pins) i);
      _mapPinWidgets[(Pins) i]->updateUI(pin._name, pin._description);
    }

  QList<int>
  _listPinsLeft = ugo->pins(UGO::LEFT),
  _listPinsRight = ugo->pins(UGO::RIGHT),
  _listLimitersLeft = ugo->limiters(UGO::LEFT),
  _listLimitersRight = ugo->limiters(UGO::RIGHT);

  for(int i = 0; i < _listPinsLeft.size(); ++i)
    {
      int _pinN = _listPinsLeft.at(i);
      PinWidget *pinWidget = _mapPinWidgets[(Pins) _pinN];
      if(_listLimitersLeft.contains(i))
        {
          QFrame *limiter = new QFrame;
          limiter->setFrameShape(QFrame::HLine);
          limiter->setStyleSheet("color: #ccc");
          _layoutLeft->addWidget(limiter);
        }
      _layoutLeft->addWidget(pinWidget);
    }
  _layoutLeft->addStretch();
  for(int i = 0; i < _listPinsRight.size(); ++i)
    {
      int _pinN = _listPinsRight.at(i);
      PinWidget *pinWidget = _mapPinWidgets[(Pins) _pinN];
      if(_listLimitersRight.contains(i))
        {
          QFrame *limiter = new QFrame;
          limiter->setFrameShape(QFrame::HLine);
          limiter->setStyleSheet("color: #ccc");
          _layoutRight->addWidget(limiter);
        }
      _layoutRight->addWidget(pinWidget);
    }
  _layoutRight->addStretch();
}
