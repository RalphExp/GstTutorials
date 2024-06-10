.PHONY: all basic playback ex

SHELL = /bin/bash

CC = gcc

GSTFLAGS += -g $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-video-1.0 gstreamer-audio-1.0 gtk+-3.0`)

G5FLAGS = $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-video-1.0 gtk+-3.0`)

G9FLAGS = $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-pbutils-1.0`)

G15FLAGS = $(shell echo `pkg-config --cflags --libs gstreamer-1.0 clutter-1.0 clutter-gst-3.0`)

all: basic playback ex

basic: $(shell \
    for i in {1..14}; \
    do\
	    if [ -e "basic-tutorial-$$i.c" ]; then\
            echo basic-tutorial-$$i;\
		fi\
    done;)

playback: $(shell \
    for i in {1..7}; \
    do\
        echo playback-tutorial-$$i;\
    done;)

basic-tutorial-5: basic-tutorial-5.c
	$(CC) -o$@ $< $(G5FLAGS)

basic-tutorial-9: basic-tutorial-9.c
	$(CC) -o$@ $< $(G9FLAGS)

basic-tutorial-15: basic-tutorial-15.c
	$(CC) -o$@ $< $(G15FLAGS)

basic-tutorial-%: basic-tutorial-%.c
	$(CC) -o$@ $< $(GSTFLAGS)

playback-tutorial-%: playback-tutorial-%.c
	$(CC) -o$@ $< $(GSTFLAGS)

ex: exercise-2

exercise-%: exercise-%.c
	$(CC) -o$@ $< $(GSTFLAGS)
	
clean: 
	@echo cleaning...
	@for i in {1..15}; do rm -rf basic-tutorial-$$i; done;
	@for i in {1..7}; do rm -rf playback-tutorial-$$i; done;
	@rm -rf exercise-2;
	@rm -rf *.dSYM;
	@echo done

