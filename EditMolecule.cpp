#include "EditMolecule.h"

///////////////////////////////////////////////////////////////////////////////
//
EditMolecule::EditMolecule(QWidget *parent)
    : QWidget(parent)
{
  this->setupUi(this);
}
//
///////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::setReadOnly(bool bRO)
{
  this->lineTitle_->setReadOnly(bRO);
  this->editSource_->setReadOnly(bRO);
}
//
///////////////////////////////////////////////////////////////////////////////
//
vtkIdType EditMolecule::readAtoms(vtkMolecule *pMol)
{
  pMol->Initialize();
  // editSource_->
  return pMol->GetNumberOfAtoms();
}
//
///////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::resetMolecule(vtkMolecule *pMol)
{
  if (pMol != ptrMolecule_)
  {
    ptrMolecule_ = pMol;
  } // read data from ptrMolecule_
    // or make them default if ptrMolecule_ == nullptr
}
//
///////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::loadMolecule()
{
  QPointer<QTextDocument> pDoc(new QTextDocument);
  editSource_->clear();
  int nAtoms = (!ptrMolecule_) ? 0 : ptrMolecule_->GetNumberOfAtoms();
  for(int j = 0; j < nAtoms; ++j)
  {
    QString strAtom;

    strAtom += '\n';
    editSource_->appendPlainText(strAtom);
  }
}
QTextDocument *EditMolecule::sourceAtoms() const
{
  return editSource_->document();
}
//
///////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::on_toolEditAtoms__triggered(void)
{
  editSource_->setReadOnly(false);
  editSource_->setFocus();
}