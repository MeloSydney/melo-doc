
https://blog.csdn.net/chensong_2000/article/details/125277794
https://blog.csdn.net/maybeYoc/article/details/121659003
https://blog.csdn.net/Rong_Toa/article/details/116802358
https://blog.csdn.net/jasonactions/category_10514811.html


// TODO - kernel/trace

/*
 * [headers]
 */
bpf_trace.h
ftrace_internal.h
pid_list.h
trace_benchmark.h
trace_dynevent.h
trace_entries.h
trace_events_filter_test.h
trace.h
trace_kprobe_selftest.h
trace_output.h
trace_probe.h
trace_probe_tmpl.h
trace_stat.h
trace_synth.h
tracing_map.h

/*
 * [tracer实现]
 */
blktrace.c
trace_hwlat.c
trace_irqsoff.c
trace_mmiotrace.c
trace_nop.c
trace_object.c
trace_sched_wakeup.c
trace_functions.c
trace_functions_graph.c

/*
 * [syscall]
 */
trace_syscalls.c

/*
 * [ring buffer]
 */
ring_buffer_benchmark.c
ring_buffer.c

/*
 * [自测用例]
 */
kprobe_event_gen_test.c
preemptirq_delay_test.c
synth_event_gen_test.c
trace_kprobe_selftest.c
trace_selftest.c
trace_selftest_dynamic.c

/*
 * [未分类]
 */
bpf_trace.c
error_report-traces.c
fgraph.c
fprobe.c
ftrace.c
pid_list.c
power-traces.c
rethook.c
rpm-traces.c
trace_benchmark.c
trace_boot.c
trace_branch.c
trace.c
trace_clock.c
trace_dynevent.c
trace_eprobe.c
trace_event_perf.c
trace_events.c
trace_events_filter.c
trace_events_hist.c
trace_events_inject.c
trace_events_synth.c
trace_events_trigger.c
trace_events_user.c
trace_export.c
trace_kdb.c
trace_kprobe.c
trace_osnoise.c
trace_output.c
trace_preemptirq.c
trace_printk.c
trace_probe.c
trace_recursion_record.c
trace_sched_switch.c
trace_seq.c
trace_stack.c
trace_stat.c
trace_uprobe.c
tracing_map.c

// TODO - enum && struct

/* trace_events.h */
enum event_trigger_type {
	ETT_NONE		= (0),
	ETT_TRACE_ONOFF		= (1 << 0),
	ETT_SNAPSHOT		= (1 << 1),
	ETT_STACKTRACE		= (1 << 2),
	ETT_EVENT_ENABLE	= (1 << 3),
	ETT_EVENT_HIST		= (1 << 4),
	ETT_HIST_ENABLE		= (1 << 5),
	ETT_EVENT_EPROBE	= (1 << 6),
};

/* trace.h */
enum trace_type {
	__TRACE_FIRST_TYPE = 0,

	TRACE_FN,
	TRACE_CTX,
	TRACE_WAKE,
	TRACE_STACK,
	TRACE_PRINT,
	TRACE_BPRINT,
	TRACE_MMIO_RW,
	TRACE_MMIO_MAP,
	TRACE_BRANCH,
	TRACE_GRAPH_RET,
	TRACE_GRAPH_ENT,
	TRACE_USER_STACK,
	TRACE_BLK,
	TRACE_BPUTS,
	TRACE_HWLAT,
	TRACE_OSNOISE,
	TRACE_TIMERLAT,
	TRACE_RAW_DATA,
	TRACE_FUNC_REPEATS,

	__TRACE_LAST_TYPE,
};


struct array_buffer {
	struct trace_array		*tr;
	struct trace_buffer		*buffer;
	struct trace_array_cpu __percpu	*data;
	int				cpu;
};


/*
 * [缓冲区]
 */
struct trace_array {
	struct list_head	list;
	char			*name;
	struct array_buffer	array_buffer;
	struct tracer		*current_trace;
	unsigned int		trace_flags;
	unsigned char		trace_flags_index[TRACE_FLAGS_MAX_SIZE];
	unsigned int		flags;
	struct trace_options	*topts;
	struct list_head	systems;
	struct list_head	events;
	struct trace_event_file *trace_marker_file;
	struct objtrace_data	*obj_data;
#ifdef CONFIG_FUNCTION_TRACER
	struct ftrace_ops	*ops;
	struct trace_pid_list	__rcu *function_pids;
	struct trace_pid_list	__rcu *function_no_pids;
	int			function_enabled;
#endif
};


struct ftrace_event_field {
	struct list_head	link;
	const char		*name;
	const char		*type;
	int			filter_type;
	int			offset;
	int			size;
	int			is_signed;
};


struct objtrace_trigger_data {
	struct ftrace_event_field *field;
	char objtrace_cmd[OBJTRACE_CMD_LEN];
	int obj_offset;
	int obj_value_type_size;
	struct trace_array *tr;
};


struct event_trigger_data {
	unsigned long			count;
	int				ref;
	int				flags;
	struct event_trigger_ops	*ops;
	struct event_command		*cmd_ops;
	struct event_filter __rcu	*filter;
	char				*filter_str;
	void				*private_data;
	struct list_head		list;
	char				*name;
	struct list_head		named_list;
	struct event_trigger_data	*named_data;
};


struct event_command {
	struct list_head	list;
	char			*name;
	enum event_trigger_type	trigger_type;
	int			flags;
	int			(*parse)(struct event_command *cmd_ops,
					 struct trace_event_file *file,
					 char *glob, char *cmd,
					 char *param_and_filter);
	int			(*reg)(char *glob,
				       struct event_trigger_data *data,
				       struct trace_event_file *file);
	void			(*unreg)(char *glob,
					 struct event_trigger_data *data,
					 struct trace_event_file *file);
	void			(*unreg_all)(struct trace_event_file *file);
	int			(*set_filter)(char *filter_str,
					      struct event_trigger_data *data,
					      struct trace_event_file *file);
	struct event_trigger_ops *(*get_trigger_ops)(char *cmd, char *param);
};

/*
 * [hwlat blk function_graph wakeup_dl wakeup_rt wakeup function nop ...] [kernel/trace下是各种实例的实现]
 */

[function_graph 函数入口/出口都有探针 所以才能获取函数执行时间]

struct tracer {
	const char		*name;
	int			(*init)(struct trace_array *tr);	/* 通过echo function > current_tracer触发 */
	void			(*reset)(struct trace_array *tr);	/* 通过echo 0 >tracing_off触发 */
	void			(*start)(struct trace_array *tr);	/* 通过echo 1 >tracing_on触发 */
	void			(*stop)(struct trace_array *tr);
	void			(*open)(struct trace_iterator *iter);
	void			(*close)(struct trace_iterator *iter);
	ssize_t			(*read)(struct trace_iterator *iter,
					struct file *filp, char __user *ubuf,
					size_t cnt, loff_t *ppos);
	struct tracer		*next;
	struct tracer_flags	*flags;
};


