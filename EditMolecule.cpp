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
//
void EditMolecule::resetMolecule(vtkMolecule *pMol)
{
  if (pMol != ptrMolecule_)
    ptrMolecule_ = pMol;
  // read data from ptrMolecule_
  // or make them default if ptrMolecule_ == nullptr
}
//
///////////////////////////////////////////////////////////////////////////////
