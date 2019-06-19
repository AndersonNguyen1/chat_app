#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GUI.h"
#include <string.h>

GtkWidget *text_view;
GtkTextBuffer *buffer;


GdkPixbuf *create_pixbuf(const gchar * filename){
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);

	if (!pixbuf){

		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}

	return pixbuf;
}

/*This is where it needs to implemented*/
void get_user(GtkWidget *widget, GtkWidget *entry){
	gchar* temp;
	temp = (char*)gtk_entry_get_text(GTK_ENTRY(entry));
	strncpy(gui_get_username, temp, sizeof(gui_get_username - 1));
	gui_get_username[sizeof(gui_get_username)-1] = '\0';
	g_print("Username - %s\n", gui_get_username);
}

void get_pass(GtkWidget *widget, GtkWidget *entry){
	gchar* temp;
	temp = (char*)gtk_entry_get_text(GTK_ENTRY(entry));
	strncpy(gui_get_password, temp, sizeof(gui_get_password - 1));
	gui_get_password[sizeof(gui_get_password)-1] = '\0';
	g_print("Password - %s\n", gui_get_password);
}
/*End*/

gboolean delete_event( GtkWidget *widget, GdkEvent  *event, gpointer data ){
	g_print ("delete event occurred\n");
	return FALSE;
}

void destroy( GtkWidget *widget, gpointer data ){
	gtk_main_quit ();
}

static void send_clicked( GtkWidget *widget, GtkWidget *entry){
	const gchar *entry_text;
	const gchar *temp;
	GtkTextIter iter;
	entry_text = gtk_entry_get_text(GTK_ENTRY (entry));
	temp = gtk_entry_get_text(GTK_ENTRY (entry));
	gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	/*gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);*/
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, entry_text, -1, "rmarg", "blue_fg", NULL);
	if(strlen(entry_text)>0){
		gtk_text_buffer_insert(buffer, &iter, "\n", -1);
	}
	strncpy(gui_send_message, temp, sizeof(gui_send_message - 1));
	gui_send_message[sizeof(gui_send_message)-1] = '\0';
}

static void enter_callback( GtkWidget *widget, GtkWidget *entry){
  const gchar *entry_text;
  entry_text = gtk_entry_get_text(GTK_ENTRY (entry));
  printf ("Entry contents: %s\n", entry_text);
}

int ChatAppHome(int argc, char *argv[], char recieve_message[512]){
	static GtkWidget *window;
	GtkWidget *scrolled_window;
	GtkWidget *button;

	GtkTextIter iter;
	GtkWidget *entry;
	GdkPixbuf *icon;

	if(recieve_message != NULL){
		printf("%s\n", recieve_message);	
	}
	

	gtk_init (&argc, &argv);

	/* Create a new dialog window for the scrolled window to be * packed into.  */
	window = gtk_dialog_new();
	g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL);
	gtk_window_set_title(GTK_WINDOW (window), "GtkScrolledWindow example");
	gtk_container_set_border_width(GTK_CONTAINER (window), 0);
	gtk_widget_set_size_request(window, 600, 600);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);



	/* create a new scrolled window. */
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_set_border_width(GTK_CONTAINER (scrolled_window), 10);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	/* The dialog window is created with a vbox packed into it. */								
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), scrolled_window, TRUE, TRUE, 0);
	gtk_widget_show(scrolled_window);


	/* create a new text view window. */
	buffer = gtk_text_buffer_new(NULL);
	text_view = gtk_text_view_new_with_buffer(buffer);
	gtk_widget_set_size_request(text_view, 256,-1);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW(text_view), FALSE);
	gtk_text_buffer_create_tag(buffer, "gap","pixels_above_lines", 30, NULL);
	gtk_text_buffer_create_tag(buffer, "lmarg", "justification", GTK_JUSTIFY_LEFT, NULL);
	gtk_text_buffer_create_tag(buffer, "rmarg", "justification", GTK_JUSTIFY_RIGHT, NULL);
	gtk_text_buffer_create_tag(buffer, "blue_fg", "foreground", "blue", NULL); 
	gtk_text_buffer_create_tag(buffer, "black_fg", "foreground", "black", NULL); 


	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert(buffer, &iter, "Plain text\n", -1);
	gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, "This is where implementation of the other person's text will go!\n", -1, "lmarg", "black_fg",  NULL);
	gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, "This is when a person types!\n", -1, "rmarg", "blue_fg", NULL);
	gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, "This is the reply from the other user!\n", -1, "lmarg", "black_fg",  NULL);
	gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, "Please enter text in the entry below and see ur messages pop up!\n", -1, "rmarg", "blue_fg",  NULL);

	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR); 
	
	/* pack the text view into the scrolled window */
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), text_view);
	gtk_widget_show(text_view);

		/*make app icon - start*/
	icon = create_pixbuf("fireturtle.png");  
	gtk_window_set_icon(GTK_WINDOW(window), icon);
	/*make app icon - end*/

	/* create a new text entry window. */
	entry = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (entry),512);
	g_signal_connect (entry, "activate", G_CALLBACK (enter_callback), entry);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), entry, FALSE, FALSE, 0);


	/* Add a "send" button to the bottom of the dialog */
	button = gtk_button_new_with_label ("Send");
	g_signal_connect (button, "clicked", G_CALLBACK (send_clicked), entry);
	gtk_box_pack_start(GTK_BOX (GTK_DIALOG (window)->action_area), button, TRUE, TRUE, 5);
	
	gtk_widget_show(entry);
	gtk_widget_show (button);


	/* Add a "close" button to the bottom of the dialog */
	button = gtk_button_new_with_label ("Close");
	g_signal_connect_swapped(button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	/* this makes it so the button is the default. */

	gtk_widget_set_can_default(button, TRUE);
	gtk_box_pack_start(GTK_BOX (GTK_DIALOG (window)->action_area), button, TRUE, TRUE, 0);

	/* This grabs this button to be the default button. Simply hitting
 * 	* the "Enter" key will cause this button to activate. */
	gtk_widget_grab_default(button);
	gtk_widget_show (button);
	gtk_widget_show(window);

	gtk_main();

	return 0;
}

