/*=========================================================================

  Program:   Visualization Toolkit Local Copy
  Module : MoleculeAcquireFileARC.cxx

  Copyright(c) ScrewDriver te Blackheadborough
  All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.See the above copyright notice for more information.

  ======================================================================== = */

#include "MoleculeAcquireFileARC.h"

#include "TraitsAcquireAtoms.h"

#include <vtkObjectFactory.h>

#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;
using namespace vtk;

//----------------------------------------------------------------------------
vtkStandardNewMacro(MoleculeAcquireFileARC);

//----------------------------------------------------------------------------
void MoleculeAcquireFileARC::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

int MoleculeAcquireFileARC::RequestInformation(vtkInformation *vtkNotUsed(request),
                                               vtkInformationVector **vtkNotUsed(inputVector),
                                               vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  if (!this->hasPath())
    return 0;

  std::ifstream file_in(this->getPath());

  if (!file_in.is_open())
  {
    vtkErrorMacro(<< "MoleculeAcquireFileARC error opening file: " << this->path());
    return 0;
  }

  int natoms = 0;
  // istream::pos_type current_pos = file_in.tellg();
  std::string str_line;
  if (!std::getline(file_in, str_line))
  {
    vtkErrorMacro(<< "MoleculeAcquireFileARC: unexpected EOF at " << this->path());
    return 0;
  }
  do
  {
    if (!file_in)
    {
      vtkErrorMacro(<< "MoleculeAcquireFileARC: unexpected EOF at " << this->path());
      return 0;
    }
    if (str_line.find("FINAL GEOMETRY OBTAINED") != std::string::npos)
      break;
  } while (std::getline(file_in, str_line));

  do
  {
    if (!std::getline(file_in, str_line))
    {
      vtkErrorMacro(<< "MoleculeAcquireFileARC: unexpected EOF at " << this->path());
      return 0;
    }

  } while (!str_line.empty());

  assert(!this->GetNumberOfAtoms()); // assured that it is read first
  natoms = Traits::MeasureStringGroup(file_in);
  this->ResetNumberOfAtoms(natoms);

  return 1;
}

int MoleculeAcquireFileARC::ReadMolecule(istream &file_in, vtkMolecule *output)
{
  if (!Traits::ScrollDownTo(file_in, "FINAL GEOMETRY OBTAINED"))
    return 0;
  string one_line;
  if (!getline(file_in, one_line) || !getline(file_in, one_line) || !getline(file_in, one_line))
  {
    vtkErrorMacro(<< "ARC file is unexpectedly finished: " << this->path());
    return 0;
  }
  // construct vtkMolecule
  int nResult = Traits::AppendAtoms(file_in, this->GetNumberOfAtoms(), output);
  if (nResult)
  {
    if (nResult > 0)
      vtkErrorMacro(<< "MoleculeAcquireFileARC error reading atom #" << nResult
                    << " from " << this->path()
                    << " Premature EOF while reading molecule.");
    if (nResult < 0)
      vtkErrorMacro(<< "MoleculeAcquireFileARC error parsing atom #" << -nResult
                    << " from " << this->path()
                    << " Premature EOF while reading molecule.");
    return 0;
  }
  return 1;
}