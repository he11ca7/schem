#include "logger_widget.h"

void LoggerWidget::createUI()
{
  _list = new QListWidget;
  _buttonClear = new QPushButton("Очистить...");
  connect(_buttonClear,
          SIGNAL(clicked(bool)),
          _list,
          SLOT(clear())
          );

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(_list);
  layout->addWidget(_buttonClear);
  setLayout(layout);
}

LoggerWidget::LoggerWidget(
    QWidget *parent) :
  QGroupBox("", parent)
{  
  createUI();
}

void LoggerWidget::addItem(
    QString text)
{
  addItem(text, CODE_REGULAR);
}

void LoggerWidget::addItem(
    QString text,
    LoggerCodes code)
{
  QListWidgetItem *item = new QListWidgetItem;
  item->setText(text);

  Qt::GlobalColor color = Qt::transparent;
  switch (code)
    {
    case CODE_REGULAR:  color = Qt::lightGray; break;
    case CODE_ERROR:    color = Qt::yellow; break;
    case CODE_CRITICAL: color = Qt::darkCyan; break;
    }

  item->setBackgroundColor(QColor(color));
  _list->insertItem(0, item);
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
