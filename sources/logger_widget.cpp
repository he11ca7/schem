#include "logger_widget.h"

void LoggerWidget::mousePressEvent(
    QMouseEvent *event)
{
  if(event->button() == Qt::RightButton)
    menu->popup(event->globalPos());
  else
    QListWidget::mousePressEvent(event);
}

LoggerWidget::LoggerWidget(
    QWidget *parent) :
  QListWidget(parent)
{  
  menu = new QMenu(this);
  menu->addAction(QIcon("://res/trash.png"), "Очистить...", this, SLOT(clear()));
}

void LoggerWidget::addItem(
    QString text,
    LoggerCodes code)
{
  QListWidgetItem *item = new QListWidgetItem;
  item->setText(text);

  Qt::GlobalColor color;
  switch (code)
    {
    case CODE_REGULAR:  color = Qt::lightGray; break;
    case CODE_ERROR:    color = Qt::yellow; break;
    case CODE_CRITICAL: color = Qt::darkCyan; break;
    }

  item->setBackgroundColor(QColor(color));
  insertItem(0, item);
}

void LoggerWidget::slotSerialConnected()
{
  addItem("Соединение установлено");
}

void LoggerWidget::slotSerialDisconnected()
{
  addItem("Соединение разорвано");
}

void LoggerWidget::slotSerialGet(
    CommandsDuino command,
    Pins pin)
{
  addItem(QString("Пин %1").arg(pin, 2, 10, QChar('0')) + " | " + CommandName(command));
}

void LoggerWidget::slotSerialError(
    Errors error)
{
  addItem(ErrorName(error), CODE_ERROR);
}

void LoggerWidget::slotSerialErrorInternal()
{
  addItem("Внутренняя ошибка", CODE_CRITICAL);
}
