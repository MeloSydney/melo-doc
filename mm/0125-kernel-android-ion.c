

//TODO - ion 保留内存

struct ion_device {
	struct miscdevice dev;    /* /dev/ion */
	struct plist_head heaps;  /* 管理所有 ion_heap */
	int heap_cnt;
};

/* heap代表一段保留内存 */
struct ion_heap {
	struct plist_node node;
	enum ion_heap_type type;
	struct ion_heap_ops *ops;
	struct dma_buf_ops buf_ops;
	unsigned int id;
	const char *name;
	u64 num_of_buffers;
	u64 num_of_alloc_bytes;
	u64 alloc_bytes_wm;
};

struct ion_heap_ops {
	int (*allocate)(struct ion_heap *heap,
			struct ion_buffer *buffer, unsigned long len,
			unsigned long flags);
	void (*free)(struct ion_buffer *buffer);
};

/* 代表从heap上分出去的一块内存 */
struct ion_buffer {
	struct list_head list;
	struct ion_heap *heap;   /* 属于 哪个 ion_heap */
	size_t size;
	void *vaddr;
	struct sg_table *sg_table;
	struct list_head attachments;
};


oms_remote: oms_remote{
	status = "okay";
	compatible = "ecarx,dfs-video-core";
	sequece-inst = /bits/ 16 <0>;	//video_##sequece-inst##dfsc
	data-id		 = <0x2ED1E3F5 0x20102A9E>;
	ctrl-id		 = <0x55F5A0D0 0xCE0153B0>;
	video-width	 = <1920>;
	video-height = <1080>;
	video-fmt 	 = "UYVY";
	video-fps    = <30>;
	dfsc-rmem	 = "dfs-cam0-rmem";   // ion 保留内存
	depend-on	 = <&dfs_device>;
};

reserved-memory {
	dfs-cam0-rmem {   // ion 保留内存
		compatible = "shared-dma-pool";
		size = <0x0 0x1000000>;
		//16M, 1920*1080*2*4 + offset(128) < 16*1024*1024
		alloc-ranges = <0xF 0x3E000000 0x0 0x1000000>;
		/*customize fixed type, ref to uapi/linux/ion.h ION_HEAP_TYPE_   */
		type = /bits/ 16 <10>;
		linux,dfs_camerai0_rmem;
	};
};


static struct ion_heap_ops ion_rmem_ops = {
	.allocate = ion_dfs_rmem_allocate,
	.free = ion_dfs_rmem_free,
};


ion_dfs_heap_init(struct dfsc_dev *ddev, const char *node_name)   /* node_name "dfs-cam0-rmem" */
	np = of_find_node_by_name(NULL, node_name);  // 找到dts node
	if (dfs_addr_info(np, &ioc_heaps->dfs_rmem_addr, &ioc_heaps->dfs_rmem_size) < 0)  /* 地址和长度 */
		if (of_property_read_u32_array(np, "alloc-ranges", mem_arr, ARRAY_SIZE(mem_arr)))
		dfs_addr = mem_arr[0];
		dfs_addr = dfs_addr << 32 | mem_arr[1];
		dfs_mem_size = mem_arr[2];
		dfs_mem_size = dfs_mem_size << 32 | mem_arr[3];
		*paddr = dfs_addr;
		*len = dfs_mem_size;
	if (of_property_read_u16(np, "type", &type))  /* type类型 */
	ioc_heaps->heap.ops = &ion_rmem_ops;
	ioc_heaps->heap.type = (enum ion_heap_type)type;
	ioc_heaps->heap.name = np->name;
	return ion_device_add_heap(&ioc_heaps->heap);   /* 系统初始化ion struct ion_heap */
		__ion_device_add_heap(heap, THIS_MODULE)
			struct ion_device *dev = internal_dev;
			ret = ion_assign_heap_id(heap, dev);
				switch (heap->type)
				case ION_HEAP_TYPE_CUSTOM ... ION_HEAP_TYPE_MAX:
					start_bit = __ffs(ION_HEAP_CUSTOM_START);
					end_bit = __ffs(ION_HEAP_CUSTOM_END);
					break;
			plist_add(&heap->node, &dev->heaps);


