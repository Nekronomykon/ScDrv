#include "ViewWorkspace.h"

ViewWorkspace::ViewWorkspace(QWidget *parent)
    : QSplitter(Qt::Vertical, parent), files_(new ViewPathList(this)), content_(new ViewFileContent(this))
{
    this->setOpaqueResize(false);
    //
    this->addWidget(files_);
    this->addWidget(content_);
}
//
///////////////////////////////////////////////////////////////////////
/// adding an unique path string
///
bool ViewWorkspace::addPathString(const QString &one)
{
    if (files_->hasString(one))
        return false;
    files_->addString(one);
    return this->hasPathTypeAssumed(one)       // --> casting a file type
           && this->isFileTypeCompatible(one); // --> preparsing file
}
//
///////////////////////////////////////////////////////////////////////
/// casting file type from its name; simply text file otherwise
///
bool ViewWorkspace::hasPathTypeAssumed(const QString & /*one*/) 
{
    return true;
}
//
///////////////////////////////////////////////////////////////////////
/// here the file itself is preparsed to know its compatibility
///
bool ViewWorkspace::isFileTypeCompatible(const QString & /*one*/) 
{
    return true;
}
//
///////////////////////////////////////////////////////////////////////
