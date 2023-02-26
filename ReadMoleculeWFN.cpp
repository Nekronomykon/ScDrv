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
  String str_line, skip;
  getline(in, str_line);
  this->ResetTitle(str_line);

  vtkIdType nAtoms, nOrbs, nFuncs;
  getline(in, str_line);
  InputString inps_sizes(str_line);
  return 0;
}

int ReadMoleculeWFN::ReadData(std::istream &in)
{
  String str_line;
  getline(in, str_line);
  this->ResetTitle(str_line);
  return 0;
}

