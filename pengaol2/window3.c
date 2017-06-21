#include "config.h"

#ifdef WITH_GUI


 #include "window3.h"
#include "window.h"

GtkWidget *vbox6;
GtkWidget *vbox7;
GtkWidget *hbox10;
GtkWidget *hbox11;
GtkWidget *hbox12;
GtkWidget *frame2;
GtkWidget *frame3;
GtkWidget *label10;
GtkWidget *label11;
GtkWidget *label12;
GtkWidget *label13;
GtkWidget *label14;
GtkWidget *combo2;
GtkWidget *combo3;
GtkWidget *entry5;
GtkWidget *button7;
GtkWidget *button8;
GtkWidget *button9;
GList     *driver_list = NULL;
GtkWidget *hbox[32];
GtkWidget *label[32];
GtkWidget *entry[32];
GtkWidget *button;

// char **DriverArgNames();
// char **DriverArgValue();

void savedriverconf()
{
  char **name;
  int x;

  window_state = 3;

  x = 0;
  name = DriverArgNames();

  while(name[x] != NULL)
    {
      gtk_widget_destroy(entry[x]);
      gtk_widget_destroy(label[x]);
      gtk_widget_destroy(hbox[x]);
      x++;
    }
  gtk_widget_destroy(button);
}

void create_driver_window()
{
  window5 = gtk_window_new (GTK_WINDOW_DIALOG);
  gtk_window_set_policy(GTK_WINDOW(window5), FALSE, FALSE, TRUE);
  gtk_widget_set_usize (window5, 400, 400);
  gtk_window_set_title (GTK_WINDOW (window5), "PengAOL v1.0 - DriversConf");
  gtk_widget_show(window5);
  {
    vbox7 = gtk_vbox_new(FALSE, 0);
    gtk_widget_ref (vbox7);
    gtk_widget_show (vbox7);
    gtk_container_add (GTK_CONTAINER (window5), vbox7);
    {
      frame3 = gtk_frame_new(NULL);
      gtk_widget_ref (frame3);
      gtk_widget_show (frame3);
      gtk_container_add (GTK_CONTAINER (vbox7), frame3);
      {
	label14 = gtk_label_new("Driver configuration");
	    gtk_widget_ref (label14);
	    gtk_widget_show (label14);
	    gtk_container_add (GTK_CONTAINER (frame3), label14);
      } // Fin de la frame3
    } // Fin de la vbox7
  } // Fin de la window5
}

void driver_window(int w)
{
  char **name;
  char **args;
  int x;

  x = 0;
  name = DriverArgNames();
  args = DriverArgValue();

  window_state = 4;
  while(name[x] != NULL)
    {
      hbox[x] = gtk_hbox_new(FALSE, 0);
      gtk_widget_ref(hbox[x]);
      gtk_container_add(GTK_CONTAINER(vbox7), hbox[x]);
      gtk_widget_show(hbox[x]);
      {
	label[x] = gtk_label_new(name[x]);
	gtk_widget_ref(label[x]);
	gtk_container_add(GTK_CONTAINER(hbox[x]), label[x]);
	gtk_widget_set_usize(label[x], 120, 25);
	gtk_widget_show(label[x]);
	
	entry[x] = gtk_entry_new();
	gtk_widget_ref(entry[x]);
	gtk_container_add(GTK_CONTAINER(hbox[x]), entry[x]);
	gtk_widget_set_usize(entry[x], 280, 25);
	gtk_widget_show(entry[x]);
	gtk_entry_set_text(GTK_ENTRY(entry[x]), args[x]);
      }
      x++;
    }
      button = gtk_button_new_with_label("Exit");
      gtk_widget_ref(button);
      gtk_container_add(GTK_CONTAINER(vbox7), button);
      gtk_widget_show(button);
      gtk_signal_connect(GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC(savedriverconf) , (gpointer) "data");	
}

void create_driver_in()
{
  driver_window(0);
}

void create_driver_out()
{
  driver_window(1);
}

//-------------------------------

void exit_config()
{
  window_state = 0;
}

void CreateDriverCombo(GtkWidget *combo)
{
  char **driver;
  int x;

  x = 0;
  driver = DriverList();

  g_list_free(driver_list);
  driver_list = NULL;
  while(driver[x] != NULL)
    {
      driver_list = g_list_append (driver_list, driver[x]);
      x++;
    }
  gtk_combo_set_popdown_strings(GTK_COMBO(combo), driver_list);
}

void config_window_show()
{
  gtk_widget_show(window4);
}

void config_window_hide()
{
  gtk_widget_hide(window4);
}

