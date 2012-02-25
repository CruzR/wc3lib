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

#ifndef WC3LIB_MAP_CUSTOMUNITS_HPP
#define WC3LIB_MAP_CUSTOMUNITS_HPP

#include <boost/ptr_container/ptr_vector.hpp>

#include "platform.hpp"
#include "value.hpp"

namespace wc3lib
{

namespace map
{

/**
 * Warcraft III RoC only (not in TFT), allows you to customize unit data.
 * Later, in Frozen Throne Blizzard introduced general object data modification (\ref CustomObjects).
 */
class CustomUnits : public FileFormat
{
	public:
		class Modification : public Format
		{
			public:
				Modification();
				virtual ~Modification();

				std::streamsize read(InputStream &istream) throw (class Exception);
				std::streamsize write(OutputStream &ostream) const throw (class Exception);

				id valueId() const;
				struct Value& value();
				const struct Value& value() const;

			protected:
				std::streamsize readList(InputStream &istream, BOOST_SCOPED_ENUM(Value::Type) type);
				std::streamsize writeList(OutputStream &ostream) const;

				std::streamsize readData(InputStream &istream) throw (class Exception);
				std::streamsize writeData(OutputStream &ostream) const throw (class Exception);

				id m_id; // from "Units\UnitMetaData.slk"
				struct Value m_value;
		};

		class Unit : public Format
		{
			public:
				typedef boost::ptr_vector<Modification> Modifications;

				Unit();
				virtual ~Unit();

				std::streamsize read(InputStream &istream) throw (class Exception);
				std::streamsize write(OutputStream &ostream) const throw (class Exception);

				bool isOriginal() { return m_customId == 0; };

				id originalId() const;
				id customId() const;
				Modifications& modifications();
				const Modifications& modifications() const;

			protected:
				virtual class Modification* createModification() const;

				id m_originalId;
				id m_customId;
				Modifications m_modifications;
		};

		typedef boost::ptr_vector<Unit> Table;

		CustomUnits();
		virtual ~CustomUnits();

		virtual std::streamsize read(InputStream &istream) throw (class Exception);
		virtual std::streamsize write(OutputStream &ostream) const throw (class Exception);

		virtual const byte* fileName() const;
		virtual const byte* fileTextId() const;
		virtual uint32 latestFileVersion() const;

		virtual uint32 version() const;

		Table& originalTable();
		const Table& originalTable() const;
		Table& customTable();
		const Table& customTable() const;

	protected:
		virtual Unit* createUnit() const;

		uint32 m_version;
		Table m_originalTable;
		Table m_customTable;
};

inline id CustomUnits::Modification::valueId() const
{
	return m_id;
}

inline Value& CustomUnits::Modification::value()
{
	return m_value;
}

inline const Value& CustomUnits::Modification::value() const
{
	return m_value;
}

inline id CustomUnits::Unit::originalId() const
{
	return m_originalId;
}

inline id CustomUnits::Unit::customId() const
{
	return m_customId;
}

inline CustomUnits::Unit::Modifications& CustomUnits::Unit::modifications()
{
	return m_modifications;
}

inline const CustomUnits::Unit::Modifications& CustomUnits::Unit::modifications() const
{
	return m_modifications;
}

inline const byte* CustomUnits::fileName() const
{
	return "war3map.w3u";
}

inline const byte* CustomUnits::fileTextId() const
{
	return "";
}

inline uint32 CustomUnits::latestFileVersion() const
{
	return 1;
}

inline uint32 CustomUnits::version() const
{
	return m_version;
}

inline CustomUnits::Table& CustomUnits::originalTable()
{
	return m_originalTable;
}

inline const CustomUnits::Table& CustomUnits::originalTable() const
{
	return m_originalTable;
}

inline CustomUnits::Table& CustomUnits::customTable()
{
	return m_customTable;
}

inline const CustomUnits::Table& CustomUnits::customTable() const
{
	return m_customTable;
}

}

}

#endif
