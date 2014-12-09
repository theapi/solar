Solar
=====

Experiments with solar panels

To set up usb serial communication between the Pi and the Arduino:

Install arduino:
```sudo apt-get install arduino```

Give the pi user permission to use the serial connection:
```sudo usermod -a -G dialout pi```

Optionally install nfs for sharing the data log
```sudo apt-get install nfs-kernel-server```

