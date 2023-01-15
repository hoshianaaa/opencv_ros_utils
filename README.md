

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

# img_sub_mono8.py

- subscribe image topic and show ( mono8 )

## Subscribe Topics

- /image_raw(sensor_msgs/Image)

## Publish Topics

- None

## Example

```
rosrun opencv_ros_utils img_sub_mono8.py
```

remap subscribe image topic name to `usb_cam/image_raw`

```
rosrun opencv_ros_utils img_sub_mono8.py image_raw:=/usb_cam/image_raw
```

# bgr8_to_mono8.py

subscribe bgr8 image & publish gray image

## Subscribe Topics

- /image_raw(sensor_msgs/Image)

## Publish Topics

- /image_gray(sensor_msgs/Image)

## Example

```
rosrun opencv_ros_utils bgr8_to_mono8.py
```

remap subscribe image topic name to `usb_cam/image_raw`

```
rosrun opencv_ros_utils bgr8_to_mono8.py image_raw:=/usb_cam/image_raw
```

# mono8_to_hist.py

output histgram in ros topic

## Subscribe Topics

- /image_gray(sensor_msgs/Image)

## Publish Topics

- /histgram(sensor_msgs/Image)

## example

```
rosrun opencv_ros_utils momo8_to_histgram.py image_raw:=/usb_cam/image_raw
```

show in 

`rosrun rqt_iage_view rqt_image_view`

![Screenshot from 2023-01-15 13-20-34](https://user-images.githubusercontent.com/40942409/212522748-fbbc74d0-a2c2-40fa-a03e-b502bc465001.png)
