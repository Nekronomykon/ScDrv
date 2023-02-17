#ifndef Edit_Molecule_h__
#define Edit_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <map>

#include <QWidget>
#include <QTextDocument>

#include "Molecule.h"

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
  vtkIdType readAtoms(vtk::Molecule * /*pMol*/);

  vtk::Molecule *getMolecule() const;
  void resetMolecule(vtk::Molecule * /*pMol*/ = nullptr);
  void loadMolecule();

  EditSource *editSource() const { return editSource_; }
  QTextDocument *getSource() const;

private slots:
  void on_toolEditAtoms__triggered(void);
  void on_toolEditTitle__triggered(void);

private:
  vtk::Molecule *ptrMolecule_ = nullptr;
};

#endif // !Edit_Molecule_h__
