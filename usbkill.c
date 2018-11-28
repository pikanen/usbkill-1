#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt
	// pr_fmt(fmt)  ==> KBUILD_MODNAME ": " fmt로 정의
	// pr_info("") 을 쓰기 위한 정의
#define N_ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))
	// N_ELEMENTS(array) ==> (sizeof(array)/sizeof((array)[0]))로 정의

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/reboot.h>
#include "config.h" // Whitelist

MODULE_LICENSE("GPL");

/*
 * returns 0 if no match, 1 if match
 *
 * Taken from drivers/usb/core/driver.c
 */

static int usb_match_device(struct usb_device *dev, const struct usb_device_id *id){
		//인자값으로 usb_device dev, usb_device_id id. 포인터
	if ((id->match_flags & USB_DEVICE_ID_MATCH_VENDOR) &&
			id->idVendor != le16_to_cpu(dev->descriptor.idVendor)) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_PRODUCT) &&
			id->idProduct != le16_to_cpu(dev->descriptor.idProduct)) { return 0; }

	/* No need to test id->bcdDevice_lo != 0, since 0 is never
	   greater than any unsigned number. */
	
	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_LO) &&
			(id->bcdDevice_lo > le16_to_cpu(dev->descriptor.bcdDevice))) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_HI) &&
			(id->bcdDevice_hi < le16_to_cpu(dev->descriptor.bcdDevice))) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_CLASS) &&
			(id->bDeviceClass != dev->descriptor.bDeviceClass)) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_SUBCLASS) &&
			(id->bDeviceSubClass != dev->descriptor.bDeviceSubClass)) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_PROTOCOL) &&
			(id->bDeviceProtocol != dev->descriptor.bDeviceProtocol)) { return 0; }

	return 1;
}

static void kill(struct usb_device *usb){	//kill 함수. kernel 종료시키는 함수.
		//인자값으로 구조체 usb_device *usb
	struct device *dev;	// 구조체 device *dev 선언

	// for문
	for (dev = &usb->dev; dev; dev = dev->parent){	// 현재 usb부터 dev 전체를 for문으로
		mutex_unlock(&dev->mutex);
	}
	printk("Powering off.\n");	//커널에 Powering off 출력
	kernel_power_off();	// 커널 종료.
}

static int usb_device_in_list(struct usb_device *dev,	// usb_device list 함수
		const struct usb_device_id *list,
		long unsigned int len) {
	long unsigned int i; 

	for(i = 0; i < len; i++) {
		if (usb_match_device(dev, &list[i])) {
			return 1;
		}
	}
	return 0;
}

static void is_device_in_whitelist(void *dev,int in_whitelist){	//whitelist에 속한 것인지 체크하는 함수
	if(in_whitelist){	//whitelist라면
		pr_info("Device is ignored.\n"); // printk와 같이 커널에 메세지 출력됨.

	}else{
		pr_info("Powering off.\n");	// printk와 같이 커널에 메세지 출력
		kill(dev);	// kill함수로 종료.
	}
}

static int notify(struct notifier_block *self, 
		unsigned long action, 
		void *dev) {
	if(action==USB_DEVICE_ADD){
		is_device_in_whitelist(dev,usb_device_in_list(dev,
					whitelist_add_devices,
					N_ELEMENTS(whitelist_add_devices)));
	}
	else if(action==USB_DEVICE_REMOVE){
		is_device_in_whitelist(dev,usb_device_in_list(dev,
					whitelist_remove_devices,
					N_ELEMENTS(whitelist_remove_devices)));
	}
	return 0;
}
static struct notifier_block usb_notify = {
	.notifier_call = notify,
};
static int __init mod_init(void) {
	usb_register_notify(&usb_notify);
	pr_info("Watching for USB devices...\n");
	return 0;
}
static void __exit mod_exit(void) {
	usb_unregister_notify(&usb_notify);
	pr_info("No longer watching for USB devices.\n");
}
module_init(mod_init);
module_exit(mod_exit);
