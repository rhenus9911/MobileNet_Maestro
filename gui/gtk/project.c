#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "test.h"

typedef struct {
	GtkTextBuffer *result_buffer;
	GtkTextBuffer *log_buffer;
	GtkTextBuffer *console_buffer;
} Buffers;

static void cpu_button_clicked(GtkButton *button, gpointer user_data) {
	Buffers *buffers = (Buffers *)user_data;

	char *output = print_sample(1);

	char *result_output = strstr(output, "Result");
	if(result_output != NULL) {
		gtk_text_buffer_set_text(buffers->result_buffer, result_output, -1);
		*result_output = '\0';
		gtk_text_buffer_set_text(buffers->log_buffer, output, -1);
	}

	//const char *result_output = "Result: Hello World\n";
	//const char *log_output = "Log: Button clicked\n";
	const char *console_output = "Console: CPU button clicked\n";
	
	GtkTextIter end;
	gtk_text_buffer_get_end_iter(buffers->console_buffer, &end);

	GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffers->console_buffer);
	GtkTextTag *green_tag = gtk_text_tag_table_lookup(tag_table, "green_fg");
	if(green_tag == NULL) {
		green_tag = gtk_text_tag_new("green_fg");
		g_object_set(green_tag, "foreground", "green", NULL);
		gtk_text_tag_table_add(tag_table, green_tag);
	}
	gtk_text_buffer_insert_with_tags_by_name(buffers->console_buffer, &end, console_output, -1, "green_fg", NULL);

	free(output);
}

static void gpio_button_clicked(GtkButton *button, gpointer user_data) {
	Buffers *buffers = (Buffers *)user_data;
	char *output = print_sample(2);
	char *result_output = strstr(output, "Result");
	if(result_output != NULL) {
		gtk_text_buffer_set_text(buffers->result_buffer, result_output, -1);
		*result_output = '\0';
		gtk_text_buffer_set_text(buffers->log_buffer, output, -1);
	}

	const char *console_output = "Console: GPIO button clicked\n";

	GtkTextIter end;
	gtk_text_buffer_get_end_iter(buffers->console_buffer, &end);
	gtk_text_buffer_insert(buffers->console_buffer, &end, console_output, -1);

	free(output);
}

static void activate(GtkApplication *app, gpointer user_data) {
	GtkWidget *window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Program");
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

	/*
	 * sidebar
	 */
	
	GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *all_button = gtk_button_new_with_label("All");
	gtk_widget_set_margin_top(all_button, 35);

	GtkWidget *cpu_button = gtk_button_new_with_label("CPU");
	gtk_widget_set_margin_top(cpu_button, 5);

	GtkWidget *gpio_button = gtk_button_new_with_label("GPIO");
	gtk_widget_set_margin_top(gpio_button, 5);

	GtkWidget *i2c_button = gtk_button_new_with_label("I2C/SPI");
	gtk_widget_set_margin_top(i2c_button, 5);

	GtkWidget *mobile_button = gtk_button_new_with_label("Mobile");
        gtk_widget_set_margin_top(mobile_button, 5);

	gtk_box_append(GTK_BOX(sidebar), all_button);
	gtk_box_append(GTK_BOX(sidebar), cpu_button);
	gtk_box_append(GTK_BOX(sidebar), gpio_button);
	gtk_box_append(GTK_BOX(sidebar), i2c_button);
	gtk_box_append(GTK_BOX(sidebar), mobile_button);

	GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_box_append(GTK_BOX(sidebar), separator);

	/*
	 * Notebook
	 */

	GtkWidget *notebook = gtk_notebook_new();
	
	/*
	 * Result tab
	 */

	GtkWidget *result_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *text_view = gtk_text_view_new();
	GtkTextBuffer *result_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	gtk_box_append(GTK_BOX(result_content), text_view);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), result_content, gtk_label_new("Result"));
	
	/*
	 * Log tab
	 */

	GtkWidget *log_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *log_view = gtk_text_view_new();
	GtkTextBuffer *log_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_view));
	gtk_box_append(GTK_BOX(log_content), log_view);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), log_content, gtk_label_new("Log"));

	gtk_widget_set_hexpand(notebook, TRUE);
	gtk_widget_set_vexpand(notebook, TRUE);

	GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_append(GTK_BOX(content_area), sidebar);
	gtk_box_append(GTK_BOX(content_area), notebook);

	/*
	 * console
	 */

	GtkWidget *console_view = gtk_text_view_new();
	GtkTextBuffer *console_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(console_view));

	gtk_text_view_set_editable(GTK_TEXT_VIEW(console_view), FALSE);
	GtkWidget *console_scroll = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(console_scroll), console_view);
	gtk_widget_set_size_request(console_scroll, -1, 150);

	gtk_box_append(GTK_BOX(main_box), content_area);
	gtk_box_append(GTK_BOX(main_box), console_scroll);

	gtk_window_set_child(GTK_WINDOW(window), main_box);

	Buffers *buffers = g_new(Buffers, 1);
	buffers->result_buffer = result_buffer;
	buffers->log_buffer = log_buffer;
	buffers->console_buffer = console_buffer;

	g_signal_connect(cpu_button, "clicked", G_CALLBACK(cpu_button_clicked), buffers);
	g_signal_connect(gpio_button, "clicked", G_CALLBACK(gpio_button_clicked), buffers);
	gtk_widget_show(window);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;

	app = gtk_application_new("kr.mnm.autoprogram", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}

