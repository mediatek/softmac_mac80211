#ifndef _BACKPORT_KUNIT_STATIC_STUB_H
#define _BACKPORT_KUNIT_STATIC_STUB_H

#if LINUX_VERSION_IS_GEQ(6,2,0)
#include_next <kunit/static_stub.h>
#else
#define KUNIT_STATIC_STUB_REDIRECT(real_fn_name, args...) do {} while (0)
#endif

#endif /* _BACKPORT_KUNIT_STATIC_STUB_H */
