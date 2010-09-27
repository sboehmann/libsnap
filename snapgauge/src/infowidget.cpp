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
#include "infowidget.h"
#include "ui_infowidget.h"

#include <libsnap/util.h>
#include <libsnap/snap.h>



QString to_binary(uint8_t value)
{
    QString result;
    for(int i = 7; i >= 0; --i) {
        result += ( value & ( 1 << i ) ) ? '1' : '0';
    }

    return result;
}



InfoWidget::InfoWidget(QWidget *parent)
  : QWidget( parent ),
    ui( new Ui::InfoWidget ),
    m_localAddress( 0 ),
    m_peerAddress( 0 ),
    m_edm( EDM_NONE )
{
    ui->setupUi( this );
    ui->icon->setPixmap( KIcon( "dialog-information" ).pixmap( 64, 64 ) );

    setEdm( EDM_NONE );
    setLocalAddress( 0 );
    setPeerAddress( 0 );
}


InfoWidget::~InfoWidget()
{
    delete ui;
}


void InfoWidget::setInputData(QByteArray data)
{
    m_inputData = data;
    updateSizes();
}


void InfoWidget::setOutputData(QByteArray data)
{
    m_outputData = data;
    updateSizes();
}


void InfoWidget::setEdm(SnapErrorDetectionMode edm)
{
    m_edm = edm;
    QString s;

    switch( edm ) {
        case EDM_NONE:
            s = i18n( "None (%1)", KGlobal::locale()->formatByteSize( 0 ) );
            break;
        case EDM_THREETIMES:
            s = i18n( "3 times (%1)", KGlobal::locale()->formatByteSize( 0 ) );
            break;
        case EDM_CHECKSUM:
            s = i18n( "8 bit checksum (%1)", KGlobal::locale()->formatByteSize( 0 ) );
            break;
        case EDM_CRC8:
            s = i18n( "8 bit crc (%1)", KGlobal::locale()->formatByteSize( 0 ) );
            break;
        case EDM_CRC16:
            s = i18n( "16 bit crc (%1)", KGlobal::locale()->formatByteSize( 0 ) );
            break;
        case EDM_CRC32:
            s = i18n( "32 bit crc (%1)", KGlobal::locale()->formatByteSize( 0 ) );
            break;
        case EDM_FEC:
            s = i18n( "fec (%1)", KGlobal::locale()->formatByteSize( 0 ) );
            break;
        case EDM_USER:
            s = i18n( "user (%1)", KGlobal::locale()->formatByteSize( 0 ) );
            break;
    }

    ui->edm->setText( s );
    updateSizes();
}


void InfoWidget::setLocalAddress(quint32 value)
{
    m_localAddress = value;
    ui->local->setText( i18n( "%1 (%2)", value,
        KGlobal::locale()->formatByteSize( snap_util_get_address_size( value ) )
    ) );
    updateSizes();
}


void InfoWidget::setPeerAddress(quint32 value)
{
    m_peerAddress = value;
    ui->peer->setText( i18n( "%1 (%2)", value,
        KGlobal::locale()->formatByteSize( snap_util_get_address_size( value ) )
    ) );
    updateSizes();
}

void InfoWidget::setAck(bool b)
{
    ui->ack->setState( b ? KLed::On : KLed::Off );
    updateSizes();
}


void InfoWidget::setCmd(bool b)
{
    ui->cmd->setState( b ? KLed::On : KLed::Off );
    updateSizes();
}


void InfoWidget::updateSizes()
{
    int size = m_inputData.size();

    snap_t snap;
    snap_init( &snap );
    snap_set_error_detection_mode( &snap, m_edm );
    snap_set_acknowledgement( &snap, ui->ack->state() == KLed::On );
    //snap_set_command_mode( &snap,  ui->cmd->state() == KLed::On );
    snap_set_local_address( &snap, m_localAddress );
    snap_set_peer_address( &snap, m_peerAddress );

    size_t total = m_outputData.size();
    size_t fill = snap_util_get_ndb_fill_bytes( size );
    size_t headersize = 3 + snap_util_get_address_size( m_localAddress )
                          + snap_util_get_address_size( m_peerAddress );


    QString hdb1( to_binary( snap_get_hdb1( &snap, size ) ) );
    QString hdb1_out( "<qt><span style=\"color:blue\">" );
    hdb1_out.append( hdb1.at( 0 ) );
    hdb1_out.append( "</span><span style=\"color:yellow\">" );
    hdb1_out.append( hdb1.at( 1 ) );
    hdb1_out.append( hdb1.at( 2 ) );
    hdb1_out.append( hdb1.at( 3 ) );
    hdb1_out.append( "</span><span style=\"color:red\">" );
    hdb1_out.append( hdb1.at( 4 ) );
    hdb1_out.append( hdb1.at( 5 ) );
    hdb1_out.append( hdb1.at( 6 ) );
    hdb1_out.append( hdb1.at( 7 ) );
    hdb1_out.append( "</span></qt>" );

    QString hdb2( to_binary( snap_get_hdb2( &snap, size ) ) );
    QString hdb2_out( "<qt><span style=\"color:blue\">" );
    hdb2_out.append( hdb2.at( 0 ) );
    hdb2_out.append( hdb2.at( 1 ) );
    hdb2_out.append( "</span><span style=\"color:yellow\">" );
    hdb2_out.append( hdb2.at( 2 ) );
    hdb2_out.append( hdb2.at( 3 ) );
    hdb2_out.append( "</span><span style=\"color:green\">" );
    hdb2_out.append( hdb2.at( 4 ) );
    hdb2_out.append( hdb2.at( 5 ) );
    hdb2_out.append( "</span><span style=\"color:red\">" );
    hdb2_out.append( hdb2.at( 6 ) );
    hdb2_out.append( hdb2.at( 7 ) );
    hdb2_out.append( "</span></qt>" );

    ui->hdb1->setText( hdb1_out );
    ui->hdb1->setToolTip( "<qt><span style=\"color:blue\">CMD</span> | <span style=\"color:yellow\">EDM</span> | <span style=\"color:red\">NDB</span></qt>" );

    ui->hdb2->setText( hdb2_out );
    ui->hdb2->setToolTip( "<qt><span style=\"color:blue\">DAB</span> | <span style=\"color:yellow\">SAB</span> | <span style=\"color:green\">PFB</span> | <span style=\"color:red\">ACK</span></qt>" );

    ui->payload->setText( KGlobal::locale()->formatByteSize( size ) );
    ui->total_size->setText( KGlobal::locale()->formatByteSize( total ) );
    ui->fill->setText( KGlobal::locale()->formatByteSize( fill ) );
    ui->overhead->setText( KGlobal::locale()->formatByteSize( total - size ) );
    ui->header_size->setText( KGlobal::locale()->formatByteSize( headersize ) );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
