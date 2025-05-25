#!/bin/bash
if [ -n "$1" ] && [ -n "$2" ]; then     
  DIR="$1"
  DIR_REALPATH=$(readlink -f "$DIR")
  FILENAME="${2}_$(date +%Y%m%d_%H%M%S).png"
else
  DIR_REALPATH="$HOME/Pictures"
  FILENAME="Screenshot_$(date +%Y%m%d_%H%M%S).png"
fi

xdotool key alt+Tab
gnome-screenshot -a -f "${DIR_REALPATH}/${FILENAME}"

feh "$DIR/$FILENAME" &

MAX_TRIES=3
COUNT=0
WIN_ID=""

while [[ -z "$WIN_ID" && $COUNT -lt $MAX_TRIES ]]; do
  sleep 1
  WIN_ID=$(xdotool search --class feh | head -1)
  ((COUNT++))
done

if [[ -z "$WIN_ID" ]]; then
  echo "Not found window feh after try $MAX_TRIES times !"
  exit 1
fi


wmctrl -i -r $WIN_ID -b add,above
xdotool windowactivate $WIN_ID
xdotool windowraise $WIN_ID