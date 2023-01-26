#include "ViewFileSystem.h"

#include <QDir>

ViewFileSystem::ViewFileSystem(QWidget *parent)
    : QSplitter(Qt::Vertical, parent),
      model_files_(new QFileSystemModel),
      tree_files_(new QTreeView(this)),
      list_files_(new QListView(this))
{
    model_files_->setRootPath(QDir::rootPath());
    //
    tree_files_->setModel(model_files_);
    tree_files_->setRootIndex(model_files_->index(QDir::currentPath()));
    //
    list_files_->setModel(model_files_);
    list_files_->setRootIndex(model_files_->index(QDir::currentPath()));
    //
    this->addWidget(tree_files_);
    this->addWidget(list_files_);
    this->setOpaqueResize(false);
}
//
///////////////////////////////////////////////////////////////////////
/// read settings from the corresponding class instance:
///
void ViewFileSystem::readSettings(QSettings &src)
{
    // splitter state
    this->restoreState(src.value(keyState()).toByteArray());
}
//
///////////////////////////////////////////////////////////////////////
/// write settings to the corresponding class instance:
///
void ViewFileSystem::saveSettings(QSettings &src)
{
    // splitter state
    src.setValue(keyState(), this->saveState());
}
//
