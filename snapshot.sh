#!/bin/sh

gst-launch -v --gst-debug=2 v4l2src num-buffers=1 \
  ! video/x-raw-yuv, width=640, height=480 \
  ! filesink location=input.yuv


