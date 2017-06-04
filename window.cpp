#include "window.h"

Window::Window(
    QWidget *parent) :
  QMainWindow(parent)
{
  _duino = new Duino;
  _duinoThread = new QThread;
  _duino->moveToThread(_duinoThread);
  _duinoThread->start();

  createUI();
}

Window::~Window()
{
  _duino->serialDisconnect();
  _duinoThread->quit();
  _duinoThread->wait();
}

void Window::createUI()
{
  //  QAction *_actionExit = new QAction("Закрыть", this);
  //  connect(_actionExit,
  //          SIGNAL(triggered(bool)),
  //          this,
  //          SLOT(close())
  //          );
  //  QMenu *_menuFile = new QMenu("Файл");
  //  _menuFile->addAction(_actionExit);
  //  menuBar()->addMenu(_menuFile);

  //  QToolButton *buttonReloadChip = new QToolButton;
  //  buttonReloadChip->setIcon(QIcon("://res/ic.png"));
  //  buttonReloadChip->setText("Перезагрузить данные микросхем");
  //  buttonReloadChip->setToolTip("Перезагрузить данные микросхем");
  //  connect(buttonReloadChip, SIGNAL(clicked(bool)), this, SLOT(slotLoadChip()));
  //  QToolBar *barData = addToolBar("Данные");
  //  barData->addWidget(buttonReloadChip);

  //-----------
  // Соединения
  //-----------

  widgetDuino = new DuinoWidget;
  connect(widgetDuino,
          SIGNAL(signalSerialConnect(QString)),
          _duino,
          SLOT(serialConnect(QString))
          );
  connect(widgetDuino,
          SIGNAL(signalSerialDisconnect()),
          _duino,
          SLOT(serialDisconnect())
          );
  connect(widgetDuino,
          SIGNAL(signalSerialPost(CommandsPC,Pins)),
          _duino,
          SLOT(serialPost(CommandsPC,Pins))
          );
  connect(_duino,
          SIGNAL(signalConnected()),
          widgetDuino,
          SLOT(slotSerialConnected())
          );
  connect(_duino,
          SIGNAL(signalDisconnected()),
          widgetDuino,
          SLOT(slotSerialDisconnected())
          );

  //---------
  // Выбор МС
  //---------

  widgetChipSelector = new ChipSelectorWidget;
  connect(widgetChipSelector,
          SIGNAL(signalChipSelected(Chip*)),
          _duino,
          SLOT(slotConfigureChip(Chip*))
          );

  //------
  // Приём
  //------

  widgetLogger = new LoggerWidget;
  connect(_duino,
          SIGNAL(signalConnected()),
          widgetLogger,
          SLOT(slotSerialConnected())
          );
  connect(_duino,
          SIGNAL(signalDisconnected()),
          widgetLogger,
          SLOT(slotSerialDisconnected())
          );
  connect(_duino,
          SIGNAL(serialGet(CommandsDuino,Pins)),
          widgetLogger,
          SLOT(slotSerialGet(CommandsDuino,Pins))
          );
  connect(_duino,
          SIGNAL(signalError(Errors)),
          widgetLogger,
          SLOT(slotSerialError(Errors))
          );
  connect(_duino,
          SIGNAL(signalErrorInternal()),
          widgetLogger,
          SLOT(slotSerialErrorInternal())
          );

  //-------
  // Тестер
  //-------

  widgetTest = new TestWidget;
  connect(_duino,
          SIGNAL(signalSetValue(Pins,bool)),
          widgetTest,
          SLOT(slotSetValue(Pins,bool))
          );
  connect(widgetTest,
          SIGNAL(signalLog(QString)),
          widgetLogger,
          SLOT(addItem(QString))
          );

  //----
  // Чип
  //----

  Chip *chipEmpty = new Chip("Не задана МС", "Нет описания");
  for(int i = PINS_BEGIN; i <= PINS_END; ++i)
    chipEmpty->addPin((Pins) i, Pin(STATE_IN, QString("%1").arg(i), "?"));

  widgetChip = new ChipWidget;
  widgetChip->updateUI(chipEmpty);
  connect(_duino,
          SIGNAL(signalSetValue(Pins,bool)),
          widgetChip,
          SLOT(slotSetValue(Pins,bool))
          );
  connect(_duino,
          SIGNAL(signalSetState(Pins,States)),
          widgetChip,
          SLOT(slotSetState(Pins,States))
          );
  connect(widgetChip,
          SIGNAL(signalSetValue(Pins,bool)),
          _duino,
          SLOT(slotSetValue(Pins,bool))
          );
  connect(widgetChipSelector,
          SIGNAL(signalChipSelected(Chip*)),
          widgetChip,
          SLOT(updateUI(Chip*))
          );

  QHBoxLayout *layout = new QHBoxLayout;
  layout->addStretch();
  layout->addWidget(widgetChip);
  layout->addStretch();
  QWidget *widget = new QWidget;
  widget->setLayout(layout);
  setCentralWidget(widget);

  dockDuino = new QDockWidget("Управление");
  dockDuino->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetVerticalTitleBar);
  dockDuino->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dockDuino->setWidget(widgetDuino);

  dockChipSelector = new QDockWidget("Настройки МС");
  dockChipSelector->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetVerticalTitleBar);
  dockChipSelector->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dockChipSelector->setWidget(widgetChipSelector);

  dockLogger = new QDockWidget("Лог");
  dockLogger->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetVerticalTitleBar);
  dockLogger->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dockLogger->setWidget(widgetLogger);

  dockTest = new QDockWidget("Тестирование");
  dockTest->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetVerticalTitleBar);
  dockTest->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dockTest->setWidget(widgetTest);

  addDockWidget(Qt::LeftDockWidgetArea, dockDuino);
  addDockWidget(Qt::LeftDockWidgetArea, dockChipSelector);
  addDockWidget(Qt::LeftDockWidgetArea, dockLogger);
  addDockWidget(Qt::RightDockWidgetArea, dockTest);

  setWindowTitle("Schem");
  setWindowIcon(QIcon("://res/ic.png"));
  setContentsMargins(16, 16, 16, 16);
  resize(1024, 768);

  slotLoadChip();
}

