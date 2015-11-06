#ifndef DUPLICATE_H
#define DUPLICATE_H

#include "duplicate_global.h"

#include <extensionsystem/iplugin.h>
#include <QTextCursor>

namespace Duplicate {
namespace Internal {

class DuplicatePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Duplicate.json")

public:
    DuplicatePlugin();
    ~DuplicatePlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
private:
    QTextCursor textCursor;
    void duplicateSelection();
    void duplicateLine();
private slots:
    void triggerAction();
};

} // namespace Internal
} // namespace Duplicate

#endif // DUPLICATE_H

