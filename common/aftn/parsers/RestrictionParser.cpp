#include "RestrictionParser.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

namespace RestrictionParser
{

    // returns true, if ++field wanted, else - false
    typedef bool (*ProcessFieldCallback)(
            const QString &field, QVariantMap &map, RestrictionErrors &errorCodes);

    typedef void (*ValidateFieldCallback)(
            QVariantMap &map, RestrictionErrors &errorCodes);

    #define MAXFIELDS 23
    struct FieldProcessor
    {
        const QString type;
        ProcessFieldCallback process[MAXFIELDS];
        ValidateFieldCallback validate[MAXFIELDS];
    };

    const int index_Limit = 5;

    static const FieldProcessor fieldProcessor[] = {
        {
            rvmSignature
            , {processMessageType, processMessageId, processMessageTypeRestriction, processMessageTimeRestriction,
               processMessageFormRestriction, processMessageLimitRestriction, processMessagePointRestriction,
               processMessageNameRestriction, 0}
            , {validateMessageType, validateMessageId,validateMessageTypeRestriction, validateMessageTimeRestriction,
               validateMessageFormRestriction, validateMessageLimitRestriction, validateMessagePointRestriction,
               validateMessageNameRestriction, 0}
        },
        {
            rviSignature
            , {processMessageType, processMessageId, 0}
            , {validateMessageType, validateMessageId, 0}
        },
        {
            rvmSignatureR
            , {processMessageType, processMessageId, processMessageTypeRestriction, processMessageTimeRestriction,
               processMessageFormRestriction, processMessageLimitRestriction, processMessagePointRestriction,
               processMessageNameRestriction, 0}
            , {validateMessageType, validateMessageId,validateMessageTypeRestriction, validateMessageTimeRestriction,
               validateMessageFormRestriction, validateMessageLimitRestriction, validateMessagePointRestriction,
               validateMessageNameRestriction, 0}
        },
        {
            rviSignatureR
            , {processMessageType, processMessageId, 0}
            , {validateMessageType, validateMessageId, 0}
        },
        {
            QString::null
            , {0}
            , {0}
        }
    };

    QVariantMap processMessage(const QString &message, RestrictionErrors &errorCodes)
    {
        QVariantMap ret;
        try
        {
            int beginMessageIndex = message.indexOf(M_BEGIN);
            if (beginMessageIndex < 0)
                throw ParserErrors::InvalidMessage;

            int endMessageIndex = message.lastIndexOf(M_END);
            if (endMessageIndex < beginMessageIndex)
                throw ParserErrors::InvalidMessage;

            QString trimmedMessage = message.mid(beginMessageIndex + 1
                                                 , endMessageIndex - beginMessageIndex - 1);
            if (trimmedMessage.isEmpty())
                throw ParserErrors::InvalidMessage;

            trimmedMessage.replace("&", "\n");
            trimmedMessage.replace("–", "-"); // not equal !!

            ret.insert(SOURCE_MESSAGE_RESTRICTION, M_BEGIN + trimmedMessage + M_END);

            QStringList fields = trimmedMessage.remove("\n").split("-");
            if (fields.isEmpty())
                throw ParserErrors::InvalidMessage;

            ret.insert(SOURCE_FIELDS_LIST_RESTRICTION, fields);

            QString type = fields[0].mid(0, 3);

            if ((type == "РЖМ" || type == "RVM")
                    && fields.count() <= 2)
            {
                errorCodes.append(ParserErrors::UnknownMessageRestriction);
                return ret;
            }

            bool found = false;
            int i = 0;

            while (!fieldProcessor[i].type.isNull())
            {
                if (fieldProcessor[i].type == type)
                {
                    found = true;

                    int j = 0;
                    int field = 0;

                    while (fieldProcessor[i].process[j])
                    {
                        if (field < fields.count())
                        {
                            bool next = fieldProcessor[i].process[j](fields[field], ret
                                                                 , errorCodes);
                            fieldProcessor[i].validate[j](ret, errorCodes);

                            if (next)
                                ++field;
                            ++j;
                        } else
                        {
                            found = false;
                            break;
                        }

                        if (errorCodes.contains(ParserErrors::UnknownMessageRestriction))
                        {
                            throw ParserErrors::UnknownMessageRestriction;
                        }
                    }
                    break;
                } else
                {
                    ++i;
                }
            }

            if (!found)
                throw ParserErrors::UnknownMessageRestriction;
        }
        catch (ParserErrors::RestrictionError error)
        {
            errorCodes.append(error);
            return ret;
        }
        catch (...)
        {
            errorCodes.append(ParserErrors::UnhandledErrorRestriction);
            return ret;
        }

        return ret;
    }

