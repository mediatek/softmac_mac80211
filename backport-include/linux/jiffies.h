#ifndef __BACKPORT_JIFFIES_H
#define __BACKPORT_JIFFIES_H
#include_next <linux/jiffies.h>
#include <linux/version.h>

#ifndef secs_to_jiffies

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

#endif /* secs_to_jiffies */

#endif /* __BACKPORT_JIFFIES_H */
