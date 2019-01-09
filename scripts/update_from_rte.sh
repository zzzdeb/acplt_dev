#!/bin/sh

RTE=$1

cp -r ${RTE}/build/acplt/system/sysbin/* ${ACPLT_HOME}/system/sysbin/
cp -r ${RTE}/build/acplt/system/sysdevbase/* ${ACPLT_HOME}/system/sysdevbase/
cp -r ${RTE}/build/acplt/system/syslibs/* ${ACPLT_HOME}/system/addonlibs/
cp -r ${RTE}/build/acplt/system/systools/* ${ACPLT_HOME}/system/systools/
cp -r ${RTE}/build/acplt/system/addonlibs/* ${ACPLT_HOME}/system/addonlibs
cp -r ${RTE}/core/ov/* ${ACPLT_HOME}/system/sysdevbase/ov/






