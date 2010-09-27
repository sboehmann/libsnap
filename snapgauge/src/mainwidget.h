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
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

/**
 * @file snapgauge/src/mainwidget.h
 * @ingroup snapgauge
 */

#include <QWidget>
#include <libsnap/global.h>

namespace Ui {
    class MainWidget;
}

class QSpinBox;


class MainWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit MainWidget(QWidget *parent = 0);
        ~MainWidget();

        QByteArray data() const;

        SnapErrorDetectionMode edm() const;

        quint32 localAddress() const;
        quint32 peerAddress() const;

        bool ack() const;
        bool cmd() const;

    public slots:
        void setData(QByteArray data);

        void setEdm(SnapErrorDetectionMode edm);

        void setLocalAddress(quint32 value);
        void setPeerAddress(quint32 value);

        void setAck(bool);
        void setCmd(bool);

    signals:
        void inputDataChanged(QByteArray data);
        void outputDataChanged(QByteArray data);

    private slots:
        void slotInputChanged();
        void slotChangeView();

    private:
        Ui::MainWidget *ui;

        quint32 m_localAddress, m_peerAddress;
        SnapErrorDetectionMode m_edm;
        bool m_ack, m_cmd;
};

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
