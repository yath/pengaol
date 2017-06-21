/************************
** Interface graphique **
** Start : 11/08/2001  **
**   Zeon@Paris.com    **
************************/
#include "config.h"
#ifdef WITH_GUI

#include <string.h>
#include "window.h"
#include "window1.h"
#include "window2.h"
#include "window3.h"
#include <signal.h> // <-- NEW !
#include <unistd.h>
#include "main.h"

GtkWidget *hbox1;
GtkWidget *cale;
GtkWidget *vbox1;
GtkWidget *vbox2;
GtkWidget *vbox3;
GtkWidget *hbox2;
GtkWidget *hbox3;
GtkWidget *hbox4;
GtkWidget *hbox5;
GtkWidget *hbox6;
GtkWidget *vbox4;
GtkWidget *image1, *image2, *image3;
GdkPixmap *pixmap1, *pixmap2;
GdkBitmap *mask1, *mask2;
GtkStyle  *style;
GtkWidget *label1;
GtkWidget *label2;
GtkWidget *label3;
GtkWidget *label4;
GtkWidget *label5;
GtkWidget *labelversion;
GtkWidget *button1;
GtkWidget *button2;
GtkWidget *button3;
GtkWidget *button4;
GtkWidget *button5;
GtkWidget *check1;
GtkWidget *combo1;
GList     *login_list = NULL;
GtkWidget *entry1;
char      buf[256];

void configuration()
{
  window_state = 3;
}

char *selected_user()
{
  return ( gtk_entry_get_text(GTK_ENTRY (GTK_COMBO(combo1)->entry)) );
}

void CreateLoginCombo()
{
  char **login;
  int x;

  x = 0;
  login = LoginList();

  g_list_free(login_list);
  login_list = NULL;
  while(login[x] != NULL)
    {
      login_list = g_list_append (login_list, login[x]);
      x++;
    }
  login_list = g_list_reverse(login_list);
}

int LoginExist(char *testedlogin)
{
  char **login;
  int x;

  x = 0;
  login = LoginList();

  while(login[x] != NULL)
    {
      if (!strcmp(testedlogin, login[x]))
	{
	  return (1);
	}
      x++;
    }
  return (0);
}

void LoginPassSet()
{
  gtk_entry_set_text(GTK_ENTRY (entry1), GetPass(gtk_entry_get_text(GTK_ENTRY (GTK_COMBO(combo1)->entry))));
}

void deleteuser()
{
  char *p;

  p = gtk_entry_get_text(GTK_ENTRY (GTK_COMBO(combo1)->entry));
  DelUser(p);
  CreateLoginCombo();
  gtk_combo_set_popdown_strings(GTK_COMBO(combo1), login_list);
  gtk_combo_set_value_in_list(GTK_COMBO(combo1), 3, TRUE);
  LoginPassSet();
}

void combochange()
{
  char *p;

  p = gtk_entry_get_text(GTK_ENTRY (GTK_COMBO(combo1)->entry));
  if (!LoginExist(p))
    {
      AddUser(p, "");
      CreateLoginCombo();
      gtk_combo_set_popdown_strings(GTK_COMBO(combo1), login_list);
      gtk_combo_set_value_in_list(GTK_COMBO(combo1), 3, TRUE);
      LoginPassSet();
    }
  else
    {
      LoginPassSet();
    }
}

void entrychange()
{
  char *login, *pass;

  login = gtk_entry_get_text(GTK_ENTRY (GTK_COMBO(combo1)->entry));
  pass = gtk_entry_get_text(GTK_ENTRY ((entry1)));
  printf("%s %s\n", login, pass);
  DelUser(login);
  AddUser(login, pass);
  printf("Password changed !\n");
}

void byebye()
{
  char *p;
  p = gtk_entry_get_text(GTK_ENTRY (GTK_COMBO(combo1)->entry));
  if (!GTK_TOGGLE_BUTTON(check1)->active)
    {
      printf("Suppression du mot de passe\n");
      DelUser(p);
      AddUser(p, "");
    }
  gtk_main_quit();
  QuitPeng();
}

