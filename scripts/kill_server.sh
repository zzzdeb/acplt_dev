#!/usr/bin/env bash

for SERVERNAME in "$@"
do
    curl "http://localhost:"$SERVERNAME"/setVar?format=plain&path=/vendor/server_run%&newvalue=0"
done
