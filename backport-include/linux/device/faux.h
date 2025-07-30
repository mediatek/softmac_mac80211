#ifndef __BACKPORT_DEVICE_FAUX_H
#define __BACKPORT_DEVICE_FAUX_H
#include <linux/version.h>
#if LINUX_VERSION_IS_GEQ(6,14,0)
#include_next <linux/device/faux.h>

#else
#include_next <linux/platform_device.h>

#ifndef faux_device
#define faux_device platform_device
#endif

#ifndef faux_device_destroy
static inline
void backport_faux_device_destroy(struct faux_device *faux_dev)
{
	platform_device_unregister(faux_dev);
}
#define faux_device_destroy LINUX_BACKPORT(faux_device_destroy)
#endif /* !faux_device_destroy */

#ifndef faux_device_create
static inline
struct faux_device *backport_faux_device_create(const char *name,
					        struct device *parent,
						const void *faux_ops)
{
	return platform_device_register_simple(name, 0, NULL, 0);
}
#define faux_device_create LINUX_BACKPORT(faux_device_create)
#endif /* !faux_device_create */

#endif /* >= 6.14 */

#endif
