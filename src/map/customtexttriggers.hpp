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

#ifndef WC3LIB_MAP_CUSTOMTEXTTRIGGERS_HPP
#define WC3LIB_MAP_CUSTOMTEXTTRIGGERS_HPP

#include "platform.hpp"

namespace wc3lib
{

namespace map
{

class CustomTextTriggers : public FileFormat
{
	public:
		typedef std::vector<string> TriggerTexts;

		virtual std::streamsize read(InputStream& istream) throw (Exception);
		virtual std::streamsize write(OutputStream& ostream) const throw (Exception);

		virtual const char8* fileTextId() const;
		virtual const char8* fileName() const;
		virtual int32 latestFileVersion() const;
		virtual uint32_t version() const;

		const TriggerTexts& triggerTexts() const;

	protected:
		int32 m_version;
		TriggerTexts m_triggerTexts;
};

inline const char8* CustomTextTriggers::fileTextId() const
{
	return "";
}

inline const char8* CustomTextTriggers::fileName() const
{
	return "war3map.wct";
}

inline int32 CustomTextTriggers::latestFileVersion() const
{
	return 0;
}

inline uint32_t CustomTextTriggers::version() const
{
	return m_version;
}

inline const CustomTextTriggers::TriggerTexts& CustomTextTriggers::triggerTexts() const
{
	return this->m_triggerTexts;
}

}

}

#endif
