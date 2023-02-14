#ifndef MakeBonds_Distance_h__
#define MakeBonds_Distance_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "Molecule.h"

#include <vtkSimpleBondPerceiver.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkSimpleBondPerceiver MakeBondsFromDistance;

typedef vtkNew<MakeBondsFromDistance>          ANewMkBondsDist;
typedef vtkSmartPointer<MakeBondsFromDistance> AMkBondsDist;

#endif // !MakeBonds_Distance_h__
