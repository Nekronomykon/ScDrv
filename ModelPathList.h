#ifndef Model_PathList_h__
#define Model_PathList_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER
#include <QModelIndex>
#include <QStringListModel>
#include <QFileIconProvider>

class ModelPathList
    : public QStringListModel
{
    Q_OBJECT
public:
    static inline QString keyNewFile() { return QStringLiteral("[: new file :]"); }

    explicit ModelPathList();
    ~ModelPathList() override = default;

    //
    QVariant data(const QModelIndex & /*index*/, int /*role*/ = Qt::DisplayRole) const override;

private:
    QFileIconProvider fip_;
};

#endif // !Model_PathList_h__
