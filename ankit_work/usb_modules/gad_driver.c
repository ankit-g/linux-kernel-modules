#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>

static int usbg_setup(struct usb_gadget * , const struct usb_ctrlrequest * );
static void usbg_unbind(struct usb_gadget * );
static void usbg_disconnect(struct usb_gadget * );


static struct usb_gadget_driver usbg_driver = {
        .function = "usb gadget driver",     
        .speed = USB_SPEED_HIGH,
        .unbind = usbg_unbind,  
        .setup = usbg_setup,    
        .disconnect = usbg_disconnect,  
        .driver = {             
                .owner = THIS_MODULE,
                .name = "usb gadget driver"  
        },
};

static void usbg_disconnect(struct usb_gadget *gadget)
{
	printk(KERN_DEBUG"%s\n", __func__);
}

static void usbg_unbind(struct usb_gadget * gadget)
{
	printk(KERN_DEBUG"%s\n", __func__);
}

static int usbg_setup(struct usb_gadget * gadget, const struct usb_ctrlrequest * ctrlreq)
{
	printk(KERN_DEBUG"%s\n", __func__);
	printk("bRequestType %x bRequest %x wValue %d wIndex %d wLength %d\n"
		, ctrlreq->bRequestType, ctrlreq->bRequest, ctrlreq->wValue
		, ctrlreq->wIndex, ctrlreq->wLength);


	return 0;
}

static int __init usbg_bind(struct usb_gadget *gadget)
{
	printk(KERN_DEBUG"%s\n", __func__);
	return 0;
}

static int __init gad_init(void)
{
	printk(KERN_DEBUG"%s\n", __func__);
	return usb_gadget_probe_driver(&usbg_driver, usbg_bind);
}

static void __exit gad_exit(void)
{
	printk(KERN_DEBUG"%s\n", __func__);
	usb_gadget_unregister_driver(&usbg_driver);
}

module_init(gad_init);
module_exit(gad_exit);

MODULE_DESCRIPTION("Hello X Module");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kopal <kopalsingh.work@gmail.com>");
