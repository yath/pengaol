#include "config.h"

#ifdef WITH_GUI

#include <malloc.h>
#include <string.h>
#include "window.h"
#include "window2.h"
#include "main.h"

char *about[10] = { "\n",
		   " PengAOL v1.0 : Prophetie\n\n",
		   " Full connection code :\n",
		   " Stephane Guth (Birdy57)\n",
		   " birdy57@pengaol.org\n\n",
		   " Graphic interface code :\n",
		   " Steeve Lefort (Zeon)\n",
		   " zeon@paris.com\n\n",
		    " Official web site :\n",
		   " http://www.pengaol.org\n"
};


GtkWidget *vbox4;
GtkWidget *vbox5;
GtkWidget *notebook1;
GtkWidget *scroll1;
GtkWidget *tablabel1;
GtkWidget *tablabel2;
GtkWidget *tablabel3;
GtkWidget *text1;
GtkWidget *text2;
GtkWidget *text3;
GtkWidget *frame1;
GtkWidget *hbox7;
GtkWidget *hbox8;
GtkWidget *hbox9;
GtkWidget *entry2;
GtkWidget *entry3;
GtkWidget *entry4;
GtkWidget *label6;
GtkWidget *label7;
GtkWidget *label8;
GtkWidget *label9;
GtkWidget *button6;
int       x;
char      buf2[128];

void active_window()
{
  // Onglet CONTACT
  scroll1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_ref(scroll1);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll1),
				 GTK_POLICY_NEVER,
				 GTK_POLICY_ALWAYS);
  gtk_widget_show(scroll1);
  tablabel1 = gtk_label_new("Contacts");
  gtk_widget_ref(tablabel1);
  gtk_widget_show(tablabel1);

  text1 = gtk_text_new(NULL, NULL);
  gtk_widget_ref (text1);
  gtk_container_add (GTK_CONTAINER (scroll1), text1);
  gtk_widget_show(text1);

  // Onglet INFOS
  text3 = gtk_text_new(NULL, NULL);
  gtk_widget_ref (text3);
  gtk_widget_show(text3);

  tablabel3 = gtk_label_new("Info");
  gtk_widget_ref(tablabel3);
  gtk_widget_show(tablabel3);

  // Onglet ABOUT
  text2 = gtk_text_new(NULL, NULL);
  gtk_widget_ref (text2);
  gtk_widget_show(text2);

  for (x = 0; x < 10; x++)
    gtk_text_insert (GTK_TEXT (text2), NULL, NULL, NULL,
		     about[x], strlen(about[x]));

  tablabel2 = gtk_label_new("About");
  gtk_widget_ref(tablabel2);
  gtk_widget_show(tablabel2);

  // Fenetre
  window3 = gtk_window_new (GTK_WINDOW_DIALOG);
  gtk_window_set_policy(GTK_WINDOW(window3), FALSE, FALSE, TRUE);
  gtk_widget_set_usize (window3, 200, 400);
  gtk_window_set_title (GTK_WINDOW (window3), "PengAOL v1.0 - Online");
  gtk_widget_show(window3);
  {
    vbox4 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox4);
    gtk_widget_show (vbox4);
    gtk_container_add (GTK_CONTAINER (window3), vbox4);
    {
      notebook1 = gtk_notebook_new();
      gtk_widget_ref (notebook1);
      gtk_container_add(GTK_CONTAINER(vbox4), notebook1);
      gtk_widget_set_usize (notebook1, 200, 250);
      gtk_widget_show (notebook1);
      {
	gtk_notebook_insert_page(GTK_NOTEBOOK (notebook1),
				 scroll1,
				 tablabel1, 0);
	gtk_notebook_insert_page(GTK_NOTEBOOK (notebook1),
				 text3,
				 tablabel3, 1);
 	gtk_notebook_insert_page(GTK_NOTEBOOK (notebook1),
				 text2,
				 tablabel2, 2);
      }

      frame1 = gtk_frame_new(NULL);
      gtk_widget_ref(frame1);
      gtk_container_add(GTK_CONTAINER(vbox4), frame1);
      gtk_widget_show(frame1);
      {
	vbox5 = gtk_vbox_new(FALSE, 0);
	gtk_widget_ref(vbox5);
	gtk_container_add(GTK_CONTAINER(frame1), vbox5);
	gtk_widget_show(vbox5);
	{
	  hbox7 = gtk_hbox_new (FALSE, 0);
	  gtk_widget_ref (hbox7);
	  gtk_widget_show (hbox7);
	  gtk_container_add (GTK_CONTAINER (vbox5), hbox7);
	  {
	    label6 = gtk_label_new("Time :");
	    gtk_widget_ref(label6);
	    gtk_container_add (GTK_CONTAINER (hbox7), label6);
	    gtk_widget_set_usize (label6, 70, 18);
	    gtk_widget_show(label6);
	
	    entry2 = gtk_entry_new();
	    gtk_widget_ref(entry2);
	    gtk_container_add (GTK_CONTAINER (hbox7), entry2);
	    gtk_widget_set_usize (entry2, 130, 22);
	    gtk_widget_show(entry2);	
	  } //fin hbox7
	
	  hbox8 = gtk_hbox_new (FALSE, 0);
	  gtk_widget_ref (hbox8);
	  gtk_widget_show (hbox8);
	  gtk_container_add (GTK_CONTAINER (vbox5), hbox8);
	  {
	    label7 = gtk_label_new("Download :");
	    gtk_widget_ref(label7);
	    gtk_container_add (GTK_CONTAINER (hbox8), label7);
	    gtk_widget_set_usize (label7, 70, 18);
	    gtk_widget_show(label7);
	
	    entry3 = gtk_entry_new();
	    gtk_widget_ref(entry3);
	    gtk_container_add (GTK_CONTAINER (hbox8), entry3);
	    gtk_widget_set_usize (entry3, 130, 22);
	    gtk_widget_show(entry3);	
	  } // fin hbox8

	  hbox9 = gtk_hbox_new (FALSE, 0);
	  gtk_widget_ref (hbox9);
	  gtk_widget_show (hbox9);
	  gtk_container_add (GTK_CONTAINER (vbox5), hbox9);
	  {
	    label8 = gtk_label_new("Upload :");
	    gtk_widget_ref(label8);
	    gtk_container_add (GTK_CONTAINER (hbox9), label8);
	    gtk_widget_set_usize (label8, 70, 18);
	    gtk_widget_show(label8);
	
	    entry4 = gtk_entry_new();
	    gtk_widget_ref(entry4);
	    gtk_container_add (GTK_CONTAINER (hbox9), entry4);
	    gtk_widget_set_usize (entry4, 130, 22);
	    gtk_widget_show(entry4);	
	  } // fin hbox9
	
	  button6 = gtk_button_new_with_label ("Disconnect");
	  gtk_widget_ref (button6);
	  gtk_widget_show (button6);
	  gtk_container_add (GTK_CONTAINER (vbox4), button6);
	  gtk_signal_connect(GTK_OBJECT(button6), "clicked",
			     GTK_SIGNAL_FUNC(disconnect),
			     (gpointer) "data");	
	  label9 = gtk_label_new ("PengAOL v1.0");
	  gtk_widget_ref (label9);
	  gtk_widget_show (label9);
	  gtk_container_add (GTK_CONTAINER (vbox4), label9);

	} // vbox5
      } // fin frame1
    } // fin vbox4
  } // fin window3
}

