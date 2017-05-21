#include "duino_widget.h"

void DuinoWidget::createUI()
{
  comboConnections = new QComboBox;
  connect(comboConnections,
          SIGNAL(currentIndexChanged(int)),
          this,
          SLOT(comboConnectionsChanged(int))
          );
  buttonRefresh = new QPushButton("Обновить");
  connect(buttonRefresh,
          SIGNAL(clicked(bool)),
          this,
          SLOT(buttonRefreshClicked())
          );
  buttonConnect = new QPushButton("Соединиться");
  buttonConnect->setEnabled(false);
  connect(buttonConnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(buttonConnectClicked())
          );
  buttonDisconnect = new QPushButton("Отсоединиться");
  buttonDisconnect->setEnabled(false);
  connect(buttonDisconnect,
          SIGNAL(clicked(bool)),
          this,
          SIGNAL(signalSerialDisconnect())
          );
  QGridLayout *groupConnectionsLayout = new QGridLayout;
  groupConnectionsLayout->addWidget(comboConnections, 0, 0);
  groupConnectionsLayout->addWidget(buttonRefresh,    0, 1);
  groupConnectionsLayout->addWidget(buttonConnect,    1, 0, 1, 2);
  groupConnectionsLayout->addWidget(buttonDisconnect, 2, 0, 1, 2);
  QGroupBox *groupConnections = new QGroupBox("Соединение");
  groupConnections->setLayout(groupConnectionsLayout);

  comboCommands = new QComboBox;
  for(int i = COMMANDSPC_BEGIN; i <= COMMANDSPC_END; ++i)
    comboCommands->addItem(CommandName((CommandsPC) i), i);
  comboPins = new QComboBox;
  for(int i = PINS_BEGIN; i <= PINS_END; ++i)
    comboPins->addItem(QString("Пин #%1").arg(i), i);
  buttonSend = new QPushButton("Отправить");
  connect(buttonSend, SIGNAL(clicked(bool)), this, SLOT(buttonSendClicked()));
  QVBoxLayout *groupCommandsLayout = new QVBoxLayout;
  groupCommandsLayout->addWidget(comboCommands);
  groupCommandsLayout->addWidget(comboPins);
  groupCommandsLayout->addWidget(buttonSend);
  QGroupBox *groupCommands = new QGroupBox("Команды");
  groupCommands->setLayout(groupCommandsLayout);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(groupConnections, 0, 0);
  layout->addWidget(groupCommands, 1, 0);
  // layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

DuinoWidget::DuinoWidget(
    QWidget *parent) :
  QWidget(parent)
{
  createUI();

  buttonRefreshClicked();
}

void DuinoWidget::slotSerialConnected()
{
  comboConnections->setEnabled(false);
  buttonRefresh->setEnabled(false);
  buttonConnect->setEnabled(false);
  buttonDisconnect->setEnabled(true);
}

void DuinoWidget::slotSerialDisconnected()
{
  comboConnections->setEnabled(true);
  buttonRefresh->setEnabled(true);
  buttonConnect->setEnabled(true);
  buttonDisconnect->setEnabled(false);
}

void DuinoWidget::comboConnectionsChanged(
    int index)
{
  bool flag = index == 0;
  buttonConnect->setEnabled(!flag);
}

void DuinoWidget::buttonRefreshClicked()
{
  comboConnections->clear();
  comboConnections->addItem("Выбор соединения...");

  foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
      if(serialPortInfo.hasVendorIdentifier() &&
         serialPortInfo.hasProductIdentifier()
         )
        {
          QString string(QString("%1")
                         //  .arg(serialPortInfo.vendorIdentifier())
                         //  .arg(serialPortInfo.productIdentifier())
                         .arg(serialPortInfo.portName())
                         );
          comboConnections->addItem(QIcon("://res/usb.png"), string, serialPortInfo.portName());
        }
    }
}

void DuinoWidget::buttonConnectClicked()
{
  if(comboConnections->currentIndex() == 0)
    return;

  QString port = comboConnections->itemData(
        comboConnections->currentIndex()
        ).toString();
  emit signalSerialConnect(port);
}

void DuinoWidget::buttonSendClicked()
{
  emit signalSerialPost(
        (CommandsPC) comboCommands->currentData().toInt(),
        (Pins) comboPins->currentData().toInt()
        );
}
