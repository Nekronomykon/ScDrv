#include "WidgetStructure.h"

using namespace vtk;
using namespace std;

namespace
{
  static inline const char *keyState() { return "Structure Widget"; }
}


WidgetStructure::WidgetStructure(QWidget *parent)
    : QSplitter(parent), viewSubstructures_(new ViewSubstructures(this))
{
  this->setOpaqueResize(false);
  //
  this->addWidget(viewSubstructures_);
}
//
///////////////////////////////////////////////////////////////////////
/// read settings from the corresponding class instance:
///
void WidgetStructure::readSettings(QSettings &src)
{
  // splitter state
  this->restoreState(src.value(keyState()).toByteArray());
}
//
///////////////////////////////////////////////////////////////////////
/// Write settings to the corresponding class instance:
///
void WidgetStructure::saveSettings(QSettings &src)
{
  // splitter state
  src.setValue(keyState(), this->saveState());
}
//
void WidgetStructure::showMolecule(Molecule* /* pMol */)
{}