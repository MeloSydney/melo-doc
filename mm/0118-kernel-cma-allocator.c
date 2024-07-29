


//TODO - kernel cma 保留内存

/* dts 定义 */
single_soc_shm_service_reserved:shm_cma {
	compatible = "shared-dma-pool";
	reusable;
	size = <0x0 0x1B000000>; //432MB for chnl
	alloc-ranges = <0xF 0x23000000 0x0 0x1B000000>;
	linux,chnl-cma;
};

/* cma_init_reserved_mem 解析dts 创建cma对象 放入cma_areas */
cma_init_reserved_mem
	cma = &cma_areas[cma_area_count];

"shared-dma-pool"


/* shmm_service 接管 */
cma_shmm_service_probe
	init_status = cma_shmm_areas_init();
		cma = cma_find_by_name(CMA_SHM_NAME);
		cma_share_dev->cma_area = cma;
		ret = dma_heap_add_cma(cma_share_dev);


/* cma_shmm_alloc_phy_addr export出来的cma分配接口 */



/* 系统 kernel 初始化 cma */
setup_arch(char **cmdline_p)
	arm_memblock_init(mdesc);
		early_init_fdt_scan_reserved_mem();
			fdt_init_reserved_mem();
				err = __reserved_mem_init_node(rmem);
					/* __reservedmem_of_table 是初始化中的一个section段，通过RESERVEDMEM_OF_DECLARE定义的都会被链接到这个段中 */
					extern const struct of_device_id __reservedmem_of_table[];
					for (i = __reservedmem_of_table; i < &__rmem_of_table_sentinel; i++) {
						reservedmem_of_init_fn initfn = i->data;   /* rmem_cma_setup */
						const char *compat = i->compatible;        /* "shared-dma-pool" */
						if (!of_flat_dt_is_compatible(rmem->fdt_node, compat))
						ret = initfn(rmem);
					}




/* cma.c */
RESERVEDMEM_OF_DECLARE(cma, "shared-dma-pool", rmem_cma_setup); /* __reservedmem_of_table 之间时什么联系 ? */
	rmem_cma_setup(struct reserved_mem *rmem)
		err = cma_init_reserved_mem(rmem->base, rmem->size, 0, rmem->name, &cma);
			struct cma *cma;
			cma = &cma_areas[cma_area_count];
			snprintf(cma->name, CMA_MAX_NAME,  "cma%d\n", cma_area_count);
			cma->base_pfn = PFN_DOWN(base);
			cma->count = size >> PAGE_SHIFT;
			cma->order_per_bit = order_per_bit;


/* google RESERVEDMEM_OF_DECLARE __reservedmem_of_table */

RESERVEDMEM_OF_DECLARE 声明一个section

#define RESERVEDMEM_OF_DECLARE(name, compat, init)			\
	_OF_DECLARE(reservedmem, name, compat, init, reservedmem_of_init_fn)


