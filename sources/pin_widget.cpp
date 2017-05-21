#include "pin_widget.h"

void PinWidget::slotClicked(
    bool toggled)
{
  toggled ? setIcon(QIcon("://res/1.png")) : setIcon(QIcon("://res/0.png"));
  emit signalSetValue(_pinN, toggled);
}

PinWidget::PinWidget(
    Pins pinN,
    QWidget *parent) :
  QToolButton(parent),
  _pinN(pinN)
{
  createUI();

  connect(this,
          SIGNAL(clicked(bool)),
          this,
          SLOT(slotClicked(bool))
          );
}

void PinWidget::createUI()
{
  slotSetState(STATE_UNUSED);
  slotSetValue(false);
  setText("?");

  setAutoRaise(true);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setSizePolicy(QSizePolicy::Preferred, sizePolicy().verticalPolicy());
  setIconSize(QSize(24, 24));
}

void PinWidget::updateUI(
    QString name,
    QString description)
{
  // slotSetState(type);

  setText(name);
  setToolTip(description);
}

void PinWidget::slotSetValue(
    bool value)
{
  switch (_state)
    {
    case STATE_IN:
      value ? setChecked(true) : setChecked(false);
    case STATE_OUT:
      value ? setIcon(QIcon("://res/1.png")) : setIcon(QIcon("://res/0.png"));
      break;
    default: break;
    }
}

void PinWidget::slotSetState(
    States state)
{
  _state = state;

  if(isCheckable())
    setChecked(false);

  switch (state)
    {
    case STATE_UNUSED:
      setIcon(QIcon("://res/x.png")); break;
    case STATE_V:
      setIcon(QIcon("://res/v.png")); break;
    case STATE_GND:
      setIcon(QIcon("://res/gnd.png")); break;
    case STATE_IN:
      setCheckable(true);
    case STATE_OUT:
      setIcon(QIcon("://res/0.png")); break;
    default: break;
    }

  switch (state)
    {
    case STATE_UNUSED:
    case STATE_V:
    case STATE_GND:
    case STATE_OUT:
      setEnabled(false);
      break;
    case STATE_IN:
      setEnabled(true);
      break;
    default: break;
    }
}
