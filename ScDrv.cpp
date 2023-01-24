#include <QVTKOpenGLNativeWidget.h>
#include <QVTKApplication.h>

#include <QSurfaceFormat>
#include <QCoreApplication>

#include <QCommandLineParser>
#include <QStringList>
#include <QString>

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

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
                      fs::path path_full(a_path_to_open.toLocal8Bit().data());
                      // ~ a_path_to_open.toLatin1().data();
                      if (path_full.is_relative())
                      {
                          // make it absolute and check the existence -->
                          // and the already-presence.
                          // If it is not actual as an existing file,
                          // try to open it as a new one
                          path_full = std::filesystem::absolute(path_full);
                      }
                      switch (fs::status(path_full).type())
                      {
                      case (fs::file_type::not_found):
                      {
                          pFrame->addPath(path_full.c_str()); // possibly a new file
                                                              // --> create a doc for the
                          break;
                      }
                      case (fs::file_type::regular):
                      {
                          pFrame->addPath(path_full.c_str()); // a regular file
                                                              // --> open it and read the doc data from it
                          break;
                      }
                      case (fs::file_type::directory):
                      {
                          // a directory
                          // --> traverse and probably recurse it?
                          break;
                      }
                      case (fs::file_type::symlink):
                      {
                          // a symbolic link
                          // --> resolve it?
                          break;
                      }
                      default: // nothing else matters; probably fifo or pipe would be useful
                          break;
                      }
                  });

    pFrame->tileAgainst()->show();

    return app.exec();
}
