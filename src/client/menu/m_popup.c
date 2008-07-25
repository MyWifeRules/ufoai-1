/**
 * @file m_popup.c
 */

/*
Copyright (C) 1997-2008 UFO:AI Team

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

#include "../client.h"
#include "m_main.h"
#include "m_popup.h"

#define POPUPLIST_MENU_NAME "popup_list"
#define POPUPLIST_NODE_NAME "popup_list"
#define POPUPBUTTON_MENU_NAME "popup_button"
#define POPUPBUTTON_NODE_NAME "popup_button_"
#define POPUPBUTTON_NODE_STRING_NAME "popup_button_string_"
#define POPUP_MENU_NAME "popup"

/** @brief strings to be used for popup when text is not static */
char popupText[MAX_SMALLMENUTEXTLEN];
char popupAction1[MAX_SMALLMENUTEXTLEN];
char popupAction2[MAX_SMALLMENUTEXTLEN];
char popupAction3[MAX_SMALLMENUTEXTLEN];

/**
 * @brief Popup in geoscape
 * @note Only use static strings here - or use popupText if you really have to
 * build the string
 */
void MN_Popup (const char *title, const char *text)
{
	mn.menuText[TEXT_POPUP] = title;
	mn.menuText[TEXT_POPUP_INFO] = text;
	if (ccs.singleplayer)
		CL_GameTimeStop();
	MN_PushMenu(POPUP_MENU_NAME);
}

/**
 * @brief Generates a popup that contains a list of selectable choices.
 * @param[in] title Title of the popup.
 * @param[in] headline First line of text written in the popup.
 * @param[in] entries List of the selectables choices.
 * @param[in] clickAction Action to perform when one clicked on the popup.
 */
menuNode_t *MN_PopupList (const char *title, const char *headline, linkedList_t* entries, const char *clickAction)
{
	menu_t* popupListMenu;
	menuNode_t* listNode;

	mn.menuText[TEXT_POPUP] = title;
	mn.menuText[TEXT_POPUP_INFO] = headline;

	/* make sure, that we are using the linked list */
	MN_MenuTextReset(TEXT_LIST);
	mn.menuTextLinkedList[TEXT_LIST] = entries;
	if (ccs.singleplayer)
		CL_GameTimeStop();

	popupListMenu = MN_GetMenu(POPUPLIST_MENU_NAME);
	if (!popupListMenu)
		Sys_Error("Could not get "POPUPLIST_MENU_NAME" menu");
	listNode = MN_GetNode(popupListMenu, POPUPLIST_NODE_NAME);
	if (!listNode)
		Sys_Error("Could not get "POPUPLIST_NODE_NAME" node in "POPUPLIST_MENU_NAME" menu");

	/* free previous actions */
	if (listNode->click) {
		assert(listNode->click->data);
		Mem_Free(listNode->click->data);
		Mem_Free(listNode->click);
		listNode->click = NULL;
	}

	if (clickAction) {
		listNode->mousefx = qtrue;
		listNode->click = MN_SetMenuAction(&listNode->click, EA_CMD, clickAction);
	} else {
		listNode->mousefx = qfalse;
		listNode->click = NULL;
	}

	MN_PushMenu(popupListMenu->name);
	return listNode;
}

/**
 * @brief Set string and action button.
 * @param[in] menu menu where buttons are modified.
 * @param[in] button Name of the node of the button.
 * @param[in] clickAction Action to perform when button is clicked.
 * @param[in] buttonText Name of the node of the string.
 * @note clickAction may be NULL if button is not needed.
 */
static void MN_SetOneButton (menu_t* menu, const char *button, const char *clickAction, const char *buttonText)
{
	menuNode_t* buttonNode;
	menuNode_t* buttonTextNode;

	buttonNode = MN_GetNode(menu, button);
	if (!buttonNode)
		Sys_Error("Could not get %s node in %s menu", button, menu->name);
	buttonTextNode = MN_GetNode(menu, buttonText);
	if (!buttonTextNode)
		Sys_Error("Could not get %s node in %s menu", buttonText, menu->name);

	/* free previous actions */
	if (buttonNode->click) {
		assert(buttonNode->click->data);
		Mem_Free(buttonNode->click->data);
		Mem_Free(buttonNode->click);
		buttonNode->click = NULL;
	}

	if (clickAction) {
		buttonNode->mousefx = qtrue;
		buttonNode->click = MN_SetMenuAction(&buttonNode->click, EA_CMD, clickAction);
		buttonNode->invis = qfalse;
		buttonTextNode->invis = qfalse;
	} else {
		buttonNode->mousefx = qfalse;
		buttonNode->click = NULL;
		buttonNode->invis = qtrue;
		buttonTextNode->invis = qtrue;
	}
}

