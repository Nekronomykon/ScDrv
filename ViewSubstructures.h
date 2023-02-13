#ifndef __View_Substructures_h__
#define __View_Substructures_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <vtkMolecule.h>
using namespace vtk;

class ViewSubstructures
    : public QTreeWidget
{
  Q_OBJECT
  //
public:
  typedef vtkMolecule Molecule;
  explicit ViewSubstructures(QWidget * /*parent*/ = nullptr);
  ~ViewSubstructures() override = default;

  // this functional may be moved towards the Model class...
  Molecule *getMolecule() const { return ptrMolecule_; }
  Molecule *resetMolecule(Molecule *pMol = nullptr)
  {
    if (pMol != ptrMolecule_)
      std::swap(ptrMolecule_, pMol);
    return pMol;
  }

private:
  Molecule *ptrMolecule_ = nullptr;
  QTreeWidgetItem *itemAtoms_ = nullptr;
  QTreeWidgetItem *itemBonds_ = nullptr;
  QTreeWidgetItem *itemAngles_ = nullptr;
  QTreeWidgetItem *itemTorsions_ = nullptr;
  QTreeWidgetItem *itemFragments_ = nullptr;
  QTreeWidgetItem *itemPaths_ = nullptr;
  QTreeWidgetItem *itemStars_ = nullptr;
  QTreeWidgetItem *itemCycles_ = nullptr;
};

#endif // !__View_Substructures_h__