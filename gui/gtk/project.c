#include <gtk/gtk.h>

typedef struct {
	GtkTextBuffer *result_buffer;
	GtkTextBuffer *log_buffer;
} Buffers;

static void cpu_button_clicked(GtkButton *button, gpointer user_data) {
	Buffers *buffers = (Buffers *)user_data;

	const char *result_output = "Result: Hello World\n";
	const char *log_output = "Log: Button clicked\n";

	gtk_text_buffer_set_text(buffers->result_buffer, result_output, -1);
	gtk_text_buffer_set_text(buffers->log_buffer, log_output, -1);
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
	gtk_widget_set_margin_top(cpu_button, 35);

	GtkWidget *cpu_button = gtk_button_new_with_label("CPU");
	gtk_widget_set_margin_top(gpio_button, 5);

	GtkWidget *i2c_button = gtk_button_new_with_label("GPIO");
	gtk_widget_set_margin_top(i2c_button, 5);

	GtkWidget *mobile_button = gtk_button_new_with_label("I2C/SPI");
	gtk_widget_set_margin_top(mobile_button, 5);

	GtkWidget *mobile_button = gtk_button_new_with_label("Mobile");
        gtk_widget_set_margin_top(mobile_button, 5);

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

	GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_append(GTK_BOX(main_box), sidebar);
	gtk_box_append(GTK_BOX(main_box), notebook);

	gtk_window_set_child(GTK_WINDOW(window), main_box);

	Buffers *buffers = g_new(Buffers, 1);
	buffers->result_buffer = result_buffer;
	buffers->log_buffer = log_buffer;

	g_signal_connect(cpu_button, "clicked", G_CALLBACK(cpu_button_clicked), buffers);
	gtk_widget_show(window);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;

	app = gtk_application_new("kr.mnm.autoprogram", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}

