#ifndef REQUESTSTRUCTS_H
#define REQUESTSTRUCTS_H

#include <QtCore/QVariant>

namespace request_handler
{

/*!
 * \struct Request
 */

struct Request
{
    QVariant data;
    void *source;

    Request() :
        source(0)
    {}
    Request(const QVariant &data_, void *source_) :
        data(data_), source(source_)
    {}
};

/*!
 * \struct Reply
 */

struct Reply
{
    QVariantList data;

    Reply()
    {}
    Reply(const QVariantList &data_) :
        data(data_)
    {}
};

} // namespace request_handler

#endif // REQUESTSTRUCTS_H