#define _OF_DECLARE(table, name, compat, fn, fn_type)			\  /* name: dma cma */
	static const struct of_device_id __of_table_##name		\
		__used __section("__" #table "_of_table")		\  /* __reservedmem_of_table */
		__aligned(__alignof__(struct of_device_id))		\
		 = { .compatible = compat,				\  /* "shared-dma-pool" */
		     .data = (fn == (fn_type)NULL) ? fn : fn  }            /* rmem_cma_setup */


struct of_device_id {
	char	name[32];
	char	type[32];
	char	compatible[128];
	const void *data;   /* fn */
};


/* 保留内存 数据结构 */
struct reserved_mem {
	const char			*name;
	unsigned long			fdt_node;
	unsigned long			phandle;
	const struct reserved_mem_ops	*ops;
	phys_addr_t			base;
	phys_addr_t			size;
	void				*priv;
};


cma_find_by_name(const char *name)
	if (0 == strcmp(cma_areas[i].name, name))
		return &cma_areas[i];


/* chnl cma 管理器 */
cma_shmm_service_probe(struct platform_device *pdev)
	init_status = cma_shmm_areas_init();
		cma = cma_find_by_name(CMA_SHM_NAME);
		cma_share_dev = cma_shmm_misc.this_device;
		cma_share_dev->cma_area = cma;
		ret = dma_heap_add_cma(cma_share_dev);
			struct cma *cma = dev_get_cma_area(dev);
			return __add_cma_heap(cma, NULL);
				struct cma_heap *cma_heap;
				struct dma_heap_export_info exp_info;
				cma_heap = kzalloc(sizeof(*cma_heap), GFP_KERNEL);
				cma_heap->cma = cma;
				exp_info.name = cma_get_name(cma);
				exp_info.ops = &cma_heap_ops;   /* cma_heap_allocate */
				exp_info.priv = cma_heap;
				cma_heap->heap = dma_heap_add(&exp_info);   /* allocate */
					struct dma_heap *heap, *err_ret;
					heap = kzalloc(sizeof(*heap), GFP_KERNEL);
					kref_init(&heap->refcount);
					heap->name = exp_info->name;
					heap->ops = exp_info->ops;
					heap->priv = exp_info->priv;
					heap->heap_devt = MKDEV(MAJOR(dma_heap_devt), minor);
					cdev_init(&heap->heap_cdev, &dma_heap_fops);
					ret = cdev_add(&heap->heap_cdev, heap->heap_devt, 1);
					heap->heap_dev = device_create(dma_heap_class,
									NULL,
									heap->heap_devt,
									NULL,
									heap->name);
					list_add(&heap->list, &heap_list);


/* cma 释放给 buddy */
cma_init_reserved_areas(void)
	cma_activate_area(&cma_areas[i]);
		cma->bitmap = bitmap_zalloc(cma_bitmap_maxno(cma), GFP_KERNEL);
		zone = page_zone(pfn_to_page(base_pfn));
		for (pfn = base_pfn; pfn < base_pfn + cma->count; pfn += pageblock_nr_pages)
			init_cma_reserved_pageblock(pfn_to_page(pfn));
				__ClearPageReserved(p);
				set_pageblock_migratetype(page, MIGRATE_CMA);
				set_page_refcounted(page);
				__free_pages(page, pageblock_order);


3 dma_buf

/* struct dmabuf */

struct dma_buf {
	size_t size;
	struct file *file;
	const struct dma_buf_ops *ops;
	const char *exp_name;
	const char *name;
	void *priv;
	struct dma_resv *resv;
}


4 dma_heap


/* struct dma_heap */
struct dma_heap {
	const char *name;
	const struct dma_heap_ops *ops;
	void *priv;
	struct list_head list;
	struct cdev heap_cdev;
	struct kref refcount;
	struct device *heap_dev;
};

struct dma_heap_ops {
	struct dma_buf *(*allocate)(struct dma_heap *heap,
				    unsigned long len,
				    unsigned long fd_flags,
				    unsigned long heap_flags);
};


static const unsigned int orders[] = {8, 4, 0};   /* (1MB, 64K, 4K)  */
#define NUM_ORDERS ARRAY_SIZE(orders)

/* dma-heap init */
system_heap_create(void)
	struct dma_heap_export_info exp_info;
	for (i = 0; i < NUM_ORDERS; i++) {
		pools[i] = dmabuf_page_pool_create(order_flags[i], orders[i]);
	}
	exp_info.name = "system";
	exp_info.ops = &system_heap_ops;
	exp_info.priv = NULL;
	sys_heap = dma_heap_add(&exp_info);
		heap = kzalloc(sizeof(*heap), GFP_KERNEL);
		heap->name = exp_info->name;
		heap->ops = exp_info->ops;
		heap->priv = exp_info->priv;
		heap->heap_devt = MKDEV(MAJOR(dma_heap_devt), minor);
		cdev_init(&heap->heap_cdev, &dma_heap_fops);
		ret = cdev_add(&heap->heap_cdev, heap->heap_devt, 1);
		heap->heap_dev = device_create(dma_heap_class,
						NULL,
						heap->heap_devt,
						NULL,
						heap->name);
		list_add(&heap->list, &heap_list);   /* heap_list 管理 所有 dmabuf heap */
	exp_info.name = "system-uncached";
	exp_info.ops = &system_uncached_heap_ops;
	exp_info.priv = NULL;
	sys_uncached_heap = dma_heap_add(&exp_info);
		...
	err = set_heap_dev_dma(dma_heap_get_dev(sys_uncached_heap));
	system_uncached_heap_ops.allocate = system_uncached_heap_allocate;


static const struct dma_heap_ops system_heap_ops = {
	.allocate = system_heap_allocate,
		system_heap_allocate(struct dma_heap *heap, unsigned long len, unsigned long fd_flags, unsigned long heap_flags)
			return system_heap_do_allocate(heap, len, fd_flags, heap_flags, false);
				buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
				DEFINE_DMA_BUF_EXPORT_INFO(exp_info);
				buffer->heap = heap;
				buffer->len = len;
				buffer->uncached = uncached;
	.get_pool_size = system_get_pool_size,
};


dma_heap_open(struct inode *inode, struct file *file)     /* "system-uncached" */
	struct dma_heap *heap;
	heap = xa_load(&dma_heap_minors, iminor(inode));
	file->private_data = heap;



dma_heap_ioctl(struct file *file, unsigned int ucmd, unsigned long arg)   /* dma_heap_fops */
	switch (kcmd)
	case DMA_HEAP_IOCTL_ALLOC:
		dma_heap_ioctl_allocate(struct file *file, void *data)
			struct dma_heap_allocation_data *heap_allocation = data;
			struct dma_heap *heap = file->private_data;
			dma_heap_bufferfd_alloc(struct dma_heap *heap, size_t len, unsigned int fd_flags, unsigned int heap_flags)
				struct dma_buf *dmabuf;
				dmabuf = dma_heap_buffer_alloc(heap, len, fd_flags, heap_flags);
					return heap->ops->allocate(heap, len, fd_flags, heap_flags);
				fd = dma_buf_fd(dmabuf, fd_flags);
			heap_allocation->fd = fd;



5 buddy

struct zone
	free_area[order]
	watermark
		high
		low
		min
	lowmen_reserved
	zone_pgdat
	zone_start_pfn
	managed_pages = present_pages - reserved_pages
	spanned_pages = zone_end_pfn - zone_start_pfn





6 highmem

