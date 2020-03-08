#!/bin/sh
ADDR="$(readelf -s ../armgcc/_build/application.out | grep lcd_buffer | awk '{print $2}')"
echo "dump_image $PWD/screen.raw 0x$ADDR 15488" | telnet localhost 4444
sleep 1
convert -depth 1 -size 176x176 RGBA:screen.raw screen.png
rm screen.raw