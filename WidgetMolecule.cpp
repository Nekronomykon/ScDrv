#include "WidgetMolecule.h"

#include <vtkRenderWindow.h>

#include <QMessageBox>

#include "ResetCursor.h"
#include "EditSource.h"

namespace
{
  static inline const char *keyState() { return "Molecule Widget"; }
}

WidgetMolecule::WidgetMolecule(QWidget *parent)
    : QSplitter(parent), editMol_(new EditMolecule(this)), view_(new ViewMolecule(this))
{
  this->setOpaqueResize(false);
  //
  this->addWidget(view_);
  this->addWidget(editMol_);
  //
  this->setCollapsible(this->indexOf(view_), false);
  this->setCollapsible(this->indexOf(editMol_), true);
  //
  view_->resetMolecule(molecule_);
  editMol_->resetMolecule(molecule_);
  //
  connect(editMol_->editSource(), &EditSource::blockCountChanged,
          this, &WidgetMolecule::on_changedBlockCount);
  connect(editMol_->editSource(), &EditSource::textChanged,
          this, &WidgetMolecule::on_changedSource);
}
//
///////////////////////////////////////////////////////////////////////
/// read settings from the corresponding class instance:
///
void WidgetMolecule::readSettings(QSettings &src)
{
  // splitter state
  this->restoreState(src.value(keyState()).toByteArray());
}
//
///////////////////////////////////////////////////////////////////////
/// Write settings to the corresponding class instance:
///
void WidgetMolecule::saveSettings(QSettings &src)
{
  // splitter state
  src.setValue(keyState(), this->saveState());
}
//
///////////////////////////////////////////////////////////////////////
/// Dispatch updating:
///
void WidgetMolecule::showMolecule()
{
  editMol_->resetMolecule();
  view_->resetMolecule();
  view_->renderWindow()->Render();
}
//
///////////////////////////////////////////////////////////////////////
/// Dispatch updating:
///
void WidgetMolecule::clearAll()
{
  ResetCursor rcc(Qt::WaitCursor);
  ANewMolecule mol;
  molecule_->DeepCopy(mol);
  this->showMolecule();
}
//
///////////////////////////////////////////////////////////////////////
/// Dispatch updating:
///
void WidgetMolecule::setReadOnly(bool bSet)
{
  editMol_->setReadOnly(bSet);
}
//
///////////////////////////////////////////////////////////////////////
/// Dispatch updating:
///
void WidgetMolecule::updateMolecule()
{
  ResetCursor rcc(Qt::WaitCursor);
  ANewMolecule mol_new;
  QTextDocument *pDoc = this->getSource();
  int nBlox = pDoc->blockCount();
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr("Block count"), tr("Now Num of blox is %1").arg(nBlox));
#endif //! QT_MESSAGE_BOX_DEBUG
       // mol_new->SetNumberOfAtoms(molecule_->GetNumberOfAtoms());
  QTextBlock block = pDoc->firstBlock();
  int j = 0;
  while (block.isValid())
  {
    QString content = block.text().trimmed();

    vtkAtom atom = mol_new->AppendAtom();
    ++j;
    block = block.next();
  }
  // copy the parsed content: molecule
  // molecule_->DeepCopy(new_mol);
}
//
///////////////////////////////////////////////////////////////////////
/// Dispatch updating:
///
void WidgetMolecule::on_changedBlockCount(int newBlockCount)
{
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr("Block count"), tr("Changed to %1").arg(newBlockCount));
#endif //! QT_MESSAGE_BOX_DEBUG
  this->updateMolecule();
}
//
///////////////////////////////////////////////////////////////////////
///
void WidgetMolecule::on_changedSource(void)
{
#ifdef QT_MESSAGE_BOX_DEBUG
  QMessageBox::information(this, tr("Block count"), tr("Changed to %1").arg(editSource()->blockCount()));
#endif //! QT_MESSAGE_BOX_DEBUG
}
//
///////////////////////////////////////////////////////////////////////
