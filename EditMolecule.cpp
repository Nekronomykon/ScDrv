#include "EditMolecule.h"

EditMolecule::EditMolecule(QWidget *parent)
    : QWidget(parent)
{
  this->setupUi(this);
}

void EditMolecule::setReadOnly(bool bRO)
{
  this->lineTitle_->setReadOnly(bRO);
  this->editSource_->setReadOnly(bRO);
}

vtkIdType EditMolecule::readAtoms(vtkMolecule *pMol)
{
  pMol->Initialize();
  // editSource_->
  return pMol->GetNumberOfAtoms();
}
