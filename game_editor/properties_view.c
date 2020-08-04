/*
Copyright 2020 Anthony Cohn-Richardby

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtk/gtk.h>

#include "main_window.h"
#include "properties_view.h"


struct _EditorPropertiesView
{
	GtkBox parent;
	
	GtkWidget *ground_props;
	GtkWidget *creature_props;
	GtkWidget *cell_props;
	GtkWidget *entity_props;
	GtkStack *stack;
};

G_DEFINE_TYPE(EditorPropertiesView, editor_properties_view, GTK_TYPE_BOX);

static void
editor_properties_view_init(EditorPropertiesView *view)
{
	gtk_widget_init_template(GTK_WIDGET(view));
}

static void
editor_properties_view_class_init(EditorPropertiesViewClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/rocks/colourful/textgame/properties_view.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView, ground_props);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView, creature_props);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView, cell_props);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView, entity_props);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView, stack);
}

EditorPropertiesView *
editor_properties_view_new()
{
	return g_object_new(EDITOR_PROPERTIES_VIEW_TYPE, NULL);
}

void
editor_properties_view_set_item(EditorPropertiesView *view, enum active_item_type type, void *item)
{
	switch (type) {
		case ACTIVE_ITEM_GROUND:
			gtk_stack_set_visible_child(view->stack, view->ground_props);
			break;
		case ACTIVE_ITEM_CELL:
			gtk_stack_set_visible_child(view->stack, view->cell_props);
			break;
		case ACTIVE_ITEM_ENTITY:
			gtk_stack_set_visible_child(view->stack, view->entity_props);
			break;
		case ACTIVE_ITEM_CREATURE:
			gtk_stack_set_visible_child(view->stack, view->creature_props);
			break;
	}
}
