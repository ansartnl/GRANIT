#include "SupervisorWidget.h"
#include <QApplication>

#include <QRectF>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QProcess>
#include <QStaticText>

#include "main.h"
#include "dialogs/ScriptProcessDialog.h"
#include "connections/supervisorServer/MonitoringSocket.h"
#include "LabelAction.h"
#include "QPushButton"

bool SupervisorWidget::sContextMenuEnabled = true;
bool SupervisorWidget::sDoubleClickEnabled = true;

SupervisorWidget::SupervisorWidget(QGraphicsItem *parent) :
    QGraphicsWidget(parent), mShowBorder(true), mState(DEFAULT), mDefaultState(OUT_OF_SCOPE),
    mSocket(0), mContextMenu(0)
{
    setFlag(QGraphicsWidget::ItemIsMovable);
    setFlag(QGraphicsWidget::ItemIsFocusable);

    const QMetaObject* meta = metaObject();
    for(int i = 0; i < meta->methodCount(); ++i)
    {
        if(meta->method(i).methodType() == QMetaMethod::Slot)
        {
            for(int j = 0; j < meta->propertyCount(); ++j)
            {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                if(QString(meta->method(i).signature()).toLower() == QString("set%1(QVariant)")
                                                                        .arg(meta->property(j).name())
                                                                        .toLower())
                    mProperySlots[QString(meta->property(j).name())] = i;
#else
                if(QString(meta->method(i).methodSignature()).toLower() == QString("set%1(QVariant)")
                                                                        .arg(meta->property(j).name())
                                                                        .toLower())
                    mProperySlots[QString(meta->property(j).name())] = i;
#endif
            }
        }
    }

    connect(&mMovie, &QMovie::frameChanged, this, [this]() {
        setForegroundPixmap(mMovie.currentPixmap());
    });
}

void SupervisorWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    if(mShowBorder)
    {
        painter->setPen(Qt::black);
        painter->setBrush(Qt::NoBrush);
    }
    else
        painter->setPen(Qt::NoPen);

    if(!mBackgroundPixmap.isNull() || !mForegroundPixmap.isNull())
    {
        if(mShowBorder)
            painter->drawRect(boundingRect());

        painter->drawPixmap(boundingRect(), mBackgroundPixmap, boundingRect());//mBackgroundPixmap.rect());     //Теперь - без масштабирования! о_О
        painter->drawPixmap(boundingRect(), mForegroundPixmap, boundingRect());//mForegroundPixmap.rect());     //
    }
    else
    {
        painter->setBrush(Qt::lightGray);
        painter->drawRect(boundingRect());
    }

    painter->setPen(Qt::black);

    drawLabels(painter);

    painter->restore();
}

QRectF SupervisorWidget::boundingRect() const
{
    QRectF rect;
    rect.setWidth(mSize.width());
    rect.setHeight(mSize.height());
    return rect.adjusted(-1, -1, 1, 1);
}

QPainterPath SupervisorWidget::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void SupervisorWidget::enableMoving(bool enable)
{
    setFlag(QGraphicsWidget::ItemIsMovable, enable);
}

void SupervisorWidget::addContextMenuAction(const QString &title, const QVariant &data)
{
    QAction *action = new QAction(title, mContextMenu);
    action->setData(data);
    mContextMenu->addAction(action);
}

void SupervisorWidget::setState(QVariant state)
{
    setState((int)state.value<SupervisorWidgetState>());
}

void SupervisorWidget::setStateNew(QVariant state)
{
    setStateNew((int)state.value<SupervisorWidgetState>());
}

