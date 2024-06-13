#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video-info.h>

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *sink;
    GMainLoop *main_loop;         /* GLib's Main Loop */
} CustomData;

void usage(char* argv[]) {
    fprintf(stderr, "usage: %s [absolute path of test clip]\n", argv[0]);
    exit(1);
}

void handle_yuv_sample(GstSample* sample) {
    GstBuffer* buffer = gst_sample_get_buffer(sample);
    if (!buffer) {
        printf("not buffer in sample\n");
        return;
    }

    GstCaps* caps = gst_sample_get_caps(sample);
    GstVideoInfo video_info;
    gst_video_info_init(&video_info);
    
    if (caps) {
        gst_video_info_from_caps(&video_info, caps);
        gst_caps_unref(caps);
    }

    GstMapInfo map_info;
    if (gst_buffer_map(buffer, &map_info, GST_MAP_READ)) {
        guint8* data = map_info.data;
        printf("data size: %ld, width: %u, height: %u\n", 
            map_info.size, video_info.width, video_info.height);

        for (int i = 0; i < map_info.size; ++i) {
            if ((i % video_info.width) == 0)  {
                printf("\n");
            }
            printf("%4d", data[i]);
        }
        printf("\n");
        gst_buffer_unref(buffer);
    }
}

GstFlowReturn on_new_preroll(GstAppSink* appsink, gpointer user_data) {
    return GST_FLOW_OK;
}

GstFlowReturn on_new_sample(GstAppSink* appsink, gpointer user_data) {
    printf("on new frame\n");
    GstSample* sample = gst_app_sink_pull_sample(appsink);
    if (sample) {
        handle_yuv_sample(sample);
        gst_sample_unref(sample);
    }
    return GST_FLOW_OK;
}

void app_sink_set_callback(GstElement* sink, CustomData* data) {
    GstAppSinkCallbacks callbacks = {0};
    callbacks.new_preroll = on_new_preroll;
    callbacks.new_sample = on_new_sample;
    gst_app_sink_set_callbacks(GST_APP_SINK(sink), &callbacks, data, NULL);
}

int real_main (int argc, char *argv[]) {
    CustomData data;

    if (argc == 1) {
        usage(argv);
    }

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    char pipeline[1024] = {0};
    snprintf(pipeline, sizeof(pipeline)-1, 
        "filesrc location=%s ! decodebin ! appsink name=sink", argv[1]);
    
    printf("pipeline = %s\n\n", pipeline);

    /* Create the empty pipeline */
    data.pipeline = gst_parse_launch(pipeline, NULL);
    // data.source = gst_bin_get_by_name(GST_BIN(data.pipeline), "src");
    data.sink = gst_bin_get_by_name(GST_BIN(data.pipeline), "sink");

    if (!data.pipeline || !data.sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }
    
    // app_sink_set_callback(data.sink, &data);

    /* Start playing */
    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    GstSample* sample = gst_app_sink_pull_sample((GstAppSink*)data.sink);
    if (sample) {
        handle_yuv_sample(sample);
        gst_sample_unref(sample);
    }

    // gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.sink);
    gst_object_unref(data.pipeline);
    return 0;
}

int main(int argc, char *argv[]) {
#if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
  return gst_macos_main ((GstMainFunc) real_main, argc, argv, NULL);
#else
  return real_main(argc, argv);
#endif
}
