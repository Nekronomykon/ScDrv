#ifndef MoleculeAcquireBase_h
#define MoleculeAcquireBase_h

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    MoleculeAcquireBase.h

  Copyright (c) ScrewDriver te Blackheadborough
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   MoleculeAcquireBase
 * @brief   Superclass for algorithms that results in vtkMolecules
 *
 * MoleculeAcquireBase is a convenience class to make writing algorithms
 * easier. There are some assumptions and defaults made by this class you
 * should be aware of. This class defaults such that your filter will have
 * one input port and one output port. If that is not the case simply change
 * it with SetNumberOfInputPorts etc. See this class constructor for the
 * default. This class also provides a FillInputPortInfo method that by
 * default says that all inputs will be vtkMolecules. If that isn't the case
 * then please override this method in your subclass. You should implement
 * the subclass's algorithm into RequestData( request, inputVec, outputVec).
*/

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

// #include "vtkDomainsChemistryModule.h" // For export macro
#include <vtkAlgorithm.h>

class vtkDataSet;
class vtkMolecule;
class MolecularStructure;

namespace vtk
{

  class /*VTKDOMAINSCHEMISTRY_EXPORT*/ MoleculeAcquireBase
      : public vtkAlgorithm
  {
  protected:
  public:
    enum
    {
      PortMolecule = 0
    };
    typedef vtkMolecule Molecule;
    // typedef MolecularStructure Molecule;

    void PrintSelf(ostream &os, vtkIndent indent) override;
    static MoleculeAcquireBase *New();
    vtkTypeMacro(MoleculeAcquireBase, vtkAlgorithm);

    inline vtkIdType GetNumberOfAtoms() const { return NumberOfAtoms_;  }
    virtual vtkIdType ResetNumberOfAtoms(vtkIdType nnew)
    {
      if (nnew != this->GetNumberOfAtoms())
      {
        std::swap(NumberOfAtoms_, nnew);
        this->Modified();
      }
      return nnew;
    }

    //@{
    /**
     * Get the output data object for a port on this algorithm.
     */
    Molecule *GetOutput();
    Molecule *GetOutput(int);
    virtual void SetOutput(Molecule *);
    //@}

    /**
     * see vtkAlgorithm for details
     */
    int ProcessRequest(vtkInformation *,
                       vtkInformationVector **,
                       vtkInformationVector *) override;

    // this method is not recommended for use, but lots of old style filters
    // use it
    vtkDataObject *GetInput();
    vtkDataObject *GetInput(int port);
    Molecule *GetMoleculeInput(int port);

    //@{
    /**
     * Set an input of this algorithm. You should not override these
     * methods because they are not the only way to connect a pipeline.
     * Note that these methods support old-style pipeline connections.
     * When writing new code you should use the more general
     * vtkAlgorithm::SetInputConnection().  These methods transform the
     * input index to the input port index, not an index of a connection
     * within a single port.
     */
    void SetInputData(vtkDataObject *);
    void SetInputData(int, vtkDataObject *);
    //@}

    //@{
    /**
     * Add an input of this algorithm.  Note that these methods support
     * old-style pipeline connections.  When writing new code you should
     * use the more general vtkAlgorithm::AddInputConnection().  See
     * SetInputData() for details.
     */
    void AddInputData(vtkDataObject *);
    void AddInputData(int, vtkDataObject *);
    //@}

  protected:
    explicit MoleculeAcquireBase(int /* nOutPorts */ = 1);
    ~MoleculeAcquireBase() override = default;

    // convenience method
    virtual int RequestInformation(vtkInformation * /*request*/
                                , vtkInformationVector ** /*inputVector*/
                                , vtkInformationVector * /*outputVector*/);

    /**
     * This is called by the superclass.
     * This is the method you should override.
     */
    virtual int RequestData(vtkInformation * /*request*/
                          , vtkInformationVector ** /*inputVector*/
                          , vtkInformationVector * /*outputVector*/);

    /**
     * This is called by the superclass.
     * This is the method you should override.
     */
    virtual int RequestUpdateExtent(vtkInformation * /*request*/
                          , vtkInformationVector ** /*inputVector*/
                          , vtkInformationVector * /*outputVector*/);

    // see algorithm for more info
    int FillOutputPortInformation(int port, vtkInformation *info) override;
    int FillInputPortInformation(int port, vtkInformation *info) override;

  private:
    MoleculeAcquireBase(const MoleculeAcquireBase &) = delete;
    void operator=(const MoleculeAcquireBase &) = delete;

    //----------------------------------------------------------------------------
    vtkIdType NumberOfAtoms_ = 0L;
  };

}; // namespace vtk

#endif // !MoleculeAcquireBase_h
