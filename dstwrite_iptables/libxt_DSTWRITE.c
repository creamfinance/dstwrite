#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <xtables.h>
#include <iptables.h>
#include <arpa/inet.h>
#include <netinet/ip6.h>

struct xt_dstwrite_target_info_ipv4 {
    __be32     daddr;
};

struct xt_dstwrite_target_info_ipv6 {
    struct in6_addr   daddr;
};

enum {
	O_TO_DST = 0
};


static void DSTWRITE_help(void)
{
	printf(
"DSTWRITE target options\n"
" --to-destination [<ipaddr>]\n"
"                  Address to map destination to.\n");
}

static const struct xt_option_entry DSTWRITE_opts[] = {
	{.name = "to-destination", .id = O_TO_DST, .type = XTTYPE_STRING, .flags = XTOPT_MAND | XTOPT_MULTI},
	XTOPT_TABLEEND
};

static int parse_to_dst(const char *s, __be32 *daddr)
{
    struct in_addr ip = *xtables_numeric_to_ipaddr(s);
   
    *daddr = ip.s_addr;

    return 0;
}

static void DSTWRITE_parse4(struct xt_option_call *cb)
{
	struct xt_dstwrite_target_info_ipv4 *info = cb->data;

	xtables_option_parse(cb);

	if (parse_to_dst(cb->arg, &info->daddr)) {
		xtables_error(PARAMETER_PROBLEM, "Unparseable ipv4 addr \"%s\"", cb->arg);
	}
}

static void DSTWRITE_print4(const void *ip, const struct xt_entry_target *target, int numeric)
{
	struct xt_dstwrite_target_info_ipv4 *info = (void *)target->data;

	printf(" to:");
	printf("%s", xtables_ipaddr_to_numeric((void*)&info->daddr));
}

static void DSTWRITE_save4(const void *ip, const struct xt_entry_target *target)
{
	const struct xt_dstwrite_target_info_ipv4 *info = (void *)target->data;

	printf(" --to-destination ");
	printf("%s", xtables_ipaddr_to_numeric((void*)&info->daddr));
}

static void DSTWRITE_parse6(struct xt_option_call *cb)
{
    const struct in6_addr *ip;
    struct xt_dstwrite_target_info_ipv6 *info = cb->data;

    xtables_option_parse(cb);

    ip = xtables_numeric_to_ip6addr(cb->arg);

    if (!ip) {
        xtables_error(PARAMETER_PROBLEM, "Unparseable ipv6 addr \"%s\"", cb->arg);
    }

    info->daddr = *ip;
}

static void DSTWRITE_print6(const void *ip, const struct xt_entry_target *target, int numeric)
{
    struct xt_dstwrite_target_info_ipv6 *info = (void *)target->data;

    printf(" to:");
    printf("%s", xtables_ip6addr_to_numeric((void*)&info->daddr));
}

static void DSTWRITE_save6(const void *ip, const struct xt_entry_target *target)
{
    struct xt_dstwrite_target_info_ipv6 *info = (void *)target->data;

    printf(" --to-destination ");
    printf("%s", xtables_ip6addr_to_numeric((void*)&info->daddr));
}


static struct xtables_target dstwrite_tg_reg[] = {
    {
	.name 		= "DSTWRITE",
	.version	= XTABLES_VERSION,
	.family		= NFPROTO_IPV4,
	.size		= XT_ALIGN(sizeof(struct xt_dstwrite_target_info_ipv4)),
	.userspacesize = XT_ALIGN(sizeof(struct xt_dstwrite_target_info_ipv4)),
	.help 		= DSTWRITE_help,
	.print 		= DSTWRITE_print4,
	.save 		= DSTWRITE_save4,
	.x6_parse 	= DSTWRITE_parse4,
    .x6_options = DSTWRITE_opts,
    },
    {
    .name       = "DSTWRITE",
    .version    = XTABLES_VERSION,
    .family     = NFPROTO_IPV6,
    .size       = XT_ALIGN(sizeof(struct xt_dstwrite_target_info_ipv6)),
    .userspacesize = XT_ALIGN(sizeof(struct xt_dstwrite_target_info_ipv6)),
    .help       = DSTWRITE_help,
    .print      = DSTWRITE_print6,
    .save       = DSTWRITE_save6,
    .x6_parse   = DSTWRITE_parse6,
    .x6_options = DSTWRITE_opts,
    }
};

void _init(void)
{
    xtables_register_target(&dstwrite_tg_reg[0]);
    xtables_register_target(&dstwrite_tg_reg[1]);
	//xtables_register_targets(dstwrite_tg_reg, 2);
}
