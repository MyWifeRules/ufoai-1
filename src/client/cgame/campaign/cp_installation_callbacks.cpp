/**
 * @file
 * @brief Menu related console command callbacks
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

#include "../../cl_shared.h"
#include "../../ui/ui_dataids.h"
#include "cp_campaign.h"
#include "cp_installation_callbacks.h"
#include "cp_installation.h"
#include "cp_map.h"
#include "cp_popup.h"
#include "cp_ufo.h"
#include "cp_uforecovery_callbacks.h"

 /**
 * @brief Sets the title of the installation to a cvar to prepare the rename menu.
 * @note it also assigns description text
 */
static void INS_SetInstallationTitle (installationType_t type)
{
	const installationTemplate_t *insTemp = INS_GetInstallationTemplateByType(type);
	char insName[MAX_VAR];

	Com_sprintf(insName, lengthof(insName), "%s #%i", (insTemp) ? _(insTemp->name) : _("Installation"), ccs.campaignStats.installationsBuilt + 1);
	Cvar_Set("mn_installation_title", insName);
	if (!insTemp || !Q_strvalid(insTemp->description))
		cgi->UI_ResetData(TEXT_BUILDING_INFO);
	else
		cgi->UI_RegisterText(TEXT_BUILDING_INFO, _(insTemp->description));
}

/**
 * @brief Select an installation when clicking on it on geoscape
 * @param[in] installation The installation to select
 * @note This is (and should be) the only place where installationCurrent is set
 * to a value that is not @c NULL
 */
void INS_SelectInstallation (installation_t *installation)
{
	const int timetobuild = std::max(0, installation->installationTemplate->buildTime - (ccs.date.day - installation->buildStart));

	Com_DPrintf(DEBUG_CLIENT, "INS_SelectInstallation: select installation with id %i\n", installation->idx);
	ccs.mapAction = MA_NONE;
	if (installation->installationStatus == INSTALLATION_WORKING) {
		Cvar_Set("mn_installation_timetobuild", "-");
	} else {
		Cvar_Set("mn_installation_timetobuild", va(ngettext("%d day", "%d days", timetobuild), timetobuild));
	}
	INS_SetCurrentSelectedInstallation(installation);

	switch (installation->installationTemplate->type) {
	case INSTALLATION_UFOYARD:
		cgi->UI_PushWindow("popup_ufoyards");
		break;
	case INSTALLATION_DEFENCE:
		cgi->UI_PushWindow("basedefence");
		break;
	default:
		cgi->UI_PushWindow("popup_installationstatus");
		break;
	}
}

/**
 * @brief Returns the installation Template for a given installation ID.
 * @param[in] id ID of the installation template to find.
 * @return corresponding installation Template, @c NULL if not found.
 */
static const installationTemplate_t* INS_GetInstallationTemplateByID (const char *id)
{
	int idx;

	for (idx = 0; idx < ccs.numInstallationTemplates; idx++) {
		const installationTemplate_t *t = &ccs.installationTemplates[idx];
		if (Q_streq(t->id, id))
			return t;
	}

	return NULL;
}

/**
 * @brief Constructs a new installation.
 */
static void INS_BuildInstallation_f (void)
{
	const installationTemplate_t *installationTemplate;

	if (Cmd_Argc() < 1) {
		Com_Printf("Usage: %s <installationType>\n", Cmd_Argv(0));
		return;
	}

	/* We shouldn't build more installations than the actual limit */
	if (B_GetInstallationLimit() <= INS_GetCount())
		return;

	installationTemplate = INS_GetInstallationTemplateByID(Cmd_Argv(1));
	if (!installationTemplate) {
		Com_Printf("The installation type %s passed for %s is not valid.\n", Cmd_Argv(1), Cmd_Argv(0));
		return;
	}

	assert(installationTemplate->cost >= 0);

	if (ccs.credits - installationTemplate->cost > 0) {
		/* set up the installation */
		installation_t *installation = INS_Build(installationTemplate, ccs.newBasePos, Cvar_GetString("mn_installation_title"));

		CP_UpdateCredits(ccs.credits - installationTemplate->cost);
		/* this cvar is used for disabling the installation build button on geoscape if MAX_INSTALLATIONS was reached */
		Cvar_SetValue("mn_installation_count", INS_GetCount());

		const nation_t *nation = MAP_GetNation(installation->pos);
		if (nation)
			Com_sprintf(cp_messageBuffer, sizeof(cp_messageBuffer), _("A new installation has been built: %s (nation: %s)"), installation->name, _(nation->name));
		else
			Com_sprintf(cp_messageBuffer, sizeof(cp_messageBuffer), _("A new installation has been built: %s"), installation->name);
		MSO_CheckAddNewMessage(NT_INSTALLATION_BUILDSTART, _("Installation building"), cp_messageBuffer, MSG_CONSTRUCTION);
	} else {
		if (installationTemplate->type == INSTALLATION_RADAR) {
			if (MAP_IsRadarOverlayActivated())
					MAP_SetOverlay("radar");
		}
		if (ccs.mapAction == MA_NEWINSTALLATION)
			ccs.mapAction = MA_NONE;

		CP_Popup(_("Notice"), _("Not enough credits to set up a new installation."));
	}
	ccs.mapAction = MA_NONE;
}

