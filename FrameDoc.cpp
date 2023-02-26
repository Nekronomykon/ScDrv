#include "FrameDoc.h"

#include <vtkPDBReader.h>
#include <vtkGaussianCubeReader.h>
#include <vtkGaussianCubeReader2.h>
#include <vtkCMLMoleculeReader.h>
#include <vtkXYZMolReader.h>
#include <vtkXYZMolReader2.h>

#include "ReadMoleculeWFN.h"
#include "ReadMoleculeWFX.h"
#include "ReadMoleculeXYZ.h"

#include <vtkPNGWriter.h>
#include <vtkTIFFWriter.h>
#include <vtkBMPWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

#include "MakeBondsDist.h"

using namespace vtk;
using namespace std;

const FrameDoc::AllFileFormats FrameDoc::AllFormats{
    // import
    {"cml", "Chemical Markup Language molecule", &FrameDoc::ReadMoleculeCML, nullptr},
    {"cube", "CUBE molecular field", &FrameDoc::ReadFieldCUBE, nullptr},
    {"pdb", "PDB molecule", &FrameDoc::ReadMoleculePDB, nullptr},
    {"wfn", "Wavefunction", &FrameDoc::ReadMoleculeWFN, nullptr},
    {"wfx", "Wavefunction Extended", &FrameDoc::ReadMoleculeWFX, nullptr},
    {"xyz", "XMol XYZ atoms", &FrameDoc::ReadMoleculeXYZ, nullptr},
    // export
    {"bmp", "Bitmap file", nullptr, &FrameDoc::ExportPixBMP},
    {"tiff", "Tagged image file", nullptr, &FrameDoc::ExportPixTIFF},
    {"png", "Portable Network Graphics file", nullptr, &FrameDoc::ExportPixPNG},
    {"jpeg", "JPEG file", nullptr, &FrameDoc::ExportPixJPEG},
    {"ps", "PostScript file", nullptr, &FrameDoc::ExportPixPostScript},
    // {nullptr, nullptr, nullptr} // invalid
};

const char FrameDoc::strAllFiles[] = ";;All files (*.*)";

FileFormat FrameDoc::FormatForSuffix(const QString &sfx)
{
  FileFormat fmt_res;
  for (const FileFormat &fmt_one : AllFormats)
  {
    if (sfx.compare(tr(fmt_one.suffix())))
      continue;
    fmt_res = fmt_one;
    break;
  }
  return fmt_res;
}

