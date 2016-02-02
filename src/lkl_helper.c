#include <stdio.h>
#include <arpa/inet.h>

#include <lkl.h>
#include <lkl_host.h>

#include "lkl_helper.h"

struct lkl_netdev *nuse_vif_vde_create(const char *switch_);
extern struct lkl_dev_net_ops vde_net_ops;

void init_lkl(size_t mem, char const *cmd) {
    int ret = 0;
    struct lkl_netdev *nd = nuse_vif_vde_create("/tmp/vde");
    struct lkl_dev_net_ops *nd_ops = &vde_net_ops;


    ret = lkl_netdev_add(nd, nd_ops, 0);
    if (ret < 0) {
        fprintf(stderr, "LKL: Failed to add network interface: %s\n", lkl_strerror(ret));
        return;
    }
    int netdev_id = ret;

    lkl_start_kernel(&lkl_host_ops, mem, cmd);

    ret = lkl_netdev_get_ifindex(netdev_id);
    if (ret < 0) {
        fprintf(stderr, "LKL: Failed to get network interface index for device %d: %s\n", netdev_id, lkl_strerror(ret));
        return;
    }
    int ifindex = ret;

    uint32_t addr = inet_addr("10.10.10.1");
    int netmask = 8;
    ret = lkl_if_set_ipv4(ifindex, addr, netmask);
    if (ret < 0) {
        fprintf(stderr, "LKL: Failed to configure network interface %d: %s\n", ifindex, lkl_strerror(ret));
        return;
    }
}

void halt_lkl() {
    lkl_sys_halt();
}
