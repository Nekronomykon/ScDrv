#include "BuildMolecule.h"

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    BuildMolecule.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <vtkCommand.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkTrivialProducer.h>

#include "Molecule.h"

using namespace std;
using namespace vtk;

//------------------------------------------------------------------------------
vtkStandardNewMacro(BuildMolecule);

//------------------------------------------------------------------------------
BuildMolecule::BuildMolecule(int nOuts, int nIns)
{
  // by default assume filters have one input and one output
  // subclasses that deviate should modify this setting
  this->SetNumberOfOutputPorts(nOuts);
  this->SetNumberOfInputPorts(nIns);
}

//------------------------------------------------------------------------------
// BuildMolecule::~BuildMolecule();

//------------------------------------------------------------------------------
void BuildMolecule::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
Molecule *BuildMolecule::GetOutput()
{
  return this->GetOutput(0);
}

//------------------------------------------------------------------------------
Molecule *BuildMolecule::GetOutput(int port)
{
  return Molecule::SafeDownCast(this->GetOutputDataObject(port));
}

//------------------------------------------------------------------------------
void BuildMolecule::SetOutput(Molecule *d)
{
  this->GetExecutive()->SetOutputData(0, d);
}

//------------------------------------------------------------------------------
vtkDataObject *BuildMolecule::GetInput()
{
  return this->GetInput(0);
}

//------------------------------------------------------------------------------
vtkDataObject *BuildMolecule::GetInput(int port)
{
  return this->GetExecutive()->GetInputData(port, 0);
}

//------------------------------------------------------------------------------
Molecule *BuildMolecule::GetMoleculeInput(int port)
{
  return Molecule::SafeDownCast(this->GetInput(port));
}

//------------------------------------------------------------------------------
vtkTypeBool BuildMolecule::ProcessRequest(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
  // generate the data
  if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
  {
    return this->RequestData(request, inputVector, outputVector);
  }

  if (request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
  {
    return this->RequestUpdateExtent(request, inputVector, outputVector);
  }

  // execute information
  if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
  {
    return this->RequestInformation(request, inputVector, outputVector);
  }

  return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}

//------------------------------------------------------------------------------
int BuildMolecule::FillOutputPortInformation(int vtkNotUsed(port), vtkInformation *info)
{
  // now add our info
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkMolecule" // Molecule::GetName() || "Molecule"
  );
  return 1;
}

//------------------------------------------------------------------------------
int BuildMolecule::FillInputPortInformation(int vtkNotUsed(port), vtkInformation *info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkMolecule" // Molecule::GetName() || "Molecule"
  );
  return 1;
}

//------------------------------------------------------------------------------
int BuildMolecule::RequestInformation(vtkInformation *vtkNotUsed(request),
                                      vtkInformationVector **vtkNotUsed(inputVector),
                                      vtkInformationVector *vtkNotUsed(outputVector))
{
  // do nothing let subclasses handle it
  return 1;
}

//------------------------------------------------------------------------------
int BuildMolecule::RequestUpdateExtent(vtkInformation *vtkNotUsed(request),
                                       vtkInformationVector **inputVector,
                                       vtkInformationVector *vtkNotUsed(outputVector))
{
  int numInputPorts = this->GetNumberOfInputPorts();
  for (int i = 0; i < numInputPorts; i++)
  {
    int numInputConnections = this->GetNumberOfInputConnections(i);
    for (int j = 0; j < numInputConnections; j++)
    {
      vtkInformation *inputInfo = inputVector[i]->GetInformationObject(j);
      inputInfo->Set(vtkStreamingDemandDrivenPipeline::EXACT_EXTENT(), 1);
    }
  }
  return 1;
}

//------------------------------------------------------------------------------
// This is the superclasses style of Execute method.  Convert it into
// an imaging style Execute method.
int BuildMolecule::RequestData(vtkInformation *vtkNotUsed(request),
                               vtkInformationVector **vtkNotUsed(inputVector),
                               vtkInformationVector *vtkNotUsed(outputVector))
{
  return 1;
}

//------------------------------------------------------------------------------
void BuildMolecule::SetInputData(vtkDataObject *input)
{
  this->SetInputData(0, input);
}

//------------------------------------------------------------------------------
void BuildMolecule::SetInputData(int index, vtkDataObject *input)
{
  this->SetInputDataInternal(index, input);
}

//------------------------------------------------------------------------------
void BuildMolecule::AddInputData(vtkDataObject *input)
{
  this->AddInputData(0, input);
}

//------------------------------------------------------------------------------
void BuildMolecule::AddInputData(int index, vtkDataObject *input)
{

  this->AddInputDataInternal(index, input);
}

vtkIdType BuildMolecule::GetNumberOfAtoms() const
{
  return numAtoms_;
}

vtkIdType BuildMolecule::ResetNumberOfAtoms(vtkIdType nAtoms)
{
  if (numAtoms_ != nAtoms)
  {
    std::swap(numAtoms_, nAtoms);
    // this->OnChangeNumberOfAtoms(nAtoms, numAtoms_); // (prev, new);
  }
  return nAtoms; // prev
}
