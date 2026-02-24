#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(5,10,0)
#include_next <linux/minmax.h>
#else
#include <linux/kernel.h>

static inline bool in_range64(u64 val, u64 start, u64 len)
{
	return (val - start) < len;
}

static inline bool in_range32(u32 val, u32 start, u32 len)
{
	return (val - start) < len;
}

/**
* in_range - Determine if a value lies within a range.
* @val: Value to test.
* @start: First value in range.
* @len: Number of values in range.
*
* This is more efficient than "if (start <= val && val < (start + len))".
* It also gives a different answer if @start + @len overflows the size of
* the type by a sufficient amount to encompass @val.  Decide for yourself
* which behaviour you want, or prove that start + len never overflow.
* Do not blindly replace one form with the other.
*/
#define in_range(val, start, len)                                       \
	((sizeof(start) | sizeof(len) | sizeof(val)) <= sizeof(u32) ?   \
		in_range32(val, start, len) : in_range64(val, start, len))
#endif
