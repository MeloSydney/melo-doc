


//TODO - vh struct tp

/* tracepoint-defs.h */
struct tracepoint {
        const char *name;                       // __tracepoint_##name
        struct static_key key;                  // STATIC_KEY_INIT_FALSE
        struct static_call_key *static_call_key;
        void *static_call_tramp;
        void *iterator;                         // 遍历 tracepoint_func->func
        int (*regfunc)(void);                   // NULL
        void (*unregfunc)(void);                // NULL
        struct tracepoint_func __rcu *funcs;    // NULL
};

/* tracepoint-defs.h */
struct tracepoint_func {
        void *func;                             // 蹦床
        void *data;
        int prio;
};

/* trace_events.h 多次展开后的数据结构 */
struct trace_event_call {
        struct list_head list;
        struct trace_event_class *class;
        union {
                char *name;                     /* __tracepoint_##name */
                /* Set TRACE_EVENT_FL_TRACEPOINT flag when using "tp" */
                struct tracepoint *tp;
        };
        struct trace_event event;
        char *print_fmt;                        /* print_fmt_##name */
        struct event_filter *filter;
        /*
         * Static events can disappear with modules,
         * where as dynamic ones need their own ref count.
         */
        union {
                void *module;
                atomic_t refcnt;
        };
        void *data;
        /* See the TRACE_EVENT_FL_* flags above */
        int flags; /* static flags of different events */

#ifdef CONFIG_PERF_EVENTS
        int				perf_refcount;
        struct hlist_head __percpu	*perf_events;
        struct bpf_prog_array __rcu	*prog_array;

        int	(*perf_perm)(struct trace_event_call *,
                             struct perf_event *);
#endif
};

/* trace_events.h */
struct trace_event_class {
	const char		*system;                                        /* TRACE_SYSTEM_STRING */
	void			*probe;                                         /* trace_event_raw_event_##name */
#ifdef CONFIG_PERF_EVENTS
	void			*perf_probe;
#endif
	int			(*reg)(struct trace_event_call *event,          /* trace_event_reg */
				       enum trace_reg type, void *data);
	struct trace_event_fields *fields_array;
	struct list_head	*(*get_fields)(struct trace_event_call *);      /* trace_event_fields_##name */
	struct list_head	fields;
	int			(*raw_init)(struct trace_event_call *);         /* trace_event_raw_init */
};

/* trace_events.h */
struct trace_event {
	struct hlist_node		node;
	int				type;
	struct trace_event_functions	*funcs;                                 /* trace_event_type_funcs_##name */
};

//TODO - vh macro


/*
 * include/linux/tracepoint.h
 * 就是定义一个名为 __tracepoint_##_name 的 struct tracepoint 结构，
 * 然后定义一个名为 __traceiter_##_name 的函数，它对 struct tracepoint::funcs[] 成员数组中的每个函数都进行调用，数组尾部要以NULL结尾
 * __traceiter_##_name                                  遍历 func
 * struct tracepoint __tracepoint_##_name
 * struct static_call_key __SCK__tp_func_##_name
 */
