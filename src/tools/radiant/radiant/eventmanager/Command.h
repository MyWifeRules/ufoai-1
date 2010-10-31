#ifndef EMCOMMAND_H_
#define EMCOMMAND_H_

#include "ieventmanager.h"
#include "generic/callback.h"

#include "gtk/gtkmenuitem.h"
#include "gtk/gtktoolbutton.h"
#include "gdk/gdk.h"

#include "Event.h"

/* greebo: A Command is an object that contains a single callback.
 *
 * Trigger the command via the execute() method (usually done by the associated accelerator).
 *
 * Connect the command to a GtkToolButton or a GtkMenuItem via the connectWidget method.
 */
class Command :
	public Event
{
	// The callback to be performed on execute()
	Callback _callback;

public:
	Command(const Callback& callback);

	// Invoke the registered callback
	void execute();

	// Override the derived keyDown method
	void keyDown();

	// Connect the given menuitem/toolbutton to this Command
	void connectWidget(GtkWidget* widget);

private:

	// The static GTK callback methods that can be connected to a ToolButton or a MenuItem
	static gboolean onToolButtonPress(GtkToolButton* toolButton, gpointer data);
	static gboolean onMenuItemClicked(GtkMenuItem* menuitem, gpointer data);

}; // class Command

#endif /*EMCOMMAND_H_*/
