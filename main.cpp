#include <QApplication>
#include "view.h"

int main (int argc, char **argv)
{
  QApplication a(argc, argv);

  View v;

  return a.exec();
}
