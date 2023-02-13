#ifndef Widget_Molecule_h__
#define Widget_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QPointer>
#include <QSettings>

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

  Molecule *getMolecule() const { return molecule_; }

  void readSettings(QSettings & /*src*/);
  void saveSettings(QSettings & /*src*/);
  //
  EditMolecule *getEdit() const { return edit_; }
  ViewMolecule *getView() const { return view_; }
  //
  void showMolecule();
  void clearAll();
  void setReadOnly(bool /* bSet */ = true);

private slots:
  void on_changedBlockCount(int /* newBlockCount */);

private:
  ANewMolecule molecule_;
  QPointer<EditMolecule> edit_;
  QPointer<ViewMolecule> view_;
};

#endif // !Widget_Molecule_h__
