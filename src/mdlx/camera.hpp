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

#ifndef WC3LIB_MDLX_CAMERA_HPP
#define WC3LIB_MDLX_CAMERA_HPP

#include "groupmdxblockmember.hpp"
#include "cameras.hpp"

namespace wc3lib
{

namespace mdlx
{

// not a child of class Object!
/**
 * MDL tag "Camera".
 */
class Camera : public GroupMdxBlockMember
{
	public:
		static const std::size_t nameSize = 0x50;
		
		Camera(class Cameras *cameras);
		virtual ~Camera();

		class Cameras* cameras() const;

		/**
		 * \return Returns name with length of \ref Camera::nameSize.
		 */
		const ascii* name() const;
		const struct VertexData& position() const;
		float32 fieldOfView() const;
		float32 farClip() const;
		float32 nearClip() const;
		const struct VertexData& target() const;
		class CameraTranslations* translations() const;
		class CameraRotationLengths* rotationLengths() const;
		class CameraTargetTranslations* targetTranslations() const;

		virtual std::streamsize readMdl(istream &istream) throw (class Exception);
		virtual std::streamsize writeMdl(ostream &ostream) const throw (class Exception);
		virtual std::streamsize readMdx(istream &istream) throw (class Exception);
		virtual std::streamsize writeMdx(ostream &ostream) const throw (class Exception);

	protected:
		//long nbytesi;
		ascii m_name[nameSize]; //(0x50)
		struct VertexData m_position;
		float32 m_fieldOfView;
		float32 m_farClip;
		float32 m_nearClip;
		struct VertexData m_target;
		class CameraTranslations *m_translations; //(KCTR)
		class CameraRotationLengths *m_rotationLengths; //(KCRL)
		class CameraTargetTranslations *m_targetTranslations; //(KTTR)
		//(BKCT) ?????????????????????????????????????????????????????????????????
};

inline class Cameras* Camera::cameras() const
{
	return boost::polymorphic_cast<class Cameras*>(this->parent());
}

inline const ascii* Camera::name() const
{
	return this->m_name;
}

inline const struct VertexData& Camera::position() const
{
	return this->m_position;
}

inline float32 Camera::fieldOfView() const
{
	return this->m_fieldOfView;
}

inline float32 Camera::farClip() const
{
	return this->m_farClip;
}

inline float32 Camera::nearClip() const
{
	return this->m_nearClip;
}

inline const struct VertexData& Camera::target() const
{
	return this->m_target;
}

inline class CameraTranslations* Camera::translations() const
{
	return this->m_translations;
}

inline class CameraRotationLengths* Camera::rotationLengths() const
{
	return this->m_rotationLengths;
}

inline class CameraTargetTranslations* Camera::targetTranslations() const
{
	return this->m_targetTranslations;
}

}

}

#endif
