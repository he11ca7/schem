#pragma once

#include <QString>
#include <QMap>
#include <QMetaType>
#include "pin.h"

/*!
 * \brief Абстракция микросхемы
 */
class Chip
{
private:

  QMap<Pins, Pin> _pins;

public:

  QString
  _name,
  _description;

  Chip(
      QString name = QString(""),
      QString description = QString("")) :
    _name(name),
    _description(description)
  {}

  void addPin(Pins id, Pin pin) {_pins.insert(id, pin);}
  Pin &pin(Pins id) {return _pins[id];}
};

Q_DECLARE_METATYPE(Chip)