/**
 * @brief Called when an installation is opened or a new installation is created on geoscape.
 * For a new installation the installationID is -1.
 */
static void INS_SelectInstallation_f (void)
{
	int installationID;
	installation_t *installation;

	if (Cmd_Argc() < 2) {
		Com_Printf("Usage: %s <installationID>\n", Cmd_Argv(0));
		return;
	}
	installationID = atoi(Cmd_Argv(1));

	installation = INS_GetByIDX(installationID);
	if (installation != NULL)
		INS_SelectInstallation(installation);
}

/**
 * @brief Creates console command to change the name of a installation.
 * Copies the value of the cvar mn_installation_title over as the name of the
 * current selected installation
 */
static void INS_ChangeInstallationName_f (void)
{
	installation_t *installation = INS_GetCurrentSelectedInstallation();

	/* maybe called without installation initialized or active */
	if (!installation)
		return;

	Q_strncpyz(installation->name, Cvar_GetString("mn_installation_title"), sizeof(installation->name));
}

/**
 * @brief console function for destroying an installation
 * @sa INS_DestroyInstallation
 */
static void INS_DestroyInstallation_f (void)
{
	installation_t *installation;

	if (Cmd_Argc() < 2 || atoi(Cmd_Argv(1)) < 0) {
		installation = INS_GetCurrentSelectedInstallation();
	} else {
		installation = INS_GetByIDX(atoi(Cmd_Argv(1)));
		if (!installation) {
			Com_DPrintf(DEBUG_CLIENT, "Installation not founded (idx %i)\n", atoi(Cmd_Argv(1)));
			return;
		}
	}

	/* Ask 'Are you sure?' by default */
	if (Cmd_Argc() < 3 || !atoi(Cmd_Argv(2))) {
		char command[MAX_VAR];

		Com_sprintf(command, sizeof(command), "mn_installation_destroy %d 1; ui_pop;", installation->idx);
		cgi->UI_PopupButton(_("Destroy Installation"), _("Do you really want to destroy this installation?"),
			command, _("Destroy"), _("Destroy installation"),
			"ui_pop;", _("Cancel"), _("Forget it"),
			NULL, NULL, NULL);
		return;
	}
	INS_DestroyInstallation(installation);
}

/**
 * @brief updates the installation limit cvar for menus
 */
static void INS_UpdateInstallationLimit_f (void)
{
	Cvar_SetValue("mn_installation_max", B_GetInstallationLimit());
}

/**
 * @brief Fills the UI with ufo yard data
 */
static void INS_FillUFOYardData_f (void)
{
	installation_t *ins;

	cgi->UI_ExecuteConfunc("ufolist_clear");
	if (Cmd_Argc() < 2 || atoi(Cmd_Argv(1)) < 0) {
		ins = INS_GetCurrentSelectedInstallation();
		if (!ins || ins->installationTemplate->type != INSTALLATION_UFOYARD)
			ins = INS_GetFirstUFOYard(false);
	} else {
		ins = INS_GetByIDX(atoi(Cmd_Argv(1)));
		if (!ins)
			Com_DPrintf(DEBUG_CLIENT, "Installation not founded (idx %i)\n", atoi(Cmd_Argv(1)));
	}

	if (ins) {
		const nation_t *nat = MAP_GetNation(ins->pos);
		const int timeToBuild = std::max(0, ins->installationTemplate->buildTime - (ccs.date.day - ins->buildStart));
		const char *buildTime = (timeToBuild > 0 && ins->installationStatus == INSTALLATION_UNDER_CONSTRUCTION) ? va(ngettext("%d day", "%d days", timeToBuild), timeToBuild) : "-";
		const int freeCap = std::max(0, ins->ufoCapacity.max - ins->ufoCapacity.cur);
		const char *nationName = nat ? _(nat->name) : "";

		cgi->UI_ExecuteConfunc("ufolist_addufoyard %d \"%s\" \"%s\" %d %d \"%s\"", ins->idx, ins->name, nationName, ins->ufoCapacity.max, freeCap, buildTime);

		US_Foreach(ufo) {
			if (ufo->installation != ins)
				continue;

			const char *ufoName = UFO_AircraftToIDOnGeoscape(ufo->ufoTemplate);
			const char *condition = va(_("Condition: %3.0f%%"), ufo->condition * 100);
			const char *status = US_StoredUFOStatus(ufo);
			cgi->UI_ExecuteConfunc("ufolist_addufo %d \"%s\" \"%s\" \"%s\" \"%s\"", ufo->idx, ufoName, condition, ufo->ufoTemplate->model, status);
		}
	}
}