GtkWidget *new_image_from_file(GtkWidget *window, GdkPixmap *pixmap, GdkBitmap *mask, GtkStyle *style, gchar *filename)
{
  style = gtk_widget_get_style(window);
  pixmap = gdk_pixmap_create_from_xpm(window->window, &mask, &style->bg[GTK_STATE_NORMAL], filename);
  return (gtk_pixmap_new(pixmap, mask));
}


int first_window()
{
  window1 = gtk_window_new (GTK_WINDOW_DIALOG);
  gtk_window_set_policy(GTK_WINDOW(window1), FALSE, FALSE, TRUE);
  gtk_widget_set_usize (window1, 400, 256);
  gtk_window_set_title (GTK_WINDOW (window1), "PengAOL v1.0");
  gtk_widget_show(window1);
  {
    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_set_usize (hbox1, 95, 255);
    gtk_widget_ref (hbox1);
    gtk_widget_show (hbox1);
    gtk_container_add (GTK_CONTAINER (window1), hbox1);
    {
      *buf = 0;
      strcat(buf, IMAGE_PATH);
      strcat(buf, "left.xpm");
      image1 = new_image_from_file(window1, pixmap1, mask1, style, buf);
      gtk_widget_set_usize (image1, 95, 256);
      gtk_container_add(GTK_CONTAINER(hbox1), image1);
      gtk_widget_show(image1);

      vbox1 = gtk_vbox_new (FALSE, 8);
      gtk_widget_ref (vbox1);
      gtk_widget_show (vbox1);
      gtk_container_add (GTK_CONTAINER (hbox1), vbox1);
      {
	*buf = 0;
	strcat(buf, IMAGE_PATH);
	strcat(buf, "up.xpm");
	image2 = new_image_from_file(window1, pixmap2, mask2, style, buf);
	gtk_container_add(GTK_CONTAINER(vbox1), image2);
	gtk_widget_set_usize (image2, 306, 50);
	gtk_widget_set_uposition (image2, 94, -1);
	gtk_widget_show(image2);
	
	// Debut de la VBOX2
	
	vbox2 = gtk_vbox_new (FALSE, 0);
	gtk_widget_ref (vbox2);
	gtk_widget_show (vbox2);
	gtk_container_add (GTK_CONTAINER (vbox1), vbox2);
	gtk_widget_set_usize (vbox2, 306, 30);
	{
	
	
	  hbox2 = gtk_hbox_new (FALSE, 0);
	  gtk_widget_ref (hbox2);
	  gtk_widget_show (hbox2);
	  gtk_widget_set_usize (hbox2, 306, 22);	
	  gtk_container_add (GTK_CONTAINER (vbox2), hbox2);
	  {
	    label1 = gtk_label_new("Login :");
	    gtk_widget_ref (label1);
	    gtk_widget_set_usize (label1, 70, 18);	
	    gtk_container_add (GTK_CONTAINER (hbox2), label1);
	    gtk_widget_show (label1);
	
	    combo1 = gtk_combo_new();
	    gtk_widget_ref (combo1);
	    gtk_widget_set_usize (combo1, 174, 22);	
	    gtk_container_add (GTK_CONTAINER (hbox2), combo1);
	    gtk_widget_show (combo1);
	    gtk_combo_set_popdown_strings(GTK_COMBO(combo1), login_list);
	    gtk_signal_connect(GTK_OBJECT(combo1), "event",
			       GTK_SIGNAL_FUNC(combochange),
			       (gpointer) "data");		
	  }
	
	  button2 = gtk_button_new_with_label("DelUser");
	  gtk_widget_ref (button2);
	  gtk_container_add (GTK_CONTAINER (hbox2), button2);
	  gtk_widget_set_usize (button2, 60, 18);	
	  gtk_widget_show (button2);
	  gtk_signal_connect(GTK_OBJECT(button2), "clicked", GTK_SIGNAL_FUNC(deleteuser) , (gpointer) "data");	
	}
	// Fin VBOX2 (Login)
	
	// Debut de la VBOX3 (Password)
	vbox3 = gtk_vbox_new (FALSE, 0);
	gtk_widget_ref (vbox3);
	gtk_widget_show (vbox3);
	gtk_container_add (GTK_CONTAINER (vbox1), vbox3);
	gtk_widget_set_usize (vbox3, 306, 60);
	{
	
	  hbox4 = gtk_hbox_new (FALSE, 0);
	  gtk_widget_ref (hbox4);
	  gtk_widget_show (hbox4);
	  gtk_container_add (GTK_CONTAINER (vbox3), hbox4);
	  gtk_widget_set_usize (hbox4, 306, 22);
	  {
	    label3 = gtk_label_new("Password :");
	    gtk_widget_ref (label3);
	    gtk_widget_set_usize (label3, 70, 18);
	    gtk_container_add (GTK_CONTAINER (hbox4), label3);
	    gtk_widget_show (label3);
	
	    entry1 = gtk_entry_new();
	    gtk_widget_ref (entry1);
	    gtk_widget_set_usize (entry1, 234, 22);	
	    gtk_container_add (GTK_CONTAINER (hbox4), entry1);
	    gtk_widget_show (entry1);
	    gtk_entry_set_visibility(GTK_ENTRY(entry1), 0);
	    gtk_signal_connect(GTK_OBJECT(entry1), "activate",
			       GTK_SIGNAL_FUNC(entrychange),
			       (gpointer) "data");	
	  }
	  hbox5 = gtk_hbox_new (TRUE, 0);
	  gtk_widget_ref (hbox5);
	  gtk_widget_show (hbox5);
	  gtk_container_add (GTK_CONTAINER (vbox3), hbox5);
	  gtk_widget_set_usize (hbox5, 306, 22);
	
	  {
	    label4 = gtk_label_new("");
	    gtk_widget_ref (label4);
	    gtk_container_add (GTK_CONTAINER (hbox5), label4);
	    gtk_widget_show (label4);
	
	    check1 = gtk_check_button_new_with_label ("Save password");
	    gtk_widget_ref (check1);
	    gtk_widget_show (check1);
	    gtk_container_add (GTK_CONTAINER (hbox5), check1);
	    gtk_widget_set_usize (check1, 306, 22);
	  }
	}
	
	// Debut de la HBOX6 (Buttons)
	hbox6 = gtk_hbox_new (TRUE, 5);
	gtk_widget_ref (hbox6);
	gtk_widget_show (hbox6);
	gtk_container_add (GTK_CONTAINER (vbox1), hbox6);
	gtk_widget_set_usize (hbox6, 0, 30);
	{
	  button3 = gtk_button_new_with_label("Connect");
	  gtk_widget_ref (button3);
	  gtk_container_add (GTK_CONTAINER (hbox6), button3);
	  gtk_widget_set_usize (button3 , 0, 30);
	  gtk_widget_show (button3);
	  gtk_signal_connect(GTK_OBJECT(button3), "clicked", GTK_SIGNAL_FUNC(connection) , (gpointer) "data");
	
	  button4 = gtk_button_new_with_label("Configure");
	  gtk_widget_ref (button4);
	  gtk_container_add (GTK_CONTAINER (hbox6), button4);
	  gtk_widget_set_usize (button4 , 0, 30);
	  gtk_widget_show (button4);
	  gtk_signal_connect(GTK_OBJECT(button4), "clicked", GTK_SIGNAL_FUNC(configuration) , (gpointer) "data");	
	  button5 = gtk_button_new_with_label("Exit");
	  gtk_widget_ref (button5);
	  gtk_container_add (GTK_CONTAINER (hbox6), button5);
	  gtk_widget_set_usize (button5 , 0, 30);
	  gtk_widget_show (button5);
	  gtk_signal_connect(GTK_OBJECT(button5), "clicked", GTK_SIGNAL_FUNC(byebye) , (gpointer) "data");	
	}
	
	labelversion = gtk_label_new("PengAOL version 1.0 Prophetie\nCode by Birdy - Interface code by Zeon");
	gtk_widget_ref (labelversion);
	gtk_container_add (GTK_CONTAINER (vbox1), labelversion);
	gtk_widget_set_usize (labelversion , 0, 50);
	gtk_widget_show (labelversion);
	
      }
    }
  }
  return (0);
}