/**
 * @brief Generates a popup that contains up to 3 buttons.
 * @param[in] title Title of the popup.
 * @param[in] text Text to display in the popup (use popupText if text is NULL).
 * @param[in] clickAction1 Action to perform when one clicked on the first button.
 * @param[in] clickText1 String that will be written in first button.
 * @param[in] tooltip1 Tooltip of first button.
 * @param[in] clickAction2 Action to perform when one clicked on the second button.
 * @param[in] clickText2 String that will be written in second button.
 * @param[in] tooltip2 Tooltip of second button.
 * @param[in] clickAction3 Action to perform when one clicked on the second button.
 * @param[in] clickText3 String that will be written in second button.
 * @param[in] tooltip3 Tooltip of second button.
 * @note clickAction AND clickText must be NULL if button should be invisible.
 */
void MN_PopupButton (const char *title, const char *text,
	const char *clickAction1, const char *clickText1, const char *tooltip1,
	const char *clickAction2, const char *clickText2, const char *tooltip2,
	const char *clickAction3, const char *clickText3, const char *tooltip3)
{
	menu_t* popupButtonMenu;

	mn.menuText[TEXT_POPUP] = title;
	if (text)
		mn.menuText[TEXT_POPUP_INFO] = text;
	else
		mn.menuText[TEXT_POPUP_INFO] = popupText;

	if (ccs.singleplayer)
		CL_GameTimeStop();

	popupButtonMenu = MN_GetMenu(POPUPBUTTON_MENU_NAME);
	if (!popupButtonMenu)
		Sys_Error("Could not get "POPUPBUTTON_MENU_NAME" menu");

	Cvar_Set("mn_popup_button_text1", clickText1);
	Cvar_Set("mn_popup_button_tooltip1", tooltip1);
	if (!clickAction1 && !clickText1) {
		MN_SetOneButton(popupButtonMenu, va("%s1", POPUPBUTTON_NODE_NAME),
			NULL, va("%s1", POPUPBUTTON_NODE_STRING_NAME));
	} else {
		MN_SetOneButton(popupButtonMenu, va("%s1", POPUPBUTTON_NODE_NAME),
			clickAction1 ? clickAction1 : popupAction1, va("%s1", POPUPBUTTON_NODE_STRING_NAME));
	}

	Cvar_Set("mn_popup_button_text2", clickText2);
	Cvar_Set("mn_popup_button_tooltip2", tooltip2);
	if (!clickAction2 && !clickText2) {
		MN_SetOneButton(popupButtonMenu, va("%s2", POPUPBUTTON_NODE_NAME),
			NULL, va("%s2", POPUPBUTTON_NODE_STRING_NAME));
	} else {
		MN_SetOneButton(popupButtonMenu, va("%s2", POPUPBUTTON_NODE_NAME),
			clickAction2 ? clickAction2 : popupAction2, va("%s2", POPUPBUTTON_NODE_STRING_NAME));
	}

	Cvar_Set("mn_popup_button_text3", clickText3);
	Cvar_Set("mn_popup_button_tooltip3", tooltip3);
	if (!clickAction3 && !clickText3) {
		MN_SetOneButton(popupButtonMenu, va("%s3", POPUPBUTTON_NODE_NAME),
			NULL, va("%s3", POPUPBUTTON_NODE_STRING_NAME));
	} else {
		MN_SetOneButton(popupButtonMenu, va("%s3", POPUPBUTTON_NODE_NAME),
			clickAction3 ? clickAction3 : popupAction3, va("%s3", POPUPBUTTON_NODE_STRING_NAME));
	}

	MN_PushMenu(popupButtonMenu->name);
}
