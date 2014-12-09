Solar
=====

Experiments with solar panels

To set up usb serial communication between the Pi and the Arduino:

Install arduino:
```sudo apt-get install arduino```

Give the pi user permission to use the serial connection:
```sudo usermod -a -G dialout pi```

Optionally install nfs for sharing the data log
From http://www.linuxuser.co.uk/tutorials/building-a-network-of-raspberry-pis

```sudo apt-get install nfs-kernel-server```

Make a shared directory
```sudo mkdir /public```

Edit /etc/exports
```/public *(rw,async,no_subtree_check)```

Ensure rpcbind starts on restart
```sudo update-rc.d rpcbind enable```

Connect to the share with
```
sudo mkdir /mnt/solar
sudo mount <SERVER_IP>:/public /mnt/solar
```
