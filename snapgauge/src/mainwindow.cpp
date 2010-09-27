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
#include "mainwindow.h"
#include "mainwidget.h"

#include "infowidget.h"
#include "settingswidget.h"

#include <QDockWidget>


#include <KApplication>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <KMenuBar>
#include <KMenu>

#include <KDebug>



MainWindow::MainWindow(QWidget *parent)
  : KXmlGuiWindow( parent ),
    ui( new MainWidget ),
    m_settingsWidget( 0 ),
    m_infoWidget( 0 )
{
    setCentralWidget( ui );

    actionCollection()->addAction( KStandardAction::Quit, "file_quit", kapp, SLOT( quit() ) );

    setupGUI();

    setupDockWidgets();

    connect( ui, SIGNAL( inputDataChanged(QByteArray) ), m_infoWidget , SLOT( setInputData(QByteArray) ) );
    connect( ui, SIGNAL( outputDataChanged(QByteArray) ), m_infoWidget, SLOT( setOutputData(QByteArray) ) );

    connect( m_settingsWidget, SIGNAL( edmChanged(SnapErrorDetectionMode) ), m_infoWidget , SLOT( setEdm(SnapErrorDetectionMode) ) );
    connect( m_settingsWidget, SIGNAL( edmChanged(SnapErrorDetectionMode) ), ui , SLOT( setEdm(SnapErrorDetectionMode) ) );

    connect( m_settingsWidget, SIGNAL( localAddressChanged(quint32) ), m_infoWidget, SLOT( setLocalAddress(quint32) ) );
    connect( m_settingsWidget, SIGNAL( localAddressChanged(quint32) ), ui, SLOT( setLocalAddress(quint32) ) );

    connect( m_settingsWidget, SIGNAL( peerAddressChanged(quint32) ), m_infoWidget, SLOT( setPeerAddress(quint32) ) );
    connect( m_settingsWidget, SIGNAL( peerAddressChanged(quint32) ), ui, SLOT( setPeerAddress(quint32) ) );

    connect( m_settingsWidget, SIGNAL( ackChanged(bool) ), m_infoWidget, SLOT( setAck(bool) ) );
    connect( m_settingsWidget, SIGNAL( ackChanged(bool) ), ui, SLOT( setAck(bool) ) );

    connect( m_settingsWidget, SIGNAL( cmdChanged(bool) ), m_infoWidget, SLOT( setCmd(bool) ) );
    connect( m_settingsWidget, SIGNAL( cmdChanged(bool) ), ui, SLOT( setCmd(bool) ) );
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupDockWidgets()
{
    {
        QDockWidget *dw = new QDockWidget( i18n( "Information" ), this );
        dw->setObjectName( "InfoDock" );

        m_infoWidget = new InfoWidget( this );
        dw->setWidget( m_infoWidget );

        this->addDockWidget( Qt::RightDockWidgetArea, dw );
    }

    {
        QDockWidget *dw = new QDockWidget( i18n( "Settings" ), this );
        dw->setObjectName( "SettignsDock" );

        m_settingsWidget = new SettingsWidget( this );
        dw->setWidget( m_settingsWidget );

        this->addDockWidget( Qt::LeftDockWidgetArea, dw );
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
