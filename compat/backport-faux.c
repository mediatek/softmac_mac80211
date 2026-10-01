// SPDX-License-Identifier: GPL-2.0

#include <linux/device/faux.h>
#include <linux/export.h>
#include <linux/slab.h>

struct faux_platform_data {
	const struct faux_device_ops *faux_ops;
};

static int faux_driver_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct faux_platform_data *faux_data = dev->platform_data;
	const struct faux_device_ops *faux_ops = faux_data->faux_ops;

	if (faux_ops && faux_ops->probe)
		return faux_data->faux_ops->probe((struct faux_device *)pdev);

	return 0;
}

static void faux_driver_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct faux_platform_data *faux_data = dev->platform_data;
	const struct faux_device_ops *faux_ops = faux_data->faux_ops;

	if (faux_ops && faux_ops->remove)
		faux_data->faux_ops->remove((struct faux_device *)pdev);
}

#if LINUX_VERSION_IS_LESS(6,11,0)
static int faux_driver_remove_wrap(struct platform_device *pdev)
{
	faux_driver_remove(pdev);

	return 0;
}
#endif

/**
 * faux_device_create_with_groups - Create and register with the driver
 *		core a faux device and populate the device with an initial
 *		set of sysfs attributes.
 * @name:	The name of the device we are adding, must be unique for
 *		all faux devices.
 * @parent:	Pointer to a potential parent struct device.  If set to
 *		NULL, the device will be created in the "root" of the faux
 *		device tree in sysfs.
 * @faux_ops:	struct faux_device_ops that the new device will call back
 *		into, can be NULL.
 * @groups:	The set of sysfs attributes that will be created for this
 *		device when it is registered with the driver core.
 *
 * Create a new faux device and register it in the driver core properly.
 * If present, callbacks in @faux_ops will be called with the device that
 * for the caller to do something with at the proper time given the
 * device's lifecycle.
 *
 * Note, when this function is called, the functions specified in struct
 * faux_ops can be called before the function returns, so be prepared for
 * everything to be properly initialized before that point in time.
 *
 * Return:
 * * NULL if an error happened with creating the device
 * * pointer to a valid struct faux_device that is registered with sysfs
 */
struct faux_device *
backport_faux_device_create_with_groups(const char *name,
					struct device *parent,
					const struct faux_device_ops *faux_ops,
					const struct attribute_group **groups)
{
	struct faux_platform_data *faux_data;
	struct platform_driver *drv;
	struct device_driver *driver;
	struct platform_device *pdev;
	struct device *dev;
	int ret;

	drv = kzalloc(sizeof(*drv), GFP_KERNEL);
	if (!drv)
		return NULL;

	drv->probe = faux_driver_probe;
#if LINUX_VERSION_IS_GEQ(6,11,0)
	drv->remove = faux_driver_remove;
#else
	drv->remove = faux_driver_remove_wrap;
#endif

	driver = &drv->driver;
	driver->name = kstrdup(name, GFP_KERNEL);
	driver->probe_type = PROBE_FORCE_SYNCHRONOUS,
	driver->suppress_bind_attrs = true,

	ret = platform_driver_register(drv);
	if (ret) {
		pr_err("%s: device_add for faux driver '%s' failed with %d\n",
		       __func__, name, ret);
		return NULL;
	}

	pdev = platform_device_alloc(name, 0);
	dev = &pdev->dev;

	faux_data = kzalloc(sizeof(*faux_data), GFP_KERNEL);
	if (!faux_data)
		return NULL;

	faux_data->faux_ops = faux_ops;

	dev->platform_data = faux_data;
	if (parent)
		dev->parent = parent;
	else
		dev->parent = &platform_bus;
	dev->bus = &platform_bus_type;
	dev->groups = groups;
	dev_set_name(dev, "%s", name);

	ret = device_add(dev);
	if (ret) {
		pr_err("%s: device_add for faux device '%s' failed with %d\n",
		       __func__, name, ret);
		put_device(dev);
		driver_unregister(driver);
		return NULL;
	}

	return (struct faux_device *)pdev;
}
EXPORT_SYMBOL_GPL(backport_faux_device_create_with_groups);

/**
 * faux_device_create - create and register with the driver core a faux device
 * @name:	The name of the device we are adding, must be unique for all
 *		faux devices.
 * @parent:	Pointer to a potential parent struct device.  If set to
 *		NULL, the device will be created in the "root" of the faux
 *		device tree in sysfs.
 * @faux_ops:	struct faux_device_ops that the new device will call back
 *		into, can be NULL.
 *
 * Create a new faux device and register it in the driver core properly.
 * If present, callbacks in @faux_ops will be called with the device that
 * for the caller to do something with at the proper time given the
 * device's lifecycle.
 *
 * Note, when this function is called, the functions specified in struct
 * faux_ops can be called before the function returns, so be prepared for
 * everything to be properly initialized before that point in time.
 *
 * Return:
 * * NULL if an error happened with creating the device
 * * pointer to a valid struct faux_device that is registered with sysfs
 */
struct faux_device *backport_faux_device_create(const char *name,
						struct device *parent,
						const struct faux_device_ops *faux_ops)
{
	return backport_faux_device_create_with_groups(name, parent, faux_ops, NULL);
}
EXPORT_SYMBOL_GPL(backport_faux_device_create);

/**
 * faux_device_destroy - destroy a faux device
 * @faux_dev:	faux device to destroy
 *
 * Unregisters and cleans up a device that was created with a call to
 * faux_device_create()
 */
void backport_faux_device_destroy(struct faux_device *faux_dev)
{
	struct device_driver *driver;
	struct platform_driver *drv;
	struct device *dev;

	if (!faux_dev)
		return;

	dev = &faux_dev->dev;
	driver = dev->driver;
	drv = to_platform_driver(driver);

	device_del(dev);

	/* The final put_device() will clean up the memory we allocated for this device. */
	put_device(dev);

	platform_driver_unregister(drv);
	kfree(drv);
}
EXPORT_SYMBOL_GPL(backport_faux_device_destroy);
