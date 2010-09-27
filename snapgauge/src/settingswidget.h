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
#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

/**
 * @file snapgauge/src/settingswidget.h
 * @ingroup snapgauge
 */

#include <QWidget>

#include <libsnap/global.h>


namespace Ui {
    class SettingsWidget;
}

class QSpinBox;


class SettingsWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit SettingsWidget(QWidget *parent = 0);
        ~SettingsWidget();

        SnapErrorDetectionMode edm() const;

        quint32 localAddress() const;
        quint8 localAddressSize() const;

        quint32 peerAddress() const;
        quint8 peerAddressSize() const;

    public slots:
        //void setInputData(QByteArray data);
        //void setOutputData(QByteArray data);

        void setEdm(SnapErrorDetectionMode edm);

        void setLocalAddress(quint32 value);
        void setLocalAddressSize(quint8 value);

        void setPeerAddress(quint32 value);
        void setPeerAddressSize(quint8 value);

    signals:
        void edmChanged(SnapErrorDetectionMode edm);
        void localAddressChanged(quint32 value);
        void peerAddressChanged(quint32 value);
        void ackChanged(bool);
        void cmdChanged(bool);

    private slots:
        void slotEdmComboChanged();
        void slotLocalAddressChanged();
        void slotLocalAddressSizeChanged();
        void slotPeerAddressChanged();
        void slotPeerAddressSizeChanged();

    private:
        void adjustAddressSpinBox(int value, QSpinBox *spinbox);
        void adjustAddressSizeSpinBox(int value, QSpinBox *spinbox);

    private:
        Ui::SettingsWidget *ui;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
