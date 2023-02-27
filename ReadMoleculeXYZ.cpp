#include "ReadMoleculeXYZ.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkExecutive.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtksys/FStream.hxx>

#include "Molecule.h"
#include "Elements.h"

#include <cmath>
#include <cstring>
#include <sstream>

using namespace vtk;
using namespace std;

//------------------------------------------------------------------------------
vtkStandardNewMacro(ReadMoleculeXYZ);

//------------------------------------------------------------------------------
ReadMoleculeXYZ::ReadMoleculeXYZ(void)
    : ReadMolecule() // (1,0)
{
}

//------------------------------------------------------------------------------
int ReadMoleculeXYZ::ReadInformation(std::istream &fileInput, vtkInformation *outInfo)
{
  // vtkInformation *outInfo = outVector->GetInformationObject(0);

  // vtksys::ifstream fileInput(this->GetPath());

  // if (!fileInput.is_open())
  // {
  //  vtkErrorMacro("ReadMoleculeXYZ error opening file: " << this->GetPath().c_str());
  //  return 0;
  // }

  vtkIdType nAtoms = 0;
  int timeStep = 0;

  while (fileInput)
  {
    istream::pos_type currentPos = fileInput.tellg();
    fileInput >> nAtoms;
    fileInput.get(); // end of line char
    if (!fileInput)
    {
      break; // reached after last timestep
    }

    // this->FilePositions.emplace_back(currentPos);

    std::string title;
    getline(fileInput, title); // second title line
    if (!title.empty())
    {
      // second title line may have a time index, time value and E?
      // search now for an optional "time = value" field and assign it if found
      std::size_t found = title.find(std::string("time"));
      if (found != std::string::npos)
      {
        std::istringstream tmp(&title[found + 6]);
        double timeValue;
        tmp >> timeValue;
        // this->TimeSteps.emplace_back(timeValue);
      }
      else
      {
        // this->TimeSteps.emplace_back(timeStep);
      }
    }
    else
    {
      // this->TimeSteps.emplace_back(timeStep);
    }
    timeStep++;

    for (int i = 0; i < nAtoms; i++)
    {
      getline(fileInput, title); // for each atom a line with symbol, x, y, z
    }
  }
  // fileInput.close();

  // outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(), this->TimeSteps.data(), timeStep);
  // double timeRange[2];
  // timeRange[0] = this->TimeSteps[0];
  // timeRange[1] = this->TimeSteps[timeStep - 1];
  // outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), timeRange, 2);
  return 1;
}

//------------------------------------------------------------------------------
int ReadMoleculeXYZ::RequestData(
    vtkInformation *, vtkInformationVector **, vtkInformationVector *outVector)
{
  Molecule *output = Molecule::SafeDownCast(vtkDataObject::GetData(outVector));
  vtkInformation *outInfo = outVector->GetInformationObject(0);

  if (!output)
  {
    vtkErrorMacro("ReadMoleculeXYZ does not have a Molecule-descendant object as output.");
    return 1;
  }

  vtksys::ifstream fileInput(this->GetPath());

  if (!fileInput.is_open())
  {
    vtkErrorMacro("ReadMoleculeXYZ error opening file: " << this->GetPath().c_str());
    return 0;
  }

  int timestep = 0;
  /*
    std::vector<double>::iterator it = this->TimeSteps.begin();
    if (outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP()))
    {
      // Get the requested time step.
      double requestedTimeStep = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP());

      // find the timestep with the closest value
      if (requestedTimeStep < *it)
      {
        requestedTimeStep = *it;
        vtkWarningMacro("ReadMoleculeXYZ using its first timestep value of " << requestedTimeStep);
      }
      for (it = this->TimeSteps.begin(); it < this->TimeSteps.end(); ++it, ++timestep)
      {
        if ((*it > requestedTimeStep))
          break;
      }

      if (it != this->TimeSteps.end())
      {
        --it;
        --timestep;
        if (fabs(*it - requestedTimeStep) > fabs(*(it + 1) - requestedTimeStep))
        {
          // closer to next timestep value
          ++timestep;
          ++it;
        }
      }
      else
      {
        --timestep;
        --it;
      }
    }
    else*/
  // {
  timestep = 0;
  // }

  // fileInput.seekg(this->FilePositions[timestep]);
  String str_line;
  GetLine(fileInput, str_line);
  InputString inps_na(str_line);

  vtkIdType nAtoms;

  inps_na >> nAtoms;
  // fileInput.get(); // end of line char
  if (nAtoms <= 0)
    return 0;

  String title;
  GetLine(fileInput, str_line); // second title line
  this->ResetTitle(str_line);

  // (de?re)construct vtkMolecule
  output->Initialize();

  /// vtkNew<vtkPeriodicTable> pT;
  vtk::Elements elements;
  for (int i = 0; i < nAtoms; i++)
  {
    // char atomType[16];
    String symbol;
    float x, y, z;
    fileInput >> symbol >> x >> y >> z;
    if (fileInput.fail()) // checking we are at end of line
    {
      vtkErrorMacro("ReadMoleculeXYZ error reading file: "
                    << this->GetPath().c_str() << " Problem reading atoms' positions.");
      fileInput.close();
      return 0;
    }
    output->AppendAtom(vtk::Elements::SymbolToNumber(symbol.c_str()), x, y, z);
  }
  // fileInput.close();

  return 1;
}

//------------------------------------------------------------------------------
void ReadMoleculeXYZ::PrintSelf(ostream &os, vtkIndent indent)
{
  // this->Superclass(os, indent);
}
