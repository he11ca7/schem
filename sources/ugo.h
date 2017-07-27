#pragma once

#include <QString>
#include <QList>

/*!
 * \brief Абстракция УГО
 */
class UGO
{
private:

  QList<int>
  _listPinsLeft,
  _listPinsRight,
  _listLimitersLeft,
  _listLimitersRight;

public:

  QString _name;

  enum Positions
  {
    LEFT,
    RIGHT
  };

  UGO(
      QString name) :
    _name(name)
  {}

  void addPins(
      QList<int> pins,
      Positions position);
  void addLimiters(
      QList<int> limiters,
      Positions position);
  QList<int> pins(
      Positions position);
  QList<int> limiters(
      Positions position);
};
