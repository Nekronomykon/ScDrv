#ifndef Molecule_h__
#define Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkMolecule.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

namespace vtk
{
  typedef vtkMolecule Molecule;
}; // namespace vtk
typedef vtkNew<vtk::Molecule> ANewMolecule;
typedef vtkSmartPointer<vtk::Molecule> AMolecule;

#endif // !Molecule_h__