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
int ReadMoleculeWFN::ReadInformation(std::istream& input, vtkInformation * /*outInfo*/)
{
  String str_line, skip;
  std::getline(input, str_line);
  this->ResetTitle(str_line);

  vtkIdType nAtoms, nOrbs, nFuncs;
  std::getline(input, str_line);
  InputString inps_sizes(str_line);
  inps_sizes >> skip >> nOrbs
  >> skip >> skip >> nFuncs
  >> skip >> nAtoms;
  this->ResetNumberOfAtoms(nAtoms);
  return 0;
}

//------------------------------------------------------------------------------
int ReadMoleculeWFN::ReadData(std::istream& input, Molecule* /*pMol*/, vtkInformation * /*outInfo*/)
{
  String str_line, skip;
  std::getline(input, str_line);
  this->ResetTitle(str_line);

  vtkIdType nAtoms, nOrbs, nFuncs;
  std::getline(input, str_line);
  InputString inps_sizes(str_line);
  return 0;
}

