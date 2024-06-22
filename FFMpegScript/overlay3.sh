ffplay -f lavfi -i testsrc -vf \
  'split=3[1][2][3];[1]pad=iw*3[A];[2]lutyuv=u=val*1.5[B];[3]lutyuv=v=val*1.5[C];[A][B]overlay=w[D];[D][C]overlay=w*2'
