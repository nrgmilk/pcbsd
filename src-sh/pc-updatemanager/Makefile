#############################################################################
# Makefile for building: pc-updatemanager
#############################################################################

PREFIX?= /usr/local

AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

first: all

all:

install_scripts: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/bin/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-updatemanager/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-updatemanager/
	-$(INSTALL_FILE) pc-updatemanager $(INSTALL_ROOT)$(PREFIX)/bin/
	-$(INSTALL_FILE) etcmerge $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-updatemanager/


uninstall_scripts:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)$(PREFIX)/bin/pc-updatemanager
	-$(DEL_DIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 


install_dochmod: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/bin/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/bin/ 
	chmod 755 $(PREFIX)/bin/pc-updatemanager

install_conf: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-updatemanager/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-updatemanager/
	-$(INSTALL_DIR) conf $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-updatemanager/


uninstall_conf:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-updatemanager/conf
	-$(DEL_DIR) $(INSTALL_ROOT)$(PREFIX)/share/pcbsd/pc-updatemanager


install:  install_scripts install_dochmod install_conf  FORCE

uninstall: uninstall_scripts uninstall_conf  FORCE

FORCE:

