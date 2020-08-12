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

#define MAX_NUM_LEN 10
#define MAX_CELL_POS_LEN 20

#include <gtk/gtk.h>

#include "main_window.h"
#include "properties_view.h"
#include "game_data.h"


struct _EditorPropertiesView
{
	GtkBox parent;
	
	gulong apply_handler;
	GtkWidget *ground_props;
	GtkWidget *creature_props;
	GtkWidget *cell_props;
	GtkWidget *entity_props;
	GtkStack *stack;
	GtkButton *apply_button;
	GtkEntry *ground_name_entry;
	GtkEntry *ground_icon_entry;
	GtkEntry *entity_name_entry;
	GtkEntry *entity_desc_entry;
	GtkEntry *entity_icon_entry;
	GtkEntry *entity_weight_entry;
	GtkEntry *entity_size_class_entry;
	GtkEntry *creature_name_entry;
	GtkEntry *creature_desc_entry;
	GtkEntry *creature_health_entry;
	GtkEntry *creature_tp_entry;
	GtkEntry *creature_inventory_size_entry;
	GtkLabel *cell_position_label;
	GtkComboBox *cell_ground_combo;
	GtkComboBox *cell_creature_combo;
	GtkComboBox *cell_entity_combo;
	
};

G_DEFINE_TYPE(EditorPropertiesView, editor_properties_view, GTK_TYPE_BOX);

static void
fill_ground_fields(EditorPropertiesView *view, struct ground *ground)
{
	gtk_entry_set_text(view->ground_name_entry, ground->name);
	char icon[2] = { ground->icon, '\0' };
	gtk_entry_set_text(view->ground_icon_entry, icon); 
}

static void
fill_entity_fields(EditorPropertiesView *view, struct entity *entity)
{
	gtk_entry_set_text(view->entity_name_entry, entity->name);
	gtk_entry_set_text(view->entity_desc_entry, entity->desc);
	char icon[2] = { entity->icon, '\0' };
	gtk_entry_set_text(view->entity_icon_entry, icon);
	char weight[MAX_NUM_LEN];
	snprintf(weight, MAX_NUM_LEN, "%d", entity->weight);
	weight[MAX_NUM_LEN - 1] = '\0';
	gtk_entry_set_text(view->entity_weight_entry, weight);
	gtk_entry_set_text(view->entity_size_class_entry, size_type_str(entity->size_class));
}

static void
fill_creature_fields(EditorPropertiesView *view, struct creature *creature)
{
	gtk_entry_set_text(view->creature_name_entry, creature->name);
	gtk_entry_set_text(view->creature_desc_entry, creature->desc);
	char health[MAX_NUM_LEN];
	snprintf(health, MAX_NUM_LEN, "%d", creature->health);
	health[MAX_NUM_LEN - 1] = '\0';
	gtk_entry_set_text(view->creature_health_entry, health);
	char tp[MAX_NUM_LEN];
	snprintf(tp, MAX_NUM_LEN, "%d", creature->tp);
	tp[MAX_NUM_LEN - 1] = '\0';
	gtk_entry_set_text(view->creature_tp_entry, tp);
	char invsz[MAX_NUM_LEN];
	snprintf(invsz, MAX_NUM_LEN, "%d", creature->inventory_size);
	invsz[MAX_NUM_LEN - 1] = '\0';
	gtk_entry_set_text(view->creature_inventory_size_entry, invsz);
}

static int
find_matching_ptr(GtkTreeModel *model, void *needle)
{
	GtkTreeIter iter;
	void *value;
	int count = 0;
	int index = -1;
	if (gtk_tree_model_get_iter_first(model, &iter)) {
		do {
			gtk_tree_model_get(model, &iter, 1, &value, -1);
			if (needle == value) {
				index = count;
				break;
			}
			count++;
		} while (gtk_tree_model_iter_next(model, &iter));
	}
	return index;
}

static void
fill_cell_fields(EditorPropertiesView *view, struct cell *cell)
{
	char position[MAX_CELL_POS_LEN];
	snprintf(position, MAX_CELL_POS_LEN, "(%d, %d)", cell->x, cell->y);
	position[MAX_CELL_POS_LEN - 1] = '\0';
	gtk_label_set_text(view->cell_position_label, position);
	gtk_combo_box_set_model(view->cell_ground_combo, editor_app_get_ground_tree_model(current_app));
	gtk_combo_box_set_model(view->cell_entity_combo, editor_app_get_entity_tree_model(current_app));
	gtk_combo_box_set_model(view->cell_creature_combo, editor_app_get_creature_tree_model(current_app));
	
	int index = find_matching_ptr(editor_app_get_ground_tree_model(current_app), cell->ground);
	gtk_combo_box_set_active(view->cell_ground_combo, index);
	index = find_matching_ptr(editor_app_get_creature_tree_model(current_app), cell->creature);
	gtk_combo_box_set_active(view->cell_creature_combo, index);
	// TODO: - make into a list...
	index = find_matching_ptr(editor_app_get_entity_tree_model(current_app), cell->entities);
	gtk_combo_box_set_active(view->cell_entity_combo, index);
}

