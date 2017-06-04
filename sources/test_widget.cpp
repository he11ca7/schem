#include "test_widget.h"

void TestWidget::createUI()
{
  comboTests = new QComboBox;
  connect(comboTests,
          SIGNAL(currentIndexChanged(int)),
          this,
          SLOT(slotComboTestsIndexChanged(int))
          );
  buttonSelect = new QPushButton("Загрузить...");
  connect(buttonSelect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(slotButtonSelectClicked())
          );
  QHBoxLayout *layoutH = new QHBoxLayout;
  layoutH->addWidget(comboTests);
  layoutH->addWidget(buttonSelect);

  table = new QTableWidget(1, 1);
  table->setItem(0, 0, new QTableWidgetItem("Не выбран тест."));
  table->horizontalHeader()->hide();
  table->verticalHeader()->hide();
  table->resizeColumnsToContents();
  table->resizeRowsToContents();
  table->setGridStyle(Qt::DotLine);
  table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setSelectionMode(QAbstractItemView::NoSelection);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(layoutH);
  layout->addWidget(table);
  setLayout(layout);
  // setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void TestWidget::resetUI()
{
  for(int i = 0; i < table->rowCount(); ++i)
    for(int j = 0; j < table->columnCount(); ++j)
      {
        QTableWidgetItem *item = table->item(i, j);
        item->setBackgroundColor(Qt::transparent);
      }
}

void TestWidget::slotComboTestsIndexChanged(
    int index)
{
  buttonSelect->setEnabled(index != 0);
}

void TestWidget::slotButtonSelectClicked()
{
  QString path = comboTests->currentData().toString();
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      qDebug() << "cant open " + path;
      return;
    }

  QTextStream stream(&file);
  stream.setCodec("UTF-8");

  // Читать заголовок
  QString titles = stream.readLine().simplified();
  QString indexes = stream.readLine().simplified();
  QStringList titlesSplit = titles.split(' ');
  QStringList indexesSplit = indexes.split(' ');

  level = -1;
  flagTestOver = false;
  mapLastValues.clear();

  table->clear();
  table->setRowCount(0);
  table->horizontalHeader()->show();

  // Загрузить заголовок
  table->setColumnCount(titlesSplit.count() + 1);
  for(int i = 0; i < titlesSplit.size(); ++i)
    {
      QTableWidgetItem *item = new QTableWidgetItem(titlesSplit.at(i));
      item->setData(Qt::UserRole, indexesSplit.at(i).toInt());
      table->setHorizontalHeaderItem(i, item);
    }
  table->setHorizontalHeaderItem(
        titlesSplit.size(),
        new QTableWidgetItem("Задание"));

  // Загрузить очередную строку
  while(!stream.atEnd())
    {
      table->setRowCount(table->rowCount() + 1);
      for(int i = 0; i < titlesSplit.size(); ++i)
        {
          int number;
          stream >> number;
          QTableWidgetItem *item = new QTableWidgetItem(QString::number(number));
          item->setTextAlignment(Qt::AlignCenter);
          table->setItem(table->rowCount() - 1, i, item);
        }
      QTableWidgetItem *item = new QTableWidgetItem(stream.readLine().simplified());
      item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
      table->setItem(table->rowCount() - 1, titlesSplit.size(), item);
    }

  //
  table->resizeColumnsToContents();
  table->resizeRowsToContents();

  // Закрыть
  file.close();
}

TestWidget::TestWidget(
    QWidget *parent) :
  QGroupBox("", parent),
  level(-1),
  flagTestOver(false)
{
  createUI();
  updateUI();
  resetUI();
}

void TestWidget::updateUI()
{
  comboTests->clear();
  comboTests->addItem("Выбор теста...");

  QDir dir(G_PATH + G_PATH_DATA);
  dir.setFilter(QDir::Files | QDir::NoSymLinks);
  dir.setSorting(QDir::Time | QDir::Reversed);
  dir.setNameFilters(QStringList() << "*.test");

  QFileInfoList list = dir.entryInfoList();
  foreach (QFileInfo info, list)
    {
      comboTests->addItem(info.baseName(), info.absoluteFilePath());
      emit signalLog("Обнаружен тест: " + info.baseName());
    }
}

void TestWidget::slotSetValue(
    Pins pin,
    bool value)
{
  if(flagTestOver)
    return;

  QList<int> indexes;
  for(int i = 0; i < table->columnCount() - 1; ++i)
    indexes.append(table->horizontalHeaderItem(i)->data(Qt::UserRole).toInt());
  if(!indexes.contains((int) pin))
    return;

  mapLastValues.insert((int) pin, value); // Отметили, что было изменено значение

  bool flagAllChanged = true; // Признак того, что все текущие пины обновились
  foreach (int index, indexes)
    if(!mapLastValues.contains(index))
      {
        flagAllChanged = false;
        break;
      }

  // Проверить, изменены ли все текущие выходы
  if(flagAllChanged)
    {
      QList<int> _keys = mapLastValues.keys();
      foreach (int key, _keys)
        qDebug()
            << "key: "
            << key
            << " value: "
            << mapLastValues[key];
      qDebug() << "\n";

      // Флаг того, что текущий набор совпадает со следующим уровнем
      bool flagNextEquals = true;
      // Сравнить со следующим уровнем
      for(int j = 0; j < table->columnCount() - 1; ++j)
        {
          int index = table->horizontalHeaderItem(j)->data(Qt::UserRole).toInt();
          QTableWidgetItem *item = table->item(level + 1, j);
          bool tableVal = (bool) item->text().toInt();
          if(tableVal != mapLastValues[index])
            {
              flagNextEquals = false;
              break;
            }
        }
      if(flagNextEquals)
        {
          ++level;
          // Покраска
          for(int j = 0; j < table->columnCount() - 1; ++j)
            table->item(level, j)->setBackgroundColor(Qt::darkCyan);
          qDebug() << "level up";
          if(level == table->rowCount() - 1)
            {
              flagTestOver = true;
              emit signalLog("Тест " + comboTests->currentText() + " успешно пройден!");
              qDebug() << "game over";
            }
        }

      //      else
      //        {
      //          if(level != -1)
      //            // Сверить с текущим уровнем
      //            // Если соответствует, ничего не предпринимать
      //            {
      //              bool flagCurrentEqual = true;
      //              for(int i = 0; i < table->columnCount(); ++i)
      //                {
      //                  QTableWidgetItem *item = table->item(level, i);
      //                  bool tableVal = (bool) item->text().toInt();
      //                  if(tableVal != mapLastValues[i])
      //                    {
      //                      flagCurrentEqual = false;
      //                      break;
      //                    }
      //                }
      //              if(!flagCurrentEqual)
      //                {
      //                  resetUI();
      //                  level = -1;
      //                  qDebug() << "game over";
      //                }
      //            }
      //        }

      mapLastValues.clear(); // Очистка до следующего раза
    }
}
