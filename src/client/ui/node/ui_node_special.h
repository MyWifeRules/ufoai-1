/**
 * @file
 */

/*
Copyright (C) 2002-2011 UFO: Alien Invasion.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef CLIENT_UI_UI_NODE_SPECIAL_H
#define CLIENT_UI_UI_NODE_SPECIAL_H

#include "../ui_nodes.h"

class uiConFuncNode : public uiNode {
	void onLoaded(uiNode_t* node) OVERRIDE;
	void onWindowOpened(uiNode_t* node, linkedList_t *params) OVERRIDE;
	void onWindowClosed(uiNode_t* node) OVERRIDE;
	void clone(uiNode_t const* source, uiNode_t* clone) OVERRIDE;
};

class uiFuncNode : public uiNode {
	void onLoaded(uiNode_t* node);
};

class uiCvarNode : public uiNode {
	void onWindowOpened(uiNode_t* node, linkedList_t *params);
	void onWindowClosed(uiNode_t* node);
	void clone(uiNode_t const* source, uiNode_t* clone);
	void deleteNode(uiNode_t* node);
};


void UI_RegisterConFuncNode(struct uiBehaviour_s *behaviour);
void UI_RegisterCvarFuncNode(struct uiBehaviour_s *behaviour);
void UI_RegisterFuncNode(struct uiBehaviour_s *behaviour);
void UI_RegisterNullNode(struct uiBehaviour_s *behaviour);

#endif
