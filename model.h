#ifndef MODEL_H
#define MODEL_H

#include <QtMultimedia>
#include "database.h"

class Model : public QObject
{
    Q_OBJECT

  private:
    static Model    *instance;
                    Model(QObject *parent);

    Database        *db;
    QMediaPlayer    *mp;
    QHash<QString, int>  currentBellTimes;
    int             currentDay;
    QString         lastPlayTime;

    int             getCurrentDayOfWeek() const;
    void            updateBellTimes();
    void            playBellSound(int id);
    void            updateTimesIfNewDay();
    void            updateCurrentDay();
    void            playSoundIfCorrectTime();

    bool            currentTimeInBellList() const;
    bool            isNewMinute();

  public:
    static Model    *getInstance(QObject *parent = nullptr);
                    ~Model();

    Database        *getDatabase() const;
    QHash<int, QString> getBellsForDay(int dayOfWeek) const;
    QString         formatedText(int id) const;

    void            addBell(QString name, QString file, QString time, int day);
    void            delBell(int id);

    static QTime    convertTime(QString time);
    static QString  convertTime(QTime time);
    static int      convertDay(QString day);
    static QString  convertDay(int day);

  private slots:
    void            checkBellTimes();
};

#endif // MODEL_H
