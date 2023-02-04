#include "FrameDoc.h"

#include <vtkPDBReader.h>
#include <vtkGaussianCubeReader.h>
#include <vtkGaussianCubeReader2.h>
#include <vtkXYZMolReader.h>
#include <vtkXYZMolReader2.h>

FrameDoc::FrameDoc(QWidget *parent)
    : QTabWidget(parent), wMol_(new WidgetMolecule(this))
{
    this->setDocumentMode(true);
    this->setTabPosition(QTabWidget::South);
    this->setTabBarAutoHide(true);
    // this->setTabsClosable(true);
    this->setUsesScrollButtons(true);
    //
    this->addTab(wMol_, tr("Edit atoms"));
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

bool FrameDoc::ReadFilePDB(Path a_path)
{
    vtkNew<vtkPDBReader> reader;
    reader->SetFileName(a_path.c_str());
    return false;
}

bool FrameDoc::ReadFileXYZ(Path a_path)
{
    vtkNew<vtkXYZMolReader> reader;
    vtkNew<vtkXYZMolReader2> reader2;
    reader->SetFileName(a_path.c_str());
    reader2->SetFileName(a_path.c_str());
    return false;
}

bool FrameDoc::ReadFileCUBE(Path a_path)
{
    vtkNew<vtkGaussianCubeReader> reader;
    vtkNew<vtkGaussianCubeReader2> reader2;
    reader->SetFileName(a_path.c_str());
    reader2->SetFileName(a_path.c_str());
    return false;
}

// bool FrameDoc::ReadFileCUBE(Path)
//{
//    return false;
//}

bool FrameDoc::isModified() const
{
    return false;
}

bool FrameDoc::doSave()
{
    return true;
}

WidgetMolecule *FrameDoc::editMolecule()
{
    if (wMol_)
        this->setCurrentWidget(wMol_);
    return wMol_;
}
WidgetMolecule *FrameDoc::getEditMolecule() const
{
    return wMol_;
}

bool FrameDoc::hasPath() const
{
    return path_.empty() ? false : true;
}

const Path &FrameDoc::getPath() const
{
    return path_;
}
Path FrameDoc::resetPath(Path pathNew)
{
    std::swap(pathNew, path_);
    return pathNew;
}

// void FrameDoc::