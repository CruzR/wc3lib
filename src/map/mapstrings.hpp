/***************************************************************************
 *   Copyright (C) 2011 by Tamino Dauth                                    *
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

#ifndef WC3LIB_MAP_MAPSTRINGS_HPP
#define WC3LIB_MAP_MAPSTRINGS_HPP

#include "platform.hpp"
#include "strings.hpp"

namespace wc3lib
{

namespace map
{

class MapStrings : public FileFormat, public Strings
{
	public:
		virtual std::streamsize read(InputStream &istream) throw (Exception)
		{
			return Strings::read(istream);
		}

		virtual std::streamsize write(OutputStream &ostream) const throw (Exception)
		{
			return Strings::write(ostream);
		}

		virtual id fileId() const;
		virtual const char8* fileName() const;
		virtual int32 latestFileVersion() const;
		virtual uint32_t version() const;
};

inline id MapStrings::fileId() const
{
	return 0;
}

inline const wc3lib::map::char8* MapStrings::fileName() const
{
	return "war3map.wts";
}

inline int32 MapStrings::latestFileVersion() const
{
	return 0;
}

inline uint32_t MapStrings::version() const
{
	return 0;
}

}

}

#endif
