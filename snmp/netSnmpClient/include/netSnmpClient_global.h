#ifndef NETSNMPCLIENT_GLOBAL_H
#define NETSNMPCLIENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NETSNMPCLIENT_LIBRARY)
#  define NETSNMPCLIENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NETSNMPCLIENTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NETSNMPCLIENT_GLOBAL_H