/*
 * [trace_iterator用于tracing结果输出的数据结构]
 */
struct trace_iterator {
	struct trace_array	*tr;
	struct tracer		*trace;
	struct array_buffer	*array_buffer;
	struct ring_buffer_iter	**buffer_iter;
};


/*
 * [ring_buffer.c] [ftrace写入的缓存区]
 */
struct trace_buffer {
	unsigned			flags;
	int				cpus;
	struct ring_buffer_per_cpu	**buffers;
};


#define IF_ASSIGN(var, entry, etype, id)			\
	if (FTRACE_CMP_TYPE(var, etype)) {			\
		var = (typeof(var))(entry);			\
		WARN_ON(id != 0 && (entry)->type != id);	\
		break;						\
	}

#define trace_assign_type(var, ent)					\
	do {								\
		IF_ASSIGN(var, ent, struct ftrace_entry, TRACE_FN);	\
		IF_ASSIGN(var, ent, struct ctx_switch_entry, 0);	\
		IF_ASSIGN(var, ent, struct stack_entry, TRACE_STACK);	\
		IF_ASSIGN(var, ent, struct userstack_entry, TRACE_USER_STACK);\
		IF_ASSIGN(var, ent, struct print_entry, TRACE_PRINT);	\
		IF_ASSIGN(var, ent, struct bprint_entry, TRACE_BPRINT);	\
		IF_ASSIGN(var, ent, struct bputs_entry, TRACE_BPUTS);	\
		IF_ASSIGN(var, ent, struct hwlat_entry, TRACE_HWLAT);	\
		IF_ASSIGN(var, ent, struct osnoise_entry, TRACE_OSNOISE);\
		IF_ASSIGN(var, ent, struct timerlat_entry, TRACE_TIMERLAT);\
		IF_ASSIGN(var, ent, struct raw_data_entry, TRACE_RAW_DATA);\
		IF_ASSIGN(var, ent, struct trace_mmiotrace_rw,		\
			  TRACE_MMIO_RW);				\
		IF_ASSIGN(var, ent, struct trace_mmiotrace_map,		\
			  TRACE_MMIO_MAP);				\
		IF_ASSIGN(var, ent, struct trace_branch, TRACE_BRANCH); \
		IF_ASSIGN(var, ent, struct ftrace_graph_ent_entry,	\
			  TRACE_GRAPH_ENT);		\
		IF_ASSIGN(var, ent, struct ftrace_graph_ret_entry,	\
			  TRACE_GRAPH_RET);		\
		IF_ASSIGN(var, ent, struct func_repeats_entry,		\
			  TRACE_FUNC_REPEATS);				\
		__ftrace_bad_type();					\
	} while (0)



/* trace_entries.h */
/*
 * trace object entry:
 */
FTRACE_ENTRY(object, trace_object_entry,

	TRACE_OBJECT,

	F_STRUCT(
		__field(	unsigned long,		ip		)
		__field(	unsigned long,		parent_ip	)
		__field(	unsigned long,		object		)
	),

	F_printk(" %ps <-- %ps object:%lx\n",
		 (void *)__entry->ip, (void *)__entry->parent_ip, __entry->object)
);




//TODO - ftrace 初始化

static struct tracer function_trace __tracer_data =
{
	.name		= "function",
	.init		= function_trace_init,
	.reset		= function_trace_reset,
	.start		= function_trace_start,
	.flags		= &func_flags,
	.set_flag	= func_set_flag,
	.allow_instances = true,
#ifdef CONFIG_FTRACE_SELFTEST
	.selftest	= trace_selftest_startup_function,
#endif
};

static struct trace_buffer *temp_buffer;

struct tracer nop_trace __read_mostly =
{
	.name		= "nop",
	.init		= nop_trace_init,
	.reset		= nop_trace_reset,
#ifdef CONFIG_FTRACE_SELFTEST
	.selftest	= trace_selftest_startup_nop,
#endif
	.flags		= &nop_flags,
	.set_flag	= nop_set_flag,
	.allow_instances = true,
};

static struct tracer function_trace __tracer_data =
{
	.name		= "function",
	.init		= function_trace_init,
	.reset		= function_trace_reset,
	.start		= function_trace_start,
	.flags		= &func_flags,
	.set_flag	= func_set_flag,
	.allow_instances = true,
#ifdef CONFIG_FTRACE_SELFTEST
	.selftest	= trace_selftest_startup_function,
#endif
};

start_kernel(void)
	[处理-pg -mrecord-mcoun]
	ftrace_init();
		extern unsigned long __start_mcount_loc[];
		extern unsigned long __stop_mcount_loc[];
		ret = ftrace_dyn_arch_init();			/* arch related */
		count = __stop_mcount_loc - __start_mcount_loc;
		ret = ftrace_process_locs(NULL, __start_mcount_loc, __stop_mcount_loc);
			struct ftrace_page *start_pg;
			struct dyn_ftrace *rec;
			sort(start, count, sizeof(*start), ftrace_cmp_ips, NULL);
			start_pg = ftrace_allocate_pages(count);
				start_pg = pg = kzalloc(sizeof(*pg), GFP_KERNEL);
				...
			p = start;
			pg = start_pg;
			while (p < end)
				[ip == addr 既是 bl <_mcount>的地址]
				addr = ftrace_call_adjust(*p++);
				rec = &pg->records[pg->index++];
				rec->ip = addr;
			ftrace_update_code(mod, start_pg);  /* mod = NULL */
				for (pg = new_pgs; pg; pg = pg->next)
					ftrace_nop_initialize(mod, p)
						ret = ftrace_init_nop(mod, rec);
							return ftrace_modify_code(pc, old, new, true);
								__patch_text((void *)pc, new);
	[1 trace_printk分配内存 2 注册tracer nop function..]
	early_trace_init();
		tracer_alloc_buffers();
			temp_buffer = ring_buffer_alloc(PAGE_SIZE, RB_FL_OVERWRITE);
			if (allocate_objtrace_data(&global_trace))
				obj_data = kzalloc(sizeof(*obj_data), GFP_KERNEL);
				obj_data->tr = tr;
				fops = &obj_data->fops;
				fops->func = trace_object_events_call;
				fops->flags = FTRACE_OPS_FL_SAVE_REGS;
				fops->private = tr;
				list_add(&obj_data->head, &obj_data_head);
				tr->obj_data = obj_data;
			register_tracer(&nop_trace);	/* register a tracer ! */
				[1 tracer_options /sys/kernel/debug/tracing/options]
				add_tracer_options(&global_trace, type);
					create_trace_option_files(tr, t);  /* (struct trace_array *tr, struct tracer *tracer) */
						struct trace_option_dentry *topts;
						struct trace_options *tr_topts;
						topts = kcalloc(cnt + 1, sizeof(*topts), GFP_KERNEL);
						tr_topts = krealloc(tr->topts, sizeof(*tr->topts) * (tr->nr_topts + 1), GFP_KERNEL);
						tr->topts = tr_topts;
						tr->topts[tr->nr_topts].tracer = tracer;
						tr->topts[tr->nr_topts].topts = topts;
						tr->nr_topts++;
			init_function_trace();
				[traceon traceoff stacktrace dump cpudump trigger中支持的cmd..]
				init_func_cmd_traceon();
					register_ftrace_command(struct ftrace_func_command *cmd)
						list_add(&cmd->list, &ftrace_commands);
				return register_tracer(&function_trace);
			apply_trace_boot_options();
	[1 批量处理trace_events]
	trace_init();
		trace_event_init();
			[trace event slab]
			event_trace_memsetup();
			[trace syscall]
			init_ftrace_syscalls();
			[处理所有trace_events]
			event_trace_enable();
				for_each_event(iter, __start_ftrace_events, __stop_ftrace_events)
					call = *iter;
					ret = event_init(call);
						[raw_init == trace_event_raw_init]
						ret = call->class->raw_init(call);
					list_add(&call->list, &ftrace_events);
				[/sys/kernel/debug/tracing/xxx/trigger 时触发]
				register_trigger_cmds();
					register_trigger_traceon_traceoff_cmds();
					register_trigger_snapshot_cmd();
					register_trigger_stacktrace_cmd();
					register_trigger_enable_disable_cmds();
					register_trigger_hist_enable_disable_cmds();
					register_trigger_hist_cmd();
					register_trigger_object_cmd();		/* objtrace */




