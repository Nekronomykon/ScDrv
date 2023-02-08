#ifndef File_Formats_h__
#define File_Formats_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <filesystem>
namespace fs = std::filesystem;
#include <vtkStdString.h>

typedef fs::path Path;
// typedef vtkStdString Path;

template <class Obj>
struct FileFormatFor
{
  typedef bool (Obj::*OperationRead)(Path);
  typedef bool (Obj::*OperationSave)(Path);

  /* data */
  const char *mask_path_ = nullptr;
  const char *description_ = nullptr;
  OperationRead opRead_ = nullptr;
  OperationSave opSave_ = nullptr;
  //
  bool hasSuffix() const { return ((mask_path_ != nullptr) && *mask_path_); }
  bool isValidFormat() const { return ((description_ != nullptr) && *description_); }
  bool hasRead() const { return (opRead_ != nullptr); }
  bool hasSave() const { return (opSave_ != nullptr); }
  //
  const char *suffix() const { return mask_path_; }
  const char *description() const { return description_; }
  //
  bool ReadTo(Path path, Obj *pDoc)
  {
    assert(hasRead());
    if (!hasRead())
      return false;
    assert(pDoc != nullptr);
    // pDoc->Initialize();
    bool bResult = (pDoc->*opRead_)(path);
    // is there anything mandatory after the read?
    // if(bResult)
    // pDoc->setModified(false);
    pDoc->updateAllViews();
    return bResult;
  }
  //
  bool SaveTo(Obj *pDoc, Path path)
  {
    assert(hasSave());
    if (!hasSave())
      return false;
    assert(pDoc != nullptr);
    bool bResult = (pDoc->*opSave_)(path);
    // is there anything mandatory after the save?
    return bResult;
  }

  //
};

#endif // !File_Formats_h__