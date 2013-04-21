#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>


#include "epautoconf.c"


/* Big enough to hold our biggest descriptor */
#define EP0_BUFSIZE     512


static struct usb_device_descriptor
device_desc = {
        .bLength =              sizeof device_desc,
        .bDescriptorType =      USB_DT_DEVICE,

        .bcdUSB =               cpu_to_le16(0x0200),
        .bDeviceClass =         USB_CLASS_VENDOR_SPEC,

        /* The next three values can be overridden by module parameters */
        .idVendor =             cpu_to_le16(0xabcd),
        .idProduct =            cpu_to_le16(0xdbca),
        .bcdDevice =            cpu_to_le16(0xffff),

        .bNumConfigurations =   1,
};


struct usbg_dev {
        struct usb_gadget       *gadget;        /* Copy of cdev->gadget */
      
	struct usb_ep           *ep0;           /* Copy of gadget->ep0 */
        struct usb_request      *ep0req;        /* Copy of cdev->req */
 
        struct usb_ep           *bulk_in;
        struct usb_request      *inreq;        /* Copy of cdev->req */

        struct usb_ep           *bulk_out;
        struct usb_request      *outreq;        /* Copy of cdev->req */
};     

static struct usbg_dev *_usbg_dev; 


static int usbg_setup(struct usb_gadget * , const struct usb_ctrlrequest * );
static void usbg_unbind(struct usb_gadget * );
static void usbg_disconnect(struct usb_gadget * );


static struct usb_gadget_driver usbg_driver = {
        .function 	= "usb gadget driver",     
        .speed 		= USB_SPEED_HIGH,
        .unbind 	= usbg_unbind,  
        .setup 		= usbg_setup,    
        .disconnect 	= usbg_disconnect,  

        .driver 	= {             
                .owner 	= THIS_MODULE,
                .name 	= "usb gadget driver"  
        },
};

static void ep0_complete(struct usb_ep *ep, struct usb_request *req)
{ 
	 printk(KERN_DEBUG"%s --> %d, %u/%u\n", __func__,
                                req->status, req->actual, req->length);
}

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
	int 			rc = 0;
	int			value;	
	int			dir = (ctrlreq->bRequestType & 0x80) ? 1 : 0;
	u16			w_index = le16_to_cpu(ctrlreq->wIndex);
        u16                     w_value = le16_to_cpu(ctrlreq->wValue);
        u16                     w_length = le16_to_cpu(ctrlreq->wLength);

	struct usb_request	*req = _usbg_dev->ep0req;

	printk(KERN_DEBUG"%s\n", __func__);
	printk(KERN_DEBUG"bRequestType %x bRequest %x wValue %x wIndex %d wLength %d\n"
		, ctrlreq->bRequestType, ctrlreq->bRequest, w_value, w_index, w_length);

	if(dir) {
		printk(KERN_DEBUG"req dir = IN");

		switch (ctrlreq->bRequest) {
		case USB_REQ_GET_DESCRIPTOR: 
			printk(KERN_DEBUG"GET_DES\n");
		  	switch (w_value >> 8) { 
			case USB_DT_DEVICE:
				printk(KERN_DEBUG"Get device descriptor\n");
				value = min(w_length, (u16)sizeof(struct usb_device_descriptor));
				req->length = value;
				memcpy(req->buf, &device_desc, value);
				value = usb_ep_queue(_usbg_dev->ep0, req, GFP_ATOMIC);
				if (value < 0) {
					req->status = 0;
					ep0_complete(_usbg_dev->ep0, req);			
				}
				break;
			}						
		}

	}	

	return rc;
}

static int __init usbg_bind(struct usb_gadget *gadget)
{
	int rc = 0;
	struct usb_request *req;

	printk(KERN_DEBUG"%s\n", __func__);
	
	_usbg_dev = kmalloc(sizeof(struct usbg_dev), GFP_KERNEL);
        if(!_usbg_dev) {
                rc = -ENOMEM;
                printk(KERN_ERR"kmalloc failed\n");
                goto err_release;
        }

        /*
         * usb_ep_autoconfig_reset - reset endpoint autoconfig state1
         */

        usb_ep_autoconfig_reset(gadget);

        _usbg_dev->gadget = gadget;
        _usbg_dev->ep0    = gadget->ep0;
        _usbg_dev->ep0->driver_data = _usbg_dev;

	_usbg_dev->ep0req = req = usb_ep_alloc_request(_usbg_dev->ep0, GFP_KERNEL);
	if (!req) {
		rc = -ENOMEM;
                printk(KERN_ERR"usb_ep_alloc_request failed\n");
                goto err_release;
	}

	req->buf = kmalloc(EP0_BUFSIZE, GFP_KERNEL);
        if (!req->buf) {
                rc = -ENOMEM;
                printk(KERN_ERR"unable to allocate req->buf\n");
                goto err_release;
	}
	
	req->complete = ep0_complete;	

err_release:

        return rc;
}

static int __init gad_init(void)
{
	printk(KERN_DEBUG"%s\n", __func__);
	return usb_gadget_probe_driver(&usbg_driver, usbg_bind);
}

static void __exit gad_exit(void)
{
	printk(KERN_DEBUG"%s\n", __func__);

	kfree(_usbg_dev->ep0req->buf);
	kfree(_usbg_dev);
	usb_gadget_unregister_driver(&usbg_driver);
}

module_init(gad_init);
module_exit(gad_exit);

MODULE_DESCRIPTION("Simple Gadget Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ankit Gupta <ankitgupta.work@gmail.com>");
