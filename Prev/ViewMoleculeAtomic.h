#ifndef ViewMolecule_Atomic_h
#define ViewMolecule_Atomic_h

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QPointer>
#include <QWidget>

#include "DelegateElementTable.h"
#include "DelegateElementSpin.h"

#include "ModelMoleculeAtomic.h"

#include <QTableView>
#include "ImplModelView.h"

class ViewMoleculeAtomic
    : public ImplModelView<ViewMoleculeAtomic,QTableView,ModelMoleculeAtomic>
{
  typedef ImplModelView<ViewMoleculeAtomic,QTableView,ModelMoleculeAtomic> _Base;
  Q_OBJECT
public:
  explicit ViewMoleculeAtomic (QWidget* /*parent*/ = nullptr);
  ~ViewMoleculeAtomic () override;

private:
  QPointer<DelegateElementTable> delegate_Mendeleev_;
  QPointer<DelegateElementSpin> delegate_ScrollElements_;
};

#endif // ViewMolecule_Atomic_h
