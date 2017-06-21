LIBDIR = -L/usr/X11R6/lib
LIBS   = -lX11
FLAGS = -O2
OBJS =  XCounter.o XCounterData.o
INCLUDES = -I/usr/X11R6/include -I/usr/X11R6/share/include

all:: clean XCounter

.c.o:
	gcc $(INCLUDES) $(FLAGS) -c -Wall $< -o $*.o

XCounter: $(OBJS)
	gcc $(FLAGS) -o XCounter $^ $(LIBDIR) $(LIBS)

clean::
	for i in $(OBJS) ; do \
		rm -f $$i ; \
	done
	rm -f XCounter
	rm -f *~

install:: XCounter
	install -s -g root -o root -m 755 XCounter /usr/local/bin/

uninstall:: XCounter
	rm -f /usr/local/bin/XCounter
