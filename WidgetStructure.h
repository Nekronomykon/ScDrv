#ifndef Widget_Structure_h__
#define Widget_Structure_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QPointer>
#include <QSettings>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include "Molecule.h"

#include "ViewSubstructures.h"

class WidgetStructure
    : public QSplitter
{
  Q_OBJECT
public:
  //
  explicit WidgetStructure(QWidget * /*parent*/ = nullptr);
  ~WidgetStructure() override = default;
  //
  void readSettings(QSettings & /*src*/);
  void saveSettings(QSettings & /*src*/);
  //
  void showMolecule(Molecule * /* pMol */ = nullptr);

private:
  QPointer<ViewSubstructures> viewSubstructures_;
};

#endif //! Widget_Structure_h__