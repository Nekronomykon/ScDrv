#ifndef View_FileContent_h__
#define View_FileContent_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QTreeWidget>

class ViewFileContent
    : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ViewFileContent(QWidget * /*parent*/ = nullptr);
    ~ViewFileContent() override = default;
};

#endif // !View_FileContent_h__
