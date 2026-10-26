/* This file is part of Maliit framework.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#include "hybridinputcontextconnection.h"
#include "waylandinputmethodconnection.h"

#include <QKeyEvent>

HybridInputContextConnection::HybridInputContextConnection(const QSharedPointer<Maliit::Server::DBus::Address> &address)
    : DBusInputContextConnection(address)
    , mWayland(new WaylandInputMethodConnection(this))
{
}

HybridInputContextConnection::~HybridInputContextConnection()
{
}

bool HybridInputContextConnection::waylandActive() const
{
    return activeConnection == WaylandInputMethodConnection::waylandConnectionId();
}

void HybridInputContextConnection::sendPreeditString(const QString &string,
                                                     const QList<Maliit::PreeditTextFormat> &preeditFormats,
                                                     int replacementStart, int replacementLength,
                                                     int cursorPos)
{
    if (waylandActive()) {
        MInputContextConnection::sendPreeditString(string, preeditFormats, replacementStart,
                                                   replacementLength, cursorPos);
        mWayland->sendPreeditString(string, preeditFormats, replacementStart, replacementLength, cursorPos);
    } else {
        DBusInputContextConnection::sendPreeditString(string, preeditFormats, replacementStart,
                                                      replacementLength, cursorPos);
    }
}

void HybridInputContextConnection::sendCommitString(const QString &string, int replaceStart,
                                                    int replaceLength, int cursorPos)
{
    if (waylandActive()) {
        MInputContextConnection::sendCommitString(string, replaceStart, replaceLength, cursorPos);
        mWayland->sendCommitString(string, replaceStart, replaceLength, cursorPos);
    } else {
        DBusInputContextConnection::sendCommitString(string, replaceStart, replaceLength, cursorPos);
    }
}

void HybridInputContextConnection::sendKeyEvent(const QKeyEvent &keyEvent,
                                                Maliit::EventRequestType requestType)
{
    if (waylandActive()) {
        MInputContextConnection::sendKeyEvent(keyEvent, requestType);
        mWayland->sendKeyEvent(keyEvent, requestType);
    } else {
        DBusInputContextConnection::sendKeyEvent(keyEvent, requestType);
    }
}

void HybridInputContextConnection::invokeAction(const QString &action, const QKeySequence &sequence)
{
    if (!waylandActive())
        DBusInputContextConnection::invokeAction(action, sequence);
}

void HybridInputContextConnection::setSelection(int start, int length)
{
    if (waylandActive())
        mWayland->setSelection(start, length);
    else
        DBusInputContextConnection::setSelection(start, length);
}

QString HybridInputContextConnection::selection(bool &valid)
{
    if (waylandActive())
        return mWayland->selection(valid);
    return DBusInputContextConnection::selection(valid);
}

void HybridInputContextConnection::setLanguage(const QString &language)
{
    // D-Bus remembers the language for new clients; Wayland sends it if active.
    DBusInputContextConnection::setLanguage(language);
    if (waylandActive())
        mWayland->setLanguage(language);
}
