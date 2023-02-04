#ifndef View_Workspace_h__
#define View_Workspace_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QSettings>
#include <QPointer>
#include <QWidget>
#include <QString>
#include <QListWidgetItem>

#include <vtkStdString.h>

#include "FileFormat.h"

#include "ViewPathList.h"
#include "ViewFileContent.h"

#include "FrameDoc.h"

typedef FileFormatFor<FrameDoc> FileFormat;

class ViewWorkspace
    : public QSplitter
{
    Q_OBJECT
public:
    typedef FileFormatFor<FrameDoc> FileFormat;
    typedef FileFormat::OperationRead OperationRead;
    typedef FileFormat::OperationSave OperationSave;
    //
    explicit ViewWorkspace(QWidget * /* parent */ = nullptr);
    ~ViewWorkspace() override = default;
    //
    void readSettings(QSettings & /*src*/);
    void saveSettings(QSettings & /*src*/);
    //
    bool addPathString(const QString & /*one*/);
    bool hasPathTypeAssumed(const QString & /*one*/);   // casting file type from its name; simply text otherwise
    bool isFileTypeCompatible(const QString & /*one*/); // here the file itself is preparsed to know its compatibility
    //
    bool invokeOperationRead(OperationRead /* read */, FrameDoc * /* pDoc */, Path /* path */);
    //
    QString getReadFilter() const;

    ViewPathList   *getPathList() const;
    ViewFileContent *getFileContent() const;
private:
    QPointer<ViewPathList> files_;
    QPointer<ViewFileContent> content_;

    static FileFormat formats[];
};
#endif // ! View_Workspace_h__
