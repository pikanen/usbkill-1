# usbkill 
`usbkill` is an anti-forensic LKM kill-switch that waits for a change on your usb ports and turns off your computer.

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

---------------------------------------------------------------------------------------------------------------------------------------

# usbkill 
usbkill 은 anti-forensic 적재 가능 커널 모듈 (Loadable kernel module, LKM) kill-switch로써 당신의 usb포트의 변화를 기다리고 변화가 생기면 컴퓨터를 종료시킨다.

실행 방법:

```shell
make
sudo insmod usbkill.ko
```

당신은 'linux-headers' 패키지의 설치가 필요할 것입니다. 만약 설치되어있지 않다면:

```shell
apt-get install linux-headers-$(uname -r)
```

usb 디바이스 목록

```shell
lsusb
```