struct ion_device {
	struct miscdevice dev;
	struct plist_head heaps;  /* 管理 所有 ion_heap plist_add */
	int heap_cnt;
};


/* ion 保留内存 申请 camera-hal */
SetDataSizeAndOffsetDfs(size_t data_size, int offset, const std::string mDevicePath)
	mIonFd = ion_open();
	ret = ion_query_heap_cnt(mIonFd, &heap_cnt);
	struct ion_heap_data heap_data[heap_cnt];
	ret = ion_query_get_heaps(mIonFd, heap_cnt, heap_data);
	if (0 == mDevicePath.compare("/dev/video0dfsc"))
		type = (enum ion_heap_type)10;//ION_HEAP_TYPE_CAM_DFSC0;
	else if (0 == mDevicePath.compare("/dev/video1dfsc"))
		type = (enum ion_heap_type)11;//ION_HEAP_TYPE_CAM_DFSC1;
	for (i = 0; i < heap_cnt; i++) {
		if (heap_data[i].type == type) {
			heap_mask = 1 << heap_data[i].heap_id;
			break;
		}
	}
	ret = ion_alloc_fd(mIonFd, requiredSize, 0, heap_mask, ION_HEAP_TYPE_DMA, &dmaBuf);


EnqueueRequest(std::shared_ptr<default_camera_hal::CaptureRequest> request)
	request_context->camera_buffer->SetDataSizeAndOffsetDfs(requiredSize - CIF_BUFFER_HEADER_LEN, \
								CIF_BUFFER_HEADER_LEN,
								getDevicePath());
	if (IoctlLocked(VIDIOC_QBUF, &device_buffer) < 0)   /* 将ion 保留内存交给dfsc */


/* ion 子系统 初始化 */
ion_device_create(void)
	idev = kzalloc(sizeof(*idev), GFP_KERNEL);
	idev->dev.minor = MISC_DYNAMIC_MINOR;
	idev->dev.name = "ion";      /* /dev/ion */
	idev->dev.fops = &ion_fops;  /* ion_ioctl */
	idev->dev.parent = NULL;
	ret = misc_register(&idev->dev);
	internal_dev = idev;


/* misc ion ioctl */
union ion_ioctl_arg {
	struct ion_allocation_data allocation;
	struct ion_heap_query query;
	u32 ion_abi_version;
};

ion_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
	union ion_ioctl_arg data;
	switch (cmd)
	case ION_IOC_ALLOC:
		fd = ion_alloc_fd(data.allocation.len, data.allocation.heap_id_mask, data.allocation.flags);
			struct dma_buf *dmabuf;
			dmabuf = ion_dmabuf_alloc(internal_dev, len, heap_id_mask, flags);
				buffer = ion_buffer_alloc(dev, len, heap_id_mask, flags);
					plist_for_each_entry(heap, &dev->heaps, node)
						if (!((1 << heap->id) & heap_id_mask))
							continue;
						buffer = ion_buffer_create(heap, dev, len, flags);
							struct ion_buffer *buffer;
							/* ion_dfs_rmem_allocate */
							ret = heap->ops->allocate(heap, buffer, len, flags);
				exp_info.ops = &dma_buf_ops;
				exp_info.size = buffer->size;
				exp_info.flags = O_RDWR;
				exp_info.priv = buffer;
				dmabuf = dma_buf_export(&exp_info);
				return dmabuf;
			fd = dma_buf_fd(dmabuf, O_CLOEXEC);
			return fd;
		data.allocation.fd = fd;
		break;
	case ION_IOC_HEAP_QUERY:
		ret = ion_query_heaps(&data.query);
			plist_for_each_entry(heap, &dev->heaps, node)
				strncpy(hdata.name, heap->name, MAX_HEAP_NAME);
				hdata.name[sizeof(hdata.name) - 1] = '\0';
				hdata.type = heap->type;
				hdata.heap_id = heap->id;
		break;
