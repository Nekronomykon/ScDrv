#include <QVTKOpenGLNativeWidget.h>
#include <QVTKApplication.h>

#include <QSurfaceFormat>
#include <QCoreApplication>

#include <QCommandLineParser>
#include <QStringList>
#include <QString>

#include <algorithm>
#include <filesystem>

#include "FrameTop.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(ViewMolecule::defaultFormat());

    QVTKApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(true); // false if one would make use of
                                                   // the tray activation system. Who would dare?
                                                   // --------------------------------------------

    QCoreApplication::setApplicationName("ScDrv Browser");
    QCoreApplication::setOrganizationName("Nekronomykon");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file(s) to open.");
    parser.process(app);

    const QStringList all_paths_to_open = parser.positionalArguments();

    FrameTop *pFrame = FrameTop::createNew();

    std::for_each(all_paths_to_open.begin(),
                  all_paths_to_open.end(),
                  [&pFrame](const QString &a_path_to_open) -> void
                  {
                      // QString --> file path::
                      std::filesystem::path path_full(a_path_to_open.toLocal8Bit().data());
                      // ~ a_path_to_open.toLatin1().data();
                      if (path_full.is_relative())
                      {
                          // make it absolute and check the existence -->
                          // and the already-presence.
                          // If it is not actual as an existing file, 
                          // try to open it as a new one
                      }
                      pFrame->addPath(path_full.c_str());
                      // Here should be path_full. Rethink the application,
                      // considering that vtkStdString is closer
                      // to std::filesystem::path than to QString
                      // (a_path_to_open.whatever().to_id_a_file())
                  });

 
    pFrame->tileAgainst()->show();

    return app.exec();
}
