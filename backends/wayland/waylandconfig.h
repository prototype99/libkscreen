/*************************************************************************************
 *  Copyright 2014 Sebastian Kügler <sebas@kde.org>                                  *
 *                                                                                   *
 *  This library is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU Lesser General Public                       *
 *  License as published by the Free Software Foundation; either                     *
 *  version 2.1 of the License, or (at your option) any later version.               *
 *                                                                                   *
 *  This library is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                *
 *  Lesser General Public License for more details.                                  *
 *                                                                                   *
 *  You should have received a copy of the GNU Lesser General Public                 *
 *  License along with this library; if not, write to the Free Software              *
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA       *
 *************************************************************************************/

#ifndef QSCREEN_CONFIG_H
#define QSCREEN_CONFIG_H

#include "../abstractbackend.h"
#include "config.h"

#include <QDir>
#include <QEventLoop>
#include <QScreen>
#include <QSize>
#include <QThread>
#include <QLoggingCategory>
#include <QSocketNotifier>


namespace KWayland {
    namespace Client {
        class ConnectionThread;
        class EventQueue;
        class Output;
        class Registry;
    }
}

namespace KScreen
{
class Output;
class WaylandOutput;
class WaylandScreen;

/**
 * @class WaylandConfig
 *
 * This class holds the basic skeleton of the configuration and takes care of
 * fetching the information from the Wayland server and synchronizing the
 * configuration out to the "clients" that receive the config from the backend.
 * We initialize a wayland connection, using a threaded event queue when
 * querying the wayland server for data.
 * Initially, the creation of a WaylandConfig blocks until all data has been
 * received, signalled by the initialized() signal. This means that the
 * wayland client has received information about all interfaces, and that all
 * outputs are completely initialized. From then on, we properly notifyUpdate().
*/
class WaylandConfig : public QObject
{
    Q_OBJECT

public:
    explicit WaylandConfig(QObject *parent = 0);
    virtual ~WaylandConfig();

    KScreen::Config *toKScreenConfig() const;
    void updateKScreenConfig(KScreen::Config *config) const;

    QMap<quint32, WaylandOutput *> outputMap() const;
    int outputId(KWayland::Client::Output *wlo);

    void addOutput(quint32 name, quint32 version);
    void removeOutput(quint32 id);

Q_SIGNALS:
    void initialized();

private Q_SLOTS:
    void setupRegistry();
    void checkInitialized();

private:
    void initConnection();
    KWayland::Client::ConnectionThread *m_connection;
    KWayland::Client::EventQueue *m_queue;
    KWayland::Client::Registry *m_registry;

    QThread m_thread;
    QMap<quint32, WaylandOutput *> m_outputMap;
    QList<quint32> m_initializingOutputs;
    bool m_registryInitialized;
    WaylandScreen *m_screen;
    int m_lastOutputId = -1;
    bool m_blockSignals;
    QEventLoop m_syncLoop;
};

} // namespace

#endif // QSCREEN_CONFIG_H
