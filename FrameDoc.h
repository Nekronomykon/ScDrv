#ifndef Frame_Doc_h__
#define Frame_Doc_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QTabWidget>

#include <QPointer>

#include <vtkStdString.h>

#include "EditMolecule.h"

// #include <filesystem>

class FrameDoc : public QTabWidget
{
    Q_OBJECT
public:
    explicit FrameDoc(QWidget * /*parent*/ = nullptr);
    ~FrameDoc() override = default;

    bool isModified() const;

    bool hasPath() const;
    const char* getPath() const;

    bool doSave();

private:
    vtkStdString path_;
    QPointer<EditMolecule> editMol_;
};

#endif //! Frame_Doc_h__
