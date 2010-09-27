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
#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <libsnap/snap.h>
#include <libsnap/util.h>

#include <KMenu>
#include <KAction>



MainWidget::MainWidget(QWidget *parent)
  : QWidget( parent ),
    ui( new Ui::MainWidget ),
    m_localAddress( 0 ),
    m_peerAddress( 0 ),
    m_edm( EDM_NONE ),
    m_ack( false ),
    m_cmd( false )
{
    ui->setupUi( this );

    ui->comboBox->addItem( KIcon( "view-presentation" ), i18n( "Hexadecimal" ), KHE::ValueColumnInterface::HexadecimalCoding );
    ui->comboBox->addItem( KIcon( "view-presentation" ), i18n( "Decimal" ), KHE::ValueColumnInterface::DecimalCoding );
    ui->comboBox->addItem( KIcon( "view-presentation" ), i18n( "Octal" ), KHE::ValueColumnInterface::OctalCoding );
    ui->comboBox->addItem( KIcon( "view-presentation" ), i18n( "Binary" ), KHE::ValueColumnInterface::BinaryCoding );

    connect( ui->comboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotChangeView() ) );
    connect( ui->data, SIGNAL( textChanged() ), this, SLOT( slotInputChanged() ) );

    slotInputChanged();
}


MainWidget::~MainWidget()
{
    delete ui;
}


QByteArray MainWidget::data() const
{
    return ui->data->toPlainText().toLatin1();
}


SnapErrorDetectionMode MainWidget::edm() const
{
    return m_edm;
}


quint32 MainWidget::localAddress() const
{
    return m_localAddress;
}


quint32 MainWidget::peerAddress() const
{
    return m_peerAddress;
}


bool MainWidget::ack() const
{
    return m_ack;
}


bool MainWidget::cmd() const
{
    return m_ack;
}


void MainWidget::setData(QByteArray data)
{
    ui->data->setPlainText( data );
    slotInputChanged();
}


void MainWidget::setEdm(SnapErrorDetectionMode edm)
{
    m_edm = edm;
    slotInputChanged();
}


void MainWidget::setLocalAddress(quint32 value)
{
    m_localAddress = value;
    slotInputChanged();
}


void MainWidget::setPeerAddress(quint32 value)
{
    m_localAddress = value;
    slotInputChanged();
}


void MainWidget::setAck(bool b)
{
    m_ack = b;
    slotInputChanged();
}


void MainWidget::setCmd(bool b)
{
    m_cmd = b;
    slotInputChanged();
}


void MainWidget::slotInputChanged()
{
    QByteArray arr = ui->data->toPlainText().toLatin1();
    int size = arr.size();
    char *data = arr.data();

    snap_t snap;
    snap_init( &snap );

    snap_set_acknowledgement( &snap, m_ack );
    //snap_set_command_mode_enabled( &snap, m_cmd );
    snap_set_local_address( &snap, m_localAddress );
    snap_set_peer_address( &snap, m_peerAddress );
    snap_set_error_detection_mode( &snap, m_edm );

    size_t out_size = snap_encode_bound( &snap, size );
    uint8_t *out = new uint8_t[ out_size ];
    size_t out_pos = 0;

    QByteArray output;
    if( snap_encode( &snap, (const uint8_t*) data, size, out, &out_pos, out_size ) ) {
        output = QByteArray( (const char*) out, out_pos - 1 );
    }
    ui->out->setData( output );

    emit inputDataChanged( ui->data->toPlainText().toLatin1() );
    emit outputDataChanged( output );
}


void MainWidget::slotChangeView()
{
    ui->out->setCoding(
        static_cast<KHE::ValueColumnInterface::KCoding>(
            ui->comboBox->itemData( ui->comboBox->currentIndex() ).toInt()
        )
    );
}

//

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
