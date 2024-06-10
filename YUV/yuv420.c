#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *sink;
    GMainLoop *main_loop;         /* GLib's Main Loop */
} CustomData;

void usage(char* argv[]) {
    fprintf(stderr, "usage: %s [absolute path of test clip]\n", argv[0]);
    exit(1);
}

GstFlowReturn new_preroll_cb(GstAppSink* appsink, gpointer user_data) {
    return GST_FLOW_OK;
}

GstFlowReturn new_sample_cb(GstAppSink* appsink, gpointer user_data) {
    printf("on new frame\n");
    GstAppSinkCallbacks callbacks;
    callbacks.eos = NULL;
    callbacks.new_event = NULL;
    callbacks.new_preroll = NULL;
    callbacks.new_sample = NULL;

    gst_app_sink_set_callbacks(appsink, &callbacks, NULL, NULL);
    CustomData* data = (CustomData* )user_data;
    gst_element_set_state (data->pipeline, GST_STATE_NULL);
    return GST_FLOW_OK;
}

void app_sink_set_callback(GstElement* sink, CustomData* data) {
    GstAppSinkCallbacks callbacks;
    callbacks.eos = NULL;
    callbacks.new_event = NULL;
    callbacks.new_preroll = new_preroll_cb;
    callbacks.new_sample = new_sample_cb;
    gst_app_sink_set_callbacks(GST_APP_SINK(sink), &callbacks, data, NULL);
}

int real_main (int argc, char *argv[]) {
    GstBus *bus;
    GstMessage *msg;
    CustomData data;

    if (argc == 1) {
        usage(argv);
    }

    /* Initialize GStreamer */
    gst_init (&argc, &argv);

    char* path = realpath(argv[1], NULL);
    char pipeline[1024] = {0};
    snprintf(pipeline, sizeof(pipeline)-1, 
        "uridecodebin name=src uri=file://%s ! appsink name=sink", path);
    
    printf("pipeline = %s\n\n", pipeline);

    /* Create the empty pipeline */
    data.pipeline = gst_parse_launch(pipeline, NULL);
    data.sink = gst_bin_get_by_name(GST_BIN(data.pipeline), "sink");

    if (!data.pipeline || !data.sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    app_sink_set_callback(data.sink, &data);

    /* Start playing */
    gst_element_set_state (data.pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus (data.pipeline);
    msg =
        gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
        GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* See next tutorial for proper error message handling/parsing */
    if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
        g_error ("An error occurred! Re-run with the GST_DEBUG=*:WARN environment "
        "variable set for more details.");
    }

    /* Free resources */
    gst_message_unref (msg);
    gst_object_unref (bus);

    gst_element_set_state (data.pipeline, GST_STATE_NULL);
    gst_object_unref (data.pipeline);

    free(path);
    return 0;
}

int main(int argc, char *argv[]) {
#if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
  return gst_macos_main ((GstMainFunc) real_main, argc, argv, NULL);
#else
  return real_main (argc, argv);
#endif
}