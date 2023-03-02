#ifndef Impl_PathBound_h__
#define Impl_PathBound_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkObjectFactory.h>

#include "FileReadBase.h"

template <class T, class TBase = BaseRead>
class ImplPathBound
    : public TBase
{
public:
  explicit ImplPathBound() {}
  //
  ///////////////////////////////////////////////////////////////////////////////
  //
  void SetPath(const char *arg)
  {
    // CRTP stuff:
    T *pThis = static_cast<T *>(this);
    // vtkDebugMacro(<< pThis->GetClassName() << ": setting nameFile to" << ((arg && *arg) ? arg : "(null)"));
    if (arg && *arg)
      this->path_.assign(arg);
    else
      this->path_.clear();

    pThis->Modified();
  }
  //
  ///////////////////////////////////////////////////////////////////////////////
  //
  Path GetPath() const { return path_; }
  bool HasPath() const { return !path_.empty(); }
  Path ResetPath(Path a_path = Path())
  {
    std::swap(path_, a_path);
    return a_path;
  }
  //
  ///////////////////////////////////////////////////////////////////////////////

protected:
  Path path_;

private:
  /* data */
};

#endif // !Impl_PathBound_h__
