#ifndef __BACKPORT_DEVICE_FAUX_H
#define __BACKPORT_DEVICE_FAUX_H

#if LINUX_VERSION_IS_GEQ(6,14,0)
#include_next <linux/device/faux.h>
#else

#include <linux/container_of.h>
#include <linux/device.h>
#include <linux/platform_device.h>

/**
 * struct faux_device - a "faux" device
 * @dev:	internal struct device of the object
 *
 * A simple faux device that can be created/destroyed.  To be used when a
 * driver only needs to have a device to "hang" something off.  This can be
 * used for downloading firmware or other basic tasks.  Use this instead of
 * a struct platform_device if the device has no resources assigned to
 * it at all.
 */
#define faux_device platform_device
#define to_faux_device(x) container_of_const((x), struct faux_device, dev)

/**
 * struct faux_device_ops - a set of callbacks for a struct faux_device
 * @probe:	called when a faux device is probed by the driver core
 *		before the device is fully bound to the internal faux bus
 *		code.  If probe succeeds, return 0, otherwise return a
 *		negative error number to stop the probe sequence from
 *		succeeding.
 * @remove:	called when a faux device is removed from the system
 *
 * Both @probe and @remove are optional, if not needed, set to NULL.
 */
struct faux_device_ops {
	int (*probe)(struct faux_device *faux_dev);
	void (*remove)(struct faux_device *faux_dev);
};

#define faux_device_create LINUX_BACKPORT(faux_device_create)
struct faux_device *backport_faux_device_create(const char *name,
				       struct device *parent,
				       const struct faux_device_ops *faux_ops);
#define faux_device_create_with_groups LINUX_BACKPORT(faux_device_create_with_groups)
struct faux_device *backport_faux_device_create_with_groups(const char *name,
						   struct device *parent,
						   const struct faux_device_ops *faux_ops,
						   const struct attribute_group **groups);
#define faux_device_destroy LINUX_BACKPORT(faux_device_destroy)
void backport_faux_device_destroy(struct faux_device *faux_dev);

static inline void *faux_device_get_drvdata(const struct faux_device *faux_dev)
{
	return dev_get_drvdata(&faux_dev->dev);
}

static inline void faux_device_set_drvdata(struct faux_device *faux_dev, void *data)
{
	dev_set_drvdata(&faux_dev->dev, data);
}

#endif

#endif /* __BACKPORT_DEVICE_FAUX_H */
