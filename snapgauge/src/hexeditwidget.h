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
#ifndef HEXEDITWIDGET_H
#define HEXEDITWIDGET_H

/**
 * @file snapgauge/src/hexditorwidget.h
 * @ingroup snapgauge
 */

#include <QWidget>
#include <QByteArray>

#include <khexedit/valuecolumninterface.h>


class QByteArray;


class HexEditorWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit HexEditorWidget(QWidget *parent = 0);
        ~HexEditorWidget();

        QByteArray data() const;
        void setData(const QByteArray &data);

        KHE::ValueColumnInterface::KCoding coding() const;
        void setCoding(KHE::ValueColumnInterface::KCoding coding);

    private:
        QByteArray m_data;
        QWidget *hexEditor;
};

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
