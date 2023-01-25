#ifndef Edit_Molecule_h__
#define Edit_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QWidget>

#include "ui_EditMolecule.h"

class EditMolecule
    : public QWidget,
      private Ui_EditMolecule
{
    Q_OBJECT
public:
    explicit EditMolecule(QWidget * /*parent*/ = nullptr);
    ~EditMolecule() override = default;

    void setReadOnly(bool /*bReadOnly*/ = true);
};

#endif // !Edit_Molecule_h__
