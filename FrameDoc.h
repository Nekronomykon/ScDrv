#ifndef Frame_Doc_h__
#define Frame_Doc_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <algorithm>
#include <deque>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <QSettings>
#include <QTabWidget>

#include <QPointer>

#include "FileFormat.h"

#include "TableElements.h"
#include "WidgetMolecule.h"
#include "EditSource.h"

class FrameDoc;

typedef FileFormatFor<FrameDoc> FileFormat;

class FrameDoc : public QTabWidget
{
  Q_OBJECT

public:
  typedef FileFormatFor<FrameDoc> FileFormat;
  typedef std::deque<FileFormat> AllFileFormats;

protected:
  static const AllFileFormats AllFormats;

public:
  static const AllFileFormats& getFormats() { return AllFormats; }

  //
  explicit FrameDoc(QWidget * /*parent*/ = nullptr);
  ~FrameDoc() override = default;
  ///////////////////////////////////////////////////////////////////////////////
  //
  bool isModified() const;
    //
  static QString getReadFilter();
    //
  static QString getExportFilter();
  //
  ///////////////////////////////////////////////////////////////////////////////
  // Path operations
  bool hasPath() const;
  const Path &getPath() const;
  Path resetPath(Path /* pathNew */);

  bool doSave();
  //
  void readSettings(QSettings & /*src*/);
  void saveSettings(QSettings & /*src*/);
  //
  WidgetMolecule *editMolecule();
  WidgetMolecule *getEditMolecule() const;
  // reader functions
  bool ReadFileCML(Path);
  bool ReadFilePDB(Path);
  bool ReadFileXYZ(Path);
  bool ReadFileCUBE(Path);
  // export functions
  bool ExportPixBMP(Path);
  bool ExportPixTIFF(Path);
  bool ExportPixPNG(Path);
  bool ExportPixJPEG(Path);
  bool ExportPixPostScript(Path);

protected:
private:
  Path path_;
  // GUI
  QPointer<TableElements> wTable_;
  QPointer<WidgetMolecule> wMol_;
  QPointer<EditSource> wText_;
};

#endif //! Frame_Doc_h__
