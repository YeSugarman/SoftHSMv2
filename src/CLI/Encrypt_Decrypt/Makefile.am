MAINTAINERCLEANFILES = $(srcdir)/Makefile.in

AM_CPPFLAGS = -I$(srcdir)/../common \
-I$(srcdir)/../../lib/ \
-I$(srcdir)/../../lib/common \
-I$(srcdir)/../../lib/crypto \
-I$(srcdir)/../../lib/data_mgr \
-I$(srcdir)/../../lib/object_store \
-I$(srcdir)/../../lib/pkcs11 \
@CRYPTO_INCLUDES@ \
@SQLITE3_INCLUDES@

bin_PROGRAMS = Encrypt_Decrypt

AUTOMAKE_OPTIONS = subdir-objects

Encrypt_Decrypt_SOURCES = main.cpp \

Encrypt_Decrypt_LDADD = @CRYPTO_LIBS@ \
@SQLITE3_LIBS@ \
../../lib/libsofthsm_convarch.la


EXTRA_DIST = $(srcdir)/CMakeLists.txt \
$(srcdir)/*.h \
$(srcdir)/*.cpp