static void
apply_ground_changes(GtkButton *button, void **userdata)
{
	g_debug("apply ground changes");
	EditorPropertiesView *view = (EditorPropertiesView*)userdata[0];
	struct ground *ground = (struct ground*)userdata[1];
	int status = editor_app_update_ground(current_app, ground,
		gtk_entry_get_text(view->ground_name_entry),
		gtk_entry_get_text(view->ground_icon_entry)[0]);
	if (!status) {
		g_debug("invalid ground fields");
	} else {
		g_debug("updated ground");
	}
	fill_ground_fields(view, ground);
}

static void
apply_entity_changes(GtkButton *button, void **userdata)
{
	g_debug("apply entity changes");
	EditorPropertiesView *view = (EditorPropertiesView*)userdata[0];
	struct entity *entity = (struct entity*)userdata[1];
	
	int weight = (int)strtol(gtk_entry_get_text(view->entity_weight_entry),
		NULL, 10);
	const char *sizestr = gtk_entry_get_text(view->entity_size_class_entry);
	enum size_type size = entity_size(sizestr);
	int status = editor_app_update_entity(current_app, entity,
		gtk_entry_get_text(view->entity_name_entry),
		gtk_entry_get_text(view->entity_desc_entry),
		gtk_entry_get_text(view->entity_icon_entry)[0],
		weight,
		size);
	if (!status) {
		g_debug("changes rejected");
	} else {
		g_debug("updated entity");
	}
	fill_entity_fields(view, entity);	
}

static void
apply_creature_changes(GtkButton *button, void **userdata)
{
	g_debug("apply creature changes");
	EditorPropertiesView *view = (EditorPropertiesView*)userdata[0];
	struct creature *creature = (struct creature*)userdata[1];
	
	int health = (int)strtol(gtk_entry_get_text(view->creature_health_entry),
		NULL, 10);

	int tp = (int)strtol(gtk_entry_get_text(view->creature_tp_entry),
		NULL, 10);
	int invsz = (int)strtol(
		gtk_entry_get_text(view->creature_inventory_size_entry), 
		NULL, 10); 
	int status = editor_app_update_creature(current_app, creature,
		gtk_entry_get_text(view->creature_name_entry),
		gtk_entry_get_text(view->creature_desc_entry),
		health, tp, invsz);
	if (!status) {
		g_debug("rejected changes");
	} else {
		g_debug("updated creature");
	}
	fill_creature_fields(view, creature);
}

static void
apply_cell_changes(GtkButton *button, void *userdata)
{
	g_debug("apply cell changes");
}


static void
editor_properties_view_init(EditorPropertiesView *view)
{
	gtk_widget_init_template(GTK_WIDGET(view));
	view->apply_handler = 0;
}

static void
editor_properties_view_class_init(EditorPropertiesViewClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/rocks/colourful/textgame/properties_view.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		ground_props);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		creature_props);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,	
		cell_props);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		entity_props);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		stack);
	
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		ground_name_entry);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		ground_icon_entry);
	
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		entity_name_entry);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		entity_desc_entry);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		entity_icon_entry);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		entity_weight_entry);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		entity_size_class_entry);
	
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		creature_name_entry);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		creature_desc_entry);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		creature_health_entry);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		creature_tp_entry);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		creature_inventory_size_entry);
	
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		cell_creature_combo);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		cell_ground_combo);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		cell_entity_combo);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		cell_position_label);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), EditorPropertiesView,
		apply_button);
}

EditorPropertiesView *
editor_properties_view_new()
{
	return g_object_new(EDITOR_PROPERTIES_VIEW_TYPE, NULL);
}

void
editor_properties_view_set_item(EditorPropertiesView *view, enum active_item_type type, void *item)
{
	if (view->apply_handler > 0) {
		g_signal_handler_disconnect(view->apply_button, view->apply_handler);
		view->apply_handler = 0;
	}
	void **userdata = malloc(2 * sizeof(void*));
	userdata[0] = view;
	userdata[1] = item;
	GCallback cb = NULL;
	switch (type) {
		case ACTIVE_ITEM_GROUND:
			cb = G_CALLBACK(apply_ground_changes);
			fill_ground_fields(view, (struct ground*)item);
			gtk_stack_set_visible_child(view->stack, view->ground_props);
			break;
		case ACTIVE_ITEM_CELL:
			cb = G_CALLBACK(apply_cell_changes);
			fill_cell_fields(view, (struct cell*)item);
			gtk_stack_set_visible_child(view->stack, view->cell_props);
			break;
		case ACTIVE_ITEM_ENTITY:
			cb = G_CALLBACK(apply_entity_changes);
			fill_entity_fields(view, (struct entity*)item);
			gtk_stack_set_visible_child(view->stack, view->entity_props);
			break;
		case ACTIVE_ITEM_CREATURE:
			cb = G_CALLBACK(apply_creature_changes);
			fill_creature_fields(view, (struct creature*)item);
			gtk_stack_set_visible_child(view->stack, view->creature_props);
			break;
	}
	view->apply_handler = g_signal_connect_data(G_OBJECT(view->apply_button), "clicked", 
				cb, userdata, (GClosureNotify)free, 0);
}
