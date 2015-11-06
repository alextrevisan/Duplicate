#include "duplicateplugin.h"
#include "duplicateconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <texteditor/texteditor.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QTextBlock>

#include <QtPlugin>

using namespace Duplicate::Internal;

DuplicatePlugin::DuplicatePlugin()
{
    // Create your members
}

DuplicatePlugin::~DuplicatePlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool DuplicatePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    QAction *action = new QAction(tr("Duplicate"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+D")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("Duplicate"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    return true;
}

void DuplicatePlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag DuplicatePlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void DuplicatePlugin::triggerAction()
{
    if (TextEditor::TextEditorWidget *widget = TextEditor::TextEditorWidget::currentTextEditorWidget()) {
        textCursor = widget->textCursor();
        if( textCursor.hasSelection())
        {
            duplicateSelection();
        }
        else
        {
            duplicateLine();
        }
    }
}

void DuplicatePlugin::duplicateSelection()
{
    int startPos =  textCursor.selectionStart();
    int endPos = textCursor.selectionEnd();

    QString selectedText = textCursor.selectedText();
    TextEditor::TextEditorWidget::currentTextEditorWidget()->insertPlainText(selectedText);
    TextEditor::TextEditorWidget::currentTextEditorWidget()->insertPlainText(selectedText);

    textCursor.setPosition(startPos);
    textCursor.setPosition(endPos, QTextCursor::KeepAnchor);

    TextEditor::TextEditorWidget::currentTextEditorWidget()->setTextCursor(textCursor);
}

void DuplicatePlugin::duplicateLine()
{
    QString lineText = QString::fromStdString(std::string("\n"))+textCursor.block().text();
    int tcPos = textCursor.position();
    textCursor.movePosition(QTextCursor::EndOfLine);
    TextEditor::TextEditorWidget::currentTextEditorWidget()->setTextCursor(textCursor);
    TextEditor::TextEditorWidget::currentTextEditorWidget()->insertPlainText(lineText);
    textCursor.setPosition(tcPos);
    TextEditor::TextEditorWidget::currentTextEditorWidget()->setTextCursor(textCursor);
}

