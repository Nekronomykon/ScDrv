#include "ViewPathList.h"

ViewPathList::ViewPathList(QWidget *parent)
    : QListView(parent), modelPathList_(new QStringListModel)
{
    this->setModel(modelPathList_);
}
