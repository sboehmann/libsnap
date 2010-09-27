/*
 * Copyright 2010  Stefan Böhmann <kde@hilefoks.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** @defgroup snapgauge snapgauge  */

#include "mainwindow.h"

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>


int main(int argc, char *argv[])
{
    KAboutData about( "snapgauge", 0, ki18n( "snapgauge" ), SNAP_VERSION );
    about.setLicense( KAboutData::License_GPL );
    about.setShortDescription( ki18n( "S.N.A.P." ) );
    about.setCopyrightStatement( ki18n( "Copyright 2010, Stefan Böhmann" ) );
    about.addAuthor( ki18n( "Stefan Böhmann" ),
                     ki18n( "Maintainer, Core Developer" ), "kde@hilefoks.org"
                   );

    KCmdLineArgs::init( argc, argv, &about );

    KApplication app;

    MainWindow *mw = new MainWindow();
    mw->show();

    return app.exec();
}

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