#define DEFINE_TRACE_FN(_name, _reg, _unreg, proto, args)        \
        static const char __tpstrtab_##_name[]                \
        __section("__tracepoints_strings") = #_name;            \
        extern struct static_call_key __SCK__tp_func_##_name;    \
        int __traceiter_##_name(void *__data, proto);            \
        struct tracepoint __tracepoint_##_name    __used __section("__tracepoints") = {    \
                .name = __tpstrtab_##_name,                \
                .key = STATIC_KEY_INIT_FALSE,                \
                .static_call_key = &__SCK__tp_func_##_name,    \
                .static_call_tramp = NULL, \
                .iterator = &__traceiter_##_name,            \
                .regfunc = _reg,                    \
                .unregfunc = _unreg,                    \
                .funcs = NULL    \
        };                    \
        __TRACEPOINT_ENTRY(_name);                    \
        int __nocfi __traceiter_##_name(void *__data, proto)            \
        {                                \
                struct tracepoint_func *it_func_ptr;            \
                void *it_func;                        \
                it_func_ptr = rcu_dereference_raw((&__tracepoint_##_name)->funcs); \
                if (it_func_ptr) {                    \
                do {                        \
                        it_func = (it_func_ptr)->func;        \
                        __data = (it_func_ptr)->data;        \
                        ((void(*)(void *, proto))(it_func))(__data, args); \
                } while ((++it_func_ptr)->func);        \
                }                            \
                return 0;                        \
        }                                \
        extern struct static_call_key __SCK__tp_func_##_name;  \
        extern typeof(__traceiter_##_name) __SCT__tp_func_##_name;         \
        struct static_call_key __SCK__tp_func_##_name = {      \
                .func = __traceiter_##_name,                        \
        }

/* static key */
#define __TRACEPOINT_ENTRY(name)					\
        asm(    "	.section \"__tracepoints_ptrs\", \"a\"		\n"	\
                "	.balign 4					\n"	\
                "	.long 	__tracepoint_" #name " - .		\n"	\
                "	.previous					\n")

/*
 * include/linux/tracepoint.h
 * 这个宏主要定义了一系列函数集合，常用的有
 * register_trace_##name
 * trace_##name##_enabled
 * rcuidle 的还特殊弄了一个函数，还可以注册带有优先级的trace
 */
#define __DECLARE_TRACE(name, proto, args, cond, data_proto, data_args) \
        extern int __traceiter_##name(data_proto);                      \
        extern struct static_call_key __SCK__tp_func_##name;            \
        extern typeof(__traceiter_##name) __SCT__tp_func_##name;        \
        extern struct tracepoint __tracepoint_##name;                   \
        static inline void __nocfi trace_##name(proto)                  \
        {                                                               \
                if (static_key_false(&__tracepoint_##name.key))         \
                do {                                                    \
                        struct tracepoint_func *it_func_ptr;            \
                        int __maybe_unused __idx = 0;                   \
                        void *__data;                                   \
                        if (!(cond))                                    \
                                return;                                 \
                        /* keep srcu and sched-rcu usage consistent */  \
                        preempt_disable_notrace();                      \
                        it_func_ptr = rcu_dereference_raw((&__tracepoint_##name)->funcs); \
                        if (it_func_ptr) {                              \
                                __data = (it_func_ptr)->data;           \
                                __traceiter_##name(data_args);          \
                        }                                               \
                        preempt_enable_notrace();                       \
                } while (0)                                             \
                if (IS_ENABLED(CONFIG_LOCKDEP) && (cond)) {             \
                        rcu_read_lock_sched_notrace();                  \
                        rcu_dereference_sched(__tracepoint_##name.funcs);\
                        rcu_read_unlock_sched_notrace();                \
                }                                                       \
        }                                                               \
        static inline void trace_##name##_rcuidle(proto)                \
        {                                                               \
                if (static_key_false(&__tracepoint_##name.key))         \
                        do {                                            \
                                struct tracepoint_func *it_func_ptr;    \
                                int __maybe_unused __idx = 0;           \
                                void *__data;                           \
                                if (!(cond))                            \
                                        return;                         \
                                /* srcu can't be used from NMI */       \
                                WARN_ON_ONCE(in_nmi());                 \
                                /* keep srcu and sched-rcu usage consistent */        \
                                preempt_disable_notrace();              \
                                /*                            \
                                * For rcuidle callers, use srcu since sched-rcu    \
                                * doesn't work from the idle path.         \
                                */                                      \
                                __idx = srcu_read_lock_notrace(&tracepoint_srcu);\
                                rcu_irq_enter_irqson();                 \
                                it_func_ptr = rcu_dereference_raw((&__tracepoint_##name)->funcs); \
                                if (it_func_ptr) {                      \
                                        __data = (it_func_ptr)->data;   \
                                        __traceiter_##name(data_args);  \
                                }                                       \
                                rcu_irq_exit_irqson();                  \
                                srcu_read_unlock_notrace(&tracepoint_srcu, __idx);\
                                preempt_enable_notrace();               \
                        } while (0)                                     \
        }                                                               \
        static inline int register_trace_##name(void (*probe)(data_proto), void *data)    \
        {                                \
                return tracepoint_probe_register(&__tracepoint_##name, (void *)probe, data);    \
        }                                \
        static inline int register_trace_prio_##name(void (*probe)(data_proto), void *data, int prio) \
        {                                \
                return tracepoint_probe_register_prio(&__tracepoint_##name, (void *)probe, data, prio); \
        }                                \
        static inline int unregister_trace_##name(void (*probe)(data_proto), void *data)    \
        {                                \
                return tracepoint_probe_unregister(&__tracepoint_##name, (void *)probe, data);    \
        }                                \
        static inline void check_trace_callback_type_##name(void (*cb)(data_proto))    \
        {                                \
        }                                \
        static inline bool trace_##name##_enabled(void)                 \
        {                                \
                return static_key_false(&__tracepoint_##name.key);      \
        }


register_trace_##name() //具体tracepoint的define位置
        tracepoint_probe_register(struct tracepoint *tp, void *probe, void *data) //tracepoint.c
                tracepoint_probe_register_prio(struct tracepoint *tp, void *probe, void *data, int prio) //tracepoint.c
                        tracepoint_add_func(struct tracepoint *tp, struct tracepoint_func *func, int prio, bool warn) //tracepoint.c
                                static_key_enable(&tp->key);


/*
 * tracepoint.h
 * 就是上面的一组函数集合，包含register_trace_##name、trace_##name##_enabled
 */
#define DECLARE_TRACE(name, proto, args)                \
        __DECLARE_TRACE(name, PARAMS(proto), PARAMS(args), cpu_online(raw_smp_processor_id()), PARAMS(void *__data, proto), PARAMS(__data, args))

/* tracepoint.h 直接映射 */
#define TRACE_EVENT(name, proto, args, struct, assign, print) \
        __DECLARE_TRACE(name, PARAMS(proto), PARAMS(args), cpu_online(raw_smp_processor_id()), PARAMS(void *__data, proto), PARAMS(__data, args))

/* tracepoint.h */
#define DECLARE_TRACE_CONDITION(name, proto, args, cond)		\
        __DECLARE_TRACE(name, PARAMS(proto), PARAMS(args), cpu_online(raw_smp_processor_id()) && (PARAMS(cond)), PARAMS(void *__data, proto))

/* define_trace.h 直接映射 */
#define TRACE_EVENT(name, proto, args, struct, assign, print) \
        DEFINE_TRACE_FN(name, NULL, NULL, PARAMS(proto), PARAMS(args));

/* define_trace.h */
#define TRACE_EVENT_FN(name, proto, args, tstruct, assign, print, reg, unreg)   \
        DEFINE_TRACE_FN(name, reg, unreg, PARAMS(proto), PARAMS(args))

/* tracepoint.h */
#define TRACE_EVENT_FN(name, proto, args, struct, assign, print, reg, unreg)			\
        DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))

/* define_trace.h */
#define TRACE_EVENT_FN_COND(name, proto, args, cond, tstruct, assign, print, reg, unreg)        \
        DEFINE_TRACE_FN(name, reg, unreg, PARAMS(proto), PARAMS(args))

/* tracepoint.h */
#define TRACE_EVENT_FN_COND(name, proto, args, cond, struct, assign, print, reg, unreg)			\
        DECLARE_TRACE_CONDITION(name, PARAMS(proto), PARAMS(args), PARAMS(cond))


/* tracepoint-defs.h */
#define DECLARE_TRACEPOINT(tp) \                                // 不一定存在
        extern struct tracepoint __tracepoint_##tp

/* tracepoint.h */
#define DEFINE_TRACE(name, proto, args)		\
        DEFINE_TRACE_FN(name, NULL, NULL, PARAMS(proto), PARAMS(args));

/* tracepoint.h 导出符号 */
#define EXPORT_TRACEPOINT_SYMBOL_GPL(name)				\
        EXPORT_SYMBOL_GPL(__tracepoint_##name);				\
        EXPORT_SYMBOL_GPL(__traceiter_##name);				\
        EXPORT_STATIC_CALL_GPL(tp_func_##name)

/* tracepoint.h 导出符号 */
#define EXPORT_TRACEPOINT_SYMBOL(name)					\
        EXPORT_SYMBOL(__tracepoint_##name);				\
        EXPORT_SYMBOL(__traceiter_##name);				\
        EXPORT_STATIC_CALL(tp_func_##name)


/* define_trace.h */
#define DECLARE_TRACE(name, proto, args)	\
        DEFINE_TRACE(name, PARAMS(proto), PARAMS(args))

//TODO - tracepoint 使用的两个步骤

1 DECLARE_
In include/trace/events/subsys.h::

        #undef TRACE_SYSTEM
        #define TRACE_SYSTEM subsys

        #if !defined(_TRACE_SUBSYS_H) || defined(TRACE_HEADER_MULTI_READ)
        #define _TRACE_SUBSYS_H

        #include <linux/tracepoint.h>

        DECLARE_TRACE(subsys_eventname,
                TP_PROTO(int firstarg, struct task_struct *p),
                TP_ARGS(firstarg, p));

        #endif /* _TRACE_SUBSYS_H */

        /* This part must be outside protection */
        #include <trace/define_trace.h>

2 DEFINE_
In subsys/file.c (where the tracing statement must be added)

        #include <trace/events/subsys.h>

        #define CREATE_TRACE_POINTS
        DEFINE_TRACE(subsys_eventname);

        void somefct(void)
        {
                ...
                trace_subsys_eventname(arg, task);
                ...
        }

/* vendor_hooks.h */
#define DECLARE_HOOK DECLARE_TRACE

/* define_trace.h */
#define DECLARE_TRACE(name, proto, args)	\
        DEFINE_TRACE(name, PARAMS(proto), PARAMS(args))



//TODO - android ptype_head

/* dev.c 使用 vendor_hook */
static inline struct list_head *ptype_head(const struct packet_type *pt)
{
        struct list_head vendor_pt = { .next  = NULL, };
        trace_android_vh_ptype_head(pt, &vendor_pt);            /* register_trace_android_vh_ptype_head 如果注册 在这触发 */
        if (vendor_pt.next)
                return vendor_pt.next;
        if (pt->type == htons(ETH_P_ALL))
                return pt->dev ? &pt->dev->ptype_all : &net_hotdata.ptype_all;
        else
                return pt->dev ? &pt->dev->ptype_specific :
                                 &ptype_base[ntohs(pt->type) & PTYPE_HASH_MASK];
}

/* vendor_hooks.c 导出 vendor_hook */
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ptype_head);

/* vendor_hooks.h 定义 vendor_hooks 需要使用的宏 */

/* net.h 定义 vendor_hook */
DECLARE_HOOK(android_vh_ptype_head,
        TP_PROTO(const struct packet_type *pt, struct list_head *vendor_pt),
        TP_ARGS(pt, vendor_pt));

//TODO -



// vmscan.h

#undef TRACE_SYSTEM
#define TRACE_SYSTEM vmscan

#define TRACE_INCLUDE_PATH trace/hooks

#if !defined(_TRACE_HOOK_VMSCAN_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_HOOK_VMSCAN_H

#include <trace/hooks/slinux_hooks.h>

DECLARE_HOOK(slinux_vh_set_scan_balance,
             TP_PROTO(unsigned long scan_type, unsigned int *scan_balance),
             TP_ARGS(scan_type, scan_balance));

DECLARE_HOOK(slinux_vh_kswapd_try_to_sleep,
             TP_PROTO(void *unused),
             TP_ARGS(unused));

DECLARE_HOOK(slinux_vh_wakeup_kswapd,
             TP_PROTO(void *unused),
             TP_ARGS(unused));

#endif /* _TRACE_HOOK_VMSCAN_H */
/* This part must be outside protection */
#include <trace/define_trace.h>





// slinux_hooks.h
#include <linux/tracepoint.h>
#define DECLARE_HOOK DECLARE_TRACE




// tracepoint.h

#define DECLARE_TRACE(name, proto, args)				\
    __DECLARE_TRACE(name, PARAMS(proto), PARAMS(args),		\
            cpu_online(raw_smp_processor_id()),		\
            PARAMS(void *__data, proto),			\
            PARAMS(__data, args))


#define __DECLARE_TRACE(name, proto, args, cond, data_proto, data_args) \
    extern int __traceiter_##name(data_proto);			\                       // __traceiter_slinux_vh_set_scan_balance
    DECLARE_STATIC_CALL(tp_func_##name, __traceiter_##name);	\                       // static key define
    extern struct tracepoint __tracepoint_##name;			\               // extern struct tracepoint __tracepoint_slinux_vh_set_scan_balance
    static inline void trace_##name(proto)				\               // trace_slinux_vh_set_scan_balance
    {								\
        if (static_key_false(&__tracepoint_##name.key))		\
            __DO_TRACE(name,				\
                TP_PROTO(data_proto),			\
                TP_ARGS(data_args),			\
                TP_CONDITION(cond), 0);			\
        if (IS_ENABLED(CONFIG_LOCKDEP) && (cond)) {		\
            rcu_read_lock_sched_notrace();			\
            rcu_dereference_sched(__tracepoint_##name.funcs);\
            rcu_read_unlock_sched_notrace();		\
        }							\
    }								\
    __DECLARE_TRACE_RCU(name, PARAMS(proto), PARAMS(args),		\
        PARAMS(cond), PARAMS(data_proto), PARAMS(data_args))	\
    static inline int						\
    register_trace_##name(void (*probe)(data_proto), void *data)	\               // register_trace_slinux_vh_set_scan_balance
    {								\
        return tracepoint_probe_register(&__tracepoint_##name,	\
                        (void *)probe, data);	\
    }								\
    static inline int						\
    register_trace_prio_##name(void (*probe)(data_proto), void *data,\                  // register_trace_prio_trace_slinux_vh_set_scan_balance
                   int prio)				\
    {								\
        return tracepoint_probe_register_prio(&__tracepoint_##name, \
                          (void *)probe, data, prio); \
    }								\
    static inline int						\
    unregister_trace_##name(void (*probe)(data_proto), void *data)	\               // unregister_trace_trace_slinux_vh_set_scan_balance
    {								\
        return tracepoint_probe_unregister(&__tracepoint_##name,\
                        (void *)probe, data);	\
    }								\
    static inline void						\
    check_trace_callback_type_##name(void (*cb)(data_proto))	\
    {								\
    }								\
    static inline bool						\
    trace_##name##_enabled(void)					\               // trace_trace_slinux_vh_set_scan_balance_enabled
    {								\
        return static_key_false(&__tracepoint_##name.key);	\
    }



refs
https://www.cnblogs.com/hellokitty2/p/15522289.html
https://mp.weixin.qq.com/s/1A02qv5SIEgTEvsN1DWzqQ                       // 深度理解 ftrace
https://www.cnblogs.com/Linux-tech/p/14110332.html                      // 内核工匠 ftrace