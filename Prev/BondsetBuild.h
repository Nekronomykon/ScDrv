#ifndef Bondset_Build_h
#define Bondset_Build_h

/*=========================================================================

  Program:   Visualization Toolkit Local Copy
  Module:    BondsetBuild.h

  Copyright (c) ScrewDriver te Blackheadborough
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/**
 * @class   BondsetBuild
 * @brief   Create a simple guess of a molecule's topology
 *
 *
 *
 * BondsetBuild performs a simple check of all interatomic distances
 * and adds a single bond between atoms that are reasonably close. If the
 * interatomic distance is less than the sum of the two atom's covalent radii
 * plus a tolerance, a single bond is added.
 *
 *
 * @warning
 * This algorithm does not consider valences, hybridization, aromaticity, or
 * anything other than atomic separations. It will not produce anything other
 * than single bonds.
*/

// #include <vtkDomainsChemistryModule.h> // For export macro in VTK
#include <vtkMoleculeAlgorithm.h>

class vtkDataSet;
class vtkMolecule;

namespace vtk {

  class /*VTKDOMAINSCHEMISTRY_EXPORT*/ BondsetBuild
      : public vtkMoleculeAlgorithm
  {
  public:
    static BondsetBuild *New();
    vtkTypeMacro(BondsetBuild, vtkMoleculeAlgorithm)
    void PrintSelf(ostream& os, vtkIndent indent) override;

    //@{
    /**
   * Set/Get the tolerance used in the comparisons. (Default: 0.45)
   */
    vtkSetMacro(Tolerance, float)
    vtkGetMacro(Tolerance, float)
    //@}

    protected:
      BondsetBuild();
    ~BondsetBuild() override = default;

    /**
   * This is called by the superclass.
   * This is the method you should override.
   */
    int RequestData(vtkInformation* request
                    , vtkInformationVector** inputVector
                    , vtkInformationVector* outputVector) override;

    float Tolerance;

  private:
    BondsetBuild(const BondsetBuild&) = delete;
    void operator=(const BondsetBuild&) = delete;
  };

}; // namespace vtk

#endif // !Bondset_Build_h
