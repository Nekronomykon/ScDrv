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

template<class Obj>
struct FileFormatFor
{
  typedef bool (Obj::*OperationRead)(Path);
  typedef bool (Obj::*OperationSave)(Path);

    /* data */
    const char* description_ = nullptr;
    const char* mask_path_ = nullptr;
    OperationRead opRead_ = nullptr;
    OperationSave opSave_ = nullptr;
};

#endif // !File_Formats_h__