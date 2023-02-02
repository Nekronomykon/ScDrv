#ifndef Frame_Doc_h__
#define Frame_Doc_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <algorithm>

#include <filesystem>
namespace fs = std::filesystem;
#include <vtkStdString.h>

// typedef fs:path Path;
typedef vtkStdString Path;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <QSettings>
#include <QTabWidget>

#include <QPointer>

#include "WidgetMolecule.h"

class FrameDoc;

typedef bool (FrameDoc::*OperationRead)(Path);

class FrameDoc : public QTabWidget
{
    Q_OBJECT
public:
    //
    explicit FrameDoc(QWidget * /*parent*/ = nullptr);
    ~FrameDoc() override = default;

    bool isModified() const;

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
    WidgetMolecule *editMolecule()
    {
        if (wMol_)
            this->setCurrentWidget(wMol_);
        return wMol_;
    }
    WidgetMolecule *getEditMolecule() const
    {
        return wMol_;
    }
    //
    bool ReadFilePDB(Path);
    bool ReadFileXYZ(Path);
    bool ReadFileCUBE(Path);

private:
    Path path_;
    // GUI
    QPointer<EditSource> wText_;
    QPointer<WidgetMolecule> wMol_;
};

#endif //! Frame_Doc_h__
