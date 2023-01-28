#ifndef View_PathList__h
#define View_PathList__h

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QListView>

#include <QString>
#include <QPointer>

#include "ModelPathList.h"

#include "ImplModelView.h"

class ViewPathList
    : public ImplModelView<ViewPathList,QListView,ModelPathList>
{
    Q_OBJECT
    typedef  ImplModelView<ViewPathList,QListView,ModelPathList> _Base;
public:
    explicit ViewPathList(QWidget * /*parent*/ = nullptr);
    ~ViewPathList() override = default;

    bool hasString(const QString& /*one*/) const;
    void addString(const QString& /*one*/);

};
#endif // ! View_PathList__h
