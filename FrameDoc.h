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

#include "ImplPathBound.h"

class FrameDoc;

typedef FileFormatFor<FrameDoc> FileFormat;

class FrameDoc : public QTabWidget,
                 public ImplPathBound<FrameDoc>
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

  enum {Angstrom = 0, Bohr = 1, Picometer = 2}; // Linear units:
  enum {Auto = 0, Cartesian = 1, ZMatrix = 2, Fractional = 3}; // Coordinate types

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
  bool hasFormat() const;
  FileFormat getFormat() const;
  FileFormat resetFormat(FileFormat /*fmt*/);
  //
  QString moleculeTitle()const;
  void setMoleculeTitle(QString);
  //
  bool doSave();
  //
  void readSettings(QSettings & /*src*/);
  void saveSettings(QSettings & /*src*/);
  //
  vtk::Molecule* getMolecule() const;
  //
  WidgetMolecule *editMolecule();
  WidgetMolecule *getEditMolecule() const;
  //
  void setLinearUnits(int /* units */);
  // reader functions
  bool ReadMoleculeCML(Path);
  bool ReadMoleculePDB(Path);
  bool ReadMoleculeXYZ(Path);
  // import functions
  bool ReadFieldCUBE(Path);
  bool ReadMoleculeWFN(Path);
  bool ReadMoleculeWFX(Path);
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
  bool bGuessBonds_ = true;
  // Path path_;
  FileFormat format_;
  // GUI
  QPointer<TableElements> wTable_;
  QPointer<EditSource> wText_;
  QPointer<WidgetMolecule> wMol_;
  QPointer<WidgetStructure> wStruct_;
};

#endif //! Frame_Doc_h__
