#ifndef Edit_Molecule_h__
#define Edit_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QPointer>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkMolecule.h>
#include <vtkMoleculeMapper.h>

#include "ViewMolecule.h"
#include "EditSource.h"

class EditMolecule
    : public QSplitter
{
    Q_OBJECT
public:
    explicit EditMolecule(QWidget * /*parent*/ = nullptr);
    ~EditMolecule() override = default;

private:
    QPointer<EditSource> source_;
    QPointer<ViewMolecule> molecule_;
};

#endif // !Edit_Molecule_h__
