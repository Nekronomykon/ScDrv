#include "ViewPathList.h"

#include "ModelPathList.h"

#include <QString>
#include <QWidget>
#include <QStringList>

ViewPathList::ViewPathList(QWidget *parent)
    : _Base(parent)
{
}

bool ViewPathList::hasString(const QString &one) const
{
  const QStringList slist = this->GetViewModel()->stringList();
  return slist.contains(one);
}

void ViewPathList::addString(const QString &one)
{
  QStringList slist = this->GetViewModel()->stringList();
  slist.push_back(one);
  this->GetViewModel()->setStringList(slist);
}
