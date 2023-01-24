#include "ViewPathList.h"

#include "ModelPathList.h"

#include <QString>
#include <QWidget>
#include <QStringList>

ViewPathList::ViewPathList(QWidget *parent)
    : QListView(parent), modelPathList_(new ModelPathList)
{
    this->setModel(modelPathList_);
}
bool ViewPathList::hasString(const QString &one) const
{
    const QStringList slist = modelPathList_->stringList();
    return slist.contains(one);
}

void ViewPathList::addString(const QString &one)
{
    QStringList slist = modelPathList_->stringList();
    slist.push_back(one);
    modelPathList_->setStringList(slist);
}
