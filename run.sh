#!/bin/sh

config_files=~/hdtn/tests/config_files
hdtn_config=$config_files/hdtn/hdtn_ingress1tcpcl_port4556_egress2tcpcl_port4557flowid1_port4558flowid2.json

cd ~/hdtn

#Router
./build/module/router/hdtn-router --contact-plan-file=contactPlan.json --dest-uri-eid=ipn:2.1 --hdtn-config-file=$hdtn_config & router_PID=$!
sleep 1