void Window1_hide()
{
  //gtk_widget_hide(window1);
}

void Window1_show()
{
  // gtk_widget_show(window1);
  window_state = 0;
}

/*************************************************
**                                              **
**            Fenetre de connection             **
**                                              **
*************************************************/

void connect_window()
{
  window2 = gtk_window_new (GTK_WINDOW_DIALOG);
  gtk_window_set_policy(GTK_WINDOW(window2), FALSE, FALSE, TRUE);
  gtk_widget_set_usize (window2, 256, 155);
  gtk_window_set_title (GTK_WINDOW (window2), "PengAOL v1.0 - Connection");
  gtk_widget_show(window2);
  {
    vbox4 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox4);
    gtk_widget_show (vbox4);
    gtk_container_add (GTK_CONTAINER (window2), vbox4);
    {
      *buf = 0;
      strcat(buf, IMAGE_PATH);
      strcat(buf, "connect.xpm");
      image3 = new_image_from_file(window2, pixmap1, mask1, style, buf);
      gtk_widget_set_usize (image3, 256, 128);
      gtk_widget_set_uposition (image3, 0, 0);
      gtk_container_add(GTK_CONTAINER(vbox4), image3);
      gtk_widget_show(image3);

      label5 = gtk_label_new("");
      gtk_widget_ref (label5);
      gtk_container_add(GTK_CONTAINER(vbox4), label5);
      gtk_widget_set_usize (label5, 256, 27);
      gtk_widget_show (label5);
    }
  }
}

