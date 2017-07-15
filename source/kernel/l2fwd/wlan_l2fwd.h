
struct l2fwd_callback {
    int (*rx)(struct sk_buff *skb, struct net_device *dev);
    int (*set_dev_mode)(int mode_flag);
};
