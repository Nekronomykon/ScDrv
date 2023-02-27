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
int ReadMoleculeXYZ::ReadInformation(std::istream &input, vtkInformation *outInfo)
{
  // vtkInformation *outInfo = outVector->GetInformationObject(0);

  // vtksys::ifstream input(this->GetPath());

  // if (!input.is_open())
  // {
  //  vtkErrorMacro("ReadMoleculeXYZ error opening file: " << this->GetPath().c_str());
  //  return 0;
  // }

  vtkIdType nAtoms = 0;
  int timeStep = 0;

  while (input)
  {
    istream::pos_type currentPos = input.tellg();
    input >> nAtoms;
    input.get(); // end of line char
    if (!input)
    {
      break; // reached after last timestep
    }

    // this->FilePositions.emplace_back(currentPos);

    std::string title;
    getline(input, title); // second title line
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
      getline(input, title); // for each atom a line with symbol, x, y, z
    }
  }
  // input.close();

  // outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(), this->TimeSteps.data(), timeStep);
  // double timeRange[2];
  // timeRange[0] = this->TimeSteps[0];
  // timeRange[1] = this->TimeSteps[timeStep - 1];
  // outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), timeRange, 2);
  return 1;
}

//------------------------------------------------------------------------------
int ReadMoleculeXYZ::ReadData(std::istream& input, Molecule* pMol, vtkInformation *outInfo)
{
  // Molecule *pMol = Molecule::SafeDownCast(vtkDataObject::GetData(outVector));
  // vtkInformation *outInfo = outVector->GetInformationObject(0);
/*
  if (!pMol)
  {
    vtkErrorMacro("ReadMoleculeXYZ does not have a Molecule-descendant object as pMol.");
    return 1;
  }

  vtksys::ifstream input(this->GetPath());

  if (!input.is_open())
  {
    vtkErrorMacro("ReadMoleculeXYZ error opening file: " << this->GetPath().c_str());
    return 0;
  }
*/
  int timestep = 0;
  /*std::vector<double>::iterator it = this->TimeSteps.begin();*/
  /*
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

  // input.seekg(this->FilePositions[timestep]);
  String str_line;
  GetLine(input, str_line);

  vtkIdType nAtoms;
  InputString inps_na(str_line);
  inps_na >> nAtoms;
  // input.get(); // end of line char
  if (nAtoms <= 0)
    return 0;

  String title;
  GetLine(input, str_line); // second title line
  this->ResetTitle(str_line);

  // (de?re)construct a (vtk)Molecule object
  pMol->Initialize();

  // maybe this 
  // pMol->ResetNumberOfAtoms( this->GetNumberOfAtoms() );

  // AtomsFormatSXYZ::ReadAtoms()... signature is unknown
  for (int i = 0; i < nAtoms; i++)
  {
    std::getline(input, str_line);
    InputString inps_atom(str_line);
    // AtomsFormatSXYZ::ReadAnAtom(inps, ...);
    String symbol;
    double x(0), y(0), z(0);
    inps_atom >> symbol >> x >> y >> z;
    char *infoRest;
    pMol->AppendAtom(vtk::Elements::SymbolToNumber(symbol.c_str(), &infoRest), x, y, z);
    // --------------------------------------------------------------------
    // AtomsFormatSXYZ::ReadAnAtoms(inps, ...); ends here:
    // --------------------------------------------------------------------
    // No additional information is required by the format,
    // including the possible infoRest information, yet...
    // --------------------------------------------------------------------
  }
  // AtomsFormatSXYZ::ReadAtoms() ends here...
  // input.close();

  return 1;
}

//------------------------------------------------------------------------------
void ReadMoleculeXYZ::PrintSelf(ostream &os, vtkIndent indent)
{
  // this->Superclass(os, indent);
}