void SupervisorWidget::setState(int state)
{
    if(mState == state)
        return;

    mState = (SupervisorWidgetState)state;
    if(mState == DEFAULT)
        mState = mDefaultState;

    mMovie.stop();
    mForegroundPixmap = QPixmap();
    if(mState < mStateImagesList.size())
    {
        QString str = mStateImagesList.at(mState);
        if( str.contains(".gif"))
        {
            mMovie.setFileName(mStateImagesList.at(mState));
            mMovie.start();
        }
        else
            mForegroundPixmap.load(mStateImagesList.at(mState));

        QString host = mID.mid(mID.indexOf(".") + 1, mID.size() - mID.indexOf("."));
        if(mSocket)
            host = mSocket->id();
        if(mLogMessageList.at(mState) != "")
        {
            supervisorMonitor()->getSQLiteManager()->writeLog(QString("%1").arg(host), "", QString("%1").arg(mLogMessageList.at(mState)), QColor("blue"));
        }
        else
        {
            supervisorMonitor()->getSQLiteManager()->writeLog(QString("%1").arg(host), "", QString("Folder DataSource item '%1' changed state to: '%2'").arg(mID).arg(mState), QColor("blue"));
        }
    }
    if(mState < mSoundBoolList.size())
        if(mSoundBoolList.at(mState) == "true" && state != -1)
        {
            if(mSoundFileList.at(mState) != "")
            {
                supervisorMonitor()->getAlarmPlayer()->setFileName(mSoundFileList.at(mState));;
                supervisorMonitor()->getAlarmPlayer()->play();
                supervisorMonitor()->getQPushButton()->setEnabled(true);
            }
            else
            {
                QFileInfo alarmPath(QApplication::applicationDirPath(), globalSettings()->value("AlarmPath").toString());
                supervisorMonitor()->getAlarmPlayer()->setFileName(alarmPath.absoluteFilePath());;
                supervisorMonitor()->getAlarmPlayer()->play();
                supervisorMonitor()->getQPushButton()->setEnabled(true);
            }
        }

    update();
}

void SupervisorWidget::setStateNew(int state)
{
    if(mState == state)
        return;

    mState = (SupervisorWidgetState)state;
    if(mState == DEFAULT)
        mState = mDefaultState;

    mMovie.stop();
    mForegroundPixmap = QPixmap();
    if(mState < mStateImagesList.size())
    {
        QString str = mStateImagesList.at(mState);
        if( str.contains(".gif"))
        {
            mMovie.setFileName(mStateImagesList.at(mState));
            mMovie.start();
        }
        else
            mForegroundPixmap.load(mStateImagesList.at(mState));
    }

    update();
}

void SupervisorWidget::setPriority(QVariant value)
{
    bool ok(false);
    int idx = value.toInt(&ok);
    if(ok)
        setZValue(idx);
}

void SupervisorWidget::setTextTL(QVariant text)
{
    mTextTL = text.toString();
    update();
}

void SupervisorWidget::setTextTC(QVariant text)
{
    mTextTC = text.toString();
    update();
}

void SupervisorWidget::setTextTR(QVariant text)
{
    mTextTR = text.toString();
    update();
}

void SupervisorWidget::setTextBL(QVariant text)
{
    mTextBL = text.toString();
    update();
}

void SupervisorWidget::setTextBC(QVariant text)
{
    mTextBC = text.toString();
    update();
}

void SupervisorWidget::setTextBR(QVariant text)
{
    mTextBR = text.toString();
    update();
}

void SupervisorWidget::setTextC(QVariant text)
{
    mTextC = text.toString();
    update();
}

SupervisorWidgetState SupervisorWidget::state() const
{
    return mState;
}

SupervisorWidgetState SupervisorWidget::stateNew() const
{
    return mState;
}

int SupervisorWidget::priority() const
{
    return qRound(zValue());
}

const QString &SupervisorWidget::textTL() const
{
    return mTextTL;
}

const QString &SupervisorWidget::textTC() const
{
    return mTextTC;
}

const QString &SupervisorWidget::textTR() const
{
    return mTextTR;
}

const QString &SupervisorWidget::textBL() const
{
    return mTextBL;
}

const QString &SupervisorWidget::textBC() const
{
    return mTextBC;
}

const QString &SupervisorWidget::textBR() const
{
    return mTextBR;
}

const QString &SupervisorWidget::textC() const
{
    return mTextC;
}

void SupervisorWidget::setWidgetGeometry(const QRect &rect)
{
    setPosition(rect.topLeft());
    setWidth(rect.width());
    setHeight(rect.height());
}

void SupervisorWidget::setDescription(const QString &text)
{
    setToolTip(text);
}

void SupervisorWidget::setDescription(QVariant description)
{
    setDescription(description.toString());
}

void SupervisorWidget::setControlledConnection(QVariant connectionID)
{
    IConnection* connection = supervisorMonitor()->getConnectionByID(connectionID.toString());

    if(dynamic_cast<MonitoringSocket*>(connection))
    {
        mSocket = dynamic_cast<MonitoringSocket*>(connection);
    }
    else
    {
        mSocket = 0;
    }
}

QString SupervisorWidget::description() const
{
    return toolTip();
}

const QString SupervisorWidget::controlledConnection() const
{
    if (mSocket)
        return mSocket->id();
    return QString();
}

