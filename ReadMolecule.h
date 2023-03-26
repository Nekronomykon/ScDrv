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

#include "FileReadBase.h"

#include "ImplPathBound.h"

namespace vtk
{
  class ReadMolecule : public BuildMolecule, public ImplPathBound<ReadMolecule,BaseRead>
  {
  public:
    static ReadMolecule *New();
    vtkTypeMacro(ReadMolecule, BuildMolecule);
    void PrintSelf(ostream &os, vtkIndent indent) override;
    //
    Molecule *GetOutput(void);
    void SetOutput(Molecule * /* putmol */);
    //
    ///////////////////////////////////////////////////////////////////////////
    // First call --> Information: initializing data structures
    //
    int RequestInformation(vtkInformation * /* request */,
                           vtkInformationVector ** /* input */,
                           vtkInformationVector * /* output */) override;
    //
    // uses the following virtual call:
    //
    virtual int ReadInformation(std::istream & /*inp*/, vtkInformation* /*outInfo*/);
    //
    ///////////////////////////////////////////////////////////////////////////
    // Second call --> Data: filling the data structures
    //
    int RequestData(vtkInformation * /* request */,
                    vtkInformationVector ** /* input */,
                    vtkInformationVector * /* output */) override;
    //
    // uses the following virtual call:
    //
    virtual int ReadData(std::istream & /*inp*/, Molecule* /*pMol*/, vtkInformation* /*outInfo*/);

    // Title operations:
    String GetTitle() const;
    String ResetTitle(String titleNew);

    // Atom Labels operations:
    BunchOfStrings GetAtomLabels() const;

  protected:
    explicit ReadMolecule(int /*nOuts */ = 1, int /*nIns*/ = 0);
    ~ReadMolecule() override = default;

  private:
    String strTitle_;
    BunchOfStrings labelAtoms_;
  };
}; // namespace vtk

#endif // !Read_Molecule_h__