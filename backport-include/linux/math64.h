#ifndef __BACKPORT_LINUX_MATH64_H
#define __BACKPORT_LINUX_MATH64_H
#include_next <linux/math64.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(5,9,0)
#define mul_u64_u64_div_u64 LINUX_BACKPORT(mul_u64_u64_div_u64)
extern u64 mul_u64_u64_div_u64(u64 a, u64 b, u64 c);
#endif /* < 5.9 */

#ifndef DIV_U64_ROUND_UP
#define DIV_U64_ROUND_UP(ll, d) \
	({ u32 _tmp = (d); div_u64((ll) + _tmp - 1, _tmp); })

static inline u64 roundup_u64(u64 x, u32 y)
{
	return DIV_U64_ROUND_UP(x, y) * y;
}
#endif /* DIV_U64_ROUND_UP */

#endif /* __BACKPORT_LINUX_MATH64_H */
