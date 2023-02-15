#ifndef Build_Molecule_h__
#define Build_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    BuildMolecule.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen,
                ScrewDriver te Blackheadborough
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   BuildMolecule
 * @brief   Superclass for algorithms that builds vtk::Molecule objects
 *
 * BuildMolecule is a convenience class to make writing algorithms
 * easier. There are some assumptions and defaults made by this class you
 * should be aware of. This class defaults such that your filter will have
 * one input port and one output port. If that is not the case simply change
 * it with SetNumberOfInputPorts etc. See this class constructor for the
 * default. This class also provides a FillInputPortInfo method that by
 * default says that all inputs will be vtkMolecules. If that isn't the case
 * then please override this method in your subclass. You should implement
 * the subclass's algorithm into RequestData( request, inputVec, outputVec).
 */

#include <vtkAlgorithm.h>

#include "Elements.h"
#include "Molecule.h"

class vtkDataSet;

namespace vtk
{
  // class Molecule;

  class /* VTKCOMMONEXECUTIONMODEL_EXPORT*/ BuildMolecule : public vtkAlgorithm
  {
  public:
    static BuildMolecule *New();
    vtkTypeMacro(BuildMolecule, vtkAlgorithm);
    void PrintSelf(ostream &os, vtkIndent indent) override;

    ///@{
    /**
     * Get the output data object for a port on this algorithm.
     */
    Molecule *GetOutput();
    Molecule *GetOutput(int);
    virtual void SetOutput(Molecule *d);
    ///@}

    /**
     * see vtkAlgorithm for details
     */
    vtkTypeBool ProcessRequest(vtkInformation *,
                               vtkInformationVector **,
                               vtkInformationVector *) override;

    // this method is not recommended for use, but lots of old style filters
    // use it
    vtkDataObject *GetInput();
    vtkDataObject *GetInput(int port);
    Molecule *GetMoleculeInput(int port);

    ///@{
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
    ///@}

    ///@{
    /**
     * Add an input of this algorithm.  Note that these methods support
     * old-style pipeline connections.  When writing new code you should
     * use the more general vtkAlgorithm::AddInputConnection().  See
     * SetInputData() for details.
     */
    void AddInputData(vtkDataObject *);
    void AddInputData(int, vtkDataObject *);
    ///@}

  protected:
    explicit BuildMolecule(int /*nOuts */ = 1, int /*nIns*/ = 1);
    ~BuildMolecule() override = default;

    // convenience method
    virtual int RequestInformation(vtkInformation *request, vtkInformationVector **inputVector,
                                   vtkInformationVector *outputVector);

    /**
     * This is called by the superclass.
     * This is the method you should override.
     */
    virtual int RequestData(vtkInformation *request, vtkInformationVector **inputVector,
                            vtkInformationVector *outputVector);

    /**
     * This is called by the superclass.
     * This is the method you should override.
     */
    virtual int RequestUpdateExtent(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

    // see algorithm for more info
    int FillOutputPortInformation(int port, vtkInformation *info) override;
    int FillInputPortInformation(int port, vtkInformation *info) override;

  private:
    BuildMolecule(const BuildMolecule &) = delete;
    void operator=(const BuildMolecule &) = delete;
  };

} // namespace vtk

#endif // ! Build_Molecule_h__