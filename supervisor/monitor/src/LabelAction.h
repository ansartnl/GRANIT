#ifndef LABELACTION_H
#define LABELACTION_H

#include <QWidgetAction>

class LabelAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit LabelAction(QWidget *parent = 0);
    LabelAction(QAction* action, QWidget *parent = 0);
    
    void setWidgetStylesheet(const QString& stylesheet);
    void setWidgetText(const QString& text);
    void setWidgetActionMethod(const QString& method);
    void setActionTargetConnection(const QString& connectionID);
    void setActionArgument(const QString& argument);
    void setConfirmMessage(const QString& message);

    bool executeAction();

protected slots:
    void evaluate();
    
protected:
    virtual QWidget *createWidget(QWidget *parent);

private:
    QString mStylesheet;
    QString mWidgetText;
    QString mActionMethod;
    QString mTargetConnection;
    QString mActionArgument;
    QString mConfirmMessage;
};

#endif // LABELACTION_H
