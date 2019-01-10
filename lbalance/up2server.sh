#!/usr/bin/env sh

THISACPLTSYSTEM=${ACPLT_HOME}
SERVERNAME="GSE"
SERVERPORT="7509"
SERVERIP="localhost"

##### Functions ######
. $THISACPLTSYSTEM/scripts/functions.sh

##### Configure server ######

##### Load the fbds #####
loadFBD "basys/TechUnits_Topology-Model1"
#config Topologyagent #TODO write function for config
PALETTELIST="\{/TechUnits/GSE/PA001\}%20\{/TechUnits/GSE/PA002\}%20\{/TechUnits/GSE/PA003\}"
curl "http://"$SERVERIP":"$SERVERPORT"/setVar?format=plain&path=/TechUnits/GSE/TopologyAgent.PalettePaths&newvalue="$PALETTELIST

echo -e "\n\033[1;32mDone\033[m"
exit
