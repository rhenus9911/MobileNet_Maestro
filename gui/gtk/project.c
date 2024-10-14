#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "test.h"

typedef struct {
	GtkTextBuffer *result_buffer;
	GtkTextBuffer *log_buffer;
	GtkTextBuffer *console_buffer;
} Buffers;

static void add_message_to_console(GtkTextBuffer *console_buffer, const char *message, const char *tag_name) {
	GtkTextIter end;
	gtk_text_buffer_get_end_iter(console_buffer, &end);
	
	GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(console_buffer);
	GtkTextTag *tag = gtk_text_tag_table_lookup(tag_table, tag_name);
	if (tag == NULL) {
		tag = gtk_text_tag_new(tag_name);
		const char *color = strcmp(tag_name, "red") == 0 ? "red" : "green";
		g_object_set(tag, "foreground", tag_name, NULL);
		gtk_text_tag_table_add(tag_table, tag);
	}
	
	gtk_text_buffer_insert_with_tags_by_name(console_buffer, &end, message, -1, tag_name, NULL);
	gtk_text_buffer_insert(console_buffer, &end, "\n", -1);
}

static void print_message(Buffers *buffers, LogEntry result) {
	char *msg_cp = strdup(result.message);
	char *token = strtok(msg_cp, "\n");

	GtkTextIter end;
	
	while (token != NULL) {
		if (strncmp(token, "[ERROR]", 7) == 0) {
			add_message_to_console(buffers->console_buffer, token, "red");
		}
		else if (strncmp(token, "[SUCCESS]", 9) == 0) {
			add_message_to_console(buffers->console_buffer, token, "green");
		}
		else if (strncmp(token, "[LOG]", 5) == 0) {
			gtk_text_buffer_get_end_iter(buffers->log_buffer, &end);
			gtk_text_buffer_insert(buffers->log_buffer, &end, token, -1);
			gtk_text_buffer_insert(buffers->log_buffer, &end, "\n", -1);
			
		}
		token = strtok(NULL, "\n");
	}
	free(msg_cp);
}

static void cpu_button_clicked(GtkButton *button, gpointer user_data) {
	Buffers *buffers = (Buffers *)user_data;
	
	// CPU
	LogEntry result = cpuNumCheck();
	if(result.message != NULL) {
		print_message(buffers, result);
	}
	if(result.level == LOG_SUCCESS) {
		result = cpuPerformCheck();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}
	if(result.level == LOG_SUCCESS) {
		result = cpuIPSCheck();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}
	if(result.level == LOG_SUCCESS) {
		result = cpuFPCheck();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}

	// Memory
	if(result.level == LOG_SUCCESS) {
		result = memoryFuncCheck();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}

	if(result.level == LOG_SUCCESS) {
		result = memoryBandWidthCheck();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}

	if(result.level == LOG_SUCCESS) {
		result = memoryErrorCheck();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}

	else {
		print_message(buffers, result);
	}
}

static void gpio_button_clicked(GtkButton *button, gpointer user_data)
{
	Buffers *buffers = (Buffers *)user_data;

	// GPIO
	LogEntry result = GpioTest();
	if(result.message != NULL) {
		print_message(buffers, result);
	}
	if(result.level == LOG_SUCCESS) {
		result = PWMTest();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}
	else {
		print_message(buffers, result);
	}
}

static void i2c_button_clicked(GtkButton *button, gpointer user_data)
{
	Buffers *buffers = (Buffers *)user_data;

	// SPI
	LogEntry result = SPITest();
	if(result.message != NULL) {
		print_message(buffers, result);
	}

	// I2C
	if(result.level == LOG_SUCCESS) {
		result = i2cTest();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}

	else {
		print_message(buffers, result);
	}
}

static void mobile_button_clicked(GtkButton *button, gpointer user_data)
{
	Buffers *buffers = (Buffers *)user_data;
	
	// Wi-Fi
	LogEntry result = wifiTest();
	if(result.message != NULL) {
		print_message(buffers, result);
	}

	if(result.level = LOG_SUCCESS) {
		result = ethernetTest();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}

	if(result.level = LOG_SUCCESS) {
		result = bluetoothTest();
		if(result.message != NULL) {
			print_message(buffers, result);
		}
	}
	
	else {
		print_message(buffers, result);
	}
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
	g_signal_connect(i2c_button, "clicked", G_CALLBACK(i2c_button_clicked), buffers);
	g_signal_connect(mobile_button, "clicked", G_CALLBACK(mobile_button_clicked), buffers);
	gtk_widget_show(window);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;
	
	// Raspberry Pi
	app = gtk_application_new("kr.mnm.autoprogram", G_APPLICATION_DEFAULT_FLAGS);
	
	// Ubuntu
	//app = gtk_application_new("kr.mnm.autoprogram", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}

