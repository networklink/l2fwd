#include <linux/module.h>
#include <linux/version.h>
#include <linux/kmod.h>

/*Local wifi interface*/
struct wifi_dev {
    struct net_device *dev;
    int flag;    
};
enum WIFI_DEV_FLAG {
    WIFI_DEV_FLAG_TYPE_MODE_AP = 1,
    WIFI_DEV_FLAG_TYPE_MODE_STA,
};

/*Local wifi interface list*/
struct network_dev_info {
    TAILQ_HEAD(wifi_dev) list;
    OS_LOCK_VAR lock;
};

/*To reach leaf node with mac address =  macaddr, the next hop is
 next_node*/
struct  leaf_node {
   char macaddr[6] ;
   struct l2fwd_node *next_hop;
    TAILQ_ENTRY(leaf_node) next;
};

/*End leaf node list. This the end client which is trying to
 connect to internet*/
struct leaf_node_table {
    TAILQ_HEAD(leaf_node) node_list;
    OS_LOCK_VAR lock;
};

/*End leaf node hash list. */
struct node_hash_list {
    struct leaf_node_table hash[32];
};


/*l2fwd node information. Each node has a AP and STA.
 */
struct l2fwd_node {
    char macaddr_ap[6];
    char macaddr_sta[6];
    int num_hops_to_gw;
    TAILQ_ENTRY(l2fwd_node) next;
};

/*List of l2fwd nodes*/
struct l2fwd_layer_list {
    /*Neighbour list*/
    TAILQ_HEAD(l2fwd_node) nb_list; 
    int number_of_neighbors;
};

/*L2fwd context. This structure is specific to my node*/
struct l2fwd_dev {
    /*My net device*/
    struct net_device *dev;
    /*List of wifi devices present in my device*/
    struct network_dev_info wifi_list;
    /*Myneighbouring L2fwd devices*/
    struct l2fwd_layer_list l2fwd_nb;
    /*list of leaf nodes, from where the a packet is originated and the packet
    is forwarded through my node*/
    struct node_hash_list leaf_node_list;
};
typedef struct l2fwd_dev L2FWD;


L2FWD *l2fwd = NULL;

static struct notifier_block wifi_dev_notifier = {
        .notifier_call  = wifi_dev_notify_event,
};

int wifi_dev_notify_event(struct notifier_block *this,
                          unsigned long event, void *ptr)
{
    struct net_device *dev = netdev_notifier_info_to_dev(ptr);

    if (event == NETDEV_REGISTER) {
        if (IS_DEV_TYPE_WIFI(dev)) {
            wlan_register_dev(&l2fwd_cb);
        }      
    }
}

int register_wifi(struct wifi_dev *wifi)
{

}

static netdev_tx_t l2fwd_xmit_override(struct sk_buff *skb, 
                                        struct net_device *dev)
{
}

void l2fwd_add_virt_dev()
{
}

static int __init l2fwd_init(void)
{
    l2fwd = (L2FWD *)kmalloc(sizeof(L2FWD));

   
    register_netdevice_notifier(&wifi_dev_notifier) ;
    
    return 0;
}

static void l2fwd_exit(void)
{
    return;
}

module_init(l2fwd_init);
module_exit(l2fwd_exit);
