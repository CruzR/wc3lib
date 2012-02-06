/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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

#ifndef WC3LIB_MAP_ENVIRONMENT_HPP
#define WC3LIB_MAP_ENVIRONMENT_HPP

#include "platform.hpp"
#include "tilepoint.hpp"

namespace wc3lib
{

namespace map
{

class Environment : public FileFormat
{
	public:
		typedef std::vector<id> Ids;
		typedef boost::shared_ptr<Tilepoint> TilepointPtr;
		typedef std::set<TilepointPtr> Tilepoints;

		/**
		 * There is a physical limit of usable tilesets per environment since each tilepoint (\ref Tilepoint) only uses 4 bits to refer on its ground texture type and its cliff texture type in its corresponding environment.
		 */
		static const uint32 maxTilesets;

		BOOST_SCOPED_ENUM_START(MainTileset) /// \todo C++11 : byte
		{
			Ashenvale = 'A',
			Barrens = 'B',
			Felwood = 'C',
			Dungeon = 'D',
			LordaeronFall = 'F',
			Underground = 'G',
			LordaeronSummer = 'L',
			Northrend = 'N',
			VillageFall = 'Q',
			Village = 'V',
			LordaeronWinter = 'W',
			Dalaran = 'X',
			Cityscape = 'Y',
			SunkenRuins = 'Z',
			Icecrown = 'I',
			DalaranRuins = 'J',
			Outland = 'O',
			BlackCitadel = 'K'
		};
		BOOST_SCOPED_ENUM_END

		Environment(class W3m *w3m);

		std::streamsize read(InputStream &istream) throw (class Exception);
		std::streamsize write(OutputStream &ostream) const throw (class Exception);

		virtual const char8* fileTextId() const;
		virtual const char8* fileName() const;
		virtual uint32 latestFileVersion() const;
		virtual uint32 version() const { return m_version; }

		uint32 mapWidth() const;
		uint32 mapHeight() const;

		BOOST_SCOPED_ENUM(MainTileset) mainTileset() const;
		bool customized() const;
		const Ids& groundTilesetsIds() const;
		const Ids& cliffTilesetsIds() const;
		uint32 maxX() const;
		uint32 maxY() const;
		float32 centerOffsetX() const;
		float32 centerOffsetY() const;

		const Tilepoints& tilepoints() const;
		Tilepoints& tilepoints();

		const TilepointPtr& tilepoint(const Position &position) const  throw (Exception);

	protected:
		class W3m *m_w3m;
		uint32 m_version;
		BOOST_SCOPED_ENUM(MainTileset) m_mainTileset;
		bool m_customized;
		Ids m_groundTilesetsIds;
		Ids m_cliffTilesetsIds;
		uint32 m_maxX;
		uint32 m_maxY;
		float32 m_centerOffsetX;
		float32 m_centerOffsetY;
		Tilepoints m_tilepoints;

};

inline const char8* Environment::fileTextId() const
{
	return "W3E!";
}

inline const char8* Environment::fileName() const
{
	return "war3map.w3e";
}

inline uint32 Environment::latestFileVersion() const
{
	return 11;
}

inline uint32 Environment::mapWidth() const
{
	return maxX() - 1;
}

inline uint32 Environment::mapHeight() const
{
	return maxY() - 1;
}

inline BOOST_SCOPED_ENUM(Environment::MainTileset) Environment::mainTileset() const
{
	return m_mainTileset;
}

inline bool Environment::customized() const
{
	return m_customized;
}

inline const Environment::Ids& Environment::groundTilesetsIds() const
{
	return m_groundTilesetsIds;
}

inline const Environment::Ids& Environment::cliffTilesetsIds() const
{
	return m_cliffTilesetsIds;
}

inline uint32 Environment::maxX() const
{
	return m_maxX;
}

inline uint32 Environment::maxY() const
{
	return m_maxY;
}

inline float32 Environment::centerOffsetX() const
{
	return m_centerOffsetX;
}

inline float32 Environment::centerOffsetY() const
{
	return m_centerOffsetY;
}

inline const Environment::Tilepoints& Environment::tilepoints() const
{
	return m_tilepoints;
}

inline Environment::Tilepoints& Environment::tilepoints()
{
	return m_tilepoints;
}


inline const Environment::TilepointPtr& Environment::tilepoint(const Position &position) const throw (Exception)
{
	TilepointPtr ptr(new Tilepoint(const_cast<Environment*>(this), position));
	Tilepoints::const_iterator iterator = tilepoints().find(ptr);

	if (iterator == tilepoints().end())
		throw Exception();

	return *iterator;
}


}

}

#endif
