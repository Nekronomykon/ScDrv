#ifndef Read_Molecule_h__
#define Read_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "BuildMolecule.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>
// #include <vtkInformation.h>

#include "ImplPathBound.h"

namespace vtk
{
  class ReadMolecule : public BuildMolecule, public ImplPathBound<ReadMolecule>
  {
  public:
    static ReadMolecule *New();
    vtkTypeMacro(ReadMolecule, BuildMolecule);
    void PrintSelf(ostream &os, vtkIndent indent) override;
    //
    Molecule *GetOutput(void);
    void SetOutput(Molecule * /* putmol */);
    //
    int RequestInformation(vtkInformation * /* request */,
                           vtkInformationVector ** /* input */,
                           vtkInformationVector * /* output */) override;
    virtual int ReadInformation(std::istream& /*inp*/);

    int RequestData(vtkInformation * /* request */,
                    vtkInformationVector ** /* input */,
                    vtkInformationVector * /* output */) override;
    virtual int ReadData(std::istream& /*inp*/);

  protected:
    explicit ReadMolecule(int /*nOuts */ = 1, int /*nIns*/ = 0);
    ~ReadMolecule() override = default;
  };
}; // namespace vtk

#endif // !Read_Molecule_h__