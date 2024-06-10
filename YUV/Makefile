.PHONY: all

SHELL = /bin/bash

CC = gcc

GSTFLAGS += -g $(shell echo `pkg-config --cflags --libs gstreamer-1.0 gstreamer-video-1.0 gstreamer-audio-1.0`)

all: yuv420

yuv420: yuv420.c
	$(CC) -o$@ $(GSTFLAGS) $<
	
clean: 
	@echo cleaning...;
	@rm -rf yuv420
	@rm -rf *.dSYM;
	@echo done;

