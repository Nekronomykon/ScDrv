#ifndef Widget_Molecule_h__
#define Widget_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QPointer>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkMolecule.h>

#include "EditMolecule.h"
#include "ViewMolecule.h"

class WidgetMolecule
    : public QSplitter
{
    Q_OBJECT
public:
    explicit WidgetMolecule(QWidget * /*parent*/ = nullptr);
    ~WidgetMolecule() override = default;

private:
    QPointer<EditMolecule> source_;
    QPointer<ViewMolecule> molecule_;
};

#endif // !Widget_Molecule_h__