static void INS_Init_f (void)
{
	linkedList_t *list = NULL;
	if (INS_GetCount() < B_GetInstallationLimit()) {
		int i;
		for (i = 0; i < ccs.numInstallationTemplates; i++) {
			const installationTemplate_t *tpl = &ccs.installationTemplates[i];
			if (tpl->tech == NULL || RS_IsResearched_ptr(tpl->tech)) {
				LIST_AddString(&list, va(_("%s\t%i\t%i c"), tpl->name, tpl->buildTime, tpl->cost));
			}
		}
	}

	if (B_GetCount() < MAX_BASES)
		LIST_AddString(&list, va(_("Base\t-\t%i c"), ccs.curCampaign->basecost));

	if (LIST_IsEmpty(list))
		cgi->UI_PopWindow(false);
	else
		cgi->UI_RegisterLinkedListText(TEXT_LIST, list);
}

static void INS_Click_f (void)
{
	if (Cmd_Argc() < 2)
		return;

	int index = atoi(Cmd_Argv(1));

	for (int i = 0; i < ccs.numInstallationTemplates; i++) {
		const installationTemplate_t *tpl = &ccs.installationTemplates[i];
		if (tpl->tech == NULL || RS_IsResearched_ptr(tpl->tech)) {
			if (index-- == 0) {
				/* if player hit the "create base" button while creating base mode is enabled
				 * that means that player wants to quit this mode */
				if (ccs.mapAction == MA_NEWINSTALLATION || INS_GetCount() >= B_GetInstallationLimit()) {
					MAP_ResetAction();
					return;
				} else {
					ccs.mapAction = MA_NEWINSTALLATION;

					/* show radar overlay (if not already displayed) */
					if (tpl->type == INSTALLATION_RADAR && !MAP_IsRadarOverlayActivated())
						MAP_SetOverlay("radar");

					INS_SetInstallationTitle(tpl->type);
					Cvar_Set("mn_installation_type", tpl->id);
				}
				/* select the installation type */
				return;
			}
		}
	}
	if (index == 0) {
		/* base is the last */
		B_SelectBase(NULL);
	}
}

void INS_InitCallbacks (void)
{
	Cmd_AddCommand("mn_installation_select", INS_SelectInstallation_f, "Parameter is the installation index. -1 will build a new one.");
	Cmd_AddCommand("mn_installation_build", INS_BuildInstallation_f);
	Cmd_AddCommand("mn_installation_changename", INS_ChangeInstallationName_f, "Called after editing the cvar installation name");
	Cmd_AddCommand("mn_installation_destroy", INS_DestroyInstallation_f, "Destroys an installation");
	Cmd_AddCommand("mn_installation_update_max_count", INS_UpdateInstallationLimit_f, "Updates the installation count limit");
	Cmd_AddCommand("mn_installation_init", INS_Init_f, "Initializes the installation menu");
	Cmd_AddCommand("mn_installation_click", INS_Click_f, "Select the installation type to build");

	Cmd_AddCommand("ui_fillufoyards", INS_FillUFOYardData_f, "Fills UFOYard UI with data");

	Cvar_SetValue("mn_installation_count", INS_GetCount());
	Cvar_Set("mn_installation_title", "");
	Cvar_Set("mn_installation_type", "");
	Cvar_Set("mn_installation_max", "");
}

void INS_ShutdownCallbacks (void)
{
	Cmd_RemoveCommand("ui_fillufoyards");

	Cmd_RemoveCommand("mn_installation_select");
	Cmd_RemoveCommand("mn_installation_build");
	Cmd_RemoveCommand("mn_installation_changename");
	Cmd_RemoveCommand("mn_installation_destroy");
	Cmd_RemoveCommand("mn_installation_update_max_count");
	Cvar_Delete("mn_installation_count");
	Cvar_Delete("mn_installation_title");
	Cvar_Delete("mn_installation_max");
	Cvar_Delete("mn_installation_type");
}
