#include "gtk.h"
#include "buttons.h"
#include "config.h"
#include "helpers.h"
#include "structdef.h"

void on_activate(GtkApplication *app) {
    
    const int width = 1280;
    const int height = 720;

    // GTK WINDOW
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    gtk_window_present(GTK_WINDOW(window));
    gtk_window_set_decorated(GTK_WINDOW(window), true);

    g_object_set(gtk_settings_get_default(),
            "gtk-application-prefer-dark-theme",
            TRUE,
            NULL);

    //DEFINE WIDGETS
    GtkWidget *window_box, *image_box, *top_row_box, *labels_box,
            *bottom_row_box, *favorite_box;
    GtkWidget *left_button, *right_button, *select_button, *random_button;

    GtkWidget *favorite_button = gtk_button_new();
    GtkWidget *image_num_label = gtk_label_new(NULL);
    GtkWidget *current_wp_label = gtk_label_new(NULL);
    GtkWidget *image = gtk_picture_new();
    GtkWidget *auto_update_checkbutton =
            gtk_check_button_new_with_label("Auto apply wallpaper?");
    GtkWidget *paths_drop_down;
    GtkWidget *label_horizontal_scroll = gtk_scrolled_window_new();

    // jank shit because for some reason i cant just use the array from the tuple
    //
    if (check_folder_exists(WALLPAPERS_PATH) != 0) {
        char *notify_title = "Wallpapers folder does not exist";
        send_notification(notify_title, WALLPAPERS_PATH);
        exit(1);
    }
    struct collection_tuple subdir_tuple = get_subdirs(WALLPAPERS_PATH);
    if (subdir_tuple.collection_array == NULL) {
        const char *strings[2];
        strings[0] = WALLPAPERS_PATH;
        strings[1] = NULL;
        paths_drop_down = gtk_drop_down_new_from_strings(strings);
    }
    else {
        const char *strings[subdir_tuple.collection_size + 1];
        strings[0] = WALLPAPERS_PATH;
        for (int i = 1; i <= subdir_tuple.collection_size + 1; i++) {
            strings[i] = subdir_tuple.collection_array[i - 1];
        }
        strings[subdir_tuple.collection_size + 1] = NULL;
        paths_drop_down = gtk_drop_down_new_from_strings(strings);
    }

    // setup the list of wallpapers and the callback struct
    struct collection_tuple tuple = get_wallpapers(WALLPAPERS_PATH);
    if (tuple.collection_array == NULL) {
        char *notify_title = "Folder must contain atleast 1 wallpaper";
        send_notification(notify_title, NULL);
        exit(1);
    }

    static struct wallpaper_callback callback;
    callback.window = window;
    callback.image = image;
    callback.label = image_num_label;
    callback.current_wp_label = current_wp_label;
    callback.favorite_button = favorite_button;
    callback.wallpapers_dir = malloc(512 + 1);
    callback.wallpapers_size = tuple.collection_size;
    callback.wallpapers_index = 0;
    callback.wallpapers = tuple.collection_array;
    callback.check_button = auto_update_checkbutton;
    callback.drop_down = GTK_DROP_DOWN(paths_drop_down);
    callback.subdirs = subdir_tuple.collection_array;
    callback.subdirs_size = subdir_tuple.collection_size;
    strcpy(callback.wallpapers_dir, WALLPAPERS_PATH);
    struct wallpaper_callback *callback_ptr = &callback;

    //DECLARE BOXES
    window_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    image_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    top_row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    labels_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    bottom_row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    favorite_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    //DECLARE LABELS
    char *temp_text = calculate_nthofnth(callback_ptr);

    update_label(GTK_LABEL(image_num_label), temp_text);
    free(temp_text);

    update_label(GTK_LABEL(current_wp_label),
            callback.wallpapers[callback.wallpapers_index]);

    //DELCLARE BUTTONS
    left_button = gtk_button_new_with_label("<-");
    right_button = gtk_button_new_with_label("->");
    select_button = gtk_button_new_with_label("Select");
    random_button = gtk_button_new_with_label("Random");

    update_button(callback_ptr);
    gtk_widget_add_css_class(select_button, "suggested-action");

    // GSIGNALS
    g_signal_connect(paths_drop_down, "notify::selected",
            G_CALLBACK(change_wallpaper_path), (gpointer)callback_ptr);

    g_signal_connect(right_button, "clicked", G_CALLBACK(button_right),
            (gpointer)callback_ptr);
    g_signal_connect(left_button, "clicked", G_CALLBACK(button_left),
            (gpointer)callback_ptr);
    g_signal_connect(select_button, "clicked", G_CALLBACK(select_wallpaper),
            (gpointer)callback_ptr);
    g_signal_connect(GTK_BUTTON(callback_ptr->favorite_button), "clicked", G_CALLBACK(button_fav),
            (gpointer)callback_ptr);
    g_signal_connect(GTK_BUTTON(random_button), "clicked", G_CALLBACK(button_random),
            (gpointer)callback_ptr);

    //EXPAND
    gtk_widget_set_hexpand(left_button, true);
    gtk_widget_set_hexpand(right_button, true);

    gtk_widget_set_hexpand(image_box, true);
    gtk_widget_set_vexpand(image_box, true);

    gtk_widget_set_hexpand(labels_box, true);
    gtk_widget_set_vexpand(labels_box, false);

    gtk_widget_set_hexpand(bottom_row_box, true);
    gtk_widget_set_hexpand(select_button, true);
    gtk_widget_set_hexpand(current_wp_label, true);

    //ALIGN 
    gtk_widget_set_halign(image_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(image_box, GTK_ALIGN_CENTER);

    //SPACING AND MARGIN
    gtk_box_set_spacing(GTK_BOX(labels_box), 14);
    gtk_box_set_spacing(GTK_BOX(bottom_row_box), 14);

    /* gtk_widget_set_size_request(select_button, width / 2, -1); */
    gtk_widget_set_size_request(label_horizontal_scroll, width / 6, -1);

    gtk_box_set_spacing(GTK_BOX(top_row_box), 5);
    gtk_box_set_spacing(GTK_BOX(window_box), 10);

    gtk_widget_set_margin_top(top_row_box, 10);
    gtk_widget_set_margin_start(top_row_box, 10);
    gtk_widget_set_margin_end(top_row_box, 10);
    gtk_widget_set_margin_bottom(top_row_box, 5);

    gtk_widget_set_margin_start(image_box, 10);
    gtk_widget_set_margin_end(image_box, 10);

    gtk_widget_set_margin_top(bottom_row_box, 10);
    gtk_widget_set_margin_start(bottom_row_box, 10);
    gtk_widget_set_margin_end(bottom_row_box, 10);
    gtk_widget_set_margin_bottom(bottom_row_box, 5);
    
    //OTHER FLAGS FOR WIDDGETS
    gtk_drop_down_set_enable_search(GTK_DROP_DOWN(paths_drop_down), true);
    gtk_drop_down_set_search_match_mode(GTK_DROP_DOWN(paths_drop_down),
            GTK_STRING_FILTER_MATCH_MODE_PREFIX);

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(label_horizontal_scroll),
            GTK_POLICY_ALWAYS, GTK_POLICY_NEVER);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(label_horizontal_scroll),
            callback_ptr->current_wp_label);
   
    //sets the default image as the first wallpaper that was read into the array
    char *temp_buffer;
    temp_buffer = malloc(strlen(callback.wallpapers[0]) +
            strlen(callback.wallpapers_dir) + 2);

    strcpy(temp_buffer, callback.wallpapers_dir);
    strcat(temp_buffer, callback.wallpapers[0]);
    gtk_picture_set_filename(GTK_PICTURE(image), temp_buffer);
    free(temp_buffer);
    gtk_picture_set_content_fit(GTK_PICTURE(image), GTK_CONTENT_FIT_COVER);


    //BOX APPEND
    gtk_box_append(GTK_BOX(image_box), image);

    gtk_box_append(GTK_BOX(labels_box), callback_ptr->label);
    gtk_box_append(GTK_BOX(labels_box), label_horizontal_scroll);
    
    gtk_box_append(GTK_BOX(top_row_box), paths_drop_down);
    gtk_box_append(GTK_BOX(top_row_box), auto_update_checkbutton);
    gtk_box_append(GTK_BOX(top_row_box), left_button);
    gtk_box_append(GTK_BOX(top_row_box), right_button);

    gtk_box_append(GTK_BOX(bottom_row_box), select_button);
    gtk_box_append(GTK_BOX(bottom_row_box), labels_box);
    gtk_box_append(GTK_BOX(bottom_row_box), favorite_box);
    gtk_box_append(GTK_BOX(favorite_box), callback_ptr->favorite_button);
    gtk_box_append(GTK_BOX(bottom_row_box), random_button);

    gtk_box_append(GTK_BOX(window_box), top_row_box);
    gtk_box_append(GTK_BOX(window_box), image_box);
    gtk_box_append(GTK_BOX(window_box), bottom_row_box);


    //FINAL
    gtk_window_set_child(GTK_WINDOW(window), window_box);
    gtk_window_present(GTK_WINDOW(window));
}
