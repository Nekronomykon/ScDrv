#include "SaveMolecule.h"

using namespace vtk;
using namespace std;

//------------------------------------------------------------------------------
vtkStandardNewMacro(SaveMolecule);

//------------------------------------------------------------------------------
SaveMolecule::SaveMolecule(int nOuts, int nIns) : BuildMolecule(nOuts, nIns) {}

//------------------------------------------------------------------------------
void SaveMolecule::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
// Molecule *SaveMolecule::GetInput()
// {
//   return Molecule::SafeDownCast(this->GetInputPort(0));
// }
