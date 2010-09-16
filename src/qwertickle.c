/**
 * @file   qwertickle.c
 * @brief  
 * @author Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 *
 * Copyright (C) 2010 -  Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <pthread.h>
#include <stdio.h>

#include <glib.h>
#include <gst/gst.h>
#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include <X11/extensions/record.h>

#include "qwertickle.h"

const gchar* uris[] = {
 "file://"DATADIR"/typewriter-key.mp3",
 "file://"DATADIR"/typewriter-return.mp3",
 "file://"DATADIR"/typewriter-space-bar.mp3",
 "file://"DATADIR"/typewriter-backspace.mp3"
};

/* global variables */
static gint uri = 0;
static gint count = 0;
static gboolean stop_thread = FALSE;
static Display* dpy = 0;
static XRecordContext rc;
static pthread_t thread;

int main(int argc, char *argv[])
{
  QwertickleGUI* gui = g_new0(QwertickleGUI, 1);

  gst_init(NULL, NULL);
  gtk_init(&argc, &argv);

  gui->statusicon = gtk_status_icon_new_from_stock("gtk-yes");
  gtk_status_icon_set_visible(gui->statusicon, TRUE);
  gui->mainmenu = GTK_MENU(gtk_menu_new());
  gui->play = GTK_MENU_ITEM(gtk_menu_item_new());
  gui->sep = GTK_SEPARATOR_MENU_ITEM(gtk_separator_menu_item_new());
  gui->quit =
    GTK_IMAGE_MENU_ITEM(gtk_image_menu_item_new_from_stock("gtk-quit", NULL));
  gui->about =
    GTK_IMAGE_MENU_ITEM(gtk_image_menu_item_new_from_stock("gtk-about", NULL));
  gui->enable =
    GTK_CHECK_MENU_ITEM(gtk_check_menu_item_new_with_label("Enable"));

  gtk_menu_shell_append(GTK_MENU_SHELL(gui->mainmenu), GTK_WIDGET(gui->enable));
  gtk_menu_shell_append(GTK_MENU_SHELL(gui->mainmenu), GTK_WIDGET(gui->about));
  gtk_menu_shell_append(GTK_MENU_SHELL(gui->mainmenu), GTK_WIDGET(gui->sep));
  gtk_menu_shell_append(GTK_MENU_SHELL(gui->mainmenu), GTK_WIDGET(gui->quit));

  /* initialize widgets */
  gtk_check_menu_item_set_active(gui->enable, TRUE);

  /* connect signals */
  g_signal_connect(gui->statusicon, "popup-menu", G_CALLBACK(on_menu_pop), gui);
  g_signal_connect(gui->enable, "toggled", G_CALLBACK(on_enable_toggled), gui);
  g_signal_connect(gui->quit, "activate", G_CALLBACK(on_quit_activate), gui);
  g_signal_connect(gui->about, "activate", G_CALLBACK(on_about_activate), gui);
  g_signal_connect(gui->play, "activate", G_CALLBACK(on_play_activate), NULL);

  gtk_widget_show_all(GTK_WIDGET(gui->mainmenu));

  pthread_create(&thread, NULL, intercept_key_thread, gui->play);
  gtk_main();

  return 0;
}


void on_menu_pop(GtkStatusIcon* widget, guint b, guint time, gpointer data) {
  QwertickleGUI* gui = (QwertickleGUI*)data;
  gtk_menu_popup(gui->mainmenu, NULL, NULL, NULL, NULL, b, time);
}

void on_quit_activate(GtkImageMenuItem* widget, gpointer data) {
  stop_intercept_key_thread();
  gtk_main_quit();
}

