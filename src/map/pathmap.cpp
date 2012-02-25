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

#include "pathmap.hpp"

namespace wc3lib
{

namespace map
{

namespace
{

struct Header
{
	int32 fileId; //[4]: file ID = 'MP3W'
	int32 version; //: file version = 0
	int32 width; //: path map width (=map_width*4)
	int32 height; //: path map height (=map_height*4)
};

}

Pathmap::Pathmap(class W3m *w3m) : m_w3m(w3m), m_version(latestFileVersion()), m_width(0), m_height(0)
{
}

Pathmap::~Pathmap()
{
}

std::streamsize Pathmap::read(InputStream &istream) throw (class Exception)
{
	struct Header header;
	std::streamsize size = 0;
	wc3lib::read(istream, header, size);
	const id requiredFileId = fileId();

	if (memcmp(&header.fileId, &requiredFileId, sizeof(header.fileId)) != 0)
		throw Exception(boost::format(_("Pathmap: Unknown file id \"%1%\". Expected \"%2%\".")) % header.fileId % fileId());

	this->m_version = header.version;
	this->m_width = header.width;
	this->m_height = header.height;

	if (!this->m_data.empty())
		this->m_data.clear();

	for (int32 width = 0; width < header.width; ++width)
	{
		for (int32 height = 0; height < header.height; ++height)
		{
			byte type;
			wc3lib::read(istream, type, size);
			this->m_data[Position(width, height)] = (BOOST_SCOPED_ENUM(Type))(type);
		}
	}

	return size;
}

std::streamsize Pathmap::write(OutputStream &ostream) const throw (class Exception)
{
	struct Header header;
	header.fileId = fileId();
	header.height = height();
	header.width = width();
	header.version = version();
	std::streamsize size = 0;
	wc3lib::write(ostream, header, size);

	for (int32 width = 0; width < header.width; ++width)
	{
		for (int32 height = 0; height < header.height; ++height)
		{
			byte type = (byte)this->m_data.find(Position(width, height))->second;
			wc3lib::write(ostream, type, size);
		}
	}

	return size;
}

}

}
