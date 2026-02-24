#ifndef __BACKPORT_JIFFIES_H
#define __BACKPORT_JIFFIES_H
#include_next <linux/jiffies.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(6,12,0)

/**
 * secs_to_jiffies: - convert seconds to jiffies
 * @_secs: time in seconds
 *
 * Conversion is done by simple multiplication with HZ
 *
 * secs_to_jiffies() is defined as a macro rather than a static inline
 * function so it can be used in static initializers.
 *
 * Return: jiffies value
 */
#define secs_to_jiffies(_secs) ((_secs) * HZ)

#endif /* < 6.12.0 */

#endif /* __BACKPORT_JIFFIES_H */
