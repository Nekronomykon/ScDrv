#ifndef View_Workspace_h__
#define View_Workspace_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QPointer>

#include "ViewPathList.h"

#include "ViewFileContent.h"

class ViewWorkspace
    : public QSplitter
{
    Q_OBJECT
public:
    explicit ViewWorkspace(QWidget * /* parent */ = nullptr);
    ~ViewWorkspace() override = default;

private:
    QPointer<ViewPathList> files_;
    QPointer<ViewFileContent> content_;
};
#endif // ! View_Workspace_h__
