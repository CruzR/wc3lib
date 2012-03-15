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

#include "triggers.hpp"
#include "w3m.hpp"
#include "triggercategory.hpp"
#include "variable.hpp"
#include "trigger.hpp"
#include "triggerdata.hpp"

namespace wc3lib
{

namespace map
{

Triggers::Triggers(class W3m *w3m) : m_w3m(w3m)
{
}

std::streamsize Triggers::read(InputStream &istream, const TriggerData &triggerData) throw (class Exception)
{
	std::streamsize size = 0;
	/*
	 * TEST
	id requiredId = fileId();
	id fileId;
	wc3lib::read(istream, fileId, size);

	if (memcmp(&fileId, &requiredId, sizeof(fileId)) != 0)
		*/
	id fileId;
	wc3lib::read(istream, fileId, size);

	if (this->fileId() != fileId)
		throw Exception(boost::format(_("Triggers: Unknown file id \"%1%\". Expected \"%2%\".")) % fileId % this->fileId());

	wc3lib::read(istream, this->m_version, size);

	if (this->version() != latestFileVersion())
		throw Exception(boost::format(_("Triggers: Version %1% is not supported (version %2% only).")) % this->version() % latestFileVersion());

	int32 number;
	wc3lib::read(istream, number, size);
	this->categories().reserve(number);

	for (int32 i = 0; i < number; ++i)
	{
		std::auto_ptr<TriggerCategory> ptr(new TriggerCategory(this));
		size += ptr->read(istream);
		categories().push_back(ptr);
	}

	wc3lib::read(istream, this->m_unknown0, size);
	wc3lib::read(istream, number, size);
	this->variables().reserve(number);

	for (int32 i = 0; i < number; ++i)
	{
		std::auto_ptr<Variable> ptr(new Variable(this));
		size += ptr->read(istream);
		variables().push_back(ptr);
	}

	wc3lib::read(istream, number, size);
	this->triggers().reserve(number);

	for (int32 i = 0; i < number; ++i)
	{
		std::auto_ptr<Trigger> ptr(new Trigger(this));
		size += ptr->read(istream);
		triggers().push_back(ptr);
	}

	return size;
}

std::streamsize Triggers::write(OutputStream &ostream) const throw (class Exception)
{
	std::streamsize size = 0;
	wc3lib::write(ostream, fileId(), size);
	wc3lib::write(ostream, this->version(), size);
	int32 number = boost::numeric_cast<int32>(this->categories().size());
	wc3lib::write(ostream, number, size);

	BOOST_FOREACH(Categories::const_reference value, categories())
		size += value.write(ostream);

	wc3lib::write(ostream, this->unknown0(), size);
	number = boost::numeric_cast<int32>(this->variables().size());
	wc3lib::write(ostream, number, size);

	BOOST_FOREACH(Variables::const_reference value, variables())
		size += value.write(ostream);

	number = boost::numeric_cast<int32>(this->triggers().size());
	wc3lib::write(ostream, number, size);

	BOOST_FOREACH(TriggerEntries::const_reference value, triggers())
		size += value.write(ostream);

	return size;
}

}

}
