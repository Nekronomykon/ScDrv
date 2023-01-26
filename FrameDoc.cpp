#include "FrameDoc.h"

FrameDoc::FrameDoc(QWidget *parent)
    : QTabWidget(parent), wMol_(new WidgetMolecule(this))
{
    this->setDocumentMode(true);
    this->setTabPosition(QTabWidget::South);
    // this->setTabBarAutoHide(true);
    this->setTabsClosable(true);
    this->setUsesScrollButtons(true);
    //
    this->addTab(wMol_, tr("Edit molecule"));
}
//
///////////////////////////////////////////////////////////////////////
/// read settings from the corresponding class instance:
///
void FrameDoc::readSettings(QSettings &src)
{
    if (wMol_)
        wMol_->readSettings(src);
}
//
///////////////////////////////////////////////////////////////////////
/// write settings to the corresponding class instance:
///
void FrameDoc::saveSettings(QSettings &src)
{
    if (wMol_)
        wMol_->saveSettings(src);
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