void SupervisorWidget::contextMenuActionTriggered(QAction *action)
{
    if(!action)
        return;

    LabelAction* labelAction = qobject_cast<LabelAction*>(action);
    if(labelAction)
    {
        labelAction->executeAction();
    }
}

void SupervisorWidget::onDoubleClick()
{
    if(mSocket)
    {
        if(mScriptProcessDialog.isNull())
        {
            mScriptProcessDialog = new ScriptProcessDialog(supervisorMonitor());
            connect(this, SIGNAL(destroyed()), mScriptProcessDialog, SLOT(close()));
            mScriptProcessDialog->setDataConnection(mSocket);
        }
        mScriptProcessDialog->showNormal();
        mScriptProcessDialog->raise();
        mScriptProcessDialog->activateWindow();
    }
}

void SupervisorWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);

    if(!sContextMenuEnabled)
        return;

    if(mContextMenu && !mContextMenu->isEmpty())
        mContextMenu->exec(QCursor::pos());
}

void SupervisorWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)

    if(!sDoubleClickEnabled)
        return;

    onDoubleClick();
}

void SupervisorWidget::drawLabels(QPainter *painter)
{
    if(!mTextTL.isEmpty())
    {
        if(mLabelStyles.styleTL.isEmpty())
            painter->drawText(boundingRect(), Qt::AlignTop | Qt::AlignLeft, mTextTL);
        else
        {
            painter->drawStaticText(boundingRect().topLeft(), QStaticText(QString("<p style=\"%1\">%2</p>")
                                    .arg(mLabelStyles.styleTL)
                                    .arg(mTextTL)));
        }
    }
    if(!mTextTC.isEmpty())
    {
        if(mLabelStyles.styleTC.isEmpty())
            painter->drawText(boundingRect(), Qt::AlignTop | Qt::AlignHCenter, mTextTC);
        else
        {
            QFontMetrics fm(painter->font());
            int x = (boundingRect().width() - fm.width(mTextTC)) / 2;
            painter->drawStaticText(boundingRect().left() + x,
                                    boundingRect().top(),
                                    QStaticText(QString("<p style=\"%1\">%2</p>")
                                        .arg(mLabelStyles.styleTC)
                                        .arg(mTextTC)));
        }
    }
    if(!mTextTR.isEmpty())
    {
        if(mLabelStyles.styleTR.isEmpty())
            painter->drawText(boundingRect(), Qt::AlignTop | Qt::AlignRight, mTextTR);
        else
        {
            QFontMetrics fm(painter->font());
            int x = boundingRect().width() - fm.width(mTextTR);
            painter->drawStaticText(boundingRect().left() + x,
                                    boundingRect().top(),
                                    QStaticText(QString("<p style=\"%1\">%2</p>")
                                        .arg(mLabelStyles.styleTR)
                                        .arg(mTextTR)));
        }
    }
    if(!mTextBL.isEmpty())
    {
        if(mLabelStyles.styleBL.isEmpty())
            painter->drawText(boundingRect(), Qt::AlignBottom | Qt::AlignLeft, mTextBL);
        else
        {
            QFontMetrics fm(painter->font());
            painter->drawStaticText(boundingRect().left(),
                                    boundingRect().bottom() - fm.height(),
                                    QStaticText(QString("<p style=\"%1\">%2</p>")
                                        .arg(mLabelStyles.styleBL)
                                        .arg(mTextBL)));
        }
    }
    //painter->drawStaticText(boundingRect().center(), QStaticText("<p style=\"color: green;\">smfing</p>"));
    if(!mTextBC.isEmpty())
    {
        if(mLabelStyles.styleBC.isEmpty())
            painter->drawText(boundingRect(), Qt::AlignBottom | Qt::AlignHCenter, mTextBC);
        else
        {
            QFontMetrics fm(painter->font());
            int x = (boundingRect().width() - fm.width(mTextBC)) / 2;
            painter->drawStaticText(boundingRect().left() + x,
                                    boundingRect().bottom() - fm.height(),
                                    QStaticText(QString("<p style=\"%1\">%2</p>")
                                        .arg(mLabelStyles.styleBC)
                                        .arg(mTextBC)));
        }
    }
    if(!mTextBR.isEmpty())
    {
        if(mLabelStyles.styleBR.isEmpty())
            painter->drawText(boundingRect(), Qt::AlignBottom | Qt::AlignRight, mTextBR);
        else
        {
            QFontMetrics fm(painter->font());
            int x = boundingRect().width() - fm.width(mTextBR);
            painter->drawStaticText(boundingRect().left() + x,
                                    boundingRect().bottom() - fm.height(),
                                    QStaticText(QString("<p style=\"%1\">%2</p>")
                                        .arg(mLabelStyles.styleBR)
                                        .arg(mTextBR)));
        }
    }
    if(!mTextC.isEmpty())
    {
        if(mLabelStyles.styleC.isEmpty())
            painter->drawText(boundingRect(), Qt::AlignCenter, mTextC);
        else
        {
            QFontMetrics fm(painter->font());
            int x = (boundingRect().width() - fm.width(mTextC)) / 2;
            int y = (boundingRect().height() - fm.height()) / 2;
            painter->drawStaticText(boundingRect().left() + x,
                                    boundingRect().top() + y,
                                    QStaticText(QString("<p style=\"%1\">%2</p>")
                                        .arg(mLabelStyles.styleC)
                                        .arg(mTextC)));
        }
    }
}


