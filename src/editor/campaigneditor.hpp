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

#ifndef WC3LIB_EDITOR_CAMPAIGNEDITOR_HPP
#define WC3LIB_EDITOR_CAMPAIGNEDITOR_HPP

#include <KUrl>

#include "module.hpp"

namespace wc3lib
{

namespace editor
{

/**
 * The campaign editor allows you to edit exactly one single campaign at the same time.
 */
class CampaignEditor : public Module
{
	Q_OBJECT

	public:
		CampaignEditor(class MpqPriorityList *source, QWidget *parent = 0, Qt::WindowFlags f = 0);

	public slots:
		void openCampaign(const KUrl &url);
		void closeCampaign();

	protected:
		virtual void createFileActions(class KMenu *menu);
		virtual void createEditActions(class KMenu *menu);
		virtual void createMenus(class KMenuBar *menuBar);
		virtual void createWindowsActions(class KMenu *menu);
		virtual void createToolButtons(class KToolBar *toolBar);
		virtual class SettingsInterface* settings();
};

}

}

#endif
