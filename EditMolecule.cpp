#include "EditMolecule.h"

EditMolecule::EditMolecule(QWidget *parent)
    : QSplitter(parent), source_(new EditSource(this)), molecule_(new ViewMolecule(this))
{
    this->addWidget(molecule_);
    this->addWidget(source_);
    // 
    this->setCollapsible(this->indexOf(molecule_), false);
    this->setCollapsible(this->indexOf(source_), true);
}
