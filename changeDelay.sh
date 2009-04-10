#!/bin/bash
sudo tc qdisc del dev lo root
sudo tc qdisc add dev lo root netem delay $1ms
