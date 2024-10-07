#include <gtk/gtk.h>

// 버튼 클릭 시 호출되는 콜백 함수
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button clicked!\n");
}

// 활성화 콜백 함수: 창과 버튼을 생성하는 역할
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;

    // 새로운 윈도우 생성
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK4 Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    // 버튼 생성 및 콜백 연결
    button = gtk_button_new_with_label("Click me");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // 윈도우에 버튼 추가
    gtk_window_set_child(GTK_WINDOW(window), button);

    // 창 보여주기
    gtk_widget_show(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // 새로운 GTK 애플리케이션 생성
    app = gtk_application_new("com.example.GTK4Example", G_APPLICATION_DEFAULT_FLAGS);
    
    // 'activate' 신호에 콜백 함수 연결
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // 애플리케이션 실행
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // 애플리케이션 메모리 해제
    g_object_unref(app);

    return status;
}

