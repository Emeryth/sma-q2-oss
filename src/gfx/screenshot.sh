#!/bin/sh
#dump_image screen.raw 0x20004eb0 15488
echo "dump_image $PWD/screen.raw 0x20005010 15488" | telnet localhost 4444
sleep 1
convert -depth 1 -size 176x176 RGBA:screen.raw screen.png