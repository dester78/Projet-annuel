bin_PROGRAMS = CScan
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
    jsonWriter.c    \
	bluetoothHandler.h  \
    convertCharHandler.h    \
    curlHandler.h   \
    fileHandler.h   \
    gtkCallbacks.h  \
    gtkHandler.h    \
    jsonHandler.h   \
    jsonReader.h    \
    jsonWriter.h    

CScan_CPPFLAGS = \
	-I$(top_srcdir)	
	-DG_LOG_DOMAIN=\"CScan\"			\
	-DDATADIR=\""$(pkgdatadir)"\"			\
	$(DISABLE_DEPRECATED)				\
	$(AM_CPPFLAGS)

CScan_CFLAGS = \
	   $(CSCAN_CFLAGS)	\
       $(GTK_CFLAGS)    \
       $(GLIB_CFLAGS)    \
       $(LIBCURL_CFLAGS)	\
       $(PANGO_CFLAGS)	\
	   $(WARN_CFLAGS)		\
	   $(AM_CFLAGS)

CScan_LDFLAGS = $(AM_LDFLAGS)


CScan_LDADD = \
	$(CSCAN_LIBS)	\
    $(GTK_LIBS) \
    $(GLIB_LIBS) \
    $(LIBCURL_LIBS)	\
    $(PANGO_LIBS)	\
 	$(INTLLIBS)

