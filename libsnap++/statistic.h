/*
 * Copyright 2010  Stefan Böhmann <kde@hilefoks.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBSNAPPP_STATISTIC_H
#define LIBSNAPPP_STATISTIC_H

/**
 * @file libsnap++/statistic.h
 * @ingroup libsnapmm
 */

#include <libsnap/snap.h>

namespace Snap
{

    class Statistic
    {
        public:
            Statistic();
            explicit Statistic(const snap_statistic_t &stat);

            virtual ~Statistic();

            uint64_t packetsRejected() const { return m_rejected_packets; }
            uint64_t packetsTransmitted() const { return m_transmitted_packets; }
            uint64_t packetsReceived() const { return m_received_packets; }
            uint64_t packetsTotal() const { return m_total_packets; }

            uint64_t bytesRejected() const { return m_rejected_bytes; }
            uint64_t bytesTransmitted() const { return m_transmitted_bytes; }
            uint64_t bytesReceived() const { return m_received_bytes; }
            uint64_t bytesTotal() const { return m_total_bytes; }

        private:
            uint64_t m_rejected_packets;
            uint64_t m_transmitted_packets;
            uint64_t m_received_packets;
            uint64_t m_total_packets;
            uint64_t m_rejected_bytes;
            uint64_t m_transmitted_bytes;
            uint64_t m_received_bytes;
            uint64_t m_total_bytes;
    };


} // EndNamespace

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