void config_window()
{
  window4 = gtk_window_new (GTK_WINDOW_DIALOG);
  gtk_window_set_policy(GTK_WINDOW(window4), FALSE, FALSE, TRUE);
  gtk_widget_set_usize (window4, 400, 150);
  gtk_window_set_title (GTK_WINDOW (window4), "PengAOL v1.0 - Configuration");
  gtk_widget_show(window4);
  {
    vbox6 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox6);
    gtk_widget_show (vbox6);
    gtk_container_add (GTK_CONTAINER (window4), vbox6);
    {
      frame2 = gtk_frame_new(NULL);
      gtk_widget_ref(frame2);
      gtk_container_add(GTK_CONTAINER(vbox6), frame2);
      gtk_widget_set_usize(frame2,0,30);
      gtk_widget_show(frame2);
      {
	label10 = gtk_label_new("Configuration");
	gtk_widget_ref(label10);
	gtk_container_add(GTK_CONTAINER(frame2), label10);
	gtk_widget_show(label10);	
      } // fin de la frame2

      hbox10 = gtk_hbox_new (FALSE, 0);
      gtk_widget_ref (hbox10);
      gtk_widget_show (hbox10);
      gtk_container_add (GTK_CONTAINER (vbox6), hbox10);
      {
	label11 = gtk_label_new("Input driver :");
	gtk_widget_ref(label11);
	gtk_container_add(GTK_CONTAINER(hbox10), label11);
	gtk_widget_set_usize (label11, 120, 30);
	gtk_widget_show(label11);	
	combo2 = gtk_combo_new();
	gtk_widget_ref(combo2);
	gtk_container_add(GTK_CONTAINER(hbox10), combo2);
	gtk_widget_set_usize (combo2, 200, 30);
	gtk_widget_show(combo2);	
	CreateDriverCombo(combo2);
	button8 = gtk_button_new_with_label("Configure");
	gtk_widget_ref(button8);
	gtk_container_add(GTK_CONTAINER(hbox10), button8);
	gtk_widget_set_usize (button8, 80, 25);
	gtk_widget_show(button8);	
	gtk_signal_connect(GTK_OBJECT(button8), "clicked",
			   GTK_SIGNAL_FUNC(create_driver_in),
			   (gpointer) "0");
	CreateDriverCombo(button8);
      }

      hbox11 = gtk_hbox_new (FALSE, 0);
      gtk_widget_ref (hbox11);
      gtk_widget_show (hbox11);
      gtk_container_add (GTK_CONTAINER (vbox6), hbox11);
      {
	label12 = gtk_label_new("Output driver :");
	gtk_widget_ref(label12);
	gtk_container_add(GTK_CONTAINER(hbox11), label12);
	gtk_widget_set_usize (label12, 120, 30);
	gtk_widget_show(label12);	
	combo3 = gtk_combo_new();
	gtk_widget_ref(combo3);
	gtk_container_add(GTK_CONTAINER(hbox11), combo3);
	gtk_widget_set_usize (combo3, 200, 30);
	gtk_widget_show(combo3);	
	CreateDriverCombo(combo3);
	button9 = gtk_button_new_with_label("Configure");
	gtk_widget_ref(button9);
	gtk_container_add(GTK_CONTAINER(hbox11), button9);
	gtk_widget_set_usize (button9, 80, 25);
	gtk_widget_show(button9);	
	gtk_signal_connect(GTK_OBJECT(button9), "clicked",
			   GTK_SIGNAL_FUNC(create_driver_out),
			   (gpointer) "0");
	CreateDriverCombo(button9);

      }

      hbox12 = gtk_hbox_new (FALSE, 0);
      gtk_widget_ref (hbox12);
      gtk_widget_show (hbox12);
      gtk_container_add (GTK_CONTAINER (vbox6), hbox12);
      {
	label13 = gtk_label_new("No-AOL DNS (IP) :");
	gtk_widget_ref(label13);
	gtk_container_add(GTK_CONTAINER(hbox12), label13);
	gtk_widget_set_usize (label13, 120, 30);
	gtk_widget_show(label13);	
	entry5 = gtk_entry_new();
	gtk_widget_ref(entry5);
	gtk_container_add(GTK_CONTAINER(hbox12), entry5);
	gtk_widget_set_usize (entry5, 280, 22);
	gtk_widget_show(entry5);	
      }

      button7 = gtk_button_new_with_label("Exit");
      gtk_widget_ref(button7);
      gtk_container_add(GTK_CONTAINER(vbox6), button7);
      //gtk_widget_set_usize (, 150, 30);
      gtk_widget_show(button7);	
      gtk_signal_connect(GTK_OBJECT(button7), "clicked", GTK_SIGNAL_FUNC(exit_config) , (gpointer) "data");

    } // fin de la vbox6
  } // fin de window4
}

#endif



