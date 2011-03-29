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
#include "statistic.h"


namespace Snap
{


Statistic::Statistic()
  : m_rejected_packets( 0 ),
    m_transmitted_packets( 0 ),
    m_received_packets( 0 ),
    m_total_packets( 0 ),
    m_rejected_bytes( 0 ),
    m_transmitted_bytes( 0 ),
    m_received_bytes( 0 ),
    m_total_bytes( 0 )
{
}


Statistic::Statistic(const snap_statistic_t &stat)
  : m_rejected_packets( stat.rejected_packets ),
    m_transmitted_packets( stat.transmitted_packets ),
    m_received_packets( stat.received_packets ),
    m_total_packets( stat.total_packets ),
    m_rejected_bytes( stat.rejected_bytes ),
    m_transmitted_bytes( stat.transmitted_bytes ),
    m_received_bytes( stat.received_bytes ),
    m_total_bytes( stat.total_bytes )
{
}


Statistic::~Statistic()
{
}


} // EndNamespace


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim: set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
