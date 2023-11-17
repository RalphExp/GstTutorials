.PHONY: all basic playback

SHELL = /bin/bash

GSTFLAGS += -g $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-1.0 gstreamer-video-1.0 gstreamer-audio-1.0`)

G5FLAGS = $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-video-1.0 gtk+-3.0`)

G9FLAGS = $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-pbutils-1.0`)

all: basic playback

basic: $(shell \
	for i in  1 2 3 4 5 6 7 8 9 12 13; \
    do\
        echo basic-tutorial-$$i;\
    done;)

basic-tutorial-5: basic-tutorial-5.c
	gcc -o$@ $< $(G5FLAGS)

basic-tutorial-9: basic-tutorial-9.c
	gcc -o$@ $< $(G9FLAGS)

basic-tutorial-%: basic-tutorial-%.c
	gcc -o$@ $< $(GSTFLAGS)

playback: $(shell for i in {1..7}; do echo playback-tutorial-$$i; done;)

playback-tutorial-%: playback-tutorial-%.c
	gcc -o$@ $< $(GSTFLAGS)

ex: exercise-2

exercise-%: exercise-%.c
	gcc -o$@ $< $(GSTFLAGS)
	
clean: 
	@echo cleaning...
	@for i in {1..15}; do rm -rf basic-tutorial-$$i; done;
	@for i in {1..7}; do rm -rf playback-tutorial-$$i; done;
	@rm -rf exercise-2;
	@echo done

