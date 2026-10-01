#ifndef __BACKPORT_TIMEKEEPING_H
#define __BACKPORT_TIMEKEEPING_H
#include <linux/version.h>
#include <linux/types.h>

#include_next <linux/timekeeping.h>

#if LINUX_VERSION_IS_LESS(5,3,0)
#define ktime_get_boottime_ns ktime_get_boot_ns
#define ktime_get_coarse_boottime_ns ktime_get_boot_ns
#endif /* < 5.3 */

#if LINUX_VERSION_IS_LESS(4,18,0)
extern time64_t ktime_get_boottime_seconds(void);

#define ktime_get_raw_ts64 LINUX_BACKPORT(ktime_get_raw_ts64)
static inline void ktime_get_raw_ts64(struct timespec64 *ts)
{
	return getrawmonotonic64(ts);
}
#endif /* < 4.18 */

#if LINUX_VERSION_IS_LESS(7,2,0)
/*
 * struct system_device_crosststamp gained a clock_id field in 7.2 and
 * sys_realtime was renamed to sys_systime. Older kernels only ever request
 * CLOCK_REALTIME cross timestamps.
 */
static inline clockid_t
crosststamp_clock_id(struct system_device_crosststamp *xtstamp)
{
	return CLOCK_REALTIME;
}

static inline void
crosststamp_set_systime(struct system_device_crosststamp *xtstamp,
			ktime_t systime)
{
	xtstamp->sys_realtime = systime;
}
#else
static inline clockid_t
crosststamp_clock_id(struct system_device_crosststamp *xtstamp)
{
	return xtstamp->clock_id;
}

static inline void
crosststamp_set_systime(struct system_device_crosststamp *xtstamp,
			ktime_t systime)
{
	xtstamp->sys_systime = systime;
}
#endif /* < 7.2 */

#endif /* __BACKPORT_TIMEKEEPING_H */