void active_window_show()
{
  //gtk_widget_show(window3);
  window_state = 2;
}

void active_window_hide()
{
  //gtk_widget_hide(window3);
}

void clear_contact()
{
  gtk_text_backward_delete (GTK_TEXT(text1),
			    gtk_text_get_length(GTK_TEXT(text1)));
}

void PrintContactList()
{
  char **contact;
  int x;

  clear_contact();
  x = 0;
  contact = ContactList();
  while(contact[x] != NULL)
    {
      gtk_text_insert (GTK_TEXT (text1), NULL, NULL, NULL,
		       contact[x], strlen(contact[x]));
      gtk_text_insert (GTK_TEXT (text1), NULL, NULL, NULL,
		       "\n", 1);
      x++;
    }
}

void PrintInfo(char *info)
{
  gtk_text_insert (GTK_TEXT (text3), NULL, NULL, NULL,
		   info, strlen(info));
}

char *n2str(int n, char *p)
{
  char *s;
  int t;
  int x;

  s = p;

  while (n>9)
    {
      for (t = 1; t * 10 < n; t = t * 10) {}
      for (x = 0; t * (x + 1) < n; x++) {}
      *s = 48 + x;
      s++;
      n = n - (t * x);
      if (n*10 < t)
	{
	  *s =48;
	  s++;
	}
    }
  *s = 48 + n;
  s++;
  *s = 0;
  return (p);
}

void PrintConnectionTime()
{
  int x;
  int h, m, s;
  char *pa;

  pa = malloc(128);
  x = ConnectionTime();
  h = x / (60*60);
  m = (x - h*(60*60)) / 60;
  s = (x - h*(60*60)) - (m * 60);
  if (h<10)
    strcpy(buf2, "0");
  n2str(h, pa);
  strcat(buf2, pa);
  strcat(buf2,"h");
  if (m<10)
    strcat(buf2, "0");
  n2str(m, pa);
  strcat(buf2, pa);
  strcat(buf2, "mn");
  if (s<10)
    strcat(buf2, "0");
  n2str(s, pa);
  strcat(buf2, pa);
  strcat(buf2, "s");
  strcpy(pa, buf2);
  gtk_entry_set_text(GTK_ENTRY(entry2), pa);
  strcpy(buf2, "Online ");
  strcat(buf2, pa);
  gtk_window_set_title (GTK_WINDOW (window3), buf2);
  free(pa);
}

void PrintLoadRate()
{
  n2str(DownloadRate(), buf2);
  gtk_entry_set_text(GTK_ENTRY(entry3), buf2);
  n2str(UploadRate(), buf2);
  gtk_entry_set_text(GTK_ENTRY(entry4), buf2);

}

#endif