//TODO - ftrace trace_object.c

#define pr_fmt(fmt) "trace_object: " fmt

#include "trace_output.h"
#include <linux/freelist.h>

static DEFINE_PER_CPU(atomic_t, trace_object_event_disable);
static struct trace_event_file event_trace_file;
static const int max_args_num = 6;
static const int max_obj_pool = 10;
static atomic_t trace_object_ref;
static int exit_trace_object(void);
static int init_trace_object(void);

struct object_instance {
	void *object;
	struct freelist_node freelist;
};

struct obj_pool {
	int maxactive;
	atomic_t nobject;
	struct freelist_head product_freelist;
	struct freelist_head customer_freelist;
};
static struct obj_pool *obj_pool;

static bool object_exist(void *obj)
{
	struct freelist_node *node;
	struct object_instance *inst;
	bool ret = false;

	node = obj_pool->customer_freelist.head;

	while (node) {
		inst = container_of(node, struct object_instance, freelist);
		if (inst->object == obj) {
			ret = true;
			goto out;
		}
		node = node->next;
	}
out:
	return ret;
}

static bool object_empty(void)
{
	bool ret;

	ret = atomic_read(&obj_pool->nobject) == 0;

	return ret;
}


static void set_trace_object(void *obj)
{
	struct freelist_node *fn;
	struct object_instance *ins;

	if (!obj)
		return;

	if (object_exist(obj))
		goto out;

	fn = freelist_try_get(&obj_pool->product_freelist);
	if (!fn) {
		trace_printk("object_pool is full, can't trace object:0x%px\n", obj);
		return;
	}

	ins = container_of(fn, struct object_instance, freelist);
	ins->object = obj;

	freelist_add(&ins->freelist, &obj_pool->customer_freelist);
	atomic_inc(&obj_pool->nobject);

out:
	return;
}

static inline void free_objects(struct freelist_node *node)
{

	struct object_instance *inst;

	while (node) {
		inst = container_of(node, struct object_instance, freelist);
		node = node->next;
		kfree(inst);
	}
}

static inline void free_object_pool(void)
{
	free_objects(obj_pool->product_freelist.head);
	free_objects(obj_pool->customer_freelist.head);
	kfree(obj_pool);
}


static int init_object_pool(void)
{
	struct object_instance *inst;
	int i, ret = 0;

	obj_pool = kzalloc(sizeof(*obj_pool), GFP_KERNEL);
	if (!obj_pool) {
		ret = -ENOMEM;
		goto out;
	}
	obj_pool->maxactive = max_obj_pool;
	obj_pool->product_freelist.head = NULL;
	obj_pool->customer_freelist.head = NULL;

	for (i = 0; i < obj_pool->maxactive; i++) {
		inst = kzalloc(sizeof(*inst), GFP_KERNEL);
		if (!inst) {
			free_object_pool();
			ret = -ENOMEM;
			goto out;
		}
		freelist_add(&inst->freelist, &obj_pool->product_freelist);
	}
out:
	return ret;
}

static void submit_trace_object(unsigned long ip, unsigned long parent_ip,
				 unsigned long object)
{

	struct trace_buffer *buffer;
	struct ring_buffer_event *event;
	struct trace_object_entry *entry;
	int pc;

	pc = preempt_count();
	event = trace_event_buffer_lock_reserve(&buffer, &event_trace_file,
			TRACE_OBJECT, sizeof(*entry), pc);
	if (!event)
		return;
	entry   = ring_buffer_event_data(event);
	entry->ip                       = ip;
	entry->parent_ip                = parent_ip;
	entry->object			= object;

	event_trigger_unlock_commit(&event_trace_file, buffer, event,
		entry, pc);
}

static void
trace_object_events_call(unsigned long ip, unsigned long parent_ip,
				struct ftrace_ops *op, struct ftrace_regs *fregs)
	struct pt_regs *pt_regs = ftrace_get_regs(fregs);
	struct trace_array *tr = op->private;
	struct trace_array_cpu *data;

	cpu = raw_smp_processor_id();
	disabled = atomic_inc_return(&per_cpu(trace_object_event_disable, cpu));

	if (disabled != 1)
		goto out;

	if (object_empty())
		goto out;

	for (n = 0; n < max_args_num; n++) {
		obj = regs_get_kernel_argument(pt_regs, n);
		if (object_exist((void *)obj))
			submit_trace_object(ip, parent_ip, obj);
	}

out:
	atomic_dec(&per_cpu(trace_object_event_disable, cpu));
	preempt_enable_notrace();


static void
trace_object_trigger(struct event_trigger_data *data, struct trace_buffer *buffer,  void *rec, struct ring_buffer_event *event)
{
	struct ftrace_event_field *field = data->private_data;
	void *obj = NULL;

	memcpy(&obj, rec + field->offset, sizeof(obj));
	set_trace_object(obj);
}