void SupervisorWidget::showBorder(bool show)
{
    mShowBorder = show;
}

bool SupervisorWidget::isShowingBorder() const
{
    return mShowBorder;
}

void SupervisorWidget::setID(const QString &id)
{
    //QScriptValue result = engine.evaluate(mID+".onDoubleClick()");
    mID = id;
}

const QString &SupervisorWidget::id() const
{
    return mID;
}

void SupervisorWidget::setParentID(const QString &id)
{
    mParentID = id;
}

void SupervisorWidget::setBackgroundImage(const QString &pathToImage)
{
    mBackgroundPixmap = QPixmap(pathToImage);
    mBackgroundImagePath = pathToImage;
}

void SupervisorWidget::setStateImage(SupervisorWidgetState state, const QString &pathToImage)
{
    setStateImage((int)state, pathToImage);
}

void SupervisorWidget::setStateImage(int state, const QString &pathToImage)
{
    while(mStateImagesList.size() <= state)
        mStateImagesList.append(QString());

    mStateImagesList[state] = pathToImage;
}

void SupervisorWidget::setLogMessage(int state, const QString &strLogMessage)
{
    while(mLogMessageList.size() <= state)
        mLogMessageList.append(QString());

    mLogMessageList[state] = strLogMessage;
}

void SupervisorWidget::setSoundBool(int state, const QString &soundValue)
{
    while(mSoundBoolList.size() <= state)
        mSoundBoolList.append(QString());

    mSoundBoolList[state] = soundValue;
}

void SupervisorWidget::setSoundFile(int state, const QString &pathToSound)
{
    while(mSoundFileList.size() <= state)
        mSoundFileList.append(QString());

    mSoundFileList[state] = pathToSound;
}

const QStringList &SupervisorWidget::stateImages() const
{
    return mStateImagesList;
}

SupervisorWidgetState SupervisorWidget::defaultState() const
{
    return mDefaultState;
}

void SupervisorWidget::setDefaultState(int state)
{
    if((SupervisorWidgetState)state == DEFAULT)
        return;

    mDefaultState = (SupervisorWidgetState)state;
    if(mState == DEFAULT)
        setStateNew(state);
}

void SupervisorWidget::bind()
{
    emit bindValues();
}

void SupervisorWidget::setContextMenu(QMenu *contextMenu)
{
    if(mContextMenu)
        mContextMenu->deleteLater();

    mContextMenu = contextMenu;
    if(mContextMenu)
        connect(mContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuActionTriggered(QAction*)));
}

const SupervisorWidgetLabelStyles &SupervisorWidget::labelStyles() const
{
    return mLabelStyles;
}

void SupervisorWidget::setLabelStyles(const SupervisorWidgetLabelStyles &styles)
{
    mLabelStyles = styles;
    update();
}


const QString &SupervisorWidget::backgroundImage() const
{
    return mBackgroundImagePath;
}


const QString &SupervisorWidget::parentID() const
{
    return mParentID;
}


void SupervisorWidget::enableContextMenu(bool enable)
{
    sContextMenuEnabled = enable;
}

void SupervisorWidget::enableDoubleClick(bool enable)
{
    sDoubleClickEnabled = enable;
}

QMetaMethod SupervisorWidget::propertySetterSlot(const QString &property) const
{
    if(mProperySlots.contains(property))
        return metaObject()->method(mProperySlots[property]);
    else
        return QMetaMethod();
}
