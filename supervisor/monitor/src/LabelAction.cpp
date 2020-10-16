#include "LabelAction.h"

#include <QApplication>
#include <QLabel>
#include <QtScript>
#include <QMessageBox>

#include "main.h"
#include "Log.h"

LabelAction::LabelAction(QWidget *parent) :
    QWidgetAction(parent)
{
}

LabelAction::LabelAction(QAction *action, QWidget *parent) :
    QWidgetAction(parent)
{
    if(action)
    {
        mWidgetText = action->text();
        connect(this, SIGNAL(triggered()), action, SIGNAL(triggered()));
    }
}

void LabelAction::setWidgetStylesheet(const QString &stylesheet)
{
    mStylesheet = stylesheet;
}

void LabelAction::setWidgetText(const QString &text)
{
    mWidgetText = text;
}

void LabelAction::setWidgetActionMethod(const QString &method)
{
    mActionMethod = method;
}

void LabelAction::setActionTargetConnection(const QString &connectionID)
{
    mTargetConnection = connectionID;
}

void LabelAction::setActionArgument(const QString &argument)
{
    mActionArgument = argument;
}

void LabelAction::setConfirmMessage(const QString &message)
{
    mConfirmMessage = message;
}

bool LabelAction::executeAction()
{
    if(mConfirmMessage.isEmpty())
    {
        evaluate();
        return true;
    }
    else
    {
        QMessageBox* mb = new QMessageBox(QApplication::activeWindow());
        mb->setWindowTitle(mWidgetText);
        mb->setText(mConfirmMessage);
        mb->setAttribute(Qt::WA_DeleteOnClose);
        mb->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        mb->raise();
        if(QApplication::activeWindow())
        {
            QWidget *active = QApplication::activeWindow();
            QPoint offset(mb->width() / 2, mb->height() / 2);
            mb->move(active->contentsRect().center() + active->pos() - offset);
        }

        if(mb->exec() == QMessageBox::Ok)
        {
            evaluate();
            return true;
        }
    }

    return false;
}

void LabelAction::evaluate()
{
    QScriptEngine* engine = scriptEngine();
    QString action = QString("%1.%2(%3)")
                                .arg(mTargetConnection)
                                .arg(mActionMethod)
                                .arg(mActionArgument.isEmpty() ? "" : QString("\"%1\"").arg(mActionArgument));

    QScriptValue result = engine->evaluate(action);

    if(result.isError())
        qLogError("Context Menu Action") << "Unable to execute script statement '" << action <<"' :" << result.toString();
    else
        qLogInfo("Context Menu Action") << QString("'%1' requested on %2")
                                           .arg(QString("%1(%2)").arg(mActionMethod)
                                                                 .arg(mActionArgument.isEmpty() ? "" : QString("\"%1\"").arg(mActionArgument)))
                                           .arg(mTargetConnection);
}


QWidget *LabelAction::createWidget(QWidget *parent)
{
    QLabel* label = new QLabel(parent);
    label->setStyleSheet(mStylesheet);
    label->setText(mWidgetText);
    label->setContentsMargins(5, 3, 5, 3);

    return label;
}
