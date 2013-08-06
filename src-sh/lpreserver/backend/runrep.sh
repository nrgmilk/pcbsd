#!/bin/sh
# Do the replication for a specific dataset
######################################################################

# Set our vars
PROGDIR="/usr/local/share/lpreserver"

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

DATASET="${1}"
TIME="${2}"

if [ -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi

check_rep_task "$DATASET" "$TIME"
status=$?

# No replication was needed / done
if [ $DIDREP -eq 0 ] ; then exit 0 ; fi

if [ $status -eq 0 ] ; then
  title="Success"
else
  title="FAILED"
fi

case $EMAILMODE in
    ALL) email_msg "Automated Replication - $title" "`echo_queue_msg`" ;;
    *) if [ $status -ne 0 ] ; then
          email_msg "Automated Replication - $title" "`echo_queue_msg`"
       fi
       ;;
esac