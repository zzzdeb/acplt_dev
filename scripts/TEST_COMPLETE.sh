#!/bin/bash
# no parameter: start server and load fbds
# parameter 1: 	-r --> restarts server
# 				-l --> only load and configure fbds
##### Custom paths #####
MAIN_ACPLT=$ACPLT_HOME

THISACPLTSYSTEM=$ACPLT_HOME
SERVERNAME="MANAGER"
SERVERPORT="7509"
SERVERIP="localhost"

##### Functions ######
. $ACPLT_HOME/scripts/functions.sh

##### start Server #####
startServer $1

echo -e "\n\033[1;32mDone\033[m"

curl "http://"$SERVERIP":"$SERVERPORT"/createObject?format=plain&factory=/acplt/sync/dummy&path=/TechUnits/GSE1"
curl "http://"$SERVERIP":"$SERVERPORT"/createObject?format=plain&factory=/acplt/sync/dummy&path=/TechUnits/GSE1_1"
##### Custom paths #####
# THISACPLTSYSTEM=$ACPLT_HOME/../acplt.test/
THISACPLTSYSTEM=$ACPLT_HOME
# export ACPLT_HOME=$THISACPLTSYSTEM
SERVERNAME="SRC"
SERVERPORT="7512"
SERVERIP="localhost"

# cp $MAIN_ACPLT/system/addonlibs/* $THISACPLTSYSTEM/system/addonlibs/
# rm $THISACPLTSYSTEM/system/addonlibs/sync.so

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

# THISACPLTSYSTEM=$ACPLT_HOME
# SERVERNAME="MANAGER"
# SERVERPORT="7509"
# SERVERIP="localhost"



# curl "http://"$SERVERIP":"$SERVERPORT"/createObject?format=plain&factory=/acplt/fbcomlib/getVar&path=/TechUnits/fbGetvar"
# curl "http://"$SERVERIP":"$SERVERPORT"/link?format=plain&path=/TechUnits/fbGetvar.taskparent&element=/Tasks/UrTask"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/fbGetvar.path&newvalue=/TechUnits/fbGetvarClient.IN1"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/fbGetvar.server&newvalue=MANAGER"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/fbGetvar.doCyclic&newvalue=1"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/fbGetvar.iexreq&newvalue=1"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/fbGetvar.actimode&newvalue=1"

# curl "http://"$SERVERIP":"$SERVERPORT"/createObject?format=plain&factory=/acplt/sync/dsync&path=/TechUnits/dsync"
# curl "http://"$SERVERIP":"$SERVERPORT"/link?format=plain&path=/TechUnits/dsync.taskparent&element=/Tasks/UrTask"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/dsync.destKS&newvalue=//localhost/MANAGER"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/dsync.srcPath&newvalue=/TechUnits/fbGetvar"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/dsync.selfHost&newvalue=localhost"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/dsync.iexreq&newvalue=1"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&plainath=/TechUnits/dsync.trigger&newvalue=1"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/CTree/Transport.path&newvalue=/TechUnits/dsyncDst"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/CTree/Transport.trigger&newvalue=1"

echo -e "\n\033[1;32mDone\033[m"
exit

