

# img_sub_bgr8.py

- subscribe image topic and show ( bgr8 )

## Subscribe Topics

- /image_raw(sensor_msgs/Image)

## Publish Topics

- None

## Example

```
rosrun opencv_ros_utils img_sub_bgr8.py
```

remap subscribe image topic name to `usb_cam/image_raw`

```
rosrun opencv_ros_utils img_sub_bgr8.py image_raw:=/usb_cam/image_raw
```

