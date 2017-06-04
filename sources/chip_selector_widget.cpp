#include "chip_selector_widget.h"

ChipSelectorWidget::ChipSelectorWidget(
    QWidget *parent) :
  QGroupBox("Микросхема", parent)
{
  createUI();
  updateUI();
}

void ChipSelectorWidget::addChip(
    Chip *chip)
{
  _listChips.append(chip);
  updateUI();
}

void ChipSelectorWidget::createUI()
{
  comboChips = new QComboBox;
  connect(comboChips,
          SIGNAL(currentIndexChanged(int)),
          this,
          SLOT(slotComboChipsIndexChanged(int))
          );

  buttonSelect = new QPushButton("Пропатчить...");
  connect(buttonSelect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(slotButtonSelectClicked())
          );

  QGridLayout *layout = new QGridLayout;
  // layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(comboChips, 0, 0);
  layout->addWidget(buttonSelect, 0, 1);
  setLayout(layout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void ChipSelectorWidget::updateUI()
{
  comboChips->clear();
  comboChips->addItem("Выбор МС...");
  foreach (Chip *chip, _listChips)
    comboChips->addItem(QIcon("://res/ic.png"), chip->_name);
}

void ChipSelectorWidget::slotButtonSelectClicked()
{
  emit signalChipSelected(_listChips.at(comboChips->currentIndex() - 1));
}

void ChipSelectorWidget::slotComboChipsIndexChanged(
    int index)
{
  buttonSelect->setEnabled(index != 0);
}
