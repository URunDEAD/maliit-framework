#include "ut_standaloneinputmethodarea.h"
#include "abstractplatform.h"
#include "windowgroup.h"
#include "maliit/standaloneinputmethodhost.h"
#include "minputcontextconnection.h"
#include <QWindow>

class AreaTestConnection : public MInputContextConnection
{
public:
    QList<QRegion> areas;
    void updateInputMethodArea(const QRegion &area) override { areas.append(area); }
};

class AreaTestPlatform : public Maliit::AbstractPlatform
{
public:
    QRegion touchMask;
    void setupInputPanel(QWindow *, Maliit::Position) override {}
    void setInputRegion(QWindow *, const QRegion &region) override { touchMask = region; }
};

void Ut_StandaloneInputMethodArea::reportsVisibleAreaWithoutChangingTouchMask()
{
    qRegisterMetaType<QRegion>("QRegion");
    auto *platform = new AreaTestPlatform;
    QSharedPointer<Maliit::AbstractPlatform> platformOwner(platform);
    Maliit::WindowGroup group(platformOwner);
    AreaTestConnection connection;
    Maliit::StandaloneInputMethodHost host(&connection, &group);
    QWindow window;
    window.setGeometry(0, 0, 372, 828);
    host.registerWindow(&window, Maliit::PositionCenterBottom);
    group.activate();
    window.show();
    QCoreApplication::processEvents();
    QSignalSpy areas(&group, SIGNAL(inputMethodAreaChanged(QRegion)));
    QVERIFY(areas.isValid());

    const QRegion mask(0, 570, 372, 258);
    const QRegion area(0, 580, 372, 248);
    host.setScreenRegion(mask, &window);
    host.setInputMethodArea(area, &window);
    QCOMPARE(areas.count(), 1);
    QCOMPARE(qvariant_cast<QRegion>(areas.last().at(0)), area.translated(window.position()));
    QCOMPARE(platform->touchMask, mask);
    QCOMPARE(connection.areas.count(), 1);
    QCOMPARE(connection.areas.last(), area.translated(window.position()));

    window.hide();
    QTRY_VERIFY(qvariant_cast<QRegion>(areas.last().at(0)).isEmpty());
    QVERIFY(connection.areas.last().isEmpty());
    window.show();
    QTRY_COMPARE(qvariant_cast<QRegion>(areas.last().at(0)), area.translated(window.position()));
    QCOMPARE(connection.areas.last(), area.translated(window.position()));
    host.setInputMethodArea(QRegion(), &window);
    QVERIFY(qvariant_cast<QRegion>(areas.last().at(0)).isEmpty());
    QVERIFY(connection.areas.last().isEmpty());
    QCOMPARE(platform->touchMask, mask);
}

QTEST_MAIN(Ut_StandaloneInputMethodArea)
