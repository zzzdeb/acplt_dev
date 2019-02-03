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

##### Custom paths #####
THISACPLTSYSTEM=$ACPLT_HOME
SERVERNAME="SRC"
SERVERPORT="7512"
SERVERIP="localhost"


##### start Server #####
startServer $1
echo -e "Waiting 1sec for Server to register at MANAGER."
sleep 4

##### Configure server ######
# Set cycletime
# echo -e "Setting cyctime to 0.05"
# curl "http://localhost:"$SERVERPORT"/setVar?format=plain&path=/Tasks/UrTask.cyctime&newvalue=0.05"
# echo -e "\n"


##### Load the fbds #####
# loadFBD "lbalance/SRC"
#config Topologyagent #TODO write function for config
# PALETTELIST="\{/TechUnits/GSE/PA001\}%20\{/TechUnits/GSE/PA002\}%20\{/TechUnits/GSE/PA003\}"

#weights

THISACPLTSYSTEM=$ACPLT_HOME
SERVERNAME="MANAGER"
SERVERPORT="7509"
SERVERIP="localhost"

curl "http://"$SERVERIP":"$SERVERPORT"/createObject?format=plain&factory=/acplt/CTree/Transport&path=/TechUnits/obj"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/CTree/Transport.targetKS&newvalue=//localhost/SRC/TechUnits/obj1"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/CTree/Transport.path&newvalue=/TechUnits/obj"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/CTree/Transport.trigger&newvalue=1"

echo -e "\n\033[1;32mDone\033[m"
exit