static void
trace_object_trigger_free(struct event_trigger_ops *ops, struct event_trigger_data *data)
{
	if (WARN_ON_ONCE(data->ref <= 0))
		return;

	data->ref--;
	if (!data->ref)
		trigger_data_free(data);
}

static void
trace_object_count_trigger(struct event_trigger_data *data, struct trace_buffer *buffer, void *rec, struct ring_buffer_event *event)
{
	if (!data->count)
		return;

	if (data->count != -1)
		(data->count)--;

	trace_object_trigger(data, buffer, rec, event);
}

static int event_object_trigger_init(struct event_trigger_ops *ops, struct event_trigger_data *data)
{
	data->ref++;
	return 0;
}

static int
event_trigger_print(const char *name, struct seq_file *m, void *data, char *filter_str)
{
	long count = (long)data;

	seq_puts(m, name);

	if (count == -1)
		seq_puts(m, ":unlimited");
	else
		seq_printf(m, ":count=%ld", count);

	if (filter_str)
		seq_printf(m, " if %s\n", filter_str);
	else
		seq_putc(m, '\n');

	return 0;
}

static int
trace_object_trigger_print(struct seq_file *m, struct event_trigger_ops *ops,
			 struct event_trigger_data *data)
{
	return event_trigger_print("objtrace", m, (void *)data->count,
				   data->filter_str);
}


static struct event_trigger_ops objecttrace_trigger_ops = {
	.func			= trace_object_trigger,
	.print			= trace_object_trigger_print,
	.init			= event_object_trigger_init,
	.free			= trace_object_trigger_free,
};

static struct event_trigger_ops objecttrace_count_trigger_ops = {
	.func			= trace_object_count_trigger,
	.print			= trace_object_trigger_print,
	.init			= event_object_trigger_init,
	.free			= trace_object_trigger_free,
};

static struct event_trigger_ops *
objecttrace_get_trigger_ops(char *cmd, char *param)
{
	return param ? &objecttrace_count_trigger_ops : &objecttrace_trigger_ops;
}

static int register_object_trigger(char *glob, struct event_trigger_ops *ops,
					struct event_trigger_data *data,
					struct trace_event_file *file)
{
	struct event_trigger_data *test;
	int ret = 0;

	lockdep_assert_held(&event_mutex);

	list_for_each_entry(test, &file->triggers, list) {
		if (test->cmd_ops->trigger_type == data->cmd_ops->trigger_type) {
			ret = -EEXIST;
			goto out;
		}
	}

	if (data->ops->init) {
		ret = data->ops->init(data->ops, data);
		if (ret < 0)
			goto out;
	}

	list_add_rcu(&data->list, &file->triggers);
	ret++;

	update_cond_flag(file);
	if (trace_event_trigger_enable_disable(file, 1) < 0) {
		list_del_rcu(&data->list);
		update_cond_flag(file);
		ret--;
	}
	init_trace_object();
out:
	return ret;
}

static void unregister_object_trigger(char *glob, struct event_trigger_ops *ops,
					struct event_trigger_data *test,
					struct trace_event_file *file)
{
	struct event_trigger_data *data;
	bool unregistered = false;

	lockdep_assert_held(&event_mutex);

	list_for_each_entry(data, &file->triggers, list) {
		if (data->cmd_ops->trigger_type == test->cmd_ops->trigger_type) {
			unregistered = true;
			list_del_rcu(&data->list);
			trace_event_trigger_enable_disable(file, 0);
			update_cond_flag(file);
			break;
		}
	}

	if (unregistered && data->ops->free) {
		data->ops->free(data->ops, data);
		exit_trace_object();
	}
}

static int
event_object_trigger_callback(struct event_command *cmd_ops,
				struct trace_event_file *file,
				char *glob, char *cmd, char *param)
{
	struct event_trigger_data *trigger_data;
	struct event_trigger_ops *trigger_ops;
	struct trace_event_call *call;
	struct ftrace_event_field *field;
	char *objtrace_cmd;
	char *trigger = NULL;
	char *arg;
	char *number;
	int ret;

	ret = -EINVAL;
	if (!param)
		goto out;

	/* separate the trigger from the filter (c:a:n [if filter]) */
	trigger = strsep(&param, " \t");
	if (!trigger)
		goto out;
	if (param) {
		param = skip_spaces(param);
		if (!*param)
			param = NULL;
	}

	objtrace_cmd = strsep(&trigger, ":");
	if (!objtrace_cmd || strcmp(objtrace_cmd, "add"))
		goto out;

	arg = strsep(&trigger, ":");
	if (!arg)
		goto out;
	call = file->event_call;
	field = trace_find_event_field(call, arg);
	if (!field)
		goto out;

	trigger_ops = cmd_ops->get_trigger_ops(cmd, trigger);

	ret = -ENOMEM;
	trigger_data = kzalloc(sizeof(*trigger_data), GFP_KERNEL);
	if (!trigger_data)
		goto out;

	trigger_data->count = -1;
	trigger_data->ops = trigger_ops;
	trigger_data->cmd_ops = cmd_ops;
	trigger_data->private_data = field;
	INIT_LIST_HEAD(&trigger_data->list);
	INIT_LIST_HEAD(&trigger_data->named_list);

	if (glob[0] == '!') {
		cmd_ops->unreg(glob+1, trigger_ops, trigger_data, file);
		kfree(trigger_data);
		ret = 0;
		goto out;
	}

	if (trigger) {
		number = strsep(&trigger, ":");

		ret = -EINVAL;
		if (!strlen(number))
			goto out_free;

		/*
		 * We use the callback data field (which is a pointer)
		 * as our counter.
		 */
		ret = kstrtoul(number, 0, &trigger_data->count);
		if (ret)
			goto out_free;
	}

	if (!param) /* if param is non-empty, it's supposed to be a filter */
		goto out_reg;

	if (!cmd_ops->set_filter)
		goto out_reg;

	ret = cmd_ops->set_filter(param, trigger_data, file);
	if (ret < 0)
		goto out_free;

 out_reg:
	/* Up the trigger_data count to make sure reg doesn't free it on failure */
	event_object_trigger_init(trigger_ops, trigger_data);
	ret = cmd_ops->reg(glob, trigger_ops, trigger_data, file);
	/*
	 * The above returns on success the # of functions enabled,
	 * but if it didn't find any functions it returns zero.
	 * Consider no functions a failure too.
	 */
	if (!ret) {
		cmd_ops->unreg(glob, trigger_ops, trigger_data, file);
		ret = -ENOENT;
	} else if (ret > 0)
		ret = 0;

	/* Down the counter of trigger_data or free it if not used anymore */
	trace_object_trigger_free(trigger_ops, trigger_data);
 out:
	return ret;

 out_free:
	if (cmd_ops->set_filter)
		cmd_ops->set_filter(NULL, trigger_data, NULL);
	kfree(trigger_data);
	goto out;
}


