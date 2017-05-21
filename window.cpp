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
  // Меню

  //  QAction *_actionLoadChip = new QAction("Перезагрузить микросхемы", this);
  //  connect(_actionLoadChip,
  //          SIGNAL(triggered(bool)),
  //          this,
  //          SLOT(slotLoadChip())
  //          );

  //  QMenu *_menuView = new QMenu("Данные");
  //  _menuView->addAction(_actionLoadChip);
  //  menuBar()->addMenu(_menuView);

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
  //----
  // Чип
  //----

  widgetChip = new ChipWidget;
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

  dockDuino = new QDockWidget("Duino");
  dockDuino->setContentsMargins(0, 16, 0, 0);
  dockDuino->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dockDuino->setWidget(widgetDuino);

  dockChipSelector = new QDockWidget("IC");
  dockChipSelector->setContentsMargins(0, 16, 0, 0);
  dockChipSelector->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dockChipSelector->setWidget(widgetChipSelector);

  dockLogger = new QDockWidget("Log");
  dockLogger->setContentsMargins(0, 16, 0, 0);
  dockLogger->setWidget(widgetLogger);

  addDockWidget(Qt::LeftDockWidgetArea, dockDuino);
  addDockWidget(Qt::LeftDockWidgetArea, dockChipSelector);
  addDockWidget(Qt::RightDockWidgetArea, dockLogger);

  setWindowTitle("Schem");
  setWindowIcon(QIcon("://res/ic.png"));
  resize(800, 640);

  slotLoadChip();
}

void Window::slotLoadChip()
{
  QDir dir(G_PATH + G_PATH_DATA);
  dir.setFilter(QDir::Files | QDir::NoSymLinks);
  dir.setSorting(QDir::Time);
  dir.setNameFilters(QStringList() << "*.xml");

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
              }
        }
      file.close();
    }
}
