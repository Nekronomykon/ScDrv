#ifndef View_Workspace_h__
#define View_Workspace_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QPointer>
#include <QWidget>
#include <QString>

#include "ViewPathList.h"

#include "ViewFileContent.h"

class ViewWorkspace
    : public QSplitter
{
    Q_OBJECT
public:
    explicit ViewWorkspace(QWidget * /* parent */ = nullptr);
    ~ViewWorkspace() override = default;

    //
    bool addPathString(const QString & /*one*/);
    bool hasPathTypeAssumed(const QString & /*one*/);    // casting file type from its name; simply text otherwise
    bool isFileTypeCompatible(const QString & /*one*/); // here the file itself is preparsed to know its compatibility

private:
    QPointer<ViewPathList> files_;
    QPointer<ViewFileContent> content_;
};
#endif // ! View_Workspace_h__
