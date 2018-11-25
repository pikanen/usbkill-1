# usbkill 
`usbkill` is an anti-forensic LKM kill-switch that waits for a change on your usb ports and turns off your computer.
usbkill 은 anti-forensic 적재 가능 커널 모듈  ( Loadable kernel module ,  LKM ) kill-switch로서 당신의 usb포트의 변화를 기다리고 변화가 생기면 컴퓨터를 종료시킨다.
To run:

```shell
make
sudo insmod usbkill.ko
```

You will need to have the `linux-headers` package installed. If you haven't:

```shell
apt-get install linux-headers-$(uname -r)
```

List usb devices:

```shell
lsusb
```
