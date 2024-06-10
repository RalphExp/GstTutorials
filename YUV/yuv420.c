#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>

void usage(char* argv[]) {
    fprintf(stderr, "usage: %s [absolute path of test clip]\n", argv[0]);
    exit(1);
}

int real_main (int argc, char *argv[]) {
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;
    GMainLoop *main_loop;

    if (argc == 1) {
        usage(argv);
    }

    /* Initialize GStreamer */
    gst_init (&argc, &argv);

    char* path = realpath(argv[1], NULL);
    char uri[1024] = {0};
    snprintf(uri, sizeof(uri), "playbin uri=file://%s", path);

    /* Build the pipeline */
    pipeline = gst_parse_launch(uri, NULL);

    // g_signal_connect (data.app_sink, "new-sample", G_CALLBACK (new_sample), &data)

    /* Start playing */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus (pipeline);
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


    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

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