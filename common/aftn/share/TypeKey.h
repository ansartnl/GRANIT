#ifndef TYPEKEY_H
#define TYPEKEY_H

#include <QtCore/QString>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

struct TypeKey
{
    QString type;
    int disambiguation;
    static const int kDefaultDisambiguation = 0;
    static const int kNonstandardDisambiguation = 1;
    static const int kAdexpDisambiguation = 2;

    TypeKey() :
        disambiguation(0)
    {}
    TypeKey(const QString &t, int d = kDefaultDisambiguation) :
        type(t), disambiguation(d)
    {}
};

inline bool operator ==(const TypeKey &k1, const TypeKey &k2)
{
    return ((k1.type == k2.type) && (k1.disambiguation == k2.disambiguation));
}
inline int qHash(const TypeKey &key)
{
    return qHash(QString("%1%2").arg(key.type).arg(key.disambiguation));
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // TYPEKEY_H
