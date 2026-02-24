#ifndef __LINUX_BACKPORT_WORKQUEUE_H
#define __LINUX_BACKPORT_WORKQUEUE_H
#include <linux/version.h>
#include_next <linux/workqueue.h>

#if LINUX_VERSION_IS_LESS(6,18,0)
#define system_dfl_wq system_unbound_wq
#endif

#endif