    bool processMessageType(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes)
    {
        QRegExp reg("(РЖМ|РЖИ|RVM|RVI)");
        if (reg.exactMatch(field.simplified()))
        {
            map.insert(TYPE_MESSAGE, reg.cap(1));
        } else
        {
            errorCodes.append(ParserErrors::ErrorTypeMessage);
            return false;
        }
        return true;
    }
    void validateMessageType(QVariantMap &map, RestrictionErrors &errorCodes)
    {
        Q_UNUSED(map)
        Q_UNUSED(errorCodes)
    }

    bool processMessageId(const QString &field, QVariantMap &map, RestrictionErrors &errorCodes)
    {
        QRegExp reg("^([A-Z]|[А-Я]|\\d){1,8}\\n?$");
        if (reg.exactMatch(field.simplified()))
        {
            map.insert(MESSAGE_ID_RESTRICTION, reg.cap(0));
        } else
        {
            if (field.count() > 11)
            {
                errorCodes.append(ParserErrors::UnknownMessageRestriction);
                return false;
            }
            else
            {
                errorCodes.append(ParserErrors::ErrorMessageIdRestriction);
            }
        }
        return true;
    }

    void validateMessageId(QVariantMap &map, RestrictionErrors &errorCodes)
    {
        Q_UNUSED(map)
        Q_UNUSED(errorCodes)
    }

    bool processMessageTypeRestriction(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes)
    {
        QRegExp reg("(ЗАП|КОП|ZAP|KOP)\\n?");
        if (reg.exactMatch(field.simplified()))
        {
            map.insert(TYPE_RESTRICTION, reg.cap(1));
        } else
        {
            errorCodes.append(ParserErrors::ErrorMessageTypeRestriction);
        }
        return true;
    }

    void validateMessageTypeRestriction(QVariantMap &map, RestrictionErrors &errorCodes)
    {
        Q_UNUSED(map)
        Q_UNUSED(errorCodes)
    }

    bool processMessageTimeRestriction(const QString &field, QVariantMap &map, RestrictionErrors &errorCodes)
    {
        QRegExp reg("((\\d{10})(\\d{10}))\\n?");
        if (reg.exactMatch(field.simplified()))
        {
            if (!reg.cap(2).isEmpty())
                map.insert(TIME_RESTRICTION_BEGIN, reg.cap(2));
            if (!reg.cap(3).isEmpty())
                map.insert(TIME_RESTRICTION_END, reg.cap(3));
        } else
        {
            errorCodes.append(ParserErrors::ErrorMessageTimeRestriction);
        }
        return true;
    }

    void validateMessageTimeRestriction(QVariantMap &map, RestrictionErrors &errorCodes)
    {
        Q_UNUSED(map)
        Q_UNUSED(errorCodes)
    }

    bool processMessageFormRestriction(const QString &field, QVariantMap &map, RestrictionErrors &errorCodes)
    {
        QRegExp reg("(К|П|K|P)\\n?");
        if (reg.exactMatch(field.simplified()))
        {
            map.insert(FORM_RESTRICTION, reg.cap(1));
        } else
        {
            errorCodes.append(ParserErrors::ErrorMessageFormRestriction);
        }
        return true;
    }

