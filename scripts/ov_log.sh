#!/usr/bin/env sh

tail -f $ACPLT_HOME/servers/$1/logfiles/log_start_server.txt \
    | sed "s,\[ACPLT/OV Error].*,$(tput setaf 1)&$(tput sgr0),;\
    s,\[ACPLT/OV Warning].*,$(tput setaf 3)&$(tput sgr0),;\
    s,\[ACPLT/OV Debug].*,$(tput setaf 4)&$(tput sgr0),; \
    s,TEST(.*PASS,$(tput setaf 2)&$(tput sgr0),;" \
2>&1 | perl -ne 'if (/file truncated/) {system 'clear'; print} else {print}'
