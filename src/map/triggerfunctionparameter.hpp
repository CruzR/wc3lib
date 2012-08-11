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

#ifndef WC3LIB_MAP_TRIGGERFUNCTIONPARAMETER_HPP
#define WC3LIB_MAP_TRIGGERFUNCTIONPARAMETER_HPP

#include <boost/ptr_container/ptr_vector.hpp>

#include "platform.hpp"

namespace wc3lib
{

namespace map
{

/**
 * \todo Add derived class TriggerFunctionParameterX.
 * \sa TriggerFunctionParameterX
 */
class TriggerFunctionParameter : public Format
{
	public:
		typedef boost::ptr_vector<class TriggerFunction> Functions;
		typedef boost::ptr_vector<TriggerFunctionParameter> Parameters;

		BOOST_SCOPED_ENUM_START(Type) /// \todo C++11 : int32
		{
			Preset,
			Variable,
			Function,
			Jass
		};
		BOOST_SCOPED_ENUM_END

		TriggerFunctionParameter();
		virtual ~TriggerFunctionParameter();

		virtual std::streamsize read(InputStream &istream) throw (class Exception);
		virtual std::streamsize write(OutputStream &ostream) const throw (class Exception);

		BOOST_SCOPED_ENUM(Type) type() const;
		const string& value() const;

		/**
		 * Appears only if \ref type() == \ref Type::Function.
		 * It's used for multiple statements like multiple if then else?
		 */
		Functions& functions();
		const Functions& functions() const;
		/**
		 * Appears only if \ref type() == \ref Type::Variable and variable is an array.
		 * It's used for defining the array's index.
		 */
		Parameters& parameters();
		const Parameters& parameters() const;

	protected:
		BOOST_SCOPED_ENUM(Type) m_type;
		string m_value;
		Functions m_functions;
		Parameters m_parameters;
};

inline BOOST_SCOPED_ENUM(TriggerFunctionParameter::Type) TriggerFunctionParameter::type() const
{
	return m_type;
}

inline const string& TriggerFunctionParameter::value() const
{
	return m_value;
}

inline TriggerFunctionParameter::Functions& TriggerFunctionParameter::functions()
{
	return this->m_functions;
}

inline const TriggerFunctionParameter::Functions& TriggerFunctionParameter::functions() const
{
	return this->m_functions;
}

inline TriggerFunctionParameter::Parameters& TriggerFunctionParameter::parameters()
{
	return this->m_parameters;
}

inline const TriggerFunctionParameter::Parameters &TriggerFunctionParameter::parameters() const
{
	return this->m_parameters;
}

}

}

#endif
