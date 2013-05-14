AUTOMAKE_OPTIONS = foreign

dupddir=/etc/init.d
dupd_DATA= dupd

lib_LTLIBRARIES=libdup.la
libdup_la_SOURCES=	dup.c split.c util.c simi.c simiw.c lookup3.c client.c server.c conf.c clientm.c hashmap.c 

libdup_la_CFLAGS=$(AM_CFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -std=c99 -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
libdup_la_CXXFLAGS=$(AM_CXXFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
libdup_la_CPPFLAGS=$(AM_CPPFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
libdup_la_LDFLAGS=$(AM_LDFLAGS) -L$(prefix)/lib -lpthread 
libdup_la_LIBADD= -L$(prefix)/lib -lm 

dupincludedir=$(includedir)/dup
dupinclude_HEADERS=	common.h data.h dup.h lookup3.h simi.h simiw.h split.h util.h runtime.h redirect.h define.h conf.h \
					server.h client.h scheduler.h serverm.h clientm.h hashmap.h 


bin_PROGRAMS=dup dupserver dupclient # batch 

dup_SOURCES=main.c
dup_LDADD=libdup.la
dup_CFLAGS=$(AM_CFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -std=c99 -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
dup_CXXFLAGS=$(AM_CXXFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
dup_CPPFLAGS=$(AM_CPPFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 

dupserver_SOURCES=serverm.c
dupserver_LDADD=libdup.la
dupserver_CFLAGS=$(AM_CFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -std=c99 -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
dupserver_CXXFLAGS=$(AM_CXXFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
dupserver_CPPFLAGS=$(AM_CPPFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 

dupclient_SOURCES=clientm.c
dupclient_LDADD=libdup.la
dupclient_CFLAGS=$(AM_CFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -std=c99 -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
dupclient_CXXFLAGS=$(AM_CXXFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
dupclient_CPPFLAGS=$(AM_CPPFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 

# batch_SOURCES=batch.c
# batch_LDADD=libdup.la
# batch_CFLAGS=$(AM_CFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -std=c99 -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
# batch_CXXFLAGS=$(AM_CXXFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 
# batch_CPPFLAGS=$(AM_CPPFLAGS) -fPIC -DPREFIX=\"$(prefix)\" -DDUP_SYSCONFIG_DIR=\"$(dup_sysconfig_dir)\" -DDUP_DATA_DIR=\"$(dup_data_dir)\" -Wno-deprecated 