FrameDoc::FrameDoc(QWidget *parent)
    : QTabWidget(parent), wTable_(new TableElements(this)), wText_(new EditSource(this)),
      wMol_(new WidgetMolecule(this)), wStruct_(new WidgetStructure(this))
{
  this->setDocumentMode(true);
  this->setTabPosition(QTabWidget::South);
  this->setTabBarAutoHide(true);
  // this->setTabsClosable(true);
  this->setUsesScrollButtons(true);
  //
  this->addTab(wTable_, tr("Elements"));
  this->addTab(wText_, tr("Comment"));
  this->addTab(wMol_, tr("Atoms"));
  this->addTab(wStruct_, tr("Structure"));
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameDoc::clearAll(bool bModified)
{
  wMol_->clearAll();
  wText_->clear();
  this->updateAllViews();
  this->setReadOnly(false);
  this->setModified(bModified);
}
//
///////////////////////////////////////////////////////////////////////
//
QString FrameDoc::getExportFilter()
{
  QString sFlt(tr("Known formats ("));
  QString sAll;
  for (const FrameDoc::FileFormat &fmt : FrameDoc::getFormats())
  {
    if (!fmt.hasSuffix() || !fmt.isValidFormat() || !fmt.hasSave())
      continue;
    //
    QString sExtFmt(tr("*."));
    sExtFmt += fmt.mask_path_;
    sFlt += sExtFmt;
    sFlt += ' ';
    //
    QString sNameFmt(tr(";;"));
    sNameFmt += fmt.description_;
    sNameFmt += " format (";
    sNameFmt += sExtFmt;
    sNameFmt += ')';
    sAll += sNameFmt;
  }
  sFlt = sFlt.trimmed();
  sFlt += ')';
  sFlt += sAll;
  sFlt += strAllFiles;
  return sFlt.simplified();
}
//
///////////////////////////////////////////////////////////////////////
//
QString FrameDoc::getReadFilter()
{
  QString sFlt(tr("Known formats ("));
  QString sAll;
  for (const FrameDoc::FileFormat &fmt : FrameDoc::getFormats())
  {
    if (!fmt.hasSuffix() || !fmt.isValidFormat() || !fmt.hasRead())
      continue;
    //
    QString sExtFmt(tr("*."));
    sExtFmt += fmt.mask_path_;
    sFlt += sExtFmt;
    sFlt += ' ';
    //
    QString sNameFmt(tr(";;"));
    sNameFmt += fmt.description_;
    sNameFmt += " format (";
    sNameFmt += sExtFmt;
    sNameFmt += ')';
    sAll += sNameFmt;
  }
  sFlt = sFlt.trimmed();
  sFlt += ')';
  sFlt += sAll;
  sFlt += strAllFiles;
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
//
///////////////////////////////////////////////////////////////////////
/// access to the only molecule object member:
///
vtk::Molecule *FrameDoc::getMolecule() const
{
  return wMol_->getMolecule();
}
//
///////////////////////////////////////////////////////////////////////
/// reads from CML format: both atoms and bonds:
///
bool FrameDoc::ReadMoleculeCML(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtk::Molecule *pMol = this->getMolecule();
  assert(pMol);
  vtkNew<vtkCMLMoleculeReader> reader;
  reader->SetFileName(a_path.c_str());
  reader->Update();
  // pMol->Initialize();
  pMol->DeepCopy(reader->GetOutput());
  this->updateAllViews();
  return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ReadMoleculePDB(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtk::Molecule *pMol = this->getMolecule();
  assert(pMol);
  vtkNew<vtkPDBReader> reader;
  reader->SetFileName(a_path.c_str());
  reader->Update();
  // pMol->Initialize();
  if (bGuessBonds_)
  {
    ANewMkBondsDist mkbonds;
    mkbonds->SetInputData(reader->GetOutput());
    mkbonds->Update();
    pMol->DeepCopy(mkbonds->GetOutput());
  }
  else
  {
    pMol->DeepCopy(reader->GetOutput());
  }
  this->updateAllViews();
  return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ReadMoleculeWFN(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtk::Molecule *pMol = this->getMolecule();
  assert(pMol);
  vtkNew<vtk::ReadMoleculeWFN> reader;
  reader->ResetPath(a_path);
  this->setLinearUnits(Bohr);
  reader->Update();
  // pMol->Initialize();
  if (bGuessBonds_)
  {
    ANewMkBondsDist mkbonds;
    mkbonds->SetInputData(reader->GetOutput());
    mkbonds->Update();
    pMol->DeepCopy(mkbonds->GetOutput());
  }
  else
  {
    pMol->DeepCopy(reader->GetOutput());
  }
  // update title:
  this->setMoleculeTitle(QString(reader->GetTitle().c_str()));
  this->updateAllViews();
  return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ReadMoleculeWFX(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtk::Molecule *pMol = this->getMolecule();
  assert(pMol);
  vtkNew<vtk::ReadMoleculeWFX> reader;
  reader->ResetPath(a_path);
  this->setLinearUnits(Bohr);
  reader->Update();
  // pMol->Initialize();
  if (bGuessBonds_)
  {
    ANewMkBondsDist mkbonds;
    mkbonds->SetInputData(reader->GetOutput());
    mkbonds->Update();
    pMol->DeepCopy(mkbonds->GetOutput());
  }
  else
  {
    pMol->DeepCopy(reader->GetOutput());
  }
  this->setMoleculeTitle(QString(reader->GetTitle().c_str()));
  this->updateAllViews();
  return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ReadMoleculeXYZ(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtk::Molecule *pMol = this->getMolecule();
  assert(pMol);
  vtkNew<vtk::ReadMoleculeXYZ> reader;
  reader->ResetPath(a_path);
  this->setLinearUnits(Angstrom);
  reader->Update();
  // pMol->Initialize();
  if (bGuessBonds_)
  {
    ANewMkBondsDist mkbonds;
    mkbonds->SetInputData(reader->GetOutput());
    mkbonds->Update();
    pMol->DeepCopy(mkbonds->GetOutput());
  }
  else
  {
    pMol->DeepCopy(reader->GetOutput());
  }
  this->setMoleculeTitle(QString(reader->GetTitle().c_str()));
  this->updateAllViews();
  return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ReadFieldCUBE(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtk::Molecule *pMol = this->getMolecule();
  assert(pMol);
  vtkNew<vtkGaussianCubeReader> reader;
  vtkNew<vtkGaussianCubeReader2> reader2;
  reader->SetFileName(a_path.c_str());
  this->setLinearUnits(Bohr);
  reader->Update();
  // pMol->Initialize();
  if (bGuessBonds_)
  {
    ANewMkBondsDist mkbonds;
    mkbonds->SetInputData(reader->GetOutput());
    mkbonds->Update();
    pMol->DeepCopy(mkbonds->GetOutput());
  }
  else
  {
    pMol->DeepCopy(reader->GetOutput());
  }
  this->updateAllViews();
  return bool(pMol->GetNumberOfAtoms() > 0);
}

bool FrameDoc::ExportPixBMP(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtkNew<vtkBMPWriter> saver;
  saver->SetFileName(a_path.c_str());
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr("Export BMP to"), tr(a_path.c_str()));
#endif //! QT_MESSAGE_BOX_DEBUG
  return this->getEditMolecule()->getView()->exportImageTo(saver);
}

bool FrameDoc::ExportPixTIFF(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtkNew<vtkTIFFWriter> saver;
  saver->SetFileName(a_path.c_str());
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr("Export TIFF to"), tr(a_path.c_str()));
#endif

  return this->getEditMolecule()->getView()->exportImageTo(saver);
}

bool FrameDoc::ExportPixPNG(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtkNew<vtkPNGWriter> saver;
  saver->SetFileName(a_path.c_str());
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr("Export PNG to"), tr(a_path.c_str()));
#endif

  return this->getEditMolecule()->getView()->exportImageTo(saver);
}

bool FrameDoc::ExportPixJPEG(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtkNew<vtkJPEGWriter> saver;
  saver->SetFileName(a_path.c_str());
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr("Export JPG to"), tr(a_path.c_str()));
#endif

  return this->getEditMolecule()->getView()->exportImageTo(saver, false); // RGB instead of RGBA ???
}

bool FrameDoc::ExportPixPostScript(Path a_path)
{
  ResetCursor rcc(Qt::WaitCursor);
  vtkNew<vtkPostScriptWriter> saver;
  saver->SetFileName(a_path.c_str());
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr("Export PostScript to"), tr(a_path.c_str()));
#endif

  return this->getEditMolecule()->getView()->exportImageTo(saver, false); // RGB instead of RGBA
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameDoc::updateAllViews()
{
  wMol_->showMolecule();
  wStruct_->showMolecule();
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameDoc::setLinearUnits(int units)
{
  wMol_->editAtoms()->getLinearUnits()->setCurrentIndex(units);
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameDoc::setReadOnly(bool bSet)
{
  wMol_->setReadOnly(bSet);
  wText_->setReadOnly(bSet);
}
//
///////////////////////////////////////////////////////////////////////
//
//
bool FrameDoc::isModified() const
{
  return isModified_;
}
//
///////////////////////////////////////////////////////////////////////
//
void FrameDoc::setModified(bool bSet)
{
  isModified_ = bSet;
}
//
///////////////////////////////////////////////////////////////////////
//
bool FrameDoc::doSave()
{
  return true;
}
//
///////////////////////////////////////////////////////////////////////
//
WidgetMolecule *FrameDoc::editMolecule()
{
  if (wMol_)
    this->setCurrentWidget(wMol_);
  return wMol_;
}
//
///////////////////////////////////////////////////////////////////////
//
WidgetMolecule *FrameDoc::getEditMolecule() const
{
  return wMol_;
}
//
///////////////////////////////////////////////////////////////////////
//
bool FrameDoc::hasFormat() const
{
  return format_.isValidFormat();
}
//
///////////////////////////////////////////////////////////////////////
//
FileFormat FrameDoc::getFormat() const
{
  return format_;
}
//
///////////////////////////////////////////////////////////////////////
//
FileFormat FrameDoc::resetFormat(FileFormat new_fmt)
{
  std::swap(new_fmt, format_);
  return new_fmt;
}
//
///////////////////////////////////////////////////////////////////////
//
QString FrameDoc::moleculeTitle() const
{
  return this->getEditMolecule()->editAtoms()->getEditTitle()->text();
  ;
}
void FrameDoc::setMoleculeTitle(QString strTitle)
{
  this->getEditMolecule()->editAtoms()->getEditTitle()->setText(strTitle);
}
//
///////////////////////////////////////////////////////////////////////
//
