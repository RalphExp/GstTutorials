SHELL = /bin/bash

GSTFLAGS += -g $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-audio-1.0`)

G5FLAGS = $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-video-1.0  gtk+-3.0`)

all: $(shell \
	for i in  1 2 3 4 5 6 7 8 9 12 13 15; \
    do\
        echo basic-tutorial-$$i;\
    done;)

basic-tutorial-5: basic-tutorial-5.c
	gcc -o$@ $< $(G5FLAGS)

basic-tutorial-%: basic-tutorial-%.c
	gcc -o$@ $< $(GSTFLAGS)

clean: 
	@for i in {1..15}; do rm -rf basic-tutorial-$$i; done;
