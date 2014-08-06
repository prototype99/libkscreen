/*************************************************************************************
 *  Copyright 2014 Sebastian K�gler <sebas@kde.org>                                  *
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

#include "qscreenbackend.h"
#include "qscreenscreen.h"
#include "qscreenoutput.h"

#include <configmonitor.h>
#include <mode.h>

#include <QtCore/QFile>
#include <QtCore/qplugin.h>
#include <QtCore/QRect>
#include <QAbstractEventDispatcher>

#include <QX11Info>
#include <QGuiApplication>
#include <QScreen>

using namespace KScreen;

static int s_kscreenqscreenbackendScreenId = -1;

int getId()
{
    s_kscreenqscreenbackendScreenId++;
    return s_kscreenqscreenbackendScreenId;
}

QScreenScreen::QScreenScreen(QScreenConfig *config)
    : QObject(config)

{
    connect(qApp, &QGuiApplication::screenAdded, this, &QScreenScreen::screenAdded);
}

QScreenScreen::~QScreenScreen()
{
}

void QScreenScreen::screenAdded(QScreen* qscreen)
{
    qCDebug(KSCREEN_QSCREEN) << "Screen added!!! Updating config..";
}

Screen* QScreenScreen::toKScreenScreen(Config* parent) const
{
    KScreen::Screen *kscreenScreen = new KScreen::Screen(parent);
    updateKScreenScreen(kscreenScreen);
    return kscreenScreen;
}

void QScreenScreen::updateKScreenScreen(Screen* screen) const
{
    auto primary = QGuiApplication::primaryScreen();
    QSize _s = primary->availableVirtualGeometry().size();

    screen->setCurrentSize(_s);
    screen->setId(1);
    screen->setMaxSize(_s);
    screen->setMinSize(_s);
    screen->setCurrentSize(_s);
    screen->setMaxActiveOutputsCount(QGuiApplication::screens().count());
}

#include "qscreenscreen.moc"
