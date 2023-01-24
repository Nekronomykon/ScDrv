#ifndef Frame_Doc_h__
#define Frame_Doc_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;
// typedef fs:path Path;
#include <vtkStdString.h>
typedef vtkStdString Path;

#include <QTabWidget>

#include <QPointer>

#include "WidgetMolecule.h"

class FrameDoc : public QTabWidget
{
    Q_OBJECT
public:
    explicit FrameDoc(QWidget * /*parent*/ = nullptr);
    ~FrameDoc() override = default;

    bool isModified() const;

    bool hasPath() const;
    const char *getPath() const;

    bool doSave();

private:
    Path path_;
    QPointer<WidgetMolecule> wMol_;
};

#endif //! Frame_Doc_h__
