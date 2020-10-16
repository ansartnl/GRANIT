#pragma once

#include <QString>


namespace AFN {


inline QString parseMessageType(const QString& message) {
    int headerSize = message.indexOf('/');
    if (headerSize == -1)
        return QString();
    else
        return message.mid(headerSize + 1, 3);
}


} // namespace AFN
