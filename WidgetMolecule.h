#ifndef Widget_Molecule_h__
#define Widget_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QPointer>
#include <QSettings>

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
    typedef vtkMolecule Molecule;
    //
    explicit WidgetMolecule(QWidget * /*parent*/ = nullptr);
    ~WidgetMolecule() override = default;

    vtkMolecule *getMolecule() const { return molecule_; }

    void readSettings(QSettings & /*src*/);
    void saveSettings(QSettings & /*src*/);
    //
    EditMolecule *getEdit() const { return edit_; }
    ViewMolecule *getView() const { return view_; }
    //
    void showMolecule();

private:
    vtkNew<Molecule> molecule_;
    QPointer<EditMolecule> edit_;
    QPointer<ViewMolecule> view_;
};

#endif // !Widget_Molecule_h__
