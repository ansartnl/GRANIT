I_PATH = $$PWD/include

INCLUDEPATH += $$I_PATH
DEPENDPATH += $$I_PATH

L_PATH = $$PWD/lib
L_PATH = $$join(L_PATH,,,$$POSTFIX)
LIBS += -L$$L_PATH

LIBS += -lnetSnmpManager
