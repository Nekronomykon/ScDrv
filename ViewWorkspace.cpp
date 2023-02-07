#include "ViewWorkspace.h"

#include <QStringLiteral>

namespace
{
    static inline const char *keyState() { return "Workspace"; }
    static inline QString keyNewFile() { return QStringLiteral("[: new file :]"); }
};

ViewWorkspace::ViewWorkspace(QWidget *parent)
    : QSplitter(Qt::Vertical, parent), files_(new ViewPathList(this)), content_(new ViewFileContent(this))
{
    this->setOpaqueResize(false);
    //
    this->addWidget(files_);
    this->addWidget(content_);
    //
    files_->addString(keyNewFile());
}
//
///////////////////////////////////////////////////////////////////////
/// read settings from the corresponding class instance:
///
void ViewWorkspace::readSettings(QSettings &src)
{
    // splitter state
    this->restoreState(src.value(keyState()).toByteArray());
}
//
///////////////////////////////////////////////////////////////////////
/// write settings to the corresponding class instance:
///
void ViewWorkspace::saveSettings(QSettings &src)
{
    // splitter state
    src.setValue(keyState(), this->saveState());
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
ViewPathList *ViewWorkspace::getPathList() const { return files_; }
//
///////////////////////////////////////////////////////////////////////
//
ViewFileContent *ViewWorkspace::getFileContent() const { return content_; }
//
///////////////////////////////////////////////////////////////////////
//
