#pragma once

#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QLayout>
#include <QList>
#include "chip.h"
#include "duino.h"

/*!
 * \brief Виджет выбора МС
 */
class ChipSelectorWidget : public QGroupBox
{
  Q_OBJECT

private:

  QList<Chip *> _listChips;

  QComboBox *comboChips;
  QPushButton *buttonSelect;

  void createUI();
  void updateUI();

private slots:

  void slotButtonSelectClicked();
  void slotComboChipsIndexChanged(
      int index);

public:

  ChipSelectorWidget(
      QWidget *parent = nullptr);

  void addChip(
      Chip *chip);

signals:

  void signalChipSelected(
      Chip *chip);
};