    void validateMessageFormRestriction(QVariantMap &map, RestrictionErrors &errorCodes)
    {
        Q_UNUSED(map)
        Q_UNUSED(errorCodes)
    }

    bool processMessageLimitRestriction(const QString &field, QVariantMap &map, RestrictionErrors &errorCodes)
    {
        Q_UNUSED(errorCodes);

        QRegExp reg("(((Ф|F)\\d{3})((Ф|F)\\d{3}))\\n?");
        if (reg.exactMatch(field.simplified()))
        {
            if (!reg.cap(2).isEmpty())
                map.insert(UPPER_LIMIT_RESTRICTION, reg.cap(2));
            if (!reg.cap(3).isEmpty())
                map.insert(LOWER_LIMIT_RESTRICTION, reg.cap(4));
        } else
        {
            return false;
        }

        return true;
    }

    void validateMessageLimitRestriction(QVariantMap &map, RestrictionErrors &errorCodes)
    {
        Q_UNUSED(map)
        Q_UNUSED(errorCodes)
    }

    bool processMessagePointRestriction(const QString &field, QVariantMap &map, RestrictionErrors &errorCodes)
    {
        if (map.contains(FORM_RESTRICTION))
        {
            if (map.value(FORM_RESTRICTION) == "П" || map.value(FORM_RESTRICTION) == "P")
            {
                QRegExp reg("((\\d{4}(С|Ю|N|S)\\d{5}(З|В|W|E)(\\s|$|\\n))+)\\n?");
                if (reg.exactMatch(field.simplified()))
                {
                    map.insert(POINTS_COORDINATE, reg.cap(1));
                } else
                {
                    QRegExp regName("((\\w{2,5}(\\s|$|\\n))+)\\n?");
                    if (regName.exactMatch(field.simplified()))
                    {
                        map.insert(POINTS_NAME, regName.cap(1));
                    } else
                    {
                        errorCodes.append(ParserErrors::ErrorMessagePointsRestriction);
                    }
                }
            } else if (map.value(FORM_RESTRICTION) == "К" || map.value(FORM_RESTRICTION) == "K")
            {
                QRegExp reg("((\\d{4}(С|Ю|N|S)\\d{5}(З|В|W|E))/(\\d{3}))\\n?");
                if (reg.exactMatch(field.simplified()))
                {
                    map.insert(CENTER_CIRCLE_COORDINATE, reg.cap(2));
                    map.insert(RADIUS, reg.cap(5));
                } else
                {
                    QRegExp regName("^((\\w{2,5})/(\\d{3}))\\n?$");
                    if (regName.exactMatch(field.simplified()))
                    {
                        map.insert(CENTER_CIRCLE_NAME, regName.cap(2));
                        map.insert(RADIUS, regName.cap(3));
                    } else
                    {
                        errorCodes.append(ParserErrors::ErrorMessagePointsRestriction);
                    }
                }
            }
        }
        return true;
    }

    void validateMessagePointRestriction(QVariantMap &map, RestrictionErrors &errorCodes)
    {
        Q_UNUSED(map)
        Q_UNUSED(errorCodes)
    }

    bool processMessageNameRestriction(const QString &field, QVariantMap &map, RestrictionErrors &errorCodes)
    {
        QRegExp reg("((\\w|\\W)+)");
        if (reg.exactMatch(field.simplified()))
        {
            map.insert(NAME, reg.cap(1));
        } else
        {
            errorCodes.append(ParserErrors::ErrorMessageNameRestriction);
        }
        return true;
    }

    void validateMessageNameRestriction(QVariantMap &map, RestrictionErrors &errorCodes)
    {
        Q_UNUSED(map)
        Q_UNUSED(errorCodes)
    }
}
// namespace RestrictionParser

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
