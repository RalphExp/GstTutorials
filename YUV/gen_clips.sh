if [ ! -e TestClips ]; then
  mkdir -p TestClips
fi

# all black pixels test
ffmpeg -y -f lavfi -i color=black -pix_fmt yuv420p -fs 0.1K TestClips/blacktest.mp4

# rgb test
ffmpeg -y -f lavfi -i rgbtestsrc -pix_fmt yuv420p -fs 0.1K TestClips/rgbtest.mp4
