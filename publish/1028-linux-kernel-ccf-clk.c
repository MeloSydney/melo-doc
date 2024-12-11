struct clk 解析
每个struct clk都对应一个前端consumer
通过ccf接口和clk_core *指针, 可以访问clk驱动提供的能力, 修改频率, 获取频率等
struct clk {                                            /* drivers/clk/clk.c consumer/provider 使用 可能有多个 */
        struct clk_core	*core;                          /* */
        struct device *dev;                             /* */
        const char *dev_id;                             /* string describing device name 设备树/platform 进行设备匹配 通过dts绑定 */
        const char *con_id;                             /* connection ID string on device 每个时钟拥有一个唯一的con_id 必须唯一 通过名字找clk */
        unsigned long min_rate;                         /* */
        unsigned long max_rate;                         /* */
        unsigned int exclusive_count;                   /* */
        struct hlist_node clks_node;                    /* list add to clk_core->clks */
};

struct clk_hw 解析
clk_hw结构体由驱动层填充, 在register_clk时作为入参
其中, clk_init_data需要在驱动层确保填充完成
之后, clk_init_data中的数据会在register_clk时被复制到其他clk_core中的某些字段
struct clk_hw {                                         /* 一个硬件clk 对应一个clk_hw */
        struct clk_core *core;                          /* 对应一个ccf层的 clk_core */
        struct clk *clk;                                /* 对应一个consumer端的struct clk *对象 */
        const struct clk_init_data *init;               /* clk_hw_register 注册ccf框架时传入的包含初始化数据的结构体 */
}; /* clk_hw */

struct clk_init_data 解析
struct clk_init_data {                                  /* 只会用于clk ccf注册 */
        const char *name;                               /* clk name 从dtb解析 */
        const struct clk_ops *ops;                      /* 驱动实现的回调 */
        const char * const *parent_names;               /* 所有parent name的集合 */
        const struct clk_parent_data *parent_data;      /* 用于填充 clk_parent_map->中多个字段 */
        const struct clk_hw **parent_hws;               /* 用于填充 clk_parent_map->hw */
        u8 num_parents;                                 /* 同 clk_core->num_parents */
        unsigned long flags;                            /* 同 clk_core->flags */
};

struct clk_parent_map 解析
clk是一个树形结构, 每个clk有自己的parent和child
struct clk_parent_map也代表了一个parent的抽象
clk_parent_data和clk_parent_map结构一样, 唯独少了一个字段 struct clk_core *core;
clk_parent_map定义在clk.c 是一个不想暴露出去的结构, 而clk_parent_data定义在include/linux/clk-provider.h, 是一个暴露出去的结构, 大概率因为不想将clk_core结构暴露出去
struct clk_parent_map {                                 /* 代表一个parent */
        const struct clk_hw	*hw;                    /* */
        struct clk_core		*core;                  /* */
        const char		*fw_name;
        const char		*name;                  /* parent name */
        int			index;                  /* 自己的index child通过index找到parent */
};

struct clk_parent_data {
	const struct clk_hw	*hw;
	const char		*fw_name;
	const char		*name;
	int			index;
};


struct clk_core 解析
每个clk设备 都会在框架层有一个对应的clk_core
struct clk_core {                                       /* ccf 使用 代表一个硬件clock在ccf的抽象 */
        const char		*name;                  /* clk name */
        const struct clk_ops	*ops;                   /* clk 驱动实现 */
        struct clk_hw		*hw;                    /* clk_hw 代表一个clk hardware的抽象 */
        struct module		*owner;                 /* */
        struct device		*dev;                   /* device抽象 可以在/sys下找到该设备文件*/
        struct device_node	*of_node;               /* dts node */
        struct clk_core		*parent;                /* 当前 parent */
        struct clk_parent_map	*parents;               /* 每个parent都是一个clk_parent_map * */
        u8			num_parents;            /* 我有几个parent */
        u8			new_parent_index;       /* 当修改parent时 用来临时保存 */
        unsigned long		rate;                   /* 当前频率 */
        unsigned long		req_rate;               /* clk_set_rate 成功后 设置该字段 consumer要求的rate */
        unsigned long		new_rate;               /* clk_calc_new_rates 根据 req_rate 计算 获取 new_rate */
        struct clk_core		*new_parent;            /* 当修改parent时 用来临时保存 */
        struct clk_core		*new_child;             /* 当clk树形结构变化时 用来临时保存 */
        unsigned long		flags;                  /* */
        bool			orphan;                 /* 是否是孤儿 初始化和树形结构变化时 会使用 */
        bool			rpm_enabled;            /* rpm 功耗 */
        unsigned int		enable_count;           /* clk_core_enable 每次enable +1 */
        unsigned int		prepare_count;          /* clk_core_prepare 每次prepare +1 */
        unsigned int		protect_count;          /* */
        unsigned long		min_rate;               /* */
        unsigned long		max_rate;               /* */
        unsigned long		accuracy;               /* recalc_accuracy 时钟精度 */
        int			phase;                  /* get_phase 时钟相位 [0 - 359] */
        struct clk_duty		duty;                   /* set_duty_cycle */
        struct hlist_head	children;               /* 挂了当前clk所有的children */
        struct hlist_node	child_node;             /* 自己作为child时 挂到children链表 */
        struct hlist_head	clks;                   /* clk_get 每当产生一个consumer时 会生成一个 clk 将新生成的clk挂接到此处 */
        unsigned int		notifier_count;         /* clk_notifier_register 感兴趣当前时钟rate变化的通知链 */
#if defined(CONFIG_DEBUG_FS) && defined(CONFIG_HAVE_CLK_DEBUG)
        struct dentry		*dentry;
        struct hlist_node	debug_node;
#endif
        struct kref		ref;                    /* atomic_t 管理生命周期 */
};