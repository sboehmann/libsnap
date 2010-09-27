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
/** @defgroup libsnapmm libsnap++, the C++ interface for libsnap */

#ifndef LIBSNAPPP_SNAP_H
#define LIBSNAPPP_SNAP_H

/**
 * @file libsnap++/snap.h
 * @ingroup libsnapmm
 * @brief C++ interface for libsnap
 *
 * C++ wrapper for libsnap
 *
 * @version 0.1
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


#include "statistic.h"

#include <libsnap/snap.h>

#include <vector>


namespace Snap
{

    /**
     * @class Snap
     * @brief C++ interface for libsnap
     *
     * C++ wrapper for libsnap
     *
     * @version 0.1
     */
    class Snap
    {
        public:
            Snap();
            virtual ~Snap();

            size_t size() const;

            void reset();

            const Statistic statistic() const;
            void clearStatistic();

            bool acknowledgementEnabled() const;
            void setAcknowledgementEnabled(bool enabled);

            size_t errorDetectionModeSize() const;
            SnapErrorDetectionMode errorDetectionMode() const;
            void setErrorDetectionMode(SnapErrorDetectionMode edm);

            size_t localAddressSize() const;
            int32_t localAddress() const;
            void setLocalAddress(int32_t address);

            size_t peerAddressSize() const;
            int32_t peerAddress() const;
            void setPeerAddress(int32_t address);

            std::vector<uint8_t> encode(const std::vector<uint8_t> &data);
            std::vector<uint8_t> encode(const uint8_t *data, size_t size);

            std::vector<uint8_t> decode(const std::vector<uint8_t> &data,
                                              std::vector<uint8_t> &response);
            std::vector<uint8_t> decode(const uint8_t *data, size_t size,
                                              std::vector<uint8_t> &response);

            uint32_t version() const;

        protected:
            uint16_t header(size_t datasize) const;

            size_t encodeBound(size_t datasize) const;
            size_t decodeResultBound(size_t datasize) const;
            size_t decodeResponseBound(size_t datasize) const;

        private:
            snap_t *snap;
    };


} // EndNamespace

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
