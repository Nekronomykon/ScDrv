#include "ReadMoleculeWFN.h"

using namespace vtk;

//------------------------------------------------------------------------------
vtkStandardNewMacro(ReadMoleculeWFN);

//------------------------------------------------------------------------------
ReadMoleculeWFN::ReadMoleculeWFN(void)
    : ReadMolecule()
{
}

//------------------------------------------------------------------------------
void ReadMoleculeWFN::PrintSelf(ostream &os, vtkIndent indent)
{
  // this->Superclass(os, indent);
}

int ReadMoleculeWFN::ReadInformation(std::istream &in)
{
  return 0;
}

int ReadMoleculeWFN::ReadData(std::istream &in)
{
  return 0;
}

