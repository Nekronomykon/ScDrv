#ifndef View_FileSystem_h__
#define View_FileSystem_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSplitter>

#include <QPointer>
#include <QTreeView>
#include <QListView>
#include <QFileSystemModel>

class ViewFileSystem
    : public QSplitter
{
    Q_OBJECT
public:
    explicit ViewFileSystem(QWidget * /*parent*/ = nullptr);
    ~ViewFileSystem() override = default;

private:
    QPointer<QFileSystemModel> model_files_;
    QPointer<QTreeView> tree_files_;
    QPointer<QListView> list_files_;
};

#endif // ! View_FileSystem_h__