event_object_trigger_parse(struct event_command *cmd_ops,
				struct trace_event_file *file,
				char *glob, char *cmd, char *param_and_filter)
	struct event_trigger_data *trigger_data;
	struct objtrace_trigger_data *obj_data;
	/* 'objtrace:add:arg1,0x28:u32:5 if comm == "cat"'
	    objtrace:add:OBJ[,OFFS][:TYPE][:COUNT] [if filter]*/
	obj_data = kzalloc(sizeof(*obj_data), GFP_KERNEL);
	obj_data->field = field;
	obj_data->obj_offset = offset;
	obj_data->obj_value_type_size = obj_value_type_size;
	obj_data->tr = file->tr;
	snprintf(obj_data->objtrace_cmd, OBJTRACE_CMD_LEN, objtrace_cmd);
	trigger_data = event_trigger_alloc(cmd_ops, cmd, param, obj_data);
	ret = event_trigger_parse_num(param, trigger_data);
	ret = event_trigger_set_filter(cmd_ops, file, filter, trigger_data);
	ret = event_trigger_register(cmd_ops, file, glob, trigger_data);


static struct event_command trigger_object_cmd = {
	.name			= "objtrace",
	.trigger_type		= ETT_TRACE_OBJECT,
	.flags			= EVENT_CMD_FL_NEEDS_REC,
	.parse			= event_object_trigger_parse,
	.reg			= register_trigger,
	.unreg			= unregister_trigger,
	.get_trigger_ops	= objecttrace_get_trigger_ops,
	.set_filter		= set_trigger_filter,
};


int register_trigger(char *glob, struct event_trigger_data *data, struct trace_event_file *file)
	if (data->ops->init)
		ret = data->ops->init(data);
			event_object_trigger_init()
				struct objtrace_trigger_data *obj_data = data->private_data;
				ret = init_trace_object(obj_data->tr);
					ret = register_ftrace_function(&tr->obj_data->fops);
						ret = register_ftrace_function_nolock(ops);
							ftrace_ops_init(ops);
							ret = ftrace_startup(ops, 0);
								ret = __register_ftrace_function(ops);
									add_ftrace_ops(&ftrace_ops_list, ops);
									ops->saved_func = ops->func;


int register_ftrace_function(struct ftrace_ops *ops)
	ret = register_ftrace_function_nolock(ops);
		ftrace_ops_init(ops);
		ret = ftrace_startup(ops, 0);



/*
 * 所有trigger被注册在这个list中
 */
static LIST_HEAD(trigger_commands);


__init int register_trigger_object_cmd(void)
{
	int ret;

	ret = register_event_command(&trigger_object_cmd);
		list_add(&cmd->list, &trigger_commands);
	WARN_ON(ret < 0);

	return ret;
}


static int init_trace_object(void)
{
	int ret;

	if (atomic_inc_return(&trace_object_ref) != 1) {
		ret = 0;
		goto out;
	}

	ret = init_object_pool();
	if (ret)
		goto out;

	event_trace_file.tr = top_trace_array();
	if (WARN_ON(!event_trace_file.tr)) {
		ret = -1;
		goto out;
	}
	ret = register_ftrace_function(&trace_ops);
out:
	return ret;
}


/* deinit */
static int exit_trace_object(void)
{
	int ret;

	if (WARN_ON_ONCE(atomic_read(&trace_object_ref) <= 0))
		goto out;

	if (atomic_dec_return(&trace_object_ref) != 0) {
		ret = 0;
		goto out;
	}

	ret = unregister_ftrace_function(&trace_ops);
	if (ret) {
		pr_err("can't unregister ftrace for trace object\n");
		goto out;
	}
	free_object_pool();
out:
	return ret;
}



//TODO - ftrace trace_events_trigger.c

static LIST_HEAD(trigger_commands);

trigger_show(struct seq_file *m, void *v)
	data = list_entry(v, struct event_trigger_data, list);
	data->ops->print(m, data);


trace_array_get_by_name(const char *name)  /* events/syscalls/sys_enter_exit/trigger */
	ret = trace_array_create_dir(tr);
		ret = event_trace_add_tracer(tr->dir, tr);
			__trace_add_event_dirs(tr);
				list_for_each_entry(call, &ftrace_events, list)
					ret = __trace_add_new_event(call, tr);
						return event_create_dir(tr->event_dir, file);
							trace_create_file("trigger", 0644, file->dir, file, &event_trigger_fops);



static struct trace_event *events[] __initdata = {
	&trace_fn_event,
	&trace_ctx_event,
	&trace_wake_event,
	&trace_stack_event,
	&trace_user_stack_event,
	&trace_bputs_event,
	&trace_bprint_event,
	&trace_print_event,
	&trace_hwlat_event,
	&trace_osnoise_event,
	&trace_timerlat_event,
	&trace_raw_data_event,
	&trace_func_repeats_event,
	&trace_object_event,
	NULL
};


init_events(void)
	struct trace_event *event;
	for (i = 0; events[i]; i++)
		event = events[i];
		ret = register_trace_event(event);
			[event_hash 包含所有events]
			hlist_add_head(&event->node, &event_hash[key]);



static LIST_HEAD(ftrace_commands);




// TODO - trace event

[trace event 静态 TRACE_EVENT]
[trace event 动态 依赖gcc编译插装_mcount]
[kprobe      动态 可以在任意位置通过插入brk实现自定义函数]


TRACE_EVENT(block_rq_complete,
	// trace_event_raw_event_block_rq_complete 函数参数
	TP_PROTO(struct request *rq, int error, unsigned int nr_bytes),
	// trace_event_raw_event_block_rq_complete 函数参数名
	TP_ARGS(rq, error, nr_bytes),
	// 构造结构体 struct trace_event_raw_block_rq_complete
	TP_STRUCT__entry(
		__field(  dev_t,        dev                     )
		__field(  sector_t,     sector                  )
		__field(  unsigned int, nr_sector               )
		__field(  int,          error                   )
		__array(  char,         rwbs,   RWBS_LEN        )
		__dynamic_array( char,  cmd,    1               )
	),
	// 给上面结构体赋值
	TP_fast_assign(
		__entry->dev       = rq->rq_disk ? disk_devt(rq->rq_disk) : 0;
		__entry->sector    = blk_rq_pos(rq);
		__entry->nr_sector = nr_bytes >> 9;
		__entry->error     = error;
		blk_fill_rwbs(__entry->rwbs, rq->cmd_flags, nr_bytes);
		__get_str(cmd)[0] = '\0';
	),
	// 数据写到环形队列trace_event_buffer_commit()
	TP_printk("%d,%d %s (%s) %llu + %u [%d]",
		MAJOR(__entry->dev), MINOR(__entry->dev),
		__entry->rwbs, __get_str(cmd),
		(unsigned long long)__entry->sector,
		__entry->nr_sector, __entry->error)
);


