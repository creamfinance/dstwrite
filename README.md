# dstwrite
DstWrite is a simple iptables extension which allows to rewrite the destination of ipv4 and ipv6 packets in ANY chain!

# Prerequisites

CentOs: 

```
yum -y install kernel-headers kernel-devel iptables-devel
yum -y groupinstall "Development Tools"
```

# How to build 

Build the kernel module:

```
cd dstwrite_kernel
./make.sh
cp xt_DSTWRITE.ko /lib/modules/$(uname -r)/kernel/net/netfilter/
depmod
modprobe xt-DSTWRITE
```

Now the kernel module should be loaded you be listed: `lsmod | grep DSTWRITE`

Build the iptables module:

```
cd dstwrite_iptables
./make.sh
cp libxt_DSTWRITE.so /lib64/xtables/
```

Now iptables should understand the target.

try it out:

`iptables -A INPUT -p icmp -d 10.21.13.5 -j DSTWRITE --to-destination 10.21.13.200`

list the rules and it should be visible:

`iptables -L`

```
DSTWRITE   icmp --  anywhere             10.21.13.5           to:10.21.13.200
```
