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
#include "settingswidget.h"
#include "ui_settingswidget.h"




SettingsWidget::SettingsWidget(QWidget *parent)
  : QWidget( parent ),
    ui( new Ui::SettingsWidget )
{
    ui->setupUi( this );
    ui->icon->setPixmap( KIcon( "configure" ).pixmap( 64, 64 ) );

    ui->edm->clear();
    ui->edm->addItem( tr( "None" ), EDM_NONE );
    ui->edm->addItem( tr( "3 times" ), EDM_THREETIMES );
    ui->edm->addItem( tr( "8-bit checksum" ), EDM_CHECKSUM );
    ui->edm->addItem( tr( "8 bit crc" ), EDM_CRC8 );
    ui->edm->addItem( tr( "16 bit crc" ), EDM_CRC16 );
    ui->edm->addItem( tr( "32 bit crc" ), EDM_CRC16 );
    ui->edm->addItem( tr( "FEC" ), EDM_FEC );
    ui->edm->addItem( tr( "USER" ), EDM_USER );

    connect( ui->edm, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotEdmComboChanged() ) );

    connect( ui->local, SIGNAL( valueChanged(int) ), this, SLOT( slotLocalAddressChanged() ) );
    connect( ui->local_size, SIGNAL( valueChanged(int) ), this, SLOT( slotLocalAddressSizeChanged() ) );
    connect( ui->peer, SIGNAL( valueChanged(int) ), this, SLOT( slotPeerAddressChanged() ) );
    connect( ui->peer_size, SIGNAL( valueChanged(int) ), this, SLOT( slotPeerAddressSizeChanged() ) );

    connect( ui->ack, SIGNAL( toggled(bool) ), this, SIGNAL( ackChanged(bool) ) );
    connect( ui->cmd, SIGNAL( toggled(bool) ), this, SIGNAL( cmdChanged(bool) ) );
}


SettingsWidget::~SettingsWidget()
{
    delete ui;
}


SnapErrorDetectionMode SettingsWidget::edm() const
{
    return static_cast<SnapErrorDetectionMode>(
                ui->edm->itemData( ui->edm->currentIndex() ).toInt()
    );
}


quint32 SettingsWidget::localAddress() const
{
    return ui->local->value();
}


quint8 SettingsWidget::localAddressSize() const
{
    return ui->local_size->value();
}


quint32 SettingsWidget::peerAddress() const
{
    return ui->peer->value();
}


quint8 SettingsWidget::peerAddressSize() const
{
    return ui->peer_size->value();
}


void SettingsWidget::setEdm(SnapErrorDetectionMode edm)
{
    ui->edm->setCurrentIndex( ui->edm->findData( edm ) );
}


void SettingsWidget::setLocalAddress(quint32 value)
{
    ui->local->setValue( value );
}


void SettingsWidget::setLocalAddressSize(quint8 value)
{
    ui->local_size->setValue( value );
}


void SettingsWidget::setPeerAddress(quint32 value)
{
   ui->peer->setValue( value );
}


void SettingsWidget::setPeerAddressSize(quint8 value)
{
    ui->peer_size->setValue( value );
}


void SettingsWidget::slotEdmComboChanged()
{
    emit edmChanged( edm() );
}


void SettingsWidget::slotLocalAddressChanged()
{
    adjustAddressSpinBox( ui->local->value(), ui->local_size );
    emit localAddressChanged( ui->local->value() );
}


void SettingsWidget::slotLocalAddressSizeChanged()
{
    adjustAddressSizeSpinBox( ui->local_size->value(), ui->local );
    emit localAddressChanged( ui->local->value() );
}


void SettingsWidget::slotPeerAddressChanged()
{
    adjustAddressSpinBox( ui->peer->value(), ui->peer_size );
    emit peerAddressChanged( ui->peer->value() );
}


void SettingsWidget::slotPeerAddressSizeChanged()
{
    adjustAddressSizeSpinBox( ui->peer_size->value(), ui->peer );
    emit peerAddressChanged( ui->peer->value() );
}


void SettingsWidget::adjustAddressSpinBox(int value, QSpinBox *spinbox)
{
    bool block = spinbox->blockSignals( true );

    if( value == 0 ) {
        spinbox->setValue( 0 );
    }
    else if( value > 0 && value < 256 ) {
        spinbox->setValue( 1 );
    }
    else if( value >= 256 && value < 65536 ) {
        spinbox->setValue( 2 );
    }
    else {
        spinbox->setValue( 3 );
    }

    spinbox->blockSignals( block );
}


void SettingsWidget::adjustAddressSizeSpinBox(int value, QSpinBox *spinbox)
{
    bool block = spinbox->blockSignals( true );

    switch( value ) {
        case 0:
            spinbox->setValue( 0 );
            break;
        case 1:
            if( spinbox->value() > 255 ) {
                spinbox->setValue( 254 );
            }
            else if( spinbox->value() <= 0 ) {
                spinbox->setValue( 1 );
            }
            break;
        case 2:
            if( spinbox->value() > 65536 ) {
                spinbox->setValue( 65535 );
            }
            else if( spinbox->value() <= 255 ) {
                spinbox->setValue( 256 );
            }
            break;
        case 3:
            if( spinbox->value() <= 65536 ) {
                spinbox->setValue( 65537 );
            }
            break;
        default:
            break;
    }

    spinbox->blockSignals( block );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
