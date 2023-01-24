#ifndef View_PathList__h
#define View_PathList__h

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QListView>

#include <QString>
#include <QPointer>
#include <QStringListModel>

class ViewPathList
    : public QListView
{
    Q_OBJECT
public:
    explicit ViewPathList(QWidget * /*parent*/ = nullptr);
    ~ViewPathList() override = default;

    bool hasString(const QString& /*one*/) const;
    void addString(const QString& /*one*/);

private:
    QPointer<QStringListModel> modelPathList_;
};
#endif // ! View_PathList__h
