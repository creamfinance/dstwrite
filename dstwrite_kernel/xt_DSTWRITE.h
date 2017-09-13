#ifndef _XT_DSTWRITE_H
#define _XT_DSTWRITE_H

//#include <linux/module.h>
//#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
//#include <uapi/linux/in6.h>
//#include <net/checksum.h>
//#include <linux/types.h>


struct xt_dstwrite_target_info_ipv6 {
    struct in6_addr daddr;
};


struct xt_dstwrite_target_info_ipv4 {
    __be32     daddr;
};

#endif /* _XT_DSTWRITE_H */
