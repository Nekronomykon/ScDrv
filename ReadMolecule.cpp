#include "ReadMolecule.h"

using namespace vtk;
using namespace std;

//------------------------------------------------------------------------------
vtkStandardNewMacro(ReadMolecule);

//------------------------------------------------------------------------------
ReadMolecule::ReadMolecule(int nOuts, int nIns) : BuildMolecule(nOuts, nIns) {}

//------------------------------------------------------------------------------
void ReadMolecule::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
