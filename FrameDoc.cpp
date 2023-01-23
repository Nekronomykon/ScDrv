#include "FrameDoc.h"

FrameDoc::FrameDoc(QWidget *parent)
    : QTabWidget(parent)
    , editMol_(new EditMolecule(this))
{
    // this->setTabPosition(TabBottom);
    //
    this->addTab(editMol_, tr("Edit molecule"));
}

bool FrameDoc::isModified() const
{
    return false;
}

bool FrameDoc::hasPath() const
{
    return path_.empty() ? false : true;
}

const char* FrameDoc::getPath() const
{
    return path_.c_str();
}

bool FrameDoc::doSave()
{
    return true;
}
