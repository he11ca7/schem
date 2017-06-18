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

void ChipSelectorWidget::addUGO(
    UGO *ugo)
{
  _listUGOs.append(ugo);
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
  Chip *chip = _listChips.at(comboChips->currentIndex() - 1);
  emit signalChipSelected(chip);

  // Найти соответствующее УГО
  UGO *ugo = NULL;
  for(int i = 0; i < _listUGOs.size(); ++i)
    {
      UGO *_ugo = _listUGOs.at(i);
      if(chip->_name == _ugo->_name)
        {
          ugo = _ugo;
          break;
        }
    }
  emit signalUGOSelected(chip, ugo);
}

void ChipSelectorWidget::slotComboChipsIndexChanged(
    int index)
{
  buttonSelect->setEnabled(index != 0);
}
