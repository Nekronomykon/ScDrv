#include "ReadMolecule.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkExecutive.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtksys/FStream.hxx>

#include "Molecule.h"
#include "Elements.h"

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

//------------------------------------------------------------------------------
Molecule *ReadMolecule::GetOutput()
{
  return Molecule::SafeDownCast(this->GetOutputDataObject(0));
}

//------------------------------------------------------------------------------
void ReadMolecule::SetOutput(Molecule *putmol)
{
  this->GetExecutive()->SetOutputData(0, putmol);
}

int ReadMolecule::RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *)
{
  vtksys::ifstream fileInput(this->GetPath());

  if (!fileInput.is_open())
  {
    vtkErrorMacro("ReadMolecule error opening file: " << this->GetPath().c_str());
    return 0;
  }

  return this->ReadInformation(fileInput);
}

int ReadMolecule::ReadInformation(std::istream &)
{
  return 0;
}

int ReadMolecule::RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *)
{
  vtksys::ifstream fileInput(this->GetPath());

  if (!fileInput.is_open())
  {
    vtkErrorMacro("ReadMolecule error opening file: " << this->GetPath().c_str());
    return 0;
  }

  return this->ReadData(fileInput);
}

int ReadMolecule::ReadData(std::istream &)
{
  return 0;
}
