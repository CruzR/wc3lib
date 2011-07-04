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

#include <boost/foreach.hpp>

#include "cameras.hpp"
#include "camera.hpp"
#include "../utilities.hpp"
#include "../internationalisation.hpp"

namespace wc3lib
{

namespace map
{

Cameras::Cameras(class W3m *w3m) : m_w3m(w3m)
{

}

Cameras::~Cameras()
{
	BOOST_FOREACH(class Camera *camera, this->m_cameras)
		delete camera;
}

std::streamsize Cameras::read(std::istream &istream) throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::read(istream, this->m_version, size);

	if (this->m_version != latestFileVersion())
		throw Exception(boost::format(_("Cameras: Unknown version \"%1%\", expected \"%2%\".")) % this->m_version % latestFileVersion());

	int32 number;
	wc3lib::read(istream, number, size);

	for ( ; number >= 0; --number)
	{
		class Camera *camera = new Camera(this);
		size += camera->read(istream);
		this->m_cameras.push_back(camera);
	}

	return size;
}

std::streamsize Cameras::write(std::ostream &ostream) const throw (class Exception)
{
	if (this->m_version != latestFileVersion())
		throw Exception(boost::format(_("Cameras: Unknown version \"%1%\", expected \"%2%\".")) % this->m_version % latestFileVersion());

	std::streamsize size = 0;
	wc3lib::write(ostream, this->m_version, size);
	int32 number = this->m_cameras.size();
	wc3lib::write(ostream, number, size);

	BOOST_FOREACH(const class Camera *camera, this->m_cameras)
		size += camera->write(ostream);

	return size;
}

}

}
