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

#ifndef CLIENT_UI_UI_DATAIDS_H
#define CLIENT_UI_UI_DATAIDS_H

/** @brief linked into ui_global.sharedData - defined in UI scripts via dataId property */
enum uiDataIDs_t {
	TEXT_NULL,		/**< default value, should not be used */
	TEXT_STANDARD,
	TEXT_LIST,
	TEXT_LIST2,
	TEXT_UFOPEDIA,
	TEXT_UFOPEDIA_REQUIREMENT,
	TEXT_BUILDINGS,
	TEXT_BUILDING_INFO,
	TEXT_RESEARCH,
	TEXT_POPUP,
	TEXT_POPUP_INFO,
	TEXT_AIRCRAFT_LIST,
	TEXT_AIRCRAFT_INFO,
	TEXT_MULTISELECTION,
	TEXT_PRODUCTION_LIST,
	TEXT_PRODUCTION_AMOUNT,
	TEXT_PRODUCTION_INFO,
	TEXT_EMPLOYEE,
	TEXT_MOUSECURSOR_RIGHT,
	TEXT_PRODUCTION_QUEUED,
	TEXT_STATS_BASESUMMARY,
	TEXT_STATS_MISSION,
	TEXT_STATS_BASES,
	TEXT_STATS_NATIONS,
	TEXT_STATS_EMPLOYEES,
	TEXT_STATS_COSTS,
	TEXT_STATS_INSTALLATIONS,
	TEXT_STATS_7,
	TEXT_BASE_LIST,
	TEXT_BASE_INFO,
	TEXT_TRANSFER_LIST,
	TEXT_TRANSFER_LIST_AMOUNT,
	TEXT_TRANSFER_LIST_TRANSFERED,
	TEXT_MOUSECURSOR_PLAYERNAMES,
	TEXT_CARGO_LIST,
	TEXT_CARGO_LIST_AMOUNT,
	TEXT_UFOPEDIA_MAILHEADER,
	TEXT_UFOPEDIA_MAIL,
	TEXT_MARKET_NAMES,
	TEXT_MARKET_STORAGE,
	TEXT_MARKET_MARKET,
	TEXT_MARKET_PRICES,
	TEXT_CHAT_WINDOW,
	TEXT_AIREQUIP_1,
	TEXT_AIREQUIP_2,
	TEXT_BASEDEFENCE_LIST,
	TEXT_TIPOFTHEDAY,
	TEXT_GENERIC,
	TEXT_XVI,
	TEXT_MOUSECURSOR_TOP,
	TEXT_MOUSECURSOR_BOTTOM,
	TEXT_MOUSECURSOR_LEFT,
	TEXT_MESSAGEOPTIONS,
	TEXT_UFORECOVERY_NATIONS,
	TEXT_UFORECOVERY_UFOYARDS,
	TEXT_UFORECOVERY_CAPACITIES,
	TEXT_MATERIAL_STAGES,
	TEXT_IRCCONTENT,
	TEXT_IRCUSERS,
	TEXT_MULTIPLAYER_USERLIST,
	TEXT_MULTIPLAYER_USERTEAM,
	TEXT_ITEMDESCRIPTION,

	OPTION_LANGUAGES,
	OPTION_JOYSTICKS,
	OPTION_VIDEO_RESOLUTIONS,
	OPTION_SINGLEPLAYER_SKINS,
	OPTION_MULTIPLAYER_SKINS,
	OPTION_UFOPEDIA,
	OPTION_UFOS,
	OPTION_DROPSHIPS,
	OPTION_BASELIST,
	OPTION_TEAMDEFS,
	OPTION_PRODUCTION_REQUIREMENTS,
	OPTION_CAMPAIGN_LIST,

	LINESTRIP_FUNDING,
	LINESTRIP_COLOR,

	UI_MAX_DATAID
};

#endif
