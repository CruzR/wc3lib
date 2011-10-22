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

#include <QtGui>

#include <KAction>
#include <KActionCollection>

#include "modulemenu.hpp"
#include "editor.hpp"

namespace wc3lib
{

namespace editor
{

ModuleMenu::ModuleMenu(Module *module) : KMenu(tr("Module"), module)
{
	foreach (Module *mod, const_cast<const Editor*>(module->editor())->modules())
		addModuleAction(mod);

	connect(module->editor(), SIGNAL(createdModule(Module*)), this, SLOT(addModuleAction(Module*)));
}

void ModuleMenu::addModuleAction(Module *module)
{
	this->addAction(const_cast<const Editor*>(this->module()->editor())->modulesActions()[module]);
}

#include "moc_modulemenu.cpp"

}

}
