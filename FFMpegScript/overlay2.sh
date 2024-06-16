ffmpeg -f lavfi -i smptebars -vf 'movie=images/google.png[logo]; [in][logo]overlay=0:0' -f sdl logo
