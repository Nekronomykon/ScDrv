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
int ReadMoleculeWFN::ReadInformation(std::istream &input, vtkInformation * /*outInfo*/)
{
  String str_line, skip;
  std::getline(input, str_line);
  this->ResetTitle(str_line);

  vtkIdType nAtoms, nOrbs, nFuncs;
  std::getline(input, str_line);
  InputString inps_sizes(str_line);
  inps_sizes >> skip >> nOrbs >> skip >> skip >> nFuncs >> skip >> nAtoms;
  this->ResetNumberOfAtoms(nAtoms);
  return 1;
}

//------------------------------------------------------------------------------
int ReadMoleculeWFN::ReadData(std::istream &input, Molecule *pMol, vtkInformation * /*outInfo*/)
{
  String str_line, skip;
  std::getline(input, str_line);
  this->ResetTitle(str_line);

  vtkIdType nAtoms, nOrbs, nFuncs;
  std::getline(input, str_line);
  InputString inps_sizes(str_line);
  inps_sizes >> skip >> nOrbs >> skip >> skip >> nFuncs >> skip >> nAtoms;
  this->ResetNumberOfAtoms(nAtoms);

  pMol->Initialize();

  for (vtkIdType i = 0; i < nAtoms; i++)
  {
    std::getline(input, str_line);
    InputString inps_atom(str_line);
    String symbol;
    char cskip;
    double x(0), y(0), z(0), q(0);
    inps_atom >> symbol >> skip >> skip >> skip >> x >> y >> z >> skip >> cskip >> q;
    //  C 1       (CENTRE  1)  -0.21492365 -4.10451431  3.27197497  CHARGE =  6.0
    pMol->AppendAtom((IndexElement)q, x, y, z);
  }

  return 1;
}
