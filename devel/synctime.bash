#!/usr/bin/env bash

sudo /etc/init.d/chrony stop
#sleep 1s
sudo ntpdate polyu-v11-Pro
#sleep 1s
sudo /etc/init.d/chrony start
