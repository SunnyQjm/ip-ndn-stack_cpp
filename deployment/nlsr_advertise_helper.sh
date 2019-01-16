#!/bin/bash
ip=$1

sudo nlsrc advertise /IP/pre/${ip}
sudo nlsrc advertise /IP/${ip}
sudo nlsrc advertise /IP/TCP/pre/${ip}
sudo nlsrc advertise /IP/TCP/${ip}