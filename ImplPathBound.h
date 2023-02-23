#ifndef Impl_Path_h__
#define Impl_Path_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkObjectFactory.h>

#include <string>
#include <sstream>
#include <cassert>

#include <vector>
#include <deque>

#include <filesystem>

typedef std::ifstream InputFile;
typedef std::ostringstream OutputString;
typedef std::istringstream InputString;
typedef std::string String;

// Class for the (sub)strings reparsed (to think on):
// typedef std::vector<String> BunchOfStrings;
typedef std::deque<String> BunchOfStrings;

typedef std::filesystem::path Path;

struct BaseRead
{
  ///////////////////////////////////////////////////////////////////////////////
  // No data...
  ///////////////////////////////////////////////////////////////////////////////
  //
  template <class _In>
  static int ScrollStrings(_In &in, int ns)
  {
    if (ns <= 0)
      return 0;
    String one;
    do
    {
      std::getline(in, one);
    } while (--ns);
    return ns;
  }
  //
  ///////////////////////////////////////////////////////////////////////////////
  //
  static std::string GetMarkupContent(std::istream &inp, const char *tag)
  {
    String keyOpen("<");
    String keyClose("</");
    String res;
    String one;
    bool bAdd(false);
    //
    assert(tag && *tag);
    keyOpen.append(tag) += '>';
    keyClose.append(tag) += '>';

    if (!std::getline(inp, one))
      return res;
    do
    {
      // one = std::trim(one)
      if (one.empty())
        continue;
      if (bAdd)
      {
        if (one.find(keyClose) != std::string::npos)
        {
          bAdd = false;
          break;
        }
        else
        {
          res += ' ';
          res.append(one);
        }
      }
      else
      {
        if (one.find(keyOpen) != std::string::npos)
          bAdd = true;
      }
      /* code */
    } while (std::getline(inp, one));

    return res;
  }
  //
  ///////////////////////////////////////////////////////////////////////////////
  // Reads a string and trim it from the end:
  template <class _In, class _String>
  static _In &GetLine(_In &in, _String &line)
  {
    std::getline(in, line);
    // trim from the end
    auto k = line.rbegin();
    while (k != line.rend())
    {
      if (!std::isspace(*k))
        break;
      *(k++) = 0;
    }
    return in;
  }
  //
  ///////////////////////////////////////////////////////////////////////////////
  //
  template <class _In, class _String>
  static _In &ScrollToPrefix(_In &in, const char *key, _String &line)
  {
    if (!key || !*key)
      return in; //

    const size_t nSym = strlen(key);
    while (GetLine(in, line))
    {
      if (!line.find(key))
      {
        do
        {
          line = _String(line, nSym);
        } while (!line.find(key));

        break;
      }
    }

    return in;
  }
  //
  ///////////////////////////////////////////////////////////////////////////////
  //
  template <class In, class Value>
  static In &ReadAfterPrefix(In &in, const char *key, Value &val)
  {
    if (key && *key)
      return in;
    String src;
    if (ScrollToPrefix(in, key, src))
    {
      InputString input(src);
      input >> val;
    }

    return in;
  }
  //
  ///////////////////////////////////////////////////////////////////////////////
  //
  template <class _In>
  static _In &ScrollToEmpty(_In &in)
  {
    String one_line;
    while (GetLine(in, one_line))
    {
      if (one_line.empty())
        break;
    }
    return in;
  }
  //
  ///////////////////////////////////////////////////////////////////////////////
  //
  template <class _In, class _String>
  static _In &GatherNonEmpty(_In &in, _String &line)
  {
    bool bNew = line.empty();
    OutputString add(line);
    if (bNew)
      add << std::endl;
    _String add_one;
    while (GetLine(in, add_one))
    {
      if (add_one.empty())
        break;
      add << add_one << std::endl;
    }
    return in;
  }
};

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

#endif // !Impl_Path_h__
