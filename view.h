#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets>
#include "model.h"

class View : public QWidget
{
    Q_OBJECT

  private:
    Model *m;

    QListWidget *lwBells;
    QPushButton *pbAddBell;
    QPushButton *pbDelBell;
    QLineEdit   *leBellName;
    QLineEdit   *leBellFile;
    QTimeEdit   *teBellTime;
    QDateEdit   *deBellDay;

    int         selectedBellId;
    int         currentDay;

    void        setUpGui();
    void        updateListWidget();

  private slots:
    void  addBell();
    void  delBell();
    void  updBell();
    void  openFileDialog();
    void  updateSelectedDay(QDate date);
    void  loadItemData(QListWidgetItem *item);

  public:
    View(QWidget *parent = nullptr);
};

#endif // VIEW_H
