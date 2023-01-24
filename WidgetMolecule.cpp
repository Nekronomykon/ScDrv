#include "WidgetMolecule.h"

WidgetMolecule::WidgetMolecule(QWidget *parent)
    : QSplitter(parent), source_(new EditMolecule(this)), molecule_(new ViewMolecule(this))
{
    this->setOpaqueResize(false);
    //
    this->addWidget(molecule_);
    this->addWidget(source_);
    // 
    this->setCollapsible(this->indexOf(molecule_), false);
    this->setCollapsible(this->indexOf(source_), true);
}
