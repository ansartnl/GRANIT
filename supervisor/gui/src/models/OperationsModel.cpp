#include "OperationsModel.h"
#include <QFile>
#include <QStringList>

// Column indexes
static const int gIndexDate = 0;
static const int gIndexOper = 1;
static const int gIndexCODE = 2;
static const int gIndexType = 3;
static const int gIndexValue = 4;

COperationsModel::COperationsModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    mOperations.clear();    
}

void COperationsModel::setOperationsData(COperationsModel::TOperationsList data)
{
    mOperations.clear();
    mOperations.append(data);
}

void COperationsModel::addOperationsData(COperationsModel::TOperationsList data)
{
    emit layoutAboutToBeChanged();
    mOperations.append(data);
    emit layoutChanged();
}

void COperationsModel::resetOperationsData()
{
    emit layoutAboutToBeChanged();
    mOperations.clear();
    emit layoutChanged();
}

bool COperationsModel::isEmpty()
{
    return mOperations.isEmpty();
}

QVariant COperationsModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || mOperations.isEmpty() )
        return QVariant();

    int column = index.column(),
            row = index.row();

    Q_ASSERT( column >= 0 && column < columnCount() );

    const RemoteOperation &object = mOperations.at(row);

    if ( role == Qt::DisplayRole )
    {
        switch(column){
        case gIndexDate:
            return QVariant(object.time);
        case gIndexOper:
            return QVariant(object.oper);
        case gIndexCODE:
            return QVariant(object.ACID);
        case gIndexType:
            return QVariant(object.type);
        case gIndexValue:
            return QVariant(object.value);
        default:
            return QVariant();
        }
    }
    else if ( role == Qt::TextAlignmentRole )
    {
        return int( Qt::AlignLeft | Qt::AlignVCenter );
    }

    return QVariant();
}

QVariant COperationsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch(section){
        case gIndexDate:
            return tr("Date");
        case gIndexOper:
            return tr("Operator");
        case gIndexCODE:
            return tr("ACID");
        case gIndexType:
            return tr("Type");
        case gIndexValue:
            return tr("Value");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags COperationsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex COperationsModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    if ( !parent.isValid() && !mOperations.isEmpty())
    {
        if ( row >= 0 && row < mOperations.count() )
            return createIndex(row, column, 0);
    }

    return QModelIndex();
}

QModelIndex COperationsModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int COperationsModel::rowCount(const QModelIndex &parent) const
{
    if ( !parent.isValid() && !mOperations.isEmpty())
        return mOperations.count();
    return 0;
}

int COperationsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 5;
}

void COperationsModel::update()
{
    emit layoutChanged();
}

void COperationsModel::loadData(const QString &logfile)
{
    QFile file(logfile);
    if(file.exists())
    {
        file.open(QIODevice::ReadOnly);
        mOperations.clear();

        while(!file.atEnd())
        {
            QString line(file.readLine());
            COperationsModel::RemoteOperation ro;
            QStringList str = line.split(tr("|"), QString::SkipEmptyParts);
            ro.oper = str[0];
            ro.time = QDateTime::fromString(str[1], QString("dd.MM.yy HH:mm"));
            ro.ACID = str[2];
            ro.type = str[3];
            ro.value = str[4];

            if(ro.time.date().year() < 2000)    //19xx to 20xx correction
                ro.time = ro.time.addYears(100);

            mOperations.append(ro);
        }
    }
}

QStringList COperationsModel::getUserlist()
{
    if(mOperations.isEmpty())
        return QStringList();

    QStringList users;
    foreach(COperationsModel::RemoteOperation op, mOperations)
        if(!users.contains(op.oper))
            users.append(op.oper);

    return users;
}
