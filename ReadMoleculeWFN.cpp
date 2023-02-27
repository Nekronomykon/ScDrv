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

//------------------------------------------------------------------------------
int ReadMoleculeWFN::ReadInformation(std::istream &in)
{
  String str_line, skip;
  std::getline(in, str_line);
  this->ResetTitle(str_line);

  vtkIdType nAtoms, nOrbs, nFuncs;
  std::getline(in, str_line);
  InputString inps_sizes(str_line);
  inps_sizes >> skip >> nOrbs
  >> skip >> skip >> nFuncs
  >> skip >> nAtoms;
  this->ResetNumberOfAtoms(nAtoms);
  return 0;
}

//------------------------------------------------------------------------------
int ReadMoleculeWFN::ReadData(std::istream &in)
{
  String str_line, skip;
  std::getline(in, str_line);
  this->ResetTitle(str_line);

  vtkIdType nAtoms, nOrbs, nFuncs;
  std::getline(in, str_line);
  InputString inps_sizes(str_line);
  return 0;
}

