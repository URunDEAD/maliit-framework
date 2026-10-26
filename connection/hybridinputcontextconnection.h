/* This file is part of Maliit framework.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#ifndef HYBRIDINPUTCONTEXTCONNECTION_H
#define HYBRIDINPUTCONTEXTCONNECTION_H

#include "dbusinputcontextconnection.h"

#include <QScopedPointer>

class WaylandInputMethodConnection;

/*! \internal
 * \brief Serves D-Bus input-context clients (Maliit Qt5 plugin) and the
 * compositor's zwp_input_method_v1 (text-input v1/v2/v3 clients) at once.
 *
 * All inbound requests land on this D-Bus connection; Wayland requests use
 * WaylandInputMethodConnection::waylandConnectionId(). Outbound text goes to
 * whichever client is active.
 */
class HybridInputContextConnection : public DBusInputContextConnection
{
    Q_OBJECT
    Q_DISABLE_COPY(HybridInputContextConnection)

public:
    explicit HybridInputContextConnection(const QSharedPointer<Maliit::Server::DBus::Address> &address);
    virtual ~HybridInputContextConnection();

    void sendPreeditString(const QString &string,
                           const QList<Maliit::PreeditTextFormat> &preeditFormats,
                           int replacementStart = 0,
                           int replacementLength = 0,
                           int cursorPos = -1) override;
    void sendCommitString(const QString &string, int replaceStart = 0,
                          int replaceLength = 0, int cursorPos = -1) override;
    void sendKeyEvent(const QKeyEvent &keyEvent,
                      Maliit::EventRequestType requestType) override;
    void invokeAction(const QString &action, const QKeySequence &sequence) override;
    void setSelection(int start, int length) override;
    QString selection(bool &valid) override;
    void setLanguage(const QString &language) override;

private:
    bool waylandActive() const;

    QScopedPointer<WaylandInputMethodConnection> mWayland;
};

#endif
