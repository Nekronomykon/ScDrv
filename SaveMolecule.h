#ifndef Save_Molecule_h__
#define Save_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "BuildMolecule.h"

#include "ImplPathBound.h"

namespace vtk
{
  class SaveMolecule : public BuildMolecule, public ImplPathBound<SaveMolecule>
  {
  public:
    static SaveMolecule *New();
    vtkTypeMacro(SaveMolecule, BuildMolecule);
    void PrintSelf(ostream &os, vtkIndent indent) override;

    // Molecule *GetInput();

  protected:
    explicit SaveMolecule(int /*nOuts */ = 0, int /*nIns*/ = 1);
    ~SaveMolecule() override = default;
  };
}; // namespace vtk

#endif // !Save_Molecule_h__