# rgb video using padding

ffmpeg -f lavfi -i color=blue -vf 'scale=320:80[1]; [1]pad=320:160:0:80:lawngreen[2]; [2]pad=320:240:0:80:red' -f sdl test
