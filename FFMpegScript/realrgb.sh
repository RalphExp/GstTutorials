ffmpeg -f lavfi -i color=red -f lavfi -i color=lawngreen -f lavfi -i color=blue -filter_complex 'vstack=3' -f sdl tes
