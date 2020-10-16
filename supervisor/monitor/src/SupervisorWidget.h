#ifndef SUPERVISORWIDGET_H
#define SUPERVISORWIDGET_H

#include <QGraphicsWidget>
#include <QSize>
#include <QMenu>
#include <QMetaMethod>
#include <QPointer>
#include <QMovie>

class MonitoringSocket;
class ScriptProcessDialog;

enum SupervisorWidgetState{
    DEFAULT = -1,
    OUT_OF_SCOPE = 0,
    UNREACHABLE = 1,
    OK,                 // 2
    OK_WARNING,         // 3
    MASTER,             // 4
    SLAVE,              // 5
    MODIFY,             // 6
    PARTIAL,            // 7
    VALID,              // 8
    DISABLED,           // 9
    NOT_SET,            // 10
    NOT_CONNECTED,      // 11
    NOT_INITIATED       // 12
};

struct SupervisorWidgetLabelStyles
{
    QString styleTL;
    QString styleTR;
    QString styleTC;
    QString styleBL;
    QString styleBC;
    QString styleBR;
    QString styleC;
};

Q_DECLARE_METATYPE(SupervisorWidgetState)

inline QString widgetStateAsString(const SupervisorWidgetState& state)
{
    switch(state)
    {
    case DEFAULT:
        return "Default";
    case OUT_OF_SCOPE:
        return "Out of Scope";
    case UNREACHABLE:
        return "Unreachable";
    case OK:
        return "OK";
    case OK_WARNING:
        return "OK Warning";
    case MASTER:
        return "Master";
    case SLAVE:
        return "Slave";
    case MODIFY:
        return "Modify";
    case PARTIAL:
        return "Partial";
    case VALID:
        return "Valid";
    case DISABLED:
        return "Disabled";
    case NOT_SET:
        return "Not Set";
    case NOT_CONNECTED:
        return "Not Connected";
    case NOT_INITIATED:
        return "Not Initiated";
    default:
        return "Unknown";
    }

    return "Unknown";
}

class SupervisorWidget : public QGraphicsWidget
{
    Q_OBJECT
    Q_ENUMS(SupervisorWidgetState)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState)
    Q_PROPERTY(SupervisorWidgetState stateNew READ stateNew WRITE setStateNew)
    Q_PROPERTY(int priority READ priority WRITE setPriority)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString controlledConnection READ controlledConnection WRITE setControlledConnection)

    Q_PROPERTY(QString textTL READ textTL WRITE setTextTL)
    Q_PROPERTY(QString textTC READ textTC WRITE setTextTC)
    Q_PROPERTY(QString textTR READ textTR WRITE setTextTR)
    Q_PROPERTY(QString textBL READ textBL WRITE setTextBL)
    Q_PROPERTY(QString textBC READ textBC WRITE setTextBC)
    Q_PROPERTY(QString textBR READ textBR WRITE setTextBR)
    Q_PROPERTY(QString textC READ textC WRITE setTextC)
public:

    static void enableContextMenu(bool enable);
    static void enableDoubleClick(bool enable);

    QMetaMethod propertySetterSlot(const QString& property) const;

    explicit SupervisorWidget(QGraphicsItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;

    inline void setWidth(int width)
    {
        mSize.setWidth(width);
        QGraphicsWidget::setGeometry(QRect(x(),y(), mSize.width(), mSize.height()));
        update();
    }
    inline void setHeight(int height)
    {
        mSize.setHeight(height);
        QGraphicsWidget::setGeometry(QRect(x(),y(), mSize.width(), mSize.height()));
        update();
    }
    inline void setPosition(const QPoint& point)
    {
        mPosition = point;
        setPos(point);
    }

    inline void setBackgroundPixmap(const QPixmap& pixmap)
    {
        mBackgroundPixmap = pixmap;
        update();
    }

    inline void setForegroundPixmap(const QPixmap& pixmap)
    {
        mForegroundPixmap = pixmap;
        update();
    }

    void enableMoving(bool enable);

    void addContextMenuAction(const QString& title, const QVariant& data);

    void setWidgetGeometry(const QRect& rect);

    void setDescription(const QString& text);

    void showBorder(bool show);
    bool isShowingBorder() const;

    void setID(const QString& id);
    const QString& id() const;

    void setParentID(const QString& id);
    const QString& parentID() const;

    void setBackgroundImage(const QString& pathToImage);
    const QString& backgroundImage() const;

    void setStateImage(SupervisorWidgetState state, const QString& pathToImage);
    void setStateImage(int state, const QString& pathToImage);
    void setLogMessage(int state, const QString& strLogMessage);
    void setSoundBool(int state, const QString& soundValue);
    void setSoundFile(int state, const QString& pathToSound);
    const QStringList& stateImages() const;

    SupervisorWidgetState defaultState() const;
    void setDefaultState(int state);

    void bind();
    void setContextMenu(QMenu *contextMenu);

    const SupervisorWidgetLabelStyles& labelStyles() const;
    void setLabelStyles(const SupervisorWidgetLabelStyles& styles);

public:     //! Properties
    SupervisorWidgetState state() const;
    SupervisorWidgetState stateNew() const;
    int priority() const;

    const QString& textTL() const;
    const QString& textTC() const;
    const QString& textTR() const;
    const QString& textBL() const;
    const QString& textBC() const;
    const QString& textBR() const;
    const QString& textC() const;

    QString description() const;
    const QString controlledConnection() const;

public slots:   //! Properties
    void setState(QVariant state);
    void setStateNew(QVariant state);
    void setState(int state);
    void setStateNew(int state);
    void setPriority(QVariant value);

    void setTextTL(QVariant text);
    void setTextTC(QVariant text);
    void setTextTR(QVariant text);
    void setTextBL(QVariant text);
    void setTextBC(QVariant text);
    void setTextBR(QVariant text);
    void setTextC(QVariant text);

    void setDescription(QVariant description);
    void setControlledConnection(QVariant connectionID);

signals:        //! Properties
    void bindValues();

protected slots:
    void contextMenuActionTriggered(QAction* action);
    void onDoubleClick();

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void drawLabels(QPainter *painter);

private:
    static bool sContextMenuEnabled;
    static bool sDoubleClickEnabled;
    QString mID;
    QString mParentID;
    QSize mSize;
    QPoint mPosition;
    bool mShowBorder;

    QString mTextTL;    // top - left
    QString mTextTC;    // top - center
    QString mTextTR;    // top - right
    QString mTextBL;    // bottom - left
    QString mTextBC;    // bottom - center
    QString mTextBR;    // bottom - right
    QString mTextC;     // center

    QStringList mStateImagesList;
    QStringList mLogMessageList;
    QStringList mSoundBoolList;
    QStringList mSoundFileList;
    QString mBackgroundImagePath;
    QPixmap mBackgroundPixmap;
    QPixmap mForegroundPixmap;
    QMovie mMovie;
    SupervisorWidgetState mState;
    SupervisorWidgetState mDefaultState;
    MonitoringSocket* mSocket;

    QHash<QString, int> mProperySlots;

    QMenu *mContextMenu;
    SupervisorWidgetLabelStyles mLabelStyles;
    QPointer<ScriptProcessDialog> mScriptProcessDialog;
};

#endif // SUPERVISORWIDGET_H
