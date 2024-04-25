

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
	ftrace_init();
		unsigned long count, flags;
		extern unsigned long __start_mcount_loc[];
		extern unsigned long __stop_mcount_loc[];
		count = __stop_mcount_loc - __start_mcount_loc;
		ret = ftrace_process_locs(NULL, __start_mcount_loc, __stop_mcount_loc);
			struct ftrace_page *start_pg;
			start_pg = ftrace_allocate_pages(count);
			ftrace_update_code(mod, start_pg);  /* mod = NULL */
	early_trace_init();
		tracer_alloc_buffers();
			temp_buffer = ring_buffer_alloc(PAGE_SIZE, RB_FL_OVERWRITE);
			register_tracer(&nop_trace);
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
				return register_tracer(&function_trace);
			if (allocate_objtrace_data(&global_trace))
				obj_data = kzalloc(sizeof(*obj_data), GFP_KERNEL);
				obj_data->tr = tr;
				fops = &obj_data->fops;
				fops->func = trace_object_events_call;
				fops->flags = FTRACE_OPS_FL_SAVE_REGS;
				fops->private = tr;
				list_add(&obj_data->head, &obj_data_head);
				tr->obj_data = obj_data;
	trace_init();
		trace_event_init();
			event_trace_enable();
				register_trigger_cmds();
					register_trigger_object_cmd();




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
{
	struct pt_regs *pt_regs = ftrace_get_regs(fregs);
	unsigned long obj;
	long disabled;
	int cpu, n;

	preempt_disable_notrace();

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
}

static struct ftrace_ops trace_ops = {
	.func  = trace_object_events_call,
	.flags = FTRACE_OPS_FL_SAVE_REGS,
};

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

static struct event_command trigger_object_cmd = {
	.name			= "objtrace",
	.trigger_type		= ETT_TRACE_OBJECT,
	.flags			= EVENT_CMD_FL_NEEDS_REC,
	.func			= event_object_trigger_callback,
	.parse			= event_object_trigger_parse,
	.reg			= register_object_trigger,
	.unreg			= unregister_object_trigger,
	.get_trigger_ops	= objecttrace_get_trigger_ops,
	.set_filter		= set_trigger_filter,
};

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