/*trace_event_raw_block_rq_complete 结构体定义*/
struct trace_event_raw_block_rq_complete
{
	struct trace_entry ent;
	dev_t dev;
	sector_t sector;
	unsigned int nr_sector;
	int error;
	char rwbs[8];
	u32 __data_loc_cmd;
	char __data[0];
};




// TODO - TRACE_EVENT 多次展开

[第一次 include/linux/tracepoint.h]

#define TRACE_EVENT(name, proto, args, struct, assign, print)           \
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))


#define DECLARE_TRACE(name, proto, args)                                \
	__DECLARE_TRACE(name, PARAMS(proto), PARAMS(args),              \
			cpu_online(raw_smp_processor_id()),             \
			PARAMS(void *__data, proto),                    \
			PARAMS(__data, args))


#define __DECLARE_TRACE(name, proto, args, cond, data_proto, data_args) \
	extern int __traceiter_##name(data_proto);                      \
	DECLARE_STATIC_CALL(tp_func_##name, __traceiter_##name);        \
	extern struct tracepoint __tracepoint_##name;                   \
	static inline void trace_##name(proto)                          \
	{                                                               \
		if (static_key_false(&__tracepoint_##name.key))         \
			__DO_TRACE(name,                                \
				TP_PROTO(data_proto),                   \
				TP_ARGS(data_args),                     \
				TP_CONDITION(cond), 0);                 \
		if (IS_ENABLED(CONFIG_LOCKDEP) && (cond)) {             \
			rcu_read_lock_sched_notrace();                  \
			rcu_dereference_sched(__tracepoint_##name.funcs);\
			rcu_read_unlock_sched_notrace();                \
		}                                                       \
	}                                                               \
	__DECLARE_TRACE_RCU(name, PARAMS(proto), PARAMS(args),          \
		PARAMS(cond), PARAMS(data_proto), PARAMS(data_args))    \
	static inline int                                               \
	register_trace_##name(void (*probe)(data_proto), void *data)    \
	{                                                               \
		return tracepoint_probe_register(&__tracepoint_##name,  \
						(void *)probe, data);   \
	}                                                               \
	static inline int                                               \
	register_trace_prio_##name(void (*probe)(data_proto), void *data,\
				int prio)                            \
	{                                                               \
		return tracepoint_probe_register_prio(&__tracepoint_##name, \
					(void *)probe, data, prio); \
	}                                                               \
	static inline int                                               \
	unregister_trace_##name(void (*probe)(data_proto), void *data)  \
	{                                                               \
		return tracepoint_probe_unregister(&__tracepoint_##name,\
						(void *)probe, data);   \
	}                                                               \
	static inline void                                              \
	check_trace_callback_type_##name(void (*cb)(data_proto))        \
	{                                                               \
	}                                                               \
	static inline bool                                              \
	trace_##name##_enabled(void)                                    \
	{                                                               \
		return static_key_false(&__tracepoint_##name.key);      \
	}

#include include/trace/define_trace.h

[第二次 include/trace/define_trace.h]

#undef TRACE_EVENT
#define TRACE_EVENT(name, proto, args, tstruct, assign, print)  \
	DEFINE_TRACE(name, PARAMS(proto), PARAMS(args))

#define DEFINE_TRACE(name, proto, args)         \
	DEFINE_TRACE_FN(name, NULL, NULL, PARAMS(proto), PARAMS(args));


#define DEFINE_TRACE_FN(_name, _reg, _unreg, proto, args)               \
	static const char __tpstrtab_##_name[]                          \
	__section("__tracepoints_strings") = #_name;                    \
	extern struct static_call_key STATIC_CALL_KEY(tp_func_##_name); \
	int __traceiter_##_name(void *__data, proto);                   \
	struct tracepoint __tracepoint_##_name  __used                  \
	__section("__tracepoints") = {                                  \
		.name = __tpstrtab_##_name,                             \
		.key = STATIC_KEY_INIT_FALSE,                           \
		.static_call_key = &STATIC_CALL_KEY(tp_func_##_name),   \
		.static_call_tramp = STATIC_CALL_TRAMP_ADDR(tp_func_##_name), \
		.iterator = &__traceiter_##_name,                       \
		.regfunc = _reg,                                        \
		.unregfunc = _unreg,                                    \
		.funcs = NULL };                                        \
	__TRACEPOINT_ENTRY(_name);                                      \
	int __traceiter_##_name(void *__data, proto)                    \
	{                                                               \
		struct tracepoint_func *it_func_ptr;                    \
		void *it_func;                                          \
									\
		it_func_ptr =                                           \
			rcu_dereference_raw((&__tracepoint_##_name)->funcs); \
		do {                                                    \
			it_func = (it_func_ptr)->func;                  \
			__data = (it_func_ptr)->data;                   \
			((void(*)(void *, proto))(it_func))(__data, args); \
		} while ((++it_func_ptr)->func);                        \
		return 0;                                               \
	}                                                               \
	DEFINE_STATIC_CALL(tp_func_##_name, __traceiter_##_name);

[第三次 include/trace/define_trace.h]

#undef DEFINE_EVENT
#define DEFINE_EVENT(template, name, proto, args) \
	DEFINE_TRACE(name, PARAMS(proto), PARAMS(args))

//include/linux/tracepoint.h
#define DEFINE_TRACE(name, proto, args)         \
	DEFINE_TRACE_FN(name, NULL, NULL, PARAMS(proto), PARAMS(args));

//include/linux/tracepoint.h
#define DEFINE_TRACE_FN(_name, _reg, _unreg, proto, args)               \
	static const char __tpstrtab_##_name[]                          \
	__section("__tracepoints_strings") = #_name;                    \
	extern struct static_call_key STATIC_CALL_KEY(tp_func_##_name); \
	int __traceiter_##_name(void *__data, proto);                   \
	struct tracepoint __tracepoint_##_name  __used                  \
	__section("__tracepoints") = {                                  \
		.name = __tpstrtab_##_name,                             \
		.key = STATIC_KEY_INIT_FALSE,                           \
		.static_call_key = &STATIC_CALL_KEY(tp_func_##_name),   \
		.static_call_tramp = STATIC_CALL_TRAMP_ADDR(tp_func_##_name), \
		.iterator = &__traceiter_##_name,                       \
		.regfunc = _reg,                                        \
		.unregfunc = _unreg,                                    \
		.funcs = NULL };                                        \
	__TRACEPOINT_ENTRY(_name);                                      \
	int __traceiter_##_name(void *__data, proto)                    \
	{                                                               \
		struct tracepoint_func *it_func_ptr;                    \
		void *it_func;                                          \
									\
		it_func_ptr =                                           \
			rcu_dereference_raw((&__tracepoint_##_name)->funcs); \
		do {                                                    \
			it_func = (it_func_ptr)->func;                  \
			__data = (it_func_ptr)->data;                   \
			((void(*)(void *, proto))(it_func))(__data, args); \
		} while ((++it_func_ptr)->func);                        \
		return 0;                                               \
	}                                                               \
	DEFINE_STATIC_CALL(tp_func_##_name, __traceiter_##_name);

[第四次 include/trace/define_trace.h]

#undef DECLARE_TRACE
#define DECLARE_TRACE(name, proto, args)        \
	DEFINE_TRACE(name, PARAMS(proto), PARAMS(args))

//include/linux/tracepoint.h
#define DEFINE_TRACE(name, proto, args)         \
	DEFINE_TRACE_FN(name, NULL, NULL, PARAMS(proto), PARAMS(args));

//include/linux/tracepoint.h
#define DEFINE_TRACE_FN(_name, _reg, _unreg, proto, args)               \
	static const char __tpstrtab_##_name[]                          \
	__section("__tracepoints_strings") = #_name;                    \
	extern struct static_call_key STATIC_CALL_KEY(tp_func_##_name); \
	int __traceiter_##_name(void *__data, proto);                   \
	struct tracepoint __tracepoint_##_name  __used                  \
	__section("__tracepoints") = {                                  \
		.name = __tpstrtab_##_name,                             \
		.key = STATIC_KEY_INIT_FALSE,                           \
		.static_call_key = &STATIC_CALL_KEY(tp_func_##_name),   \
		.static_call_tramp = STATIC_CALL_TRAMP_ADDR(tp_func_##_name), \
		.iterator = &__traceiter_##_name,                       \
		.regfunc = _reg,                                        \
		.unregfunc = _unreg,                                    \
		.funcs = NULL };                                        \
	__TRACEPOINT_ENTRY(_name);                                      \
	int __traceiter_##_name(void *__data, proto)                    \
	{                                                               \
		struct tracepoint_func *it_func_ptr;                    \
		void *it_func;                                          \
									\
		it_func_ptr =                                           \
			rcu_dereference_raw((&__tracepoint_##_name)->funcs); \
		do {                                                    \
			it_func = (it_func_ptr)->func;                  \
			__data = (it_func_ptr)->data;                   \
			((void(*)(void *, proto))(it_func))(__data, args); \
		} while ((++it_func_ptr)->func);                        \
		return 0;                                               \
	}                                                               \
	DEFINE_STATIC_CALL(tp_func_##_name, __traceiter_##_name);



// TODO - trace enable

# echo 1 > /sys/kernel/debug/tracing/events/block/block_rq_complete/enable

event_enable_write(struct file *filp, const char __user *ubuf, size_t cnt,loff_t *ppos)
    |--struct trace_event_file *file
    |--kstrtoul_from_user(ubuf, cnt, 10, &val)
    |--tracing_update_buffers()
    |  //前面在trace_create_file的时候会将trace_event_file保存在inode->i_private
    |--file = event_file_data(filp)
    |--ftrace_event_enable_disable(file, val)
	   |  //此处以enable为1举例
	   |--__ftrace_event_enable_disable(file, enable, 0)
		  |--trace_event_reg(call, type, data)
			 |  //将tracepoint与probe绑定
			 |--tracepoint_probe_register(call->tp,call->class->probe,file);
				|--tracepoint_probe_register_prio(tp, probe, data, TRACEPOINT_DEFAULT_PRIO)
				       |--struct tracepoint_func tp_func;
				       |  tp_func.func = probe;
				       |  tp_func.data = data;
				       |  tp_func.prio = prio;
				       |--tracepoint_add_func(tp, &tp_func, prio);
					      |--struct tracepoint_func *old, *tp_funcs;
					      |--tp_funcs = rcu_dereference_protected(tp->funcs,
					      |                   lockdep_is_held(&tracepoints_mutex));
					      |--old = func_add(&tp_funcs, func, prio);
					      |       |--struct tracepoint_func *old, *new;
					      |       |--new = allocate_probes(nr_probes + 2)
					      |       |--new[pos] = *tp_func
					      |       |--new[nr_probes + 1].func = NULL;
					      |       |  //new保存到tp_funcs
					      |       |--*funcs = new;
					      |--rcu_assign_pointer(tp->funcs, tp_funcs);
					      |--tracepoint_update_call(tp, tp_funcs, false);
					      |  //使能key
					      |--static_key_enable(&tp->key);


[__DO_TRACE 输出打印]

#define __DO_TRACE(name, proto, args, cond, rcuidle)                    \
	do {                                                            \
		struct tracepoint_func *it_func_ptr;                    \
		int __maybe_unused __idx = 0;                           \
		void *__data;                                           \
									\
		if (!(cond))                                            \
			return;                                         \
									\
		/* srcu can't be used from NMI */                       \
		WARN_ON_ONCE(rcuidle && in_nmi());                      \
									\
		/* keep srcu and sched-rcu usage consistent */          \
		preempt_disable_notrace();                              \
									\
		/*                                                      \
		 * For rcuidle callers, use srcu since sched-rcu        \
		 * doesn't work from the idle path.                     \
		 */                                                     \
		if (rcuidle) {                                          \
			__idx = srcu_read_lock_notrace(&tracepoint_srcu);\
			rcu_irq_enter_irqson();                         \
		}                                                       \
		//此处__tracepoint_##name可知为__tracepoint_block_rq_complete
		it_func_ptr =                                           \
			rcu_dereference_raw((&__tracepoint_##name)->funcs); \
		if (it_func_ptr) {                                      \
			__data = (it_func_ptr)->data;                   \
			__DO_TRACE_CALL(name)(args);                    \
		}                                                       \
									\
		if (rcuidle) {                                          \
			rcu_irq_exit_irqson();                          \
			srcu_read_unlock_notrace(&tracepoint_srcu, __idx);\
		}                                                       \
									\
		preempt_enable_notrace();                               \
	} while (0)


[__DO_TRACE_CALL 找到traceiter]

#define __DO_TRACE_CALL(name)   __traceiter_##name


[tracepoint 使用三步骤]

在头文件include/trace/events/subsys.h中通过DECLARE_TRACE宏添加tracepoint声明
在系统文件subsys/file.c中通过DEFINE_TRACE创建trace point
通过register_trace_subsys_eventname将tracepoint与probe关联



[brk #0x4 之后的异常]

#0  kprobe_handler (regs=0xffff80001253bcf0) at arch/arm64/kernel/probes/kprobes.c:352
#1  kprobe_breakpoint_handler (regs=0xffff80001253bcf0, esr=<optimized out>) at arch/arm64/kernel/probes/kprobes.c:404
#2  0xffff8000100148c4 in call_break_hook (regs=regs@entry=0xffff80001253bcf0, esr=esr@entry=4060086276) at arch/arm64/kernel/debug-monitors.c:322
#3  0xffff800010014a00 in brk_handler (unused=<optimized out>, esr=4060086276, regs=0xffff80001253bcf0) at arch/arm64/kernel/debug-monitors.c:329
#4  0xffff800010036180 in do_debug_exception (addr_if_watchpoint=addr_if_watchpoint@entry=5651652, esr=esr@entry=4060086276, regs=regs@entry=0xffff80001253bcf0) at arch/arm64/mm/fault.c:848
#5  0xffff800010cad220 in el1_dbg (regs=0xffff80001253bcf0, esr=4060086276) at arch/arm64/kernel/entry-common.c:190
#6  0xffff800010cad468 in el1_sync_handler (regs=<optimized out>) at arch/arm64/kernel/entry-common.c:227
#7  0xffff8000100119bc in el1_sync () at arch/arm64/kernel/entry.S:627



[trace events 查看现有events]
cat /sys/kernel/debug/tracing/available_events


[trace events trigger 当该trigger被触发时 执行 command]

# echo 'command[:count] [if filter]' > trigger
# echo '!command[:count] [if filter]' > trigger


/*
 * [command = enable_event / disable_event]
 */

# echo 'enable_event:kmem:kmalloc:1' > \
    /sys/kernel/debug/tracing/events/syscalls/sys_enter_read/trigger


# echo 'disable_event:kmem:kmalloc' > \
    /sys/kernel/debug/tracing/events/syscalls/sys_exit_read/trigger

/*
 * [command = stacktrace / stacktrace 执行dump stack]
 */

# echo 'stacktrace' > \
      /sys/kernel/debug/tracing/events/kmem/kmalloc/trigger


# echo 'stacktrace:5 if bytes_req >= 65536' > \
      /sys/kernel/debug/tracing/events/kmem/kmalloc/trigger


[ftrace 函数入口插入mcount] [kprobe 任意位置插入断点]



[kprobe events 定义]

echo 'p:myprobe do_sys_open+32 dfd=%x1 filename=%x2 flags=%x3 mode=+4($stack)' > /sys/kernel/debug/tracing/kprobe_events
echo 1 > /sys/kernel/debug/tracing/events/kprobes/myprobe/enable
...
echo 0 > /sys/kernel/debug/tracing/events/kprobes/myprobe/enable
echo   > /sys/kernel/debug/tracing/kprobe_events

cat /sys/kernel/debug/tracing/events/kprobes/myprobe/format
name: myprobe
ID: 780
format:
	field:unsigned short common_type;       offset:0;       size:2; signed:0;
	field:unsigned char common_flags;       offset:2;       size:1; signed:0;
	field:unsigned char common_preempt_count;       offset:3; size:1;signed:0;
	field:int common_pid;   offset:4;       size:4; signed:1;

	field:unsigned long __probe_ip; offset:12;      size:4; signed:0;
	field:int __probe_nargs;        offset:16;      size:4; signed:1;
	field:unsigned long dfd;        offset:20;      size:4; signed:0;
	field:unsigned long filename;   offset:24;      size:4; signed:0;
	field:unsigned long flags;      offset:28;      size:4; signed:0;
	field:unsigned long mode;       offset:32;      size:4; signed:0;

print fmt: "(%lx) dfd=%lx filename=%lx flags=%lx mode=%lx", REC->__probe_ip,
REC->dfd, REC->filename, REC->flags, REC->mode


[p:probe1 定义动态events probe1名字]
echo "p:probe1 input_event type=%x1 code=%x2 value=%x3" > /sys/kernel/debug/tracing/kprobe_events

[perf probe -d * 删除events]


// TODO - [kretprobe events 定义] [do_sys_open 通过available_filter_functions查看]

echo 'r:myretprobe do_sys_open $retval' >> /sys/kernel/debug/tracing/kprobe_events
echo 1 > /sys/kernel/debug/tracing/events/kprobes/myretprobe/enable
...
echo 0 > /sys/kernel/debug/tracing/events/kprobes/myretprobe/enable
echo   > /sys/kernel/debug/tracing/kprobe_events


// TODO - [arm64 栈帧解析]

https://blog.csdn.net/jasonactions/article/details/118353455




// TODO - [DEFINE_EVENT 静态 tracepoint] [TRACE_EVENT 静态 traceevent 在./events目录下]



// TODO - [perf-tools brendangregg]



// TODO - [trace-cmd record]

[trace-cmd 是一个ftrace子系统的前端工具]

[-p 探针类型]
[-g funct_graph追踪的符号]
[-O option to enable]
[ls 执行目标]
trace-cmd record -p function_graph -g do_sys_open -O funcgraph-proc ls

[-l function追踪目]
trace-cmd record -p function -l do_page_fault

[-P PID过滤]
trace-cmd record -p function -P 25314

[-e 追踪事件 模块:函数]
trace-cmd record -e sched:sched_switch

[-M cpu mask]
trace-cmd record -M 2 -p function_graph ./iotest

[-n 不追踪的符号]

[--func-stack 记录调用栈 追踪系统调用open]
trace-cmd record -p function -l do_sys_open --func-stack ls

// TODO - [trace-cmd show]

trace-cmd show

// TODO - [trace-cmd start]

[start 记录ftrace 但是不输出文件]
[-e irq事件]
trace-cmd start -e irq

[-p 探针类型 用于reset]
trace-cmd start -p nop

// TODO - [trace-cmd list]

[-t 查看tracer]
trace-cmd list -t

[-f 查看可追踪的符号]
trace-cmd list -f


// TODO - [trace-cmd report]

[--cpu 解析指定cpuX的数据]
trace-cmd report --cpu 1


// TODO - [trace-cmd reset]

trace-cmd reset

// TODO - [systemtap]

[systemtap 通过stap工具 将stp脚本生成c文件 并通过gcc编译成ko]
[systemtap 使用了kprobe机制]


// TODO - [strace]

[strace 专门记录系统调用]



// TODO - [kprobe 模块化 refs]

https://www.cnblogs.com/dongxb/p/17297613.html