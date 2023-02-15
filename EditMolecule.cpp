#include "EditMolecule.h"

using namespace std;
using namespace vtk;

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
vtkIdType EditMolecule::readAtoms(Molecule *pMol)
{
  ANewMolecule new_mol;
  // QTextCursor
  pMol->Initialize();
  // editSource_->
  return pMol->GetNumberOfAtoms();
}
//
///////////////////////////////////////////////////////////////////////////////
//
Molecule *EditMolecule::getMolecule() const
{
  return ptrMolecule_;
}
//
///////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::resetMolecule(Molecule *pMol)
{
  if (pMol != ptrMolecule_)
  {
    ptrMolecule_ = pMol;
  } // read data from ptrMolecule_
  // or make them default if ptrMolecule_ == nullptr
  this->loadMolecule();
}
//
///////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::loadMolecule()
{
  editSource_->clear();
  if (!ptrMolecule_)
    return;

  int nAtoms = ptrMolecule_->GetNumberOfAtoms();
  QPointer<QTextDocument> pDoc(new QTextDocument);

  for (int j = 0; j < nAtoms; ++j)
  {
    QString strAtom(tr("  Atom %1: \n").arg(j+1) );

    strAtom += '\n';
    editSource_->appendPlainText(strAtom.trimmed());
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
//
///////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::on_toolEditTitle__triggered(void)
{
  lineTitle_->setReadOnly(false);
  lineTitle_->setFocus();
}