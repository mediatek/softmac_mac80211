#ifndef __BACKPORT_SOCKET_H
#define __BACKPORT_SOCKET_H
#include <linux/version.h>
#include_next <linux/socket.h>

#ifndef SOL_NFC
/*
 * backport SOL_NFC -- see commit:
 * NFC: llcp: Implement socket options
 */
#define SOL_NFC		280
#endif

#if LINUX_VERSION_IS_LESS(6,19,0)
/**
 * struct sockaddr_unsized - Unspecified size sockaddr for callbacks
 * @sa_family: Address family (AF_UNIX, AF_INET, AF_INET6, etc.)
 * @sa_data: Flexible array for address data
 *
 * This structure is designed for callback interfaces where the
 * total size is known via the sockaddr_len parameter. Unlike struct
 * sockaddr which has a fixed 14-byte sa_data limit or struct
 * sockaddr_storage which has a fixed 128-byte sa_data limit, this
 * structure can accommodate addresses of any size, but must be used
 * carefully.
 */
struct sockaddr_unsized {
	__kernel_sa_family_t	sa_family;	/* address family, AF_xxx */
	char			sa_data[];	/* flexible address data */
};
#endif /* < 6.19 */

#ifndef __sockaddr_check_size
#define __sockaddr_check_size(size)	\
	BUILD_BUG_ON(((size) > sizeof(struct __kernel_sockaddr_storage)))
#endif

#endif /* __BACKPORT_SOCKET_H */
