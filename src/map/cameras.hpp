/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef WC3LIB_MAP_CAMERAS_HPP
#define WC3LIB_MAP_CAMERAS_HPP

#include "platform.hpp"

namespace wc3lib
{

namespace map
{

/**
 * "war3map.w3c" file of maps contains camera information.
 */
class Cameras : public FileFormat
{
	public:
		Cameras(class W3m *w3m);
		~Cameras();

		std::streamsize read(std::istream &istream) throw (class Exception);
		std::streamsize write(std::ostream &ostream) const throw (class Exception);

		virtual const char8* fileName() const;
		virtual int32 latestFileVersion() const;
		
		virtual int32 version() const { return m_version; }

	protected:
		class W3m *m_w3m;
		int32 m_version;
		std::list<class Camera*> m_cameras;
};

inline const char8* Cameras::fileName() const
{
	return "war3map.w3c";
}

inline int32 Cameras::latestFileVersion() const
{
	return 0;
}

}

}

#endif
