#ifndef ReadMolecule_WFN_h__
#define ReadMolecule_WFN_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ReadMolecule.h"

namespace vtk
{
  class ReadMoleculeWFN
      : public ReadMolecule
  {
  private:
    /* data */
  public:
    static ReadMoleculeWFN *New();
    vtkTypeMacro(ReadMoleculeWFN, ReadMolecule);
    void PrintSelf(ostream &os, vtkIndent indent) override;

  protected:
    explicit ReadMoleculeWFN(void);
    ~ReadMoleculeWFN() override = default;
  };

}; // namespace vtk

#endif // !ReadMolecule_WFN_h__
