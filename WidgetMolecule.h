#ifndef Widget_Molecule_h__
#define Widget_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QPointer>
#include <QSettings>
#include <QTextDocument>

#include "Molecule.h"

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include "EditMolecule.h"
#include "ViewMolecule.h"

class WidgetMolecule
    : public QSplitter
{
  Q_OBJECT
public:
  //
  explicit WidgetMolecule(QWidget * /*parent*/ = nullptr);
  ~WidgetMolecule() override = default;

  vtk::Molecule *getMolecule() const { return molecule_; }

  void readSettings(QSettings & /*src*/);
  void saveSettings(QSettings & /*src*/);
  //
  EditMolecule *editAtoms() const { return editMol_; }
  EditSource *editSource() const { return editMol_->editSource(); }
  QTextDocument *getSource() const { return editMol_->getSource(); }
  ViewMolecule *getView() const { return view_; }
  //
  void showMolecule();
  void clearAll();
  void setReadOnly(bool /* bSet */ = true);
  void updateMolecule();

private slots:
  void on_changedBlockCount(int /* newBlockCount */);
  void on_changedSource(void);

private:
  ANewMolecule molecule_;
  QPointer<EditMolecule> editMol_;
  QPointer<ViewMolecule> view_;
};

#endif // !Widget_Molecule_h__
