ffplay -f lavfi -i testsrc -vf 'split[1][2]; [1]pad=iw:ih*2[A]; [2]lutrgb=g=256[B]; [A][B]overlay=0:h'
