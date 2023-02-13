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
#include "EditSource.h"
#include "WidgetMolecule.h"
#include "WidgetStructure.h"

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
  static const char strAllFiles[];

public:
  inline static const AllFileFormats &getFormats() { return AllFormats; }

  static FileFormat FormatForSuffix(const QString &);

  //
  explicit FrameDoc(QWidget * /*parent*/ = nullptr);
  ~FrameDoc() override = default;
  ///////////////////////////////////////////////////////////////////////////////
  //
  void clearAll(bool /*bModeified*/ = false);
  //
  ///////////////////////////////////////////////////////////////////////////////
  //
  static QString getReadFilter();
  static QString getExportFilter();
  //
  bool isModified() const;
  void setModified(bool /*bSet*/ = true);
  //
  ///////////////////////////////////////////////////////////////////////////////
  // Path operations
  bool hasPath() const;
  const Path &getPath() const;
  Path resetPath(Path /* pathNew */ = Path() );
  //
  bool hasFormat() const;
  FileFormat getFormat() const;
  FileFormat resetFormat(FileFormat /*fmt*/);

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
  //
  void updateAllViews();
  void setReadOnly(bool /* bSet */ = true);

protected:
private:
  bool isModified_ = false;
  Path path_;
  FileFormat format_;
  // GUI
  QPointer<TableElements> wTable_;
  QPointer<EditSource> wText_;
  QPointer<WidgetMolecule> wMol_;
  QPointer<WidgetStructure> wStruct_;
};

#endif //! Frame_Doc_h__
