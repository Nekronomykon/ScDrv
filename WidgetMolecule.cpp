#include "WidgetMolecule.h"

#include <vtkRenderWindow.h>

#include "EditSource.h"

namespace
{
  static inline const char *keyState() { return "Molecule Widget"; }
}

WidgetMolecule::WidgetMolecule(QWidget *parent)
    : QSplitter(parent), edit_(new EditMolecule(this)), view_(new ViewMolecule(this))
{
  this->setOpaqueResize(false);
  //
  this->addWidget(view_);
  this->addWidget(edit_);
  //
  this->setCollapsible(this->indexOf(view_), false);
  this->setCollapsible(this->indexOf(edit_), true);
  //
  view_->resetMolecule(molecule_);
  edit_->resetMolecule(molecule_);
  //
  QObject::connect(edit_->sourceAtoms(), &QTextDocument::blockCountChanged,
                   this, &WidgetMolecule::on_changedBlockCount);
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
  edit_->resetMolecule();
  view_->resetMolecule();
  view_->renderWindow()->Render();
}
//
///////////////////////////////////////////////////////////////////////
/// Dispatch updating:
///
void WidgetMolecule::clearAll()
{
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
  edit_->setReadOnly(bSet);
}
//
///////////////////////////////////////////////////////////////////////
/// Dispatch updating:
///
void WidgetMolecule::on_changedBlockCount(int newBlockCount)
{
}
//
///////////////////////////////////////////////////////////////////////
