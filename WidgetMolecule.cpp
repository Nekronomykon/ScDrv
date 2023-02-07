#include "WidgetMolecule.h"

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
    view_->resetMolecule();
    edit_->resetMolecule();
}