void connect_window_set_text(char *text)
{
  gtk_label_set_text(GTK_LABEL(label5), text);
}

void connect_window_hide()
{
  // gtk_widget_hide(window2);
}

void connect_window_show()
{
  // gtk_widget_show(window2);
  window_state = 1;
}

/*************************************************
**                                              **
**                  DEMARRAGE                   **
**                                              **
*************************************************/

void SigAlarmNew()
{
  printf("Window state = %d\n", window_state);
  if (!window_state)
    {
      gtk_widget_show(window1);
      gtk_widget_hide(window2);
      gtk_widget_hide(window3);
      gtk_widget_hide(window4);
      gtk_widget_hide(window5);
    }
  if (window_state == 1)
    {
      gtk_widget_hide(window1);
      gtk_widget_show(window2);
      gtk_widget_hide(window3);
      gtk_widget_hide(window4);
      gtk_widget_hide(window5);
    }
  if (window_state == 2)
    {
      gtk_widget_hide(window1);
      gtk_widget_hide(window2);
      gtk_widget_show(window3);
      gtk_widget_hide(window4);
      gtk_widget_hide(window5);
    }
  if (window_state == 3)
    {
      gtk_widget_hide(window1);
      gtk_widget_hide(window2);
      gtk_widget_hide(window3);
      gtk_widget_show(window4);
      gtk_widget_hide(window5);
    }
  if (window_state == 4)
    {
      gtk_widget_hide(window1);
      gtk_widget_hide(window2);
      gtk_widget_hide(window3);
      gtk_widget_hide(window4);
      gtk_widget_show(window5);
    }
}

int RunGTK()
{
  gtk_init(NULL, NULL);

  window_state = 0;


  connect_window();
  connect_window_hide();

  create_driver_window();

  CreateLoginCombo();
  first_window();
  Window1_show();

  active_window();
  active_window_hide();
  config_window();

  signal(SIGALRM, SigAlarmNew);
  ualarm(1,500000);

  gtk_main();

  return (0);
}



#endif

