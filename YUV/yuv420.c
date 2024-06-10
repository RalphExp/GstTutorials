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


void on_eos(GstAppSink* appsink, gpointer user_data) {
    printf("============ eos ===============\n\n");
}

GstFlowReturn on_new_preroll(GstAppSink* appsink, gpointer user_data) {
    return GST_FLOW_OK;
}

GstFlowReturn on_new_sample(GstAppSink* appsink, gpointer user_data) {
    printf("on new frame\n");
    // GstAppSinkCallbacks callbacks;
    // callbacks.eos = NULL;
    // callbacks.new_event = NULL;
    // callbacks.new_preroll = NULL;
    // callbacks.new_sample = NULL;

    // gst_app_sink_set_callbacks(appsink, &callbacks, NULL, NULL);
    // CustomData* data = (CustomData* )user_data;
    // gst_element_set_state (data->pipeline, GST_STATE_NULL);
    return GST_FLOW_OK;
}

void app_sink_set_callback(GstElement* sink, CustomData* data) {
    GstAppSinkCallbacks callbacks = {0};
    callbacks.eos = on_eos;
    callbacks.new_preroll = on_new_preroll;
    callbacks.new_sample = on_new_sample;
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
    gst_init(&argc, &argv);

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
    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus(data.pipeline);
    gboolean terminate = FALSE;

    do {
        msg = gst_bus_pop_filtered (bus, GST_MESSAGE_STATE_CHANGED |
            GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

        /* Parse message */
        if (msg != NULL) {
            GError *err;
            gchar *debug_info;

            switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr ("Error received from element %s: %s\n",
                    GST_OBJECT_NAME (msg->src), err->message);
                g_printerr ("Debugging information: %s\n",
                    debug_info ? debug_info : "none");
                g_clear_error (&err);
                g_free (debug_info);
                terminate = TRUE;
                break;
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                terminate = TRUE;
                break;
            case GST_MESSAGE_STATE_CHANGED:
                /* We are only interested in state-changed messages from the pipeline */
                if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data.pipeline)) {
                    GstState old_state;
                    GstState new_state;
                    GstState pending_state;
                    gst_message_parse_state_changed (msg, &old_state, &new_state,
                        &pending_state);
                    g_print ("Pipeline state changed from %s to %s:\n",
                        gst_element_state_get_name (old_state),
                        gst_element_state_get_name (new_state));
                }
                break;
            default:
                /* We should not reach here */
                g_printerr ("Unexpected message received.\n");
                break;
            }
            gst_message_unref(msg);
        }
    } while (!terminate);

    /* Free resources */
    free(path);
    gst_message_unref (msg);
    gst_object_unref (bus);
    gst_element_set_state (data.pipeline, GST_STATE_NULL);
    gst_object_unref (data.pipeline);
    return 0;
}

int main(int argc, char *argv[]) {
#if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
  return gst_macos_main ((GstMainFunc) real_main, argc, argv, NULL);
#else
  return real_main (argc, argv);
#endif
}
