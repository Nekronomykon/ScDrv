#include "ModelPathList.h"

ModelPathList::ModelPathList()
    : QStringListModel()
{
  QStringList slist(tr("[:: new ::]"));
  this->setStringList(slist);
}
