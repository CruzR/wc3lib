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

#ifndef WC3LIB_MDLX_PARTICLEEMITTER2SPEED_HPP
#define WC3LIB_MDLX_PARTICLEEMITTER2SPEED_HPP

#include "mdlxalpha.hpp"
#include "particleemitter2speeds.hpp"

namespace wc3lib
{

namespace mdlx
{

class ParticleEmitter2Speed : public MdlxAlpha
{
	public:
		ParticleEmitter2Speed(class ParticleEmitter2Speeds *speeds);

		class ParticleEmitter2Speeds* speeds() const;
};

inline class ParticleEmitter2Speeds* ParticleEmitter2Speed::speeds() const
{
	return boost::polymorphic_cast<class ParticleEmitter2Speeds*>(this->mdlxAlphas());
}

}

}

#endif
