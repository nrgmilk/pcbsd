#!/bin/sh
# Script which does some first time init

case $1 in
 desktop|server) ;;
              *) echo "Error: Please specify 'desktop' or 'server'"
                 exit 1
                 ;;
esac

################################################
# Do generic init
################################################

if [ ! -d "/usr/home" ] ; then
   mkdir /usr/home
fi

# Setup /home link
if [ ! -e "/home" ] ; then
  ln -s /usr/home /home
fi

# Init the firewall
sh /usr/local/share/pcbsd/scripts/reset-firewall

# Update XDG menus
/usr/local/bin/pc-xdgutil updatemenu

# Update ports overlay files
/usr/local/bin/pc-extractoverlay ports

# Add our notice to top of loader.conf
if [ -e "/boot/loader.conf" ] ; then
  mv /boot/loader.conf /boot/loader.conf.tmp
fi

cat <<EOF >/boot/loader.conf
# * IMPORTANT NOTICE *
# Run 'grub-mkconfig -o /boot/grub/grub.cfg' after making changes to this file
###############################################################################
EOF

if [ -e "/boot/loader.conf.tmp" ] ; then
  cat /boot/loader.conf.tmp >> /boot/loader.conf
  rm /boot/loader.conf.tmp
fi

################################################
# Do desktop specific init
################################################
if [ "$1" = "desktop" ] ;then
  # Allow shutdown / reboot from hal
  polkit-action --set-defaults-any org.freedesktop.hal.power-management.shutdown yes
  polkit-action --set-defaults-any org.freedesktop.hal.power-management.reboot yes

  # Init the flash plugin for all users
  cd /home
  for i in `ls -d * 2>/dev/null`
  do
    su ${i} -c "flashpluginctl off"
    su ${i} -c "flashpluginctl on"
  done

  # Enable the system updater tray
  pbreg set /PC-BSD/SystemUpdater/runAtStartup true

  # Set running desktop
  pbreg set /PC-BSD/SysType PCBSD
  touch /etc/defaults/pcbsd
  chflags schg /etc/defaults/pcbsd

  # Init the desktop
  /usr/local/bin/pc-extractoverlay desktop --sysinit

  # Need to save a language?
  if [ -n "$2" ] ; then
     echo "$2" > /etc/pcbsd-lang
  fi
  exit $?
fi

################################################
# Do server specific init
################################################
if [ "$1" = "server" ] ; then
  # Set running a server
  pbreg set /PC-BSD/SysType TRUEOS
  touch /etc/defaults/trueos
  chflags schg /etc/defaults/trueos

  # Init the server
  /usr/local/bin/pc-extractoverlay server --sysinit
  exit $?
fi
