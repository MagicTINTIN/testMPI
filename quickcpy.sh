#!/bin/env bash

PREF="10.1.15"
echo $PREF
echo "##################################### $PREF.$1 #################################################"
ssh-keygen -f "/home/serviere/.ssh/known_hosts" -R "$PREF.$1"
ssh-copy-id $PREF.$1