void on_about_activate(GtkImageMenuItem* widget, gpointer data) {
    GError* err = NULL;
    //GdkPixbuf* icon = gdk_pixbuf_new_from_file_at_size
    //    (DATADIR"/gummi-beta.png", 60, 60, &err);
    const gchar* authors[] = { "Wei-Ning Huang\n<aitjcize@gmail.com>\n", NULL};

    GtkAboutDialog* dialog = GTK_ABOUT_DIALOG(gtk_about_dialog_new());
    gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog), TRUE);
    gtk_about_dialog_set_authors(dialog, authors);
    gtk_about_dialog_set_program_name(dialog, PACKAGE_NAME);
    gtk_about_dialog_set_version(dialog, PACKAGE_VERSION);
    gtk_about_dialog_set_website(dialog, PACKAGE_URL);
    gtk_about_dialog_set_copyright(dialog, PACKAGE_COPYRIGHT);
    gtk_about_dialog_set_license(dialog, PACKAGE_LICENSE);
    //gtk_about_dialog_set_logo(dialog, icon);
    gtk_about_dialog_set_comments(dialog, PACKAGE_COMMENTS);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_enable_toggled(GtkCheckMenuItem* enable, gpointer data) {
  QwertickleGUI* gui = (QwertickleGUI*)data;
  if (gtk_check_menu_item_get_active(enable)) {
    stop_thread = FALSE;
    pthread_create(&thread, NULL, intercept_key_thread, gui->play);
  } else
    stop_intercept_key_thread();
}

void on_play_activate(GtkWidget* widget, GdkEventKey* event, gpointer data) {
  if (count >= 30) return;
  GstElement* pipeline = NULL;
  GstElement* audio_sink = NULL;
  GstBus* bus = NULL;

  pipeline = gst_element_factory_make("playbin", "playbin");
  audio_sink = gst_element_factory_make("autoaudiosink", "audio-sink");	
  g_object_set(G_OBJECT(pipeline), "audio-sink", audio_sink, NULL);

  bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
  gst_bus_add_watch(bus, gst_bus_callback, pipeline);
  gst_object_unref(bus);

  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_READY);  
  g_object_set(G_OBJECT(pipeline), "uri", uris[uri], NULL);    
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  ++count;
}

gboolean gst_bus_callback(GstBus *bus, GstMessage *message, gpointer data) {
  switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_EOS:
      gst_element_set_state(GST_ELEMENT(data), GST_STATE_NULL);  
      gst_object_unref(GST_ELEMENT(data));
      --count;
  }
  return TRUE;
}

void* intercept_key_thread(void* data) {
  XRecordClientSpec rcs;
  XRecordRange* rr;
  dpy = XOpenDisplay(0);
  int screen = DefaultScreen(dpy);
  Window rootwin = RootWindow(dpy, screen);

  if (!(rr = XRecordAllocRange())) {
    printf("XRecordAllocRange error\n");
    exit(1);
  }
  rr->device_events.first = KeyPress;
  rr->device_events.last = MotionNotify;
  rcs = XRecordAllClients;

  if (!(rc = XRecordCreateContext(dpy, 0, &rcs, 1, &rr, 1))) {
    printf("XRecordCreateContext error\n");
    exit(1);
  }
  if (!XRecordEnableContext(dpy, rc, key_pressed_cb, data)) {
    printf("XRecordEnableContextAsync error\n");
    exit(1);
  }
}

void stop_intercept_key_thread(void) {
  void* result;
  stop_thread = TRUE;
  if (0 != pthread_join(thread, &result)) {
    printf("pthread_join error\n");
    exit(1);
  }
}

void key_pressed_cb(XPointer arg, XRecordInterceptData *d) {
  if (stop_thread) {
    if (!XRecordDisableContext(dpy, rc)) {
      printf("XRecordDisableContext error\n");
      exit(1);
    }
    if (!XRecordFreeContext(dpy, rc)) {
      printf("XRecordFreeContext error\n");
      exit(1);
    }
    pthread_exit(0);
  }

  if (d->category != XRecordFromServer) return;
  unsigned char type = ((unsigned char*)d->data)[0] & 0x7F;
  unsigned char detail = ((unsigned char*)d->data)[1];
  if (type == KeyPress) {
    switch (detail) {
      case 36:
        uri = 1;
        break;
      case 65:
        uri = 2;
        break;
      case 22:
        uri = 3;
        break;
      default:
        uri = 0;
    }
    g_signal_emit_by_name(arg, "activate");
  }
}
