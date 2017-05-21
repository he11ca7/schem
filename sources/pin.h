#pragma once

#include <QString>
#include "protocol.h"

/*!
 * \brief Абстракция пина
 */
// TODO Ввести возможность выбирать тип входа -- нормальный, инверсный, фронт
struct Pin
{
  States _type;

  QString
  _name,
  _description;

  Pin(
      States type = STATE_UNUSED,
      QString name = QString(""),
      QString description = QString("")) :
    _type(type),
    _name(name),
    _description(description)
  {}
};
