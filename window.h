#pragma once

#include <QThread>
#include <QWidget>
#include <QMainWindow>
#include <QDockWidget>
#include <QLayout>
#include <QToolButton>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include "duino.h"
#include "duino_widget.h"
#include "logger_widget.h"
#include "chip_widget.h"
#include "chip_selector_widget.h"
#include "chip.h"
#include "test_widget.h"
#include "pin.h"

class Window : public QMainWindow
{
  Q_OBJECT

private:

  Duino *_duino;
  QThread *_duinoThread;

  DuinoWidget *widgetDuino;
  LoggerWidget *widgetLogger;
  ChipWidget *widgetChip;
  ChipSelectorWidget *widgetChipSelector;
  TestWidget *widgetTest;

  QDockWidget *dockChipSelector;
  QDockWidget *dockDuino;
  QDockWidget *dockLogger;
  QDockWidget *dockTest;

  void createUI();

private slots:

  void slotLoadChip();

public:

  Window(
      QWidget *parent = nullptr);
  virtual ~Window();
};
