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
#ifndef INFOWIDGET_H
#define INFOWIDGET_H

/**
 * @file snapgauge/src/infowidget.h
 * @ingroup snapgauge
 */

#include <QWidget>

#include <libsnap/global.h>


namespace Ui {
    class InfoWidget;
}


class InfoWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit InfoWidget(QWidget *parent = 0);
        ~InfoWidget();

    public slots:
        void setInputData(QByteArray data);
        void setOutputData(QByteArray data);
        void setEdm(SnapErrorDetectionMode edm);
        void setLocalAddress(quint32 value);
        void setPeerAddress(quint32 value);
        void setAck(bool);
        void setCmd(bool);

    private:
        void updateSizes();

    private:
        Ui::InfoWidget *ui;
        quint32 m_localAddress, m_peerAddress;
        QByteArray m_inputData, m_outputData;
        SnapErrorDetectionMode m_edm;
};

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
