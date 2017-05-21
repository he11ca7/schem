#include "chip_selector_widget.h"

ChipSelectorWidget::ChipSelectorWidget(
    QWidget *parent) :
  QGroupBox("Микросхема", parent)
{
  createUI();

  //  Chip *chipEmpty = new Chip("Тест", "Нет описания");
  //  for(int i = PINS_BEGIN; i <= PINS_END; ++i)
  //    chipEmpty->addPin((Pins) i, Pin(STATE_IN, QString("%1").arg(i), "?"));

  //  Chip *chipSN74HC08N = new Chip(
  //        "SN74HC08N",
  //        "ЧЕТЫРЕ ЭЛЕМЕНТА 2-И");
  //  chipSN74HC08N->addPin(PIN_00, Pin(STATE_IN, "1A", "1A"));
  //  chipSN74HC08N->addPin(PIN_01, Pin(STATE_IN, "1B", "1B"));
  //  chipSN74HC08N->addPin(PIN_02, Pin(STATE_OUT, "1Y", "1Y"));
  //  chipSN74HC08N->addPin(PIN_03, Pin(STATE_IN, "2A", "2A"));
  //  chipSN74HC08N->addPin(PIN_04, Pin(STATE_IN, "2B", "2B"));
  //  chipSN74HC08N->addPin(PIN_05, Pin(STATE_OUT, "2Y", "2Y"));
  //  chipSN74HC08N->addPin(PIN_06, Pin(STATE_GND, "0V", "Общий вывод"));
  //  chipSN74HC08N->addPin(PIN_07, Pin(STATE_UNUSED, "", ""));
  //  chipSN74HC08N->addPin(PIN_08, Pin(STATE_UNUSED, "", ""));
  //  chipSN74HC08N->addPin(PIN_09, Pin(STATE_OUT, "3Y", "3Y"));
  //  chipSN74HC08N->addPin(PIN_10, Pin(STATE_IN, "3A", "3A"));
  //  chipSN74HC08N->addPin(PIN_11, Pin(STATE_IN, "3B", "3B"));
  //  chipSN74HC08N->addPin(PIN_12, Pin(STATE_OUT, "4Y", "4Y"));
  //  chipSN74HC08N->addPin(PIN_13, Pin(STATE_IN, "4A", "4A'"));
  //  chipSN74HC08N->addPin(PIN_14, Pin(STATE_IN, "4B", "4B"));
  //  chipSN74HC08N->addPin(PIN_15, Pin(STATE_V, "Ucc", "Вывод питания"));

  //  Chip *chipIE7 = new Chip(
  //        "КР1533ИЕ7",
  //        "ЧЕТЫРЕХРАЗРЯДНЫЙ ДВОИЧНЫЙ РЕВЕРСИВНЫЙ СЧЕТЧИК");
  //  chipIE7->addPin(PIN_00, Pin(STATE_IN, "D2", "Вход предзаписи 2"));
  //  chipIE7->addPin(PIN_01, Pin(STATE_OUT, "Q2", "Выход разряда 2"));
  //  chipIE7->addPin(PIN_02, Pin(STATE_OUT, "Q1", "Выход разряда 1"));
  //  chipIE7->addPin(PIN_03, Pin(STATE_IN, "-1", "Вход 'Обратный счёт'"));
  //  chipIE7->addPin(PIN_04, Pin(STATE_IN, "+1", "Вход 'Прямой счёт'"));
  //  chipIE7->addPin(PIN_05, Pin(STATE_OUT, "Q3", "Выход разряда 3"));
  //  chipIE7->addPin(PIN_06, Pin(STATE_OUT, "Q4", "Выход разряда 4"));
  //  chipIE7->addPin(PIN_07, Pin(STATE_GND, "0V", "Общий вывод"));
  //  chipIE7->addPin(PIN_08, Pin(STATE_IN, "D4", "Вход предзаписи 4"));
  //  chipIE7->addPin(PIN_09, Pin(STATE_IN, "D3", "Вход предзаписи 3"));
  //  chipIE7->addPin(PIN_10, Pin(STATE_IN, "C", "Вход строб. предв. записи"));
  //  chipIE7->addPin(PIN_11, Pin(STATE_OUT, "CR", "Выход 'Прямой перенос'"));
  //  chipIE7->addPin(PIN_12, Pin(STATE_OUT, "BR", "Выход 'Обратный перенос'"));
  //  chipIE7->addPin(PIN_13, Pin(STATE_IN, "R", "Вход 'Сброс'"));
  //  chipIE7->addPin(PIN_14, Pin(STATE_IN, "D1", "Вход предзаписи 1"));
  //  chipIE7->addPin(PIN_15, Pin(STATE_V, "Ucc", "Вывод питания"));

  //  Chip *chipID7 = new Chip(
  //        "К555ИД7",
  //        "ДЕШИФРАТОР-ДЕМУЛЬТИПЛЕКСОР 3 НА 8");
  //  chipID7->addPin(PIN_00, Pin(STATE_IN, "D1", "Информационный вход 0"));
  //  chipID7->addPin(PIN_01, Pin(STATE_IN, "D2", "Информационный вход 1"));
  //  chipID7->addPin(PIN_02, Pin(STATE_IN, "D3", "Информационный вход 2"));
  //  chipID7->addPin(PIN_03, Pin(STATE_IN, "C2", "Вход разрешения"));
  //  chipID7->addPin(PIN_04, Pin(STATE_IN, "C3", "Вход разрешения"));
  //  chipID7->addPin(PIN_05, Pin(STATE_IN, "C1", "Вход разрешения"));
  //  chipID7->addPin(PIN_06, Pin(STATE_OUT, "Y7", "Выход"));
  //  chipID7->addPin(PIN_07, Pin(STATE_GND, "0V", "Общий вывод"));
  //  chipID7->addPin(PIN_08, Pin(STATE_OUT, "Y6", "Выход"));
  //  chipID7->addPin(PIN_09, Pin(STATE_OUT, "Y5", "Выход"));
  //  chipID7->addPin(PIN_10, Pin(STATE_OUT, "Y4", "Выход"));
  //  chipID7->addPin(PIN_11, Pin(STATE_OUT, "Y3", "Выход"));
  //  chipID7->addPin(PIN_12, Pin(STATE_OUT, "Y2", "Выход"));
  //  chipID7->addPin(PIN_13, Pin(STATE_OUT, "Y1", "Выход"));
  //  chipID7->addPin(PIN_14, Pin(STATE_OUT, "Y0", "Выход"));
  //  chipID7->addPin(PIN_15, Pin(STATE_V, "Ucc", "Вывод питания"));

  //  _listChips.append(chipEmpty);
  //  _listChips.append(chipIE7);
  //  _listChips.append(chipID7);
  //  _listChips.append(chipSN74HC08N);

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

  buttonSelect = new QPushButton("Выбрать");
  buttonSelect->setCheckable(true);
  connect(buttonSelect,
          SIGNAL(toggled(bool)),
          this,
          SLOT(slotButtonSelectClicked(bool))
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

void ChipSelectorWidget::slotButtonSelectClicked(
    bool toggled)
{
  comboChips->setEnabled(!toggled);
  if(toggled)
    emit signalChipSelected(_listChips.at(comboChips->currentIndex() - 1));
}

void ChipSelectorWidget::slotComboChipsIndexChanged(
    int index)
{
  buttonSelect->setEnabled(index != 0);
}
