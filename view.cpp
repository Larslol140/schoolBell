#include "view.h"

void View::setUpGui()
{
  QLabel *lBellTime = new QLabel("Bell Time:", this);
  QLabel *lBellDay = new QLabel("Bell Day:", this);
  QLabel *lBellName = new QLabel("Bell Name:", this);
  QLabel *lBellFile = new QLabel("Bell File:", this);

  teBellTime = new QTimeEdit(this);
  teBellTime->setTimeRange(QTime::fromString("00:00", "HH:mm"), QTime::fromString("23:59", "HH:mm"));
  teBellTime->setDisplayFormat("HH:mm");

  deBellDay = new QDateEdit(this);
  deBellDay->setDisplayFormat("dddd");

  leBellFile = new QLineEdit(this);
  leBellName = new QLineEdit(this);

  pbDelBell = new QPushButton("Delete Bell", this);
  pbAddBell = new QPushButton("Add Bell", this);
  QPushButton *pbOpenFile = new QPushButton("Search File", this);
  QPushButton *pbUpdBell = new QPushButton("Update Bell", this);

  lwBells = new QListWidget(this);

  QVBoxLayout *vlBellTime = new QVBoxLayout;
  QVBoxLayout *vlBellDay = new QVBoxLayout;
  QVBoxLayout *vlBellName = new QVBoxLayout;
  QVBoxLayout *vlBellFile = new QVBoxLayout;

  QHBoxLayout *hlFileOpen = new QHBoxLayout;

  vlBellFile->setAlignment(Qt::AlignBottom);
  vlBellName->setAlignment(Qt::AlignBottom);
  vlBellDay->setAlignment(Qt::AlignBottom);
  vlBellTime->setAlignment(Qt::AlignBottom);

  hlFileOpen->addWidget(leBellFile);
  hlFileOpen->addWidget(pbOpenFile);

  vlBellTime->addWidget(lBellTime);
  vlBellTime->addWidget(teBellTime);

  vlBellDay->addWidget(lBellDay);
  vlBellDay->addWidget(deBellDay);

  vlBellName->addWidget(lBellName);
  vlBellName->addWidget(leBellName);

  vlBellFile->addWidget(lBellFile);
  vlBellFile->addLayout(hlFileOpen);

  QHBoxLayout *hlNameFile = new QHBoxLayout;
  QHBoxLayout *hlTimeDay = new QHBoxLayout;
  QHBoxLayout *hlAddDel = new QHBoxLayout;

  hlAddDel->setAlignment(Qt::AlignRight);

  hlNameFile->addLayout(vlBellName);
  hlNameFile->addLayout(vlBellFile);

  hlTimeDay->addLayout(vlBellTime);
  hlTimeDay->addLayout(vlBellDay);

  hlAddDel->addWidget(pbAddBell);
  hlAddDel->addWidget(pbUpdBell);
  hlAddDel->addWidget(pbDelBell);

  QVBoxLayout *vlInputs = new QVBoxLayout;

  vlInputs->setAlignment(Qt::AlignBottom);

  vlInputs->addLayout(hlNameFile);
  vlInputs->addLayout(hlTimeDay);
  vlInputs->addLayout(hlAddDel);

  QHBoxLayout *hlMain = new QHBoxLayout(this);

  hlMain->addWidget(lwBells);
  hlMain->addLayout(vlInputs);

  setLayout(hlMain);

  connect(pbOpenFile, SIGNAL(clicked()), this, SLOT(openFileDialog()));
  connect(pbAddBell, SIGNAL(clicked()), this, SLOT(addBell()));
  connect(pbDelBell, SIGNAL(clicked()), this, SLOT(delBell()));
  connect(pbUpdBell, SIGNAL(clicked()), this, SLOT(updBell()));
  connect(deBellDay, SIGNAL(dateChanged(QDate)), this, SLOT(updateSelectedDay(QDate)));
  connect(lwBells, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(loadItemData(QListWidgetItem *)));

}

void View::updateListWidget()
{
  lwBells->clear();
  QHash<int, QString> bells = m->getBellsForDay(currentDay);

  while (lwBells->count())
    delete lwBells->item(1);

  for (int i = 0; i < bells.keys().length(); i++)
  {
    QListWidgetItem *tmp = new QListWidgetItem(bells[bells.keys()[i]]);
    tmp->setToolTip(QString::number(bells.keys()[i]));
    lwBells->addItem(tmp);
  }

}

void View::showWindow()
{
  show();
  tray->hide();
}

void View::showTray()
{
  hide();
  tray->show();
}

void View::closeEvent(QCloseEvent *e)
{
  bool selection = QMessageBox::information(this, "Closing", "Do you want to minimize the application to tray?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes ? true : false;
  if (selection)
  {
    e->ignore();
    showTray();
  }
  else
  {
    e->accept();
  }
}

void View::loadItemData(QListWidgetItem *item)
{
  int id = item->toolTip().toInt();

  leBellName->setText(m->getDatabase()->getBellName(id));
  leBellFile->setText(m->getDatabase()->getBellFile(id));
  deBellDay->setDate(QDate::fromString(QString::number(m->getDatabase()->getBellDay(id)), "d"));
  teBellTime->setTime(Model::convertTime(m->getDatabase()->getBellTime(id)));
}

void View::addBell()
{
  QString name = leBellName->text();
  QString file = leBellFile->text();
  QString time = Model::convertTime(teBellTime->time());
  m->addBell(name, file, time, currentDay);
  updateListWidget();
}

void View::delBell()
{
  if(!lwBells->selectedItems().count())
    return;
  m->delBell(lwBells->selectedItems().first()->toolTip().toInt());
  updateListWidget();
}

void View::updBell()
{
  if(!lwBells->selectedItems().count())
    return;
  int     id   = lwBells->selectedItems().first()->toolTip().toInt();
  QString name = leBellName->text();
  QString file = leBellFile->text();
  QString time = Model::convertTime(teBellTime->time());
  m->updBell(id, name, file, time, currentDay);
  updateListWidget();
}

void View::openFileDialog()
{
  leBellFile->setText(QFileDialog::getOpenFileUrl().toString().remove("file://"));
}

void View::updateSelectedDay(QDate date)
{
  currentDay = Model::convertDay(Model::convertDay(date.dayOfWeek()));

  updateListWidget();
}

View::View(QWidget *parent) : QWidget (parent)
{
  m = Model::getInstance(this);
  tray = new QSystemTrayIcon(this);
  connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showWindow()));
  tray->setIcon(QPixmap(":/src/bell-icon.png"));
  tray->hide();
  setUpGui();
  setWindowTitle("School Bell");
  setMinimumSize(720, 480);
  show();
}
