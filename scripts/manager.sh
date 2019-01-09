#!/bin/bash
# no parameter: start server and load fbds
# parameter 1: 	-r --> restarts server
# 				-l --> only load and configure fbds
##### Custom paths #####
THISACPLTSYSTEM=$ACPLT_HOME
SERVERNAME="MANAGER"
SERVERPORT="7509"
SERVERIP="localhost"

##### Functions ######
. $ACPLT_HOME/scripts/functions.sh

##### start Server #####
startServer $1

echo -e "\n\033[1;32mDone\033[m"
exit
