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

#ifndef WC3LIB_EDITOR_MODULEMENU_HPP
#define WC3LIB_EDITOR_MODULEMENU_HPP

#include <QMap>

#include <KMenu>

#include "module.hpp"
#include "platform.hpp"

namespace wc3lib
{

namespace editor
{

/**
 * Menu entry which lists actions for all Editor modules.
 * Entries are created via signal when modules are created.
 * Should never be used without an \ref Editor instance.
 */
class ModuleMenu : public KMenu
{
	public:
		typedef QMap<class Module*, class QAction*> Actions;

		ModuleMenu(Module *module);

		Module* module() const;
		/**
		* Holds all modules actions (without separators).
		* Required by class Module.
		*/
		const Actions& actions() const;

	protected slots:
		void addModuleAction(Module *module, class KAction *action);

	private:
		Actions m_actions;
};

inline Module* ModuleMenu::module() const
{
	return boost::polymorphic_cast<Module*>(parent());
}

inline const ModuleMenu::Actions& ModuleMenu::actions() const
{
	return this->m_actions;
}

}

}

#endif
