#!/bin/bash
# no parameter: start server and load fbds
# parameter 1: 	-r --> restarts server
# 				-l --> only load and configure fbds
##### Custom paths #####
THISACPLTSYSTEM=$ACPLT_HOME
SERVERNAME="MANAGER"
SERVERPORT="7509"
SERVERIP="192.168.3.145"
MYIP="192.168.3.145"
DST1="192.168.3.146"
DST2="192.168.3.15"
##### Functions ######
#. $THISACPLTSYSTEM/scripts/functions.sh

##### start Server #####
#startServer $1
echo -e "Waiting 1sec for Server to register at MANAGER."
sleep 1

##### Configure server ######
# Set cycletime
# echo -e "Setting cyctime to 0.05"
# curl "http://localhost:"$SERVERPORT"/setVar?format=plain&path=/Tasks/UrTask.cyctime&newvalue=0.05"
# echo -e "\n"


##### Load the fbds #####
# loadFBD "lbalance/SRC.fdb"
#config Topologyagent #TODO write function for config
# PALETTELIST="\{/TechUnits/GSE/PA001\}%20\{/TechUnits/GSE/PA002\}%20\{/TechUnits/GSE/PA003\}"
# curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/GSE/TopologyAgent.PalettePaths&newvalue="$PALETTELIST
# weights 

curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.mock.selfHost&newvalue=$MYIP"

curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.mock.actimode&newvalue=1"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.mock.iexreq&newvalue=1"
servers="\{$DST1/MANAGER\t0\t$DST1\t7509\tMANAGER\}%20\{$DST2/MANAGER\t0\t$DST2\t7509\tMANAGER\}"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.mock.ovServers&newvalue="$servers
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.nbInformer.myIP%&newvalue=$MYIP"

curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.actimode&newvalue=1"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.iexreq&newvalue=1"

curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.reqSender.sender.queueLength&newvalue=20"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/data/lbalance/BRAD.nbInformer.broadcaster.queueLength&newvalue=20"
echo -e "\n\033[1;32mDone\033[m"
exit
