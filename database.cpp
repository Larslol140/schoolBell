#include "database.h"

Database *Database::instance = nullptr;

Database::Database(QString file)
{
  if (fileExists(file))
    openDatabase(file);
  else
    createDatabase(file);
}

bool Database::fileExists(QString file) const
{
  return QFile::exists(file);
}

void Database::openDatabase(QString file) const
{
  QSqlDatabase db = getNewDatabase(file);
}

void Database::createDatabase(QString file) const
{
  QSqlDatabase db = getNewDatabase(file);
  db.exec(DB_CREATE);
}

QSqlDatabase Database::getNewDatabase(QString file) const
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "MAIN");
  db.setDatabaseName(file);
  db.open();
  return db;
}

QSqlDatabase Database::getDatabase() const
{
  return QSqlDatabase::database("MAIN");
}

QSqlQuery Database::getQuery() const
{
  return QSqlQuery(getDatabase());
}

Database *Database::getInstance(QString file)
{
  if (instance == nullptr)
    instance = new Database(file);
  return instance;
}

Database::~Database()
{
  instance = nullptr;
}

QList<int> Database::getAllBells() const
{
  QSqlQuery q = getQuery();
  q.exec(DB_ALL_BELLS);

  QList<int> result;

  while (q.next())
    result.append(q.value(0).toInt());

  return result;
}

QList<int> Database::getBellsForDay(int day) const
{
  QSqlQuery q = getQuery();
  q.prepare(DB_BELLS_FOR_DAY);
  q.bindValue(":bell_day", day);
  q.exec();

  QList<int> results;

  while (q.next())
    results.append(q.value(0).toInt());

  return results;
}

QString Database::getBellTime(int id) const
{
  QSqlQuery q = getQuery();
  q.prepare(DB_BELL_TIME);
  q.bindValue(":bell_id", id);
  q.exec();

  if (q.first())
    return q.value(0).toString();

  return "ERROR";
}

QString Database::getBellName(int id) const
{
  QSqlQuery q = getQuery();
  q.prepare(DB_BELL_NAME);
  q.bindValue(":bell_id", id);
  q.exec();

  if (q.first())
    return q.value(0).toString();

  return "ERROR";
}

QString Database::getBellFile(int id) const
{
  QSqlQuery q = getQuery();
  q.prepare(DB_BELL_FILE);
  q.bindValue(":bell_id", id);
  q.exec();

  if (q.first())
    return q.value(0).toString();

  return "ERROR";
}

int Database::getBellDay(int id) const
{
  QSqlQuery q = getQuery();
  q.prepare(DB_BELL_DAY);
  q.bindValue(":bell_id", id);
  q.exec();

  if (q.first())
    return q.value(0).toInt();

  return -1;
}

void Database::addBell(QString name, QString file, QString time, int day) const
{
  QSqlQuery q = getQuery();
  q.prepare(DB_ADD_BELL);
  q.bindValue(":bell_name", name);
  q.bindValue(":bell_file", file);
  q.bindValue(":bell_time", time);
  q.bindValue(":bell_day", day);
  q.exec();
}

void Database::delBell(int id) const
{
  QSqlQuery q = getQuery();
  q.prepare(DB_DEL_BELL);
  q.bindValue(":bell_id", id);
  q.exec();
}

void Database::updateBell(int id, QString name, QString file, QString time, int day) const
{
  QSqlQuery q = getQuery();
  q.prepare(DB_UPD_BELL);
  q.bindValue(":bell_id", id);
  q.bindValue(":bell_name", name);
  q.bindValue(":bell_file", file);
  q.bindValue(":bell_time", time);
  q.bindValue(":bell_day", day);
  q.exec();
}
