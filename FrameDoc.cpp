#include "FrameDoc.h"

FrameDoc::FrameDoc(QWidget *parent)
    : QTabWidget(parent), editMol_(new EditMolecule(this))
{
    this->setDocumentMode(true);
    this->setTabPosition(QTabWidget::South);
    // this->setTabBarAutoHide(true);
    this->setTabsClosable(true);
    this->setUsesScrollButtons(true);
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

const char *FrameDoc::getPath() const
{
    return path_.c_str();
}

bool FrameDoc::doSave()
{
    return true;
}
