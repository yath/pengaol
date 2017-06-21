####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = pengaol2
pengaol2_SOURCES = window3.c window2.c window1.c pengemul.c caim.cpp threadELV3.cpp cppptools.cpp cppp.cpp ccrypto.cpp cusermanager.cpp crouteurlinux.cpp crouteur.cpp caolcmd30.cpp cclienttoaol30.cpp caolheader30.cpp caoltoclient30.cpp ctcpipheader.cpp vjcompress.c cvjcompress.cpp ckernel.cpp kernel30.cpp ctextlinux.cpp ciptools.cpp ccommand.cpp cmsgerror.cpp cloader.cpp nulldriver.cpp cparamconfig.cpp cconfig.cpp modemdriver.cpp ctuntapdriver.cpp TunTapErrors.cpp CableDriver.cpp main.cpp 
pengaol2_LDADD   = -lstdc++ -lpthread

SUBDIRS = docs 

EXTRA_DIST = main.cpp CableDriver.cpp CableDriver.h TunTapErrors.cpp ctuntapdriver.cpp ctuntapdriver.h modemdriver.cpp modemdriver.h type.h cconfig.cpp cconfig.h cparamconfig.cpp cparamconfig.h nulldriver.cpp nulldriver.h cloader.cpp cloader.h cmsgerror.cpp cmsgerror.h PengMessages.txt ccommand.cpp ccommand.h ciptools.cpp ciptools.h ctextlinux.cpp ctextlinux.h kernel30.cpp kernel30.h ckernel.cpp ckernel.h cvjcompress.cpp cvjcompress.h vjcompress.c vjcompress.h ctcpipheader.cpp ctcpipheader.h caoltoclient30.cpp caoltoclient30.h caolheader30.cpp caolheader30.h cclienttoaol30.cpp cclienttoaol30.h caolcmd30.cpp caolcmd30.h crouteur.cpp crouteur.h crouteurlinux.cpp crouteurlinux.h cusermanager.cpp cusermanager.h ccrypto.cpp ccrypto.h cppp.cpp cppp.h cppptools.cpp cppptools.h threadELV3.cpp threadELV3.h caim.cpp caim.h main.h connect.xpm left.xpm pengemul.c pengemul.h up.xpm window1.c window1.h window2.c window2.h window3.c window3.h window.h 

install-data-local:
	$(mkinstalldirs) /usr/
	$(INSTALL_DATA) $(srcdir)/main.cpp /usr/bin
	$(mkinstalldirs) /etc/
	$(INSTALL_DATA) $(srcdir)/PengMessages.txt /etc/PengMessages.txt

uninstall-local:
	-rm -f /usr/bin
	-rm -f /etc/PengMessages.txt

####### kdevelop will overwrite this part!!! (end)############
bin_PROGRAMS = pengaol2

# set the include path found by configure
INCLUDES= $(all_includes)
 
# the library search path.
pengaol2_LDFLAGS = $(all_libraries) 
                                                                                
