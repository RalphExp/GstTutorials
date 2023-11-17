.PHONY: all basic playback

SHELL = /bin/bash

GSTFLAGS += -g $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-1.0 gstreamer-video-1.0 gstreamer-audio-1.0`)

G5FLAGS = $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-video-1.0 gtk+-3.0`)

G9FLAGS = $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-pbutils-1.0`)

all: basic playback

basic: $(shell \
    for i in {1..15}; \
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
	gcc -o$@ $< $(G5FLAGS)

basic-tutorial-9: basic-tutorial-9.c
	gcc -o$@ $< $(G9FLAGS)

basic-tutorial-15: basic-tutorial-15.c
	gcc -o$@ $< -I/usr/include/clutter-1.0 \
        -I/usr/include/clutter-gst-3.0\
		-I/usr/include/cairo\
        -I/usr/include/cogl\
		-I/usr/include/atk-1.0\
		-I/usr/include/json-glib-1.0\
		-I/usr/include/gdk-pixbuf-2.0\
        -I/usr/include/pango-1.0 -lclutter-1.0 -lclutter-gst-3.0 $(GSTFLAGS)

basic-tutorial-%: basic-tutorial-%.c
	gcc -o$@ $< $(GSTFLAGS)

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

