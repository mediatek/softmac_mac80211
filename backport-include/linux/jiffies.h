#ifndef _BACKPORTS_LINUX_JIFFIES_H
#define _BACKPORTS_LINUX_JIFFIES_H 1

#include_next <linux/jiffies.h>

#if LINUX_VERSION_IS_LESS(6,13,0)
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
#define secs_to_jiffies(_secs) (unsigned long)((_secs) * HZ)
#endif

#endif /* _BACKPORTS_LINUX_JIFFIES_H */
