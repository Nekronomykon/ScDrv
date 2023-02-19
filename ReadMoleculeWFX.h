#ifndef ReadMolecule_WFX_h__
#define ReadMolecule_WFX_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ReadMolecule.h"

namespace vtk
{
  class ReadMoleculeWFX
      : public ReadMolecule
  {
  private:
    /* data */
  public:
    static ReadMoleculeWFX *New();
    vtkTypeMacro(ReadMoleculeWFX, ReadMolecule);
    void PrintSelf(ostream &os, vtkIndent indent) override;

  protected:
    explicit ReadMoleculeWFX(void);
    ~ReadMoleculeWFX() override = default;
  };

}; // namespace vtk

#endif // !ReadMolecule_WFX_h__
