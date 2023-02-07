#include "FrameDoc.h"

#include <vtkPDBReader.h>
#include <vtkGaussianCubeReader.h>
#include <vtkGaussianCubeReader2.h>
#include <vtkCMLMoleculeReader.h>
#include <vtkXYZMolReader.h>
#include <vtkXYZMolReader2.h>

#include <vtkPNGWriter.h>
#include <vtkTIFFWriter.h>
#include <vtkBMPWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

const FrameDoc::AllFileFormats FrameDoc::AllFormats{
    // import
    {"xyz", "XYZ atoms", &FrameDoc::ReadFileXYZ, nullptr},
    {"cml", "Chemical Markup Language molecule", &FrameDoc::ReadFileCML, nullptr},
    {"pdb", "PDB molecule", &FrameDoc::ReadFilePDB, nullptr},
    {"cube", "CUBE molecular field", &FrameDoc::ReadFileCUBE, nullptr},
    // export
    {"bmp", "Bitmap file", nullptr, &FrameDoc::ExportPixBMP},
    {"tiff", "Tagged image file", nullptr, &FrameDoc::ExportPixTIFF},
    {"png", "Portable Network Graphics file", nullptr, &FrameDoc::ExportPixPNG},
    {"jpeg", "JPEG file", nullptr, &FrameDoc::ExportPixJPEG},
    {"ps", "Bitmap file", nullptr, &FrameDoc::ExportPixPostScript},
    // {nullptr, nullptr, nullptr} // invalid
};

FrameDoc::FrameDoc(QWidget *parent)
    : QTabWidget(parent), wTable_(new TableElements(this)), wMol_(new WidgetMolecule(this)), wText_(new EditSource(this))
{
    this->setDocumentMode(true);
    this->setTabPosition(QTabWidget::South);
    this->setTabBarAutoHide(true);
    // this->setTabsClosable(true);
    this->setUsesScrollButtons(true);
    //
    this->addTab(wTable_, tr("Elements"));
    this->addTab(wMol_, tr("Atoms"));
    this->addTab(wText_, tr("Comment"));
}
//
///////////////////////////////////////////////////////////////////////
//
QString FrameDoc::getExportFilter()
{
    QString sFlt(tr("Known formats ( "));
    QString sAll;
    for (const FrameDoc::FileFormat &fmt : FrameDoc::getFormats())
    {
        if (!fmt.description_ || !fmt.mask_path_ || !fmt.opSave_)
            continue;
        //
        QString sExtFmt(tr("*."));
        sExtFmt += fmt.mask_path_;
        sFlt += sExtFmt;
        sFlt += ' ';
        //
        QString sNameFmt(tr(";;"));
        sNameFmt += fmt.description_;
        sNameFmt += " format ( ";
        sNameFmt += sExtFmt;
        sNameFmt += " )";
        sAll += sNameFmt;
    }
    sFlt += ')';
    sFlt += sAll;
    sFlt += ";;All files ( *.* )";
    return sFlt.simplified();
}
//
///////////////////////////////////////////////////////////////////////
//
QString FrameDoc::getReadFilter()
{
    QString sFlt(tr("Known formats ( "));
    QString sAll;
    for (const FrameDoc::FileFormat &fmt : FrameDoc::getFormats())
    {
        if (!fmt.description_ || !fmt.mask_path_ || !fmt.opRead_)
            continue;
        //
        QString sExtFmt(tr("*."));
        sExtFmt += fmt.mask_path_;
        sFlt += sExtFmt;
        sFlt += ' ';
        //
        QString sNameFmt(tr(";;"));
        sNameFmt += fmt.description_;
        sNameFmt += " format ( ";
        sNameFmt += sExtFmt;
        sNameFmt += " )";
        sAll += sNameFmt;
    }
    sFlt += ')';
    sFlt += sAll;
    sFlt += ";;All files ( *.* )";
    return sFlt.simplified();
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

bool FrameDoc::ReadFileCML(Path a_path)
{
    vtkNew<vtkCMLMoleculeReader> reader;
    reader->SetFileName(a_path.c_str());
    reader->Update();
    WidgetMolecule::Molecule *pMol = wMol_->getMolecule();
    assert(pMol != nullptr);
    // pMol->Initialize();
    pMol->DeepCopy(reader->GetOutput());
    return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ReadFilePDB(Path a_path)
{
    vtkNew<vtkPDBReader> reader;
    reader->SetFileName(a_path.c_str());
    reader->Update();
    WidgetMolecule::Molecule *pMol = wMol_->getMolecule();
    assert(pMol != nullptr);
    // pMol->Initialize();
    pMol->DeepCopy(reader->GetOutput());
    return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ReadFileXYZ(Path a_path)
{
    vtkNew<vtkXYZMolReader> reader;
    vtkNew<vtkXYZMolReader2> reader2;
    reader->SetFileName(a_path.c_str());
    reader2->SetFileName(a_path.c_str());
    reader->Update();
    WidgetMolecule::Molecule *pMol = wMol_->getMolecule();
    assert(pMol != nullptr);
    // pMol->Initialize();
    pMol->DeepCopy(reader->GetOutput());
    return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ReadFileCUBE(Path a_path)
{
    vtkNew<vtkGaussianCubeReader> reader;
    vtkNew<vtkGaussianCubeReader2> reader2;
    reader->SetFileName(a_path.c_str());
    reader2->SetFileName(a_path.c_str());
    reader->Update();
    WidgetMolecule::Molecule *pMol = wMol_->getMolecule();
    assert(pMol != nullptr);
    // pMol->Initialize();
    pMol->DeepCopy(reader->GetOutput());
    return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ExportPixBMP(Path a_path)
{
    vtkNew<vtkBMPWriter> saver;
    saver->SetFileName(a_path.c_str());
    return this->getEditMolecule()->getView()->exportImageTo(saver);
}

bool FrameDoc::ExportPixTIFF(Path a_path)
{
    vtkNew<vtkTIFFWriter> saver;
    saver->SetFileName(a_path.c_str());
    return this->getEditMolecule()->getView()->exportImageTo(saver);
}

bool FrameDoc::ExportPixPNG(Path a_path)
{
    vtkNew<vtkPNGWriter> saver;
    saver->SetFileName(a_path.c_str());
    return this->getEditMolecule()->getView()->exportImageTo(saver);
}

bool FrameDoc::ExportPixJPEG(Path a_path)
{
    vtkNew<vtkJPEGWriter> saver;
    saver->SetFileName(a_path.c_str());
    return this->getEditMolecule()->getView()->exportImageTo(saver);
}

bool FrameDoc::ExportPixPostScript(Path a_path)
{
    vtkNew<vtkPostScriptWriter> saver;
    saver->SetFileName(a_path.c_str());
    return this->getEditMolecule()->getView()->exportImageTo(saver);
}

//
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
