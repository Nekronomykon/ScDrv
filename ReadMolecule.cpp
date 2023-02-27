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

String ReadMolecule::GetTitle() const
{
  return strTitle_;
}

String ReadMolecule::ResetTitle(String titleNew)
{
  std::swap(titleNew, strTitle_);
  return titleNew;
}

int ReadMolecule::RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *outVector)
{
  vtksys::ifstream fileInput(this->GetPath());

  if (!fileInput.is_open())
  {
    vtkErrorMacro("ReadMolecule error opening file: " << this->GetPath().c_str());
    return 0;
  }

  return this->ReadInformation(fileInput, outVector->GetInformationObject(0));
}

int ReadMolecule::ReadInformation(std::istream &input, vtkInformation *outInfo)
{
  return 0;
}

int ReadMolecule::RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *outVector)
{
  Molecule* pMol = Molecule::SafeDownCast(vtkDataObject::GetData(outVector));
  if (!pMol)
  {
    vtkErrorMacro("ReadMolecule does not have a Molecule-descendant object as output ");
    return 1;
  }


  vtksys::ifstream fileInput(this->GetPath());
  if (!fileInput.is_open())
  {
    vtkErrorMacro("ReadMolecule error opening file: " << this->GetPath().c_str());
    return 0;
  }

  return this->ReadData(fileInput, pMol, outVector->GetInformationObject(0));
}

int ReadMolecule::ReadData(std::istream& /*input*/, Molecule* /*pMol*/, vtkInformation * /*outInfo*/)
{
  return 0;
}
