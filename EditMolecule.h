#ifndef Edit_Molecule_h__
#define Edit_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <map>

#include <QWidget>

#include <vtkMolecule.h>

#include "Elements.h"

#include "ui_EditMolecule.h"

typedef std::map<IndexElement, vtkIdType> ChemFormula;

class EditMolecule
    : public QWidget,
      private Ui_EditMolecule
{
    Q_OBJECT

public:
    explicit EditMolecule(QWidget * /*parent*/ = nullptr);
    ~EditMolecule() override = default;

    void setReadOnly(bool /*bReadOnly*/ = true);
    vtkIdType readAtoms(vtkMolecule * /*pMol*/);

    void resetMolecule(vtkMolecule * /*pMol*/ = nullptr);

private:
    vtkMolecule *ptrMolecule_ = nullptr;
};

#endif // !Edit_Molecule_h__
