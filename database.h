#ifndef DATABASE_H
#define DATABASE_H

#include <QtCore>
#include <QtSql>
#include "config.h"

class Database
{
  private:
    static        Database *instance;
                  Database(QString file);

    bool          fileExists(QString file) const;
    void          openDatabase(QString file) const;
    void          createDatabase(QString file) const;

    QSqlDatabase  getNewDatabase(QString file) const;
    QSqlDatabase  getDatabase() const;
    QSqlQuery     getQuery() const;

  public:
    static        Database *getInstance(QString file = DB_FILE);
                  ~Database();

    QList<int>    getAllBells() const;
    QList<int>    getBellsForDay(int day) const;
    QString       getBellTime(int id) const;
    QString       getBellName(int id) const;
    QString       getBellFile(int id) const;
    int           getBellDay(int id) const;
    void          addBell(QString name, QString file, QString time, int day) const;
    void          delBell(int id) const;
    void          updateBell(int id, QString name, QString file, QString time, int day) const;
};

#endif // DATABASE_H
