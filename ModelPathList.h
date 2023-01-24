#ifndef Model_PathList_h__
#define Model_PathList_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QStringListModel>

class ModelPathList
    : public QStringListModel
{
    Q_OBJECT
public:
    explicit ModelPathList();
    ~ModelPathList() override = default;
};

#endif // !Model_PathList_h__
