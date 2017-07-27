#include "ugo.h"

void UGO::addPins(
    QList<int> pins,
    UGO::Positions position)
{
  switch (position)
    {
    case LEFT:
      _listPinsLeft.append(pins);
      break;
    case RIGHT:
      _listPinsRight.append(pins);
      break;
    default: break;
    }
}

void UGO::addLimiters(
    QList<int> limiters,
    UGO::Positions position)
{
  switch (position)
    {
    case LEFT:
      _listLimitersLeft.append(limiters);
      break;
    case RIGHT:
      _listLimitersRight.append(limiters);
      break;
    default: break;
    }
}

QList<int> UGO::pins(
    UGO::Positions position)
{
  QList<int> _pins;
  switch (position)
    {
    case LEFT: _pins = _listPinsLeft; break;
    case RIGHT: _pins = _listPinsRight; break;
    default: break;
    }
  return _pins;
}

QList<int> UGO::limiters(
    UGO::Positions position)
{
  QList<int> _limiters;
  switch (position)
    {
    case LEFT: _limiters = _listLimitersLeft; break;
    case RIGHT: _limiters = _listLimitersRight; break;
    default: break;
    }
  return _limiters;
}
