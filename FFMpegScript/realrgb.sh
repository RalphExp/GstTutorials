# ffmpeg -f lavfi -i color=red -f lavfi -i color=lawngreen -f lavfi -i color=blue -filter_complex 'vstack=3' -f sdl test

ffmpeg -f lavfi -i color=red -f lavfi -i color=lawngreen -f lavfi -i color=blue \
	-filter_complex "[0:v]scale=320:80,setsar=1:1[a]; [1:v]scale=320:80[b]; [2:v]scale=320:80[c]; [a][b][c]vstack=inputs=3" \
	-f sdl test


