#ifndef MoleculeAcquire_File_h
#define MoleculeAcquire_File_h

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <ostream>
#include <istream>
#include <fstream>

// #include "vtkDomainsChemistryModule.h" // For export macro
#include "MoleculeAcquireBase.h"
#include "ImplReadFile.h"

#include "TraitsAcquireAtoms.h"

#include <vtkStdString.h>

namespace vtk
{

class MoleculeAcquireFile
    : public MoleculeAcquireBase
    , public ImplFileName<MoleculeAcquireFile, vtkStdString>
{
protected:
  typedef MoleculeAcquireBase::Molecule Molecule;
  typedef TraitsBase::BaseInput FileInput;
  typedef FileInput::pos_type FileInputPos;

public:
  static MoleculeAcquireFile *New();
  vtkTypeMacro(MoleculeAcquireFile, MoleculeAcquireBase);
  void PrintSelf(std::ostream &os, vtkIndent indent) override;

  virtual int ReadMolecule(std::istream &src, vtkMolecule *)
  {
    assert(0);
    return 1; // debug stop
  }

  virtual int ParseStreamData(std::istream& src, vtkInformationVector *out)
  {
    assert(out);
    if (!out)
      return 0;

    vtkInformation *outInfo = out->GetInformationObject(0);

    vtkMolecule *molxyz = vtkMolecule::SafeDownCast(vtkDataObject::GetData(out));
    if (!molxyz)
    {
      vtkErrorMacro(<< "We do not have a vtkMolecule as output.");
      return 1;
    }
    return ReadMolecule(src, molxyz);
  }
  //
  // void ResetPos(FileInputPos pos) { posRead_ = pos; }
  // FileInputPos GetPos(FileInputPos pos) const { return posRead_; }
  // std::ifstream& Scroll(std::ifstream& is) const { return is.seekg(posRead_,SEEK_SET); }

protected:
  explicit MoleculeAcquireFile(int /* nOutPorts */ = 1);
  ~MoleculeAcquireFile() override = default;

private:
  MoleculeAcquireFile(const MoleculeAcquireFile &) = delete;
  void operator=(const MoleculeAcquireFile &) = delete;

  // members:
private:
  // FileInputPos posRead_;
};

}; // namespace vtk

#endif // !MoleculeAcquire_File_h
