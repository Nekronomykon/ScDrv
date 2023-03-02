#ifndef FileRead_Base_h__
#define FileRead_Base_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <string>
#include <sstream>
#include <cassert>

#include <vector>
#include <deque>

#include <filesystem>

#include <QString>

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
  //
  ///////////////////////////////////////////////////////////////////////////////
  //
  template <class _In>
  static BunchOfStrings ReadInParse(_In &in, bool bSkipEmpty = true)
  {
    BunchOfStrings allstr;
    String str_one;

    if (std::getline(in, str_one) && bSkipEmpty)
    {
      if (!str_one.empty())
      {
        QString qstr_it(QString(str_one.c_str()).simplified());
        // trimmed() is for simple LR-trimming();
        // simplified() also reduces the intrastring whitespaces
        if (!qstr_it.isEmpty())
          str_one = String(qstr_it.toLocal8Bit().data());
      }
      while (str_one.empty())
      {
        if (!std::getline(in, str_one))
          break; // End-Of-Stream signaled
        QString qstr_it(QString(str_one.c_str()).simplified());
        if (!qstr_it.isEmpty())
        {
          str_one = String(qstr_it.toLocal8Bit().data());
          break;
        }
      }
    }
    if (!str_one.empty())
    {
      InputString inps(str_one);
      String str_it;
      do
      {
        inps >> str_it;
        if (!str_it.empty()) // apparently any times if read
          allstr.push_back(str_it);
      } while (inps);
    }
    return allstr;
  }
};


#endif // !FileRead_Base_h__