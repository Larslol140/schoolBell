#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

const QString DB_FILE   = "data.db";
const QString DB_CREATE = "CREATE TABLE bells ("
                          " bell_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          " bell_name VARCHAR(64) NOT NULL,"
                          " bell_file VARCHAR(255) NOT NULL,"
                          " bell_day INTEGER NOT NULL,"
                          " bell_time VARCHAR(5) NOT NULL"
                          ");";

const QString DB_ALL_BELLS      = "SELECT bell_id FROM bells;";
const QString DB_BELLS_FOR_DAY  = "SELECT bell_id FROM bells "
                                  "WHERE bell_day = :bell_day;";
const QString DB_BELL_NAME      = "SELECT bell_name FROM bells "
                                  "WHERE bell_id = :bell_id;";
const QString DB_BELL_FILE      = "SELECT bell_file FROM bells "
                                  "WHERE bell_id = :bell_id;";
const QString DB_BELL_DAY       = "SELECT bell_day FROM bells "
                                  "WHERE bell_id = :bell_id;";
const QString DB_BELL_TIME      = "SELECT bell_time FROM bells "
                                  "WHERE bell_id = :bell_id;";
const QString DB_ADD_BELL       = "INSERT INTO bells (bell_name, bell_file, bell_day, bell_time) "
                                  "VALUES (:bell_name, :bell_file, :bell_day, :bell_time);";
const QString DB_UPD_BELL       = "UPDATE bells "
                                  "SET bell_name = :bell_name, bell_file = :bell_file, bell_time = :bell_time, bell_day = :bell_day "
                                  "WHERE bell_id = :bell_id";
const QString DB_DEL_BELL       = "DELETE FROM bells "
                                  "WHERE bell_id = :bell_id;";

#endif // CONFIG_H
