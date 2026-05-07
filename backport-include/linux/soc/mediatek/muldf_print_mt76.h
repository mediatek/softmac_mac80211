#ifndef __BACKPORT_MULDF_PRINT_MT76_H_
#define __BACKPORT_MULDF_PRINT_MT76_H_
#include <linux/printk.h>
#include <linux/dev_printk.h>
#include <linux/soc/mediatek/muldf.h>

#define MULDF_MODULE_NAME	"mt76"

#ifndef dev_fmt
#define dev_fmt(fmt) fmt
#endif

#define __muldf_print_unspec(level, fmt, ...)					\
	muldf_print(level, MULDF_MODULE_NAME, "UNSPEC", "UNSPEC", NULL,		\
		    __func__, __LINE__, fmt "\n", ##__VA_ARGS__)

#define __muldf_err(fmt, ...)	__muldf_print_unspec(MTK_MSG_ERROR, fmt, ##__VA_ARGS__)
#define __muldf_warn(fmt, ...)	__muldf_print_unspec(MTK_MSG_WARNING, fmt, ##__VA_ARGS__)
#define __muldf_info(fmt, ...)	__muldf_print_unspec(MTK_MSG_INFO, fmt, ##__VA_ARGS__)
#define __muldf_dbg(fmt, ...)	__muldf_print_unspec(MTK_MSG_DEBUG, fmt, ##__VA_ARGS__)

#undef dev_err
#define dev_err(dev, fmt, ...) do {	\
	__muldf_err(fmt, ##__VA_ARGS__);						\
	dev_printk_index_wrap(_dev_err, KERN_ERR, dev, dev_fmt(fmt), ##__VA_ARGS__);	\
} while (0)

#undef dev_warn
#define dev_warn(dev, fmt, ...) do {	\
	__muldf_warn(fmt, ##__VA_ARGS__);						\
	dev_printk_index_wrap(_dev_warn, KERN_WARNING, dev, dev_fmt(fmt), ##__VA_ARGS__); \
} while (0)

#undef dev_info
#define dev_info(dev, fmt, ...) do {	\
	__muldf_info(fmt, ##__VA_ARGS__);						\
	dev_printk_index_wrap(_dev_info, KERN_INFO, dev, dev_fmt(fmt), ##__VA_ARGS__);	\
} while (0)

#undef dev_dbg
#if defined(CONFIG_DYNAMIC_DEBUG) || \
	(defined(CONFIG_DYNAMIC_DEBUG_CORE) && defined(DYNAMIC_DEBUG_MODULE))
#define __dev_dbg(dev, fmt, ...)		\
	dynamic_dev_dbg(dev, dev_fmt(fmt), ##__VA_ARGS__)
#elif defined(DEBUG)
#define __dev_dbg(dev, fmt, ...)		\
	dev_printk(KERN_DEBUG, dev, dev_fmt(fmt), ##__VA_ARGS__)
#else
#define __dev_dbg(dev, fmt, ...)		\
	dev_no_printk(KERN_DEBUG, dev, dev_fmt(fmt), ##__VA_ARGS__)
#endif

#define dev_dbg(dev, fmt, ...) do {				\
	__muldf_dbg(fmt, ##__VA_ARGS__);			\
	__dev_dbg(dev, fmt, ##__VA_ARGS__);			\
} while (0)

#ifndef pr_fmt
#define pr_fmt(fmt) fmt
#endif

#undef pr_err
#define pr_err(fmt, ...) do {	\
	__muldf_err(fmt, ##__VA_ARGS__);				\
	printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__);			\
} while (0)

#undef pr_warn
#define pr_warn(fmt, ...) do {	\
	__muldf_warn(fmt, ##__VA_ARGS__);				\
	printk(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__);		\
} while (0)

#undef pr_info
#define pr_info(fmt, ...) do {	\
	__muldf_info(fmt, ##__VA_ARGS__);				\
	printk(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__);			\
} while (0)

#undef pr_debug
#if defined(CONFIG_DYNAMIC_DEBUG) || \
	(defined(CONFIG_DYNAMIC_DEBUG_CORE) && defined(DYNAMIC_DEBUG_MODULE))
#include <linux/dynamic_debug.h>

#define __pr_debug(fmt, ...)	\
	dynamic_pr_debug(fmt, ##__VA_ARGS__)
#elif defined(DEBUG)
#define __pr_debug(fmt, ...)	\
	printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
#else
#define __pr_debug(fmt, ...)	\
	no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
#endif

#define pr_debug(fmt, ...) do {	\
	__muldf_dbg(fmt, ##__VA_ARGS__);			\
	__pr_debug(fmt, ##__VA_ARGS__);				\
} while (0)

#endif /* __BACKPORT_MULDF_PRINT_MT76_H_ */
