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
#include "hexeditwidget.h"

#include <QLabel>
#include <QVBoxLayout>

#include <khexedit/byteseditinterface.h>
#include <khexedit/valuecolumninterface.h>
#include <khexedit/charcolumninterface.h>


HexEditorWidget::HexEditorWidget(QWidget *parent)
  : QWidget( parent ),
    hexEditor( 0 )
{
    hexEditor = KHE::createBytesEditWidget( this );

    if( !hexEditor ) {
        hexEditor = new QLabel( i18n( "Could not find a hexedit component (Okular for example)." ), this );
    }
    else {
        hexEditor->setEnabled( true );

        KHE::BytesEditInterface *bytesEdit = KHE::bytesEditInterface( hexEditor );
        Q_ASSERT( bytesEdit );

        bytesEdit->setData( 0, 0, -1 );
        bytesEdit->setMaxDataSize( 2048 );
        bytesEdit->setReadOnly( true );
        bytesEdit->setAutoDelete( false );

        KHE::ValueColumnInterface *valueColumn = KHE::valueColumnInterface( hexEditor );
        if( valueColumn ) {
            valueColumn->setCoding( KHE::ValueColumnInterface::HexadecimalCoding );

            //valueColumn->setByteSpacingWidth( 2 );
            //valueColumn->setNoOfGroupedBytes( 1 );
            //valueColumn->setGroupSpacingWidth( 6 );
            //valueColumn->setNoOfBytesPerLine( 10 );
        }
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( hexEditor );

    setLayout( layout );
}


HexEditorWidget::~HexEditorWidget()
{

}


QByteArray HexEditorWidget::data() const
{
    return m_data;
}


void HexEditorWidget::setData(const QByteArray &data)
{
    m_data = data;

    if( hexEditor ) {
        KHE::BytesEditInterface *bytesEdit = KHE::bytesEditInterface( hexEditor );
        Q_ASSERT( bytesEdit );

        bytesEdit->setData( m_data.data(), m_data.size() );
    }
}


KHE::ValueColumnInterface::KCoding HexEditorWidget::coding() const
{
    if( hexEditor ) {
        KHE::ValueColumnInterface *valueColumn = KHE::valueColumnInterface( hexEditor );

        if( valueColumn ) {
            return valueColumn->coding();
        }
    }

    return KHE::ValueColumnInterface::HexadecimalCoding;
}


void HexEditorWidget::setCoding(KHE::ValueColumnInterface::KCoding coding)
{
    if( hexEditor ) {
        KHE::ValueColumnInterface *valueColumn = KHE::valueColumnInterface( hexEditor );

        if( valueColumn ) {
            valueColumn->setCoding( coding );
        }
    }
}

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
