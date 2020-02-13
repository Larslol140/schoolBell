#include "model.h"

Model *Model::instance = nullptr;

Model::Model(QObject *parent) : QObject (parent)
{
  db = Database::getInstance();
  mp = new QMediaPlayer(this);
  currentDay = getCurrentDayOfWeek();
  updateBellTimes();
  checkBellTimes();
}

void Model::updateBellTimes()
{
  currentBellTimes.clear();
  QList<int> tmpBellIds = db->getBellsForDay(currentDay);

  for (int i = 0; i < tmpBellIds.length(); i++)
    currentBellTimes[db->getBellTime(tmpBellIds[i])] = tmpBellIds[i];
}

Model *Model::getInstance(QObject *parent)
{
  if (instance == nullptr)
    instance = new Model(parent);
  return instance;
}

Model::~Model()
{
  instance = nullptr;
}

Database *Model::getDatabase() const
{
  return db;
}

QHash<int, QString> Model::getBellsForDay(int day) const
{
  QList<int> ids = db->getBellsForDay(day);

  QHash<int, QString> response;

  for (int i = 0; i < ids.length(); i++)
    response[ids[i]] = formatedText(ids[i]);
  return response;
}

QString Model::formatedText(int id) const
{
  QString day = convertDay(db->getBellDay(id));
  QString name = db->getBellName(id);
  QString time = db->getBellTime(id);
  return name+" will play on "+day+" at "+time;
}

void Model::addBell(QString name, QString file, QString time, int day)
{
  db->addBell(name, file, time, day);
  updateBellTimes();
}

void Model::delBell(int id)
{
  db->delBell(id);
  updateBellTimes();
}

void Model::updBell(int id, QString name, QString file, QString time, int day)
{
  db->updateBell(id, name, file, time, day);
  updateBellTimes();
}

int Model::getCurrentDayOfWeek() const
{
  return QDate::currentDate().dayOfWeek();
}

void Model::playBellSound(int id)
{
  mp->setVolume(100);
  mp->setMedia(QMediaContent("file://"+db->getBellFile(id)));
  mp->play();
}

void Model::updateTimesIfNewDay()
{
  if (currentDay != getCurrentDayOfWeek())
  {
    updateBellTimes();
    updateCurrentDay();
  }
}

void Model::updateCurrentDay()
{
  currentDay = QDate::currentDate().dayOfWeek();
}

void Model::playSoundIfCorrectTime()
{
  if (currentTimeInBellList())
    playBellSound(currentBellTimes[convertTime(QTime::currentTime())]);
}

bool Model::currentTimeInBellList() const
{
  if (currentBellTimes.isEmpty())
    return false;
  return currentBellTimes.keys().contains(convertTime(QTime::currentTime()));
}

bool Model::isNewMinute()
{
  if (lastPlayTime == convertTime(QTime::currentTime()))
    return false;
  lastPlayTime = convertTime(QTime::currentTime());
  return true;
}

QTime Model::convertTime(QString time)
{
  return QTime::fromString(time, "HH:mm");
}

QString Model::convertTime(QTime time)
{
  return time.toString("HH:mm");
}

int Model::convertDay(QString day)
{
  return QDate::fromString(day, "dddd").dayOfWeek();
}

QString Model::convertDay(int day)
{
  return QDate::fromString(QString::number(day), "d").toString("dddd");
}

void Model::checkBellTimes()
{
  if (!isNewMinute())
  {
    QTimer::singleShot(1000, this, SLOT(checkBellTimes()));
    return;
  }

  updateTimesIfNewDay();
  playSoundIfCorrectTime();

  QTimer::singleShot(1000, this, SLOT(checkBellTimes()));
}