void Window::slotLoadChip()
{
  QDir dir(G_PATH + G_PATH_DATA);
  dir.setFilter(QDir::Files | QDir::NoSymLinks);
  dir.setSorting(QDir::Time | QDir::Reversed);
  dir.setNameFilters(QStringList() << "*.chip");

  QFileInfoList list = dir.entryInfoList();
  foreach (QFileInfo info, list)
    {
      QFile file(info.absoluteFilePath());
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
          qDebug() << "cant open " + info.absoluteFilePath();
          continue;
        }
      QXmlStreamReader xml(&file);

      // Обработка
      QString xml_name;
      while (!xml.atEnd() && !xml.hasError())
        {
          QXmlStreamReader::TokenType token = xml.readNext();

          if (token == QXmlStreamReader::StartDocument)
            continue;

          if (token == QXmlStreamReader::StartElement)
            if(xml.name() == "chip")
              {
                Chip *chip = new Chip;

                while(xml.readNextStartElement())
                  {
                    xml_name = xml.name().toString();

                    if (xml_name == "name")
                      {
                        xml.readNext();
                        chip->_name = xml.text().toString();
                        xml.skipCurrentElement();
                      }
                    if (xml_name == "description")
                      {
                        xml.readNext();
                        chip->_description = xml.text().toString();
                        xml.skipCurrentElement();
                      }
                    if (xml_name == "pin")
                      {
                        Pin pin;

                        QString
                            _id,
                            _state,
                            _name,
                            _description;

                        QXmlStreamAttributes attributes = xml.attributes();
                        if (attributes.hasAttribute("id"))
                          _id = attributes.value("id").toString();
                        if (attributes.hasAttribute("state"))
                          _state = attributes.value("state").toString();

                        while(xml.readNextStartElement())
                          {
                            xml_name = xml.name().toString();
                            xml.readNext();

                            if (xml_name == "name")
                              _name = xml.text().toString();
                            if (xml_name == "description")
                              _description = xml.text().toString();

                            xml.skipCurrentElement();
                          }

                        pin._type = (States) findEnum(
                              _state,
                              StateName,
                              STATES_BEGIN,
                              STATES_END);
                        pin._name = _name;
                        pin._description = _description;

                        chip->addPin(
                              (Pins) _id.toInt(),
                              pin);
                      }
                  }

                widgetChipSelector->addChip(chip);
                widgetLogger->addItem(QString("Добавлена МС: ") + chip->_name);
              }
        }
      file.close();
    }
}