int LoginScreen(int argc, char *argv[]) {

	GtkWidget *window;
	GtkWidget *table;

	GtkWidget *label1;
	GtkWidget *label2;
	GtkWidget *MainText;
	GtkWidget *templabel;

	GtkWidget *entry1;
	GtkWidget *entry2;

	GtkWidget *sendbutton;
	GtkWidget *signup;

	GdkPixbuf *icon;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), " Login Screen ");
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
	g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL);

	/*make entry fields - start*/
	table = gtk_table_new(4, 2, FALSE);


	gtk_container_add(GTK_CONTAINER(window), table);

	label1 = gtk_label_new("Username");
	label2 = gtk_label_new("Password");
	templabel = gtk_label_new(NULL);


	MainText = gtk_label_new("Welcome! Please Log In!");
	gtk_table_attach(GTK_TABLE(table), MainText, 0, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 25);
	gtk_label_set_justify (GTK_LABEL(MainText),GTK_JUSTIFY_CENTER);
	gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 1, 2, GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 2, 3, GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

	entry1 = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (entry1),20);
	entry2 = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (entry2),20);

	gtk_table_attach(GTK_TABLE(table), entry1, 1, 2, 1, 2,GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 5);
	gtk_table_attach(GTK_TABLE(table), entry2, 1, 2, 2, 3,GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 5);
	gtk_entry_set_visibility(GTK_ENTRY(entry2), FALSE);
	/*make entry fields - end*/

	/*make send button - start*/
	sendbutton = gtk_button_new_with_label("   Sign In   ");
	g_signal_connect(sendbutton, "clicked", G_CALLBACK(get_user), entry1);
	g_signal_connect(sendbutton, "clicked", G_CALLBACK(get_pass), entry2);
	g_signal_connect_swapped(sendbutton, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	/*make send button - end*/

	/*make signup button - start*/
	signup = gtk_button_new_with_label("   New Account   ");
	g_signal_connect_swapped (signup, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	/*make signup button - end*/

	/*Attach buttons to table - start*/
	gtk_table_attach(GTK_TABLE(table), sendbutton, 0, 1, 3, 4, GTK_SHRINK, GTK_SHRINK, 10, 10);
	gtk_table_attach(GTK_TABLE(table), signup, 1, 2, 3, 4, GTK_SHRINK, GTK_SHRINK, 10, 10);
	gtk_button_set_alignment(GTK_BUTTON(sendbutton), 0.5, 1);
	gtk_button_set_alignment(GTK_BUTTON(signup), 0.5, 1);
	/*Attach buttons to table - end*/

	/*make app icon - start*/
	icon = create_pixbuf("fireturtle.png");  
	gtk_window_set_icon(GTK_WINDOW(window), icon);
	/*make app icon - end*/

	gtk_widget_show_all(window);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();
	return 0;
}

int RegisterScreen(int argc, char *argv[]){

	GtkWidget *window;
	GtkWidget *table;

	GtkWidget *label1;
	GtkWidget *label2;
	GtkWidget *MainText;

	GtkWidget *entry1;
	GtkWidget *entry2;

	GtkWidget *regbutton;

	GdkPixbuf *icon;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), " Register Screen ");
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	/*make entry fields - start*/
	table = gtk_table_new(4, 2, FALSE);


	gtk_container_add(GTK_CONTAINER(window), table);

	label1 = gtk_label_new("Username");
	label2 = gtk_label_new("Password");

	MainText = gtk_label_new("Register Here!");
	gtk_table_attach(GTK_TABLE(table), MainText, 0, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 25);
	gtk_label_set_justify (GTK_LABEL(MainText),GTK_JUSTIFY_CENTER);
	gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 1, 2,GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 2, 3,GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

	entry1 = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY(entry1),20);
	entry2 = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY(entry2),20);

	gtk_table_attach(GTK_TABLE(table), entry1, 1, 2, 1, 2,GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 5);
	gtk_table_attach(GTK_TABLE(table), entry2, 1, 2, 2, 3,GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 5);
	/*make entry fields - end*/

	/*make Sign Up button - start*/
	regbutton = gtk_button_new_with_label("   Sign Up   ");
	g_signal_connect(regbutton, "clicked", G_CALLBACK(get_user), entry1);
	g_signal_connect(regbutton, "clicked", G_CALLBACK(get_pass), entry2);
	g_signal_connect_swapped (regbutton, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	/*make Sign Up button - end*/


	/*Attach buttons to table - start*/
	gtk_table_attach(GTK_TABLE(table), regbutton, 0, 2, 3, 4, GTK_SHRINK, GTK_SHRINK, 10, 10);
	gtk_button_set_alignment(GTK_BUTTON(regbutton), 0.5, 1);
	/*Attach buttons to table - end*/

	/*make app icon - start*/
	icon = create_pixbuf("fireturtle.png");  
	gtk_window_set_icon(GTK_WINDOW(window), icon);
	/*make app icon - end*/

	gtk_widget_show_all(window);

	g_signal_connect(window, "destroy",
	G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();
	return 0;
}
