/**
 * @file   qwertickle.h
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

#ifndef QWERTICKLE_H
#define QWERTICKLE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define DATADIR "/home/aitjcize/Work/qwertickle/src"
# define PACKAGE "qwertickle"
# define PACKAGE_NAME "Qwertickle"
# define PACKAGE_VERSION "git"
# define PACKAGE_URL "http://github.com/Aitjcize/Qwertickle"
#endif
#define PACKAGE_COMMENTS "A typewriter sound scheme for your keyboard"
#define PACKAGE_COPYRIGHT "Copyright \xc2\xa9 2010 Wei-Ning Huang"
#define PACKAGE_LICENSE \
"Copyright (C) 2010 -  Wei-Ning Huang (AZ) <aitjcize@gmail.com>\n" \
"All Rights reserved.\n" \
"\n" \
"This program is free software; you can redistribute it and/or modify\n" \
"it under the terms of the GNU General Public License as published by\n" \
"the Free Software Foundation; either version 2 of the License, or\n" \
"(at your option) any later version.\n" \
"\n" \
"This program is distributed in the hope that it will be useful,\n" \
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n" \
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" \
"GNU General Public License for more details.\n" \
"\n" \
"You should have received a copy of the GNU General Public License\n" \
"along with this program; if not, write to the Free Software Foundation,\n" \
"Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.\n" \
"\n" \
"\n" \
"The sound effect used in this program is from\n" \
"http://www.soundjay.com ,\n" \
"any form of redistribution is prohibited.\n"

typedef struct _QwertickleGUI {
  GtkStatusIcon* statusicon;
  GtkMenu* mainmenu;
  GtkImageMenuItem* quit;
  GtkImageMenuItem* about;
  GtkCheckMenuItem* enable;
  GtkMenuItem* play;
  GtkSeparatorMenuItem* sep;
} QwertickleGUI;

void on_menu_pop(GtkStatusIcon* widget, guint b, guint time, gpointer data);
void on_quit_activate(GtkImageMenuItem* widget, gpointer data);
void on_about_activate(GtkImageMenuItem* widget, gpointer data);
void on_enable_toggled(GtkCheckMenuItem* enable, gpointer data);
void on_play_activate(GtkWidget* widget, GdkEventKey* event, gpointer data);
gboolean gst_bus_callback(GstBus *bus, GstMessage *message, gpointer data);
void* intercept_key_thread(void* data);
void stop_intercept_key_thread(void);
void key_pressed_cb(XPointer arg, XRecordInterceptData *d);

#endif /* QWERTICKLE_H */
