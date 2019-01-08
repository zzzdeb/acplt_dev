#!/bin/bash
# no parameter: start server and load fbds
# parameter 1: 	-r --> restarts server
# 				-l --> only load and configure fbds
##### Custom paths #####
THISACPLTSYSTEM=$ACPLT_HOME
SERVERNAME="SRC"
SERVERPORT="7512"
SERVERIP="localhost"

##### Functions ######
. $THISACPLTSYSTEM/scripts/functions.sh

##### start Server #####
startServer $1
echo -e "Waiting 1sec for Server to register at MANAGER."
sleep 1

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
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.appMonitor.weights&newvalue=%7BGSE1%2610%7D%20%7BGSE2%2620%7D"

curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.actimode&newvalue=1"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.iexreq&newvalue=1"

nbIPs="\{localhost\}"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.mock.neighborIPs&newvalue="$nbIPs
servers="\{DST\}"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.mock.serverNames&newvalue="$servers

curl "http://"$SERVERIP":"$SERVERPORT"/createObject?format=plain&factory=/acplt/ov/domain&path=/TechUnits/GSE1"

curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.appMonitor.appPaths&newvalue=/TechUnits"
echo -e "\n\033[1;32mDone\033[m"
exit
