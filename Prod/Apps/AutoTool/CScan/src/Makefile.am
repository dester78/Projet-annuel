bin_PROGRAMS = CScan
include_HEADERS=    \
    bluetoothHandler.h  \
    convertCharHandler.h    \
    curlHandler.h   \
    fileHandler.h   \
    gtkCallbacks.h  \
    gtkHandler.h    \
    jsonHandler.h   \
    jsonReader.h    \
    jsonWriter.h    

CScan_SOURCES =\
    main.c  \
    bluetoothHandler.c  \
    convertCharHandler.c    \
    curlHandler.c   \
    fileHandler.c   \
    gtkCallbacks.c  \
    gtkHandler.c    \
    jsonHandler.c   \
    jsonReader.c    \
    jsonWriter.c    


AM_CPPFLAGS = -DCSCANLOCALEDIR=\"${CSCANLOCALEDIR}\"

CScan_CPPFLAGS = \
	-I $(top_srcdir) \
    -DG_LOG_DOMAIN=\"GnomeHello\"			\
	-DDATADIR=\""$(pkgdatadir)"\"			\
	-DCSCANLOCALEDIR=\""$(datadir)/locale"\"	\
    $(CSCAN_CFLAGS)	


CScan_CFLAGS = \
        -pedantic   \
        -Wall   \
        -Wextra \
	   $(WARN_CFLAGS)\
	   $(AM_CFLAGS)

CScan_LDFLAGS = $(AM_LDFLAGS)


CScan_LDADD = \
	$(CSCAN_LIBS)	\
 	$(INTLLIBS)

