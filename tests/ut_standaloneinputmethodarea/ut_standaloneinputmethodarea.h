#ifndef UT_STANDALONEINPUTMETHODAREA_H
#define UT_STANDALONEINPUTMETHODAREA_H
#include <QtTest/QtTest>
class Ut_StandaloneInputMethodArea : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void reportsVisibleAreaWithoutChangingTouchMask();
};
#endif
