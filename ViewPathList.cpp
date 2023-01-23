#include "ViewPathList.h"

ViewPathList::ViewPathList(QWidget *parent)
    : QListView(parent), modelPathList_(new QStringListModel)
{
    this->setViewModel(modelPathList_);
}
