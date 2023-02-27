#ifndef ReadMolecule_XYZ_h__
#define ReadMolecule_XYZ_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ReadMolecule.h"

namespace vtk
{
  class ReadMoleculeXYZ
      : public ReadMolecule
  {
  private:
    /* data */
  public:
    static ReadMoleculeXYZ *New();
    vtkTypeMacro(ReadMoleculeXYZ, ReadMolecule);
    void PrintSelf(ostream &os, vtkIndent indent) override;
    //
    ///////////////////////////////////////////////////////////////////////////
    // First call --> Information: initializing data structures
    //
    // int RequestInformation(vtkInformation * /* request */,
    //                        vtkInformationVector ** /* input */,
    //                        vtkInformationVector * /* output */) override;
    //
    // uses the following virtual call:
    //
    int ReadInformation(std::istream & /*inp*/, vtkInformation* /*outInfo*/);
    //
    ///////////////////////////////////////////////////////////////////////////
    // Second call --> Data: filling the data structures
    //
    // int RequestData(vtkInformation * /* request */,
    //                 vtkInformationVector ** /* input */,
    //                 vtkInformationVector * /* output */) override;
    //
    // uses the following virtual call:
    //
    int ReadData(std::istream & /*inp*/, Molecule* /*pMol*/, vtkInformation* /*outInfo*/) override;

  protected:
    explicit ReadMoleculeXYZ(void);
    ~ReadMoleculeXYZ() override = default;
  };

}; // namespace vtk

#endif // !ReadMolecule_XYZ_h__
