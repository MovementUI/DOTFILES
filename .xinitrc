setxkbmap hu &
exec dbus-launch &
xrdb ~/.Xresources &
/usr/bin/feh --bg-scale ~/Window-Manager/Accessories/bg.jpg &
unclutter -noevents -root -idle 5 &
statusbar &
xrandr --output DP-4 --mode 1920x1080 --rate 164.92 &
exec dwm > ~/.dwm.log
