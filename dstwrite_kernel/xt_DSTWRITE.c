#include <linux/module.h>
#include <linux/skbuff.h>
//#include <linux/ip.h>
//#include <linux/ipv6.h>
#include <net/checksum.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter/x_tables.h>
#include "xt_DSTWRITE.h"

MODULE_AUTHOR("Thomas Rosenstein <thomas.rosenstein@creamfinance.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Xtables: Packet Dst change");
MODULE_ALIAS("ipt_DSTWRITE");
MODULE_ALIAS("ip6t_DSTWRITE");


static unsigned int
dstwrite_tg6(struct sk_buff *skb, const struct xt_action_param *par)
{
    const struct xt_dstwrite_target_info_ipv6 *dstinfo = par->targinfo;
    struct ipv6hdr *iph = ipv6_hdr(skb);

    iph->daddr = dstinfo->daddr;

    return XT_CONTINUE;
}


static unsigned int
dstwrite_tg4(struct sk_buff *skb, const struct xt_action_param *par)
{
    const struct xt_dstwrite_target_info_ipv4 *dstinfo = par->targinfo;
    struct iphdr *iph = ip_hdr(skb);

    iph->daddr = dstinfo->daddr;
    iph->check = 0;
    iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);

    return XT_CONTINUE;
}

static struct xt_target dstwrite_tg_reg[] __read_mostly = {
    {
        .name             = "DSTWRITE",
        .family            = NFPROTO_IPV6,
        .revision        = 0,
        .target         = dstwrite_tg6,
        .targetsize        = sizeof(struct xt_dstwrite_target_info_ipv6),
        .me             = THIS_MODULE
    },
    {
        .name             = "DSTWRITE",
        .family            = NFPROTO_IPV4,
        .revision        = 0,
        .target         = dstwrite_tg4,
        .targetsize        = sizeof(struct xt_dstwrite_target_info_ipv4),
        .me             = THIS_MODULE
    }
};


static int __init dstwrite_tg_init(void)
{
    return xt_register_targets(dstwrite_tg_reg, ARRAY_SIZE(dstwrite_tg_reg));
}

static void dstwrite_tg_exit(void)
{
    xt_unregister_targets(dstwrite_tg_reg, ARRAY_SIZE(dstwrite_tg_reg));
}

module_init(dstwrite_tg_init);
module_exit(dstwrite_tg_exit);
