ffmpeg -f lavfi -i color=blue -vf drawtext="text=helloworld:fontsize=40:fontcolor=yellow:x=-tw+t" -f sdl test

