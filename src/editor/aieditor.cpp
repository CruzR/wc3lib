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

#include "aieditor.hpp"

namespace wc3lib
{

namespace editor
{

AiEditor::AiEditor(MpqPriorityList* source, QWidget* parent, Qt::WindowFlags f): Module(source, parent, f)
{
}

void AiEditor::createFileActions(class KMenu *menu)
{
}

void AiEditor::createEditActions(class KMenu *menu)
{
}

void AiEditor::createMenus(class KMenuBar *menuBar)
{
}

void AiEditor::createWindowsActions(class WindowsMenu *menu)
{
}

void AiEditor::createToolButtons(class ModuleToolBar *toolBar)
{
}

class SettingsInterface* AiEditor::settings()
{
	/// @todo FIXME
	return 0;
}

void AiEditor::onSwitchToMap(Map *map)
{
}

}

}
