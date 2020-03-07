#!/bin/sh
convert -depth 1 icons_font.png RGBA:icons.dat
xxd -i icons.dat > icon_data.c
sed -i 's/unsigned char/const unsigned char/g' icon_data.c

convert -depth 1 weather_font.png RGBA:weather.dat
xxd -i weather.dat > weather_data.c
sed -i 's/unsigned char/const unsigned char/g' weather_data.c