#ifndef __BACKPORT_LINUX_NET_H
#define __BACKPORT_LINUX_NET_H
#include <linux/version.h>
#include_next <linux/net.h>
#include <linux/static_key.h>
#include <linux/socket.h>

#if LINUX_VERSION_IS_LESS(6,19,0)
/*
 * The in-kernel socket API takes struct sockaddr_unsized since 6.19.
 * Older kernels still expect struct sockaddr here.
 */
static inline int backport_kernel_connect(struct socket *sock,
					  struct sockaddr_unsized *addr,
					  int addrlen, int flags)
{
	return kernel_connect(sock, (struct sockaddr *)addr, addrlen, flags);
}
#define kernel_connect backport_kernel_connect
#endif /* < 6.19 */

#endif /* __BACKPORT_LINUX_NET_H */
