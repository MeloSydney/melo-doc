
//TODO - kernel v4l2 doc

https://www.cnblogs.com/LoyenWang/p/15456230.html
https://ecarxgroup.feishu.cn/file/boxcns9nibYT4lp020tozHZ7m5g
https://www.cnblogs.com/silence-hust/p/4464291.html
https://deepinout.com/v4l2-tutorials/linux-v4l2-architecture.html
https://blog.csdn.net/seiyaaa/article/details/120199720
https://github.com/hceng/learn/blob/master/tiny4412/07_vivid/vivid_core%E5%88%86%E6%9E%90.txt
https://blog.csdn.net/weixin_43503508/article/details/107901790
https://blog.csdn.net/hnllei/article/details/45564125
https://zhuanlan.zhihu.com/p/443727970
https://ciellee.blog.csdn.net/article/details/109628739  /*ion和v4l2 dmabuf*/

//TODO - kernel dfsc

drivers/media/platform/dfs-camera/dfsc-core.c         /* vicam v4l2驱动 */
drivers/media/platform/dfs-camera/dfsc-vid-cap.c
drivers/media/platform/dfs-camera/dfsc-vid-common.c
drivers/media/platform/dfs-camera/ion_dfs_heap.c      /* 保留内存 ion */
drivers/media/platform/siengine/sensor/max9xx_cam.c   /* 物理摄像头 sensor */
drivers/misc/ecarxdfs/dfs_oms_impl.c                  /* dfs内核态驱动 */
drivers/misc/ecarxdfs/dfs_rpmsg_impl.c


/*
  1. 如何分配内存 ? 应用层先申请ion内存 有对应的ion设备节点和heap, 然后通过video的ioctl QBUF将dmabuf fd传给内核态v4l2
  2. 数据流如何工作 ? 应用层有epoll机制等待数据帧, dfs内核态收到数据后 触发v4l2的done_list机制 并唤醒上层应用
  3. 上层应用如何工作 ? epoll阻塞等待底层dfsc数据, 如果被唤醒 DQBUF取出对应的内存消费, 消费完毕后 QBUF将内存还给v4l2, v4l2再通知远端dfs该内存可用
  4. 双端内存如何分配 ? master是物理摄像头端, 使用系统分配的dmabuf 通过elink dma匿名页, slave是虚拟摄像头端, 系统有保留的4*8M=32M内存, 提供给v4l2使用
*/

/* vb2 buf 状态 */
enum vb2_buffer_state {
	VB2_BUF_STATE_DEQUEUED,    /* dqbuf   vidioc_create_bufs */
	VB2_BUF_STATE_IN_REQUEST,  /* qbuf    __buf_prepare */
	VB2_BUF_STATE_PREPARING,
	VB2_BUF_STATE_QUEUED,      /* vb2_buffer_done qbuf */
	VB2_BUF_STATE_ACTIVE,      /* start_streaming */
	VB2_BUF_STATE_DONE,
	VB2_BUF_STATE_ERROR,
};


/* v4l2-ioctl.c
   v4l2_ioctls 提供了非常多的ioctl供上层使用, 例如打开, 关闭, QBUF, DQBUF
 */

struct v4l2_ioctl_info {
	unsigned int ioctl;
	u32 flags;
	const char * const name;
	int (*func)(const struct v4l2_ioctl_ops *ops, struct file *file,
		    void *fh, void *p);
	void (*debug)(const void *arg, bool write_only);
};

#define IOCTL_INFO(_ioctl, _func, _debug, _flags)		\
	[_IOC_NR(_ioctl)] = {					\
		.ioctl = _ioctl,				\
		.flags = _flags,				\
		.name = #_ioctl,				\
		.func = _func,					\
		.debug = _debug,				\
	}

static const struct v4l2_ioctl_info v4l2_ioctls[] = {
	IOCTL_INFO(VIDIOC_STREAMON, v4l_streamon, v4l_print_buftype, INFO_FL_PRIO | INFO_FL_QUEUE),    /* _ioctl, _func, _debug, _flags */
	IOCTL_INFO(VIDIOC_STREAMOFF, v4l_streamoff, v4l_print_buftype, INFO_FL_PRIO | INFO_FL_QUEUE),
	...
}

/* v4l2 stream off */
IOCTL_INFO(VIDIOC_STREAMOFF, v4l_streamoff, v4l_print_buftype, INFO_FL_PRIO | INFO_FL_QUEUE),
	v4l_streamoff(const struct v4l2_ioctl_ops *ops, struct file *file, void *fh, void *arg)
		return ops->vidioc_streamoff(file, fh, *(unsigned int *)arg); /* dfsc_streamoff */
			dfsc_streamoff(struct file *file, void *priv, enum v4l2_buf_type type)
				struct dfsc_dev *ddev = video_drvdata(file);
				ddev->dfs_stream_status = 0;
				ret = vb2_ioctl_streamoff(file, priv, type);
					return vb2_streamoff(vdev->queue, i);
						return vb2_core_streamoff(q, type);
							__vb2_queue_cancel(q);
								if (q->start_streaming_called)
									call_void_qop(q, stop_streaming, q);
								if (WARN_ON(atomic_read(&q->owned_by_drv_count))) {
									for (i = 0; i < q->num_buffers; ++i)
										if (q->bufs[i]->state == VB2_BUF_STATE_ACTIVE)
											vb2_buffer_done(q->bufs[i], VB2_BUF_STATE_ERROR);
									WARN_ON(atomic_read(&q->owned_by_drv_count)); /* Must be zero now */
								}
								q->streaming = 0;
								q->start_streaming_called = 0;
								q->queued_count = 0;
								q->error = 0;
								q->uses_requests = 0;
								q->uses_qbuf = 0;
								atomic_set(&q->owned_by_drv_count, 0);
								wake_up_all(&q->done_wq);
								for (i = 0; i < q->num_buffers; ++i) {
									struct vb2_buffer *vb = q->bufs[i];
									__vb2_buf_mem_finish(vb);
									__vb2_dqbuf(vb);
										call_void_bufop(q, init_buffer, vb); /* __init_vb2_v4l2_buffer */
											__init_vb2_v4l2_buffer(struct vb2_buffer *vb)
												struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);
												vbuf->request_fd = -1;
								}
							q->waiting_for_buffers = !q->is_output;
							q->last_buffer_dequeued = false;

/* v4l2的 ioctl 机制 */
struct v4l2_ioctl_info {
	unsigned int ioctl;
	u32 flags;
	const char * const name;
	int (*func)(const struct v4l2_ioctl_ops *ops, struct file *file,
		    void *fh, void *p);
	void (*debug)(const void *arg, bool write_only);
};


__video_do_ioctl(struct file *file, unsigned int cmd, void *arg)
	struct video_device *vfd = video_devdata(file);
	const struct v4l2_ioctl_ops *ops = vfd->ioctl_ops;
	info = &v4l2_ioctls[_IOC_NR(cmd)];
	ret = info->func(ops, file, fh, arg);


/* dfs_rpmsg_impl.c
   dfs_rpmsg_recv_work 负责接收dfs传递的rpmsg控制消息 并处理 理论上这个线程33ms被唤醒一次
 */
dfs_drv_probe(struct platform_device *pdev)
	dfs_rpmsg_rdata.rpmsg_recv_task = kthread_create(dfs_rpmsg_recv_work, NULL, "dfs_rpmsg_recv_task");
		dfs_rpmsg_recv_work(void *args)
			while(!kthread_should_stop()) {
				dfs_rpmsg_ringbuf_consume(get_ringbuf(), local_buf, &local_buflen);
				init_rpmsg_info(&info, local_buf, local_buflen);
				dfs_rpmsg_recv_cb(&info);
					struct dfs_header_info* header_info = NULL;
					header_info =  get_header_info(info);
					if (header_info->message_type == MESSAGE_SIMPLE)
						dfs_handle_message(simple_info);
							received_func(simple_info->msg_data, simple_info->message_size);
					else if (header_info->message_type == MESSAGE_COMPLEX)
						dfs_handle_frame(complex_info);
							return dfs_oms_handle_frame(complex_info, dquebuf_func);
								return dfs_oms_dequeuebuf(complex_info, dquebuf_func);
									ret = dfs_oms_acquire_frame(complex_info, &frame_addr, &frame_size);   /* 从ecarxlink 中 根据key 获取对应信息 pa size */
										ret = getAddrFromVircamList(key, paddr, psize);
											return get_src_dst_physAddr_from_shmInfoList(key,vicam_addr,NULL,srcsize,NULL,NULL);
									dquebuf_func(frame_addr, complex_info->block_index, frame_size);
			}



/* videobuf2-core.h */
struct vb2_plane {
	void			*mem_priv;
	struct dma_buf		*dbuf;
	unsigned int		dbuf_mapped;
	unsigned int		bytesused;
	unsigned int		length;
	unsigned int		min_length;
	union {
		unsigned int	offset;
		unsigned long	userptr;
		int		fd;                /* dmabuf fd */
	} m;
	unsigned int		data_offset;
};

/* videobuf2-v4l2.h */
struct vb2_v4l2_buffer {
	struct vb2_buffer	vb2_buf;  /* vb2_dma_contig_plane_dma_addr */
	__u32			flags;
	__u32			field;
	struct v4l2_timecode	timecode;
	__u32			sequence;
	__s32			request_fd;
	bool			is_held;
	struct vb2_plane	planes[VB2_MAX_PLANES];
};


/* dfsc-core.c */

struct dfsc_buffer {
	struct vb2_v4l2_buffer  vb;
	struct list_head	list;
};


struct dfsc_dev {
	struct v4l2_device		v4l2_dev;
	struct video_device		vid_cap_dev;
	u32				        vid_cap_caps;
	struct vb2_queue		vb_vid_cap_q;
	struct list_head		vid_cap_active;
	struct task_struct		*kthread_vid_cap;
}


/* /dev/videoX */
static const struct v4l2_file_operations dfsc_fops = {
	.owner		= THIS_MODULE,
//	.open           = v4l2_fh_open,
	.open           = dfsc_fop_open,
	.release        = dfsc_fop_release,
	.read           = vb2_fop_read,
	.write          = vb2_fop_write,
	.poll		    = vb2_fop_poll,
	.unlocked_ioctl = video_ioctl2,
	.mmap           = vb2_fop_mmap,
};


dfsc_fop_release(struct file *file)
	return vb2_fop_release(file);
		struct video_device *vdev = video_devdata(file);
		struct mutex *lock = vdev->queue->lock ? vdev->queue->lock : vdev->lock;
		return _vb2_fop_release(file, lock);
			vb2_queue_release(vdev->queue);
				vb2_core_queue_release(q);
					__vb2_queue_free(q, q->num_buffers);
						q->num_buffers -= buffers;

/* ioctl 交互
   v4l2_ioctls 最终会调用到这个地方
 */
static const struct v4l2_ioctl_ops dfsc_ioctl_ops = {
	.vidioc_querycap		    = vidioc_querycap,
	.vidioc_enum_fmt_vid_cap	= dfsc_enum_fmt_cap,
	.vidioc_g_fmt_vid_cap		= dfsc_g_fmt_cap,
	.vidioc_try_fmt_vid_cap		= dfsc_try_fmt_cap,
	.vidioc_s_fmt_vid_cap		= dfsc_s_fmt_cap,
	.vidioc_g_fmt_vid_cap_mplane	= dfsc_g_fmt_cap_mplane,
	.vidioc_try_fmt_vid_cap_mplane	= dfsc_try_fmt_cap_mplane,
	.vidioc_s_fmt_vid_cap_mplane	= dfsc_s_fmt_cap_mplane,
	.vidioc_enum_framesizes		= vidioc_enum_framesizes,
	.vidioc_enum_frameintervals	= vidioc_enum_frameintervals,
	.vidioc_reqbufs			= vb2_ioctl_reqbufs,
	.vidioc_create_bufs		= vb2_ioctl_create_bufs,
	.vidioc_prepare_buf		= vb2_ioctl_prepare_buf,
	.vidioc_querybuf		= vb2_ioctl_querybuf,
	.vidioc_qbuf			= vb2_ioctl_qbuf,
	.vidioc_dqbuf			= vb2_ioctl_dqbuf,
	.vidioc_expbuf			= vb2_ioctl_expbuf,
	.vidioc_streamon		= dfsc_streamon,
	.vidioc_streamoff		= dfsc_streamoff,
	.vidioc_unsubscribe_event	= v4l2_event_unsubscribe,
};

__video_do_ioctl(struct file *file, unsigned int cmd, void *arg)
	info = &v4l2_ioctls[_IOC_NR(cmd)];
	ret = info->func(ops, file, fh, arg);

vb2_ioctl_qbuf(struct file *file, void *priv, struct v4l2_buffer *p)
	return vb2_qbuf(vdev->queue, vdev->v4l2_dev->mdev, p);



/* dfsc_core.c
   dfsc 初始化时 调用dfs内核态驱动接口 dfs_init 初始化rpmsg控制通道
*/
dfsc_probe(struct platform_device *pdev)
	data_attrs.dquebuf_func = dfs_data_callback_func;
	ret = dfs_init(CHANNEL_ID, &data_attrs);


/* 当dfs内核态收到master传来的数据时 通知v4l2 */
dfs_data_callback_func(const uint64_t phyaddr, uint32_t index, uint32_t size)
	if (!list_empty(&(dfsc_devs[0]->vid_cap_active))) {
		list_for_each_entry_safe(vid_cap_buf, n,  &(dfsc_devs[0]->vid_cap_active), list) {
			if (phyaddr == vb2_dma_contig_plane_dma_addr(&vid_cap_buf->vb.vb2_buf, 0)) {
				sucess = 1;
				list_del(&vid_cap_buf->list);
			}
		}
	}
	/* https://www.kernel.org/doc/html/v5.0/media/kapi/v4l2-videobuf2.html
	 * This function should be called by the driver after a hardware operation on a buffer is finished and the buffer may be returned to userspace.
	*/
	vb2_buffer_done(&vid_cap_buf->vb.vb2_buf, VB2_BUF_STATE_DONE);


/* v4l2 vb2_queue 用来管理 v4l2的 vb2_buffer */
struct vb2_queue {
	const struct vb2_ops		*ops;
	const struct vb2_mem_ops	*mem_ops;  /* alloc get_dmabuf get_userptr put_userptr */
			/* 内核提供了三种ops vmalloc dma_sg contig */
	const struct vb2_buf_ops	*buf_ops;
	struct vb2_buffer		*bufs[VB2_MAX_FRAME];
	struct list_head		queued_list; /* 应用层归还给v4l2 等待填充数据 */
	struct list_head		done_list;   /* 应用层可以从v4l2拿取 已经填充了数据 */
	unsigned int			streaming:1;
	unsigned int			num_buffers;
	struct vb2_buffer		*bufs[VB2_MAX_FRAME];
}


struct vb2_buffer {
	struct vb2_queue	*vb2_queue;
	unsigned int		index;
	struct list_head	queued_entry; /* 属于哪个queue */
	struct list_head	done_entry;   /* done 状态队列 */
}


/* /dev/videoX 设备节点 */
struct video_device {
	const struct v4l2_file_operations *fops;  /* /dev/videoX open release write read mmap */
	struct v4l2_device *v4l2_dev;
	struct vb2_queue *queue;
	const struct v4l2_ioctl_ops *ioctl_ops;   /* querycap enum_fmt_* g_fmt* s_fmt* */
}

/* v4l2 子系统 核心数据结构 */
struct v4l2_device {
	struct device *dev;
	struct list_head subdevs;
	char name[V4L2_DEVICE_NAME_SIZE];
};

static unsigned int allocators[DFSC_MAX_DEVS] = { [0 ... (DFSC_MAX_DEVS - 1)] = 1 };   /* 默认使用 dma-contig */
MODULE_PARM_DESC(allocators, " memory allocator selection, default is 0.\n"
			     "\t\t    0 == vmalloc\n"
			     "\t\t    1 == dma-contig");

/* dfsc_core.c
   dfsc 初始化时 初始化 vb2_queue
*/
dfsc_probe(struct platform_device *pdev)
	dfsc_create_instance(struct platform_device *pdev, int inst)
		snprintf(dev->v4l2_dev.name, sizeof(dev->v4l2_dev.name), "%s-%03d", DFSC_MODULE_NAME, inst);
		ret = v4l2_device_register(&pdev->dev, &dev->v4l2_dev);
		ret = dfsc_detect_feature_set(dev);
			dev->multiplanar = 1;
			dev->has_vid_cap = 0x0001;
		dfsc_set_capabilities(dev);        /* V4L2_CAP_VIDEO_CAPTURE_MPLANE | V4L2_CAP_STREAMING | V4L2_CAP_READWRITE */
		dev->fmt_cap = &dfsc_formats[0];
		dfsc_create_queues(struct dfsc_dev *dev)
			ret = dfsc_create_queue(dev, &dev->vb_vid_cap_q, V4L2_BUF_TYPE_VIDEO_CAPTURE, 2, &dfsc_vid_cap_qops);
				q->min_buffers_needed = min_buffers_needed;  /* 2 */
				/* dfsc_create_queue(struct dfsc_dev *dev, struct vb2_queue *q, u32 buf_type, unsigned int min_buffers_needed, const struct vb2_ops *ops) */
				if (buf_type == V4L2_BUF_TYPE_VIDEO_CAPTURE && dev->multiplanar)
					buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
				q->type = buf_type;
				q->io_modes = VB2_MMAP | VB2_DMABUF;
				q->io_modes |= V4L2_TYPE_IS_OUTPUT(buf_type) ?  VB2_WRITE : VB2_READ;
				q->ops = ops;      /* dfsc_vid_cap_qops */
				q->mem_ops = allocators[dev->inst] == 1 ? &vb2_dma_contig_memops : &vb2_vmalloc_memops;     /* vb2_dma_contig_memops */
				q->min_buffers_needed = min_buffers_needed;    /* 2 */
				q->lock = &dev->mutex;
				q->supports_requests = true;
				q->allow_cache_hints = (cache_hints[dev->inst] == 1);
				return vb2_queue_init(q);
					return vb2_queue_init_name(q, NULL);
						q->buf_ops = &v4l2_buf_ops;         /* videobuf2-v4l2.c */
						q->is_multiplanar = V4L2_TYPE_IS_MULTIPLANAR(q->type);
						q->is_output = V4L2_TYPE_IS_OUTPUT(q->type);
						q->name[0] = '\0';
						return vb2_core_queue_init(q);
							q->memory = VB2_MEMORY_UNKNOWN;
							if (q->name[0] == '\0')
								snprintf(q->name, sizeof(q->name), "%s-%p", q->is_output ? "out" : "cap", q);
		ret = dfsc_create_devnodes(pdev, dev, inst);
			struct video_device *vfd;
			vfd = &dev->vid_cap_dev;
			snprintf(vfd->name, sizeof(vfd->name), "dfsc-%03d-vid-cap", inst);
			snprintf(dev->device_names[inst], sizeof( dev->device_names[inst] ), "video%u%s", inst, DFSC_MODULE_NAME);
			vfd->fops = &dfsc_fops;
			vfd->ioctl_ops = &dfsc_ioctl_ops;
			vfd->device_caps = dev->vid_cap_caps;
			vfd->queue = &dev->vb_vid_cap_q;
			vfd->dev.init_name = dev->device_names[inst];
			ret = video_register_device(vfd, VFL_TYPE_VIDEO, vid_cap_nr[inst]);


/* videobuf2-dma-contig.c camera的物理连续内存分配 ops */
const struct vb2_mem_ops vb2_dma_contig_memops = {
	.alloc		= vb2_dc_alloc,
	.put		= vb2_dc_put,
	.get_dmabuf	= vb2_dc_get_dmabuf,
	.cookie		= vb2_dc_cookie,
	.vaddr		= vb2_dc_vaddr,
	.mmap		= vb2_dc_mmap,
	.get_userptr	= vb2_dc_get_userptr,
	.put_userptr	= vb2_dc_put_userptr,
	.prepare	= vb2_dc_prepare,
	.finish		= vb2_dc_finish,
	.map_dmabuf	= vb2_dc_map_dmabuf,
	.unmap_dmabuf	= vb2_dc_unmap_dmabuf,
	.attach_dmabuf	= vb2_dc_attach_dmabuf,
	.detach_dmabuf	= vb2_dc_detach_dmabuf,
	.num_users	= vb2_dc_num_users,
};

/* 生成 dmabuf_attach */
vb2_dc_attach_dmabuf(struct device *dev, struct dma_buf *dbuf, unsigned long size, enum dma_data_direction dma_dir)
	struct vb2_dc_buf *buf;
	struct dma_buf_attachment *dba;
	buf = kzalloc(sizeof(*buf), GFP_KERNEL);
	buf->dev = dev;
	buf->vb = vb;
	dba = dma_buf_attach(dbuf, buf->dev);
		return dma_buf_dynamic_attach(dmabuf, dev, NULL, NULL);
	buf->dma_dir = vb->vb2_queue->dma_dir;
	buf->size = size;
	buf->db_attach = dba;
	return buf;


vb2_dc_cookie(struct vb2_buffer *vb, void *buf_priv)
	struct vb2_dc_buf *buf = buf_priv;
	return &buf->dma_addr;


/* dma-buf.c */
dma_buf_dynamic_attach(struct dma_buf *dmabuf, struct device *dev, const struct dma_buf_attach_ops *importer_ops, void *importer_priv)
	struct dma_buf_attachment *attach;
	attach = kzalloc(sizeof(*attach), GFP_KERNEL);
	attach->dev = dev;
	attach->dmabuf = dmabuf;
	attach->importer_ops = importer_ops;
	attach->importer_priv = importer_priv;
	ret = dmabuf->ops->attach(dmabuf, attach);
	if (dma_buf_attachment_is_dynamic(attach) !=dma_buf_is_dynamic(dmabuf)) {
		struct sg_table *sgt;
		sgt = dmabuf->ops->map_dma_buf(attach, DMA_BIDIRECTIONAL);
		attach->sgt = sgt;
		attach->dir = DMA_BIDIRECTIONAL;
	}





/* dfsc-vid-cap.c
   vb2_queue的ops 通过 call_qop 这个宏来调用 queue ops
*/
const struct vb2_ops dfsc_vid_cap_qops = {
	.queue_setup		= vid_cap_queue_setup,
	.buf_prepare		= vid_cap_buf_prepare,
	.buf_finish		= vid_cap_buf_finish,
	.buf_queue		= vid_cap_buf_queue,		/* 更新 key */
	.start_streaming	= vid_cap_start_streaming,
	.stop_streaming		= vid_cap_stop_streaming,
	.buf_request_complete	= vid_cap_buf_request_complete,
	.wait_prepare		= vb2_ops_wait_prepare,
	.wait_finish		= vb2_ops_wait_finish,
};

dfsc_streamoff(struct file *file, void *priv, enum v4l2_buf_type type)
	vb2_ioctl_streamoff(struct file *file, void *priv, enum v4l2_buf_type i)
		if (vb2_queue_is_busy(vdev->queue, file))  /* return q->owner && q->owner != file->private_data; */
			return -EBUSY;
		vb2_streamoff(struct vb2_queue *q, enum v4l2_buf_type type)
			if (vb2_fileio_is_active(q))
				return -EBUSY;
			vb2_core_streamoff(struct vb2_queue *q, unsigned int type)
				__vb2_queue_cancel(struct vb2_queue *q)
				q->waiting_for_buffers = !q->is_output;
				q->last_buffer_dequeued = false;
					vid_cap_stop_streaming(struct vb2_queue *vq)
						dfs_warp_make_clean(get_dfs_oms_key(i));
							return rmNodeOfVircam(key);
								ret = ecarxlink_request_remove_remote_vicamNode(&local_sb_info);

/* v4l2 set format */
dfsc_s_fmt_vid_cap(struct file *file, void *priv, struct v4l2_format *f)
	struct vb2_queue *q = &dev->vb_vid_cap_q;
	if (vb2_is_busy(q))   /* return (q->num_buffers > 0); */
		return -EBUSY;

/* request buf 这个地方并没有真正申请数据帧的内存 只是向驱动查询 */
ioctl(fd, VIDIOC_REQBUFS, xx)
	IOCTL_INFO(VIDIOC_REQBUFS, v4l_reqbufs, v4l_print_requestbuffers, INFO_FL_PRIO | INFO_FL_QUEUE),
		v4l_reqbufs(const struct v4l2_ioctl_ops *ops, struct file *file, void *fh, void *arg)
			struct v4l2_requestbuffers *p = arg;
			return ops->vidioc_reqbufs(file, fh, p);
				vb2_ioctl_reqbufs(struct file *file, void *priv, struct v4l2_requestbuffers *p)
					fill_buf_caps(vdev->queue, &p->capabilities);
					res = vb2_core_reqbufs(vdev->queue, p->memory, &p->count); /* V4L2_MEMORY_DMABUF */
						num_buffers = max_t(unsigned int, *count, q->min_buffers_needed);
						num_buffers = min_t(unsigned int, num_buffers, VB2_MAX_FRAME); /* 64 */
						q->memory = memory;
						ret = call_qop(q, queue_setup, q, &num_buffers, &num_planes, plane_sizes, q->alloc_devs);
							vid_cap_queue_setup(struct vb2_queue *vq, unsigned *nbuffers, unsigned *nplanes, unsigned sizes[], struct device *alloc_devs[])
								*nplanes = dev->pix.num_planes;
								for (p = 0; p < *nplanes; p++)
									sizes[p] =  dev->pix.plane_fmt[p].sizeimage;
						allocated_buffers = __vb2_queue_alloc(q, memory, num_buffers, num_planes, plane_sizes);
							struct vb2_buffer *vb;
							for (buffer = 0; buffer < num_buffers; ++buffer)
								vb = kzalloc(q->buf_struct_size, GFP_KERNEL);
								vb->index = q->num_buffers + buffer;
								vb->num_planes = num_planes;
								vb->vb2_queue = q;
								call_void_bufop(q, init_buffer, vb); /* /* __init_vb2_v4l2_buffer */ */
								q->bufs[vb->index] = vb;
							return buffer;
						q->num_buffers = allocated_buffers;
						*count = allocated_buffers;  /* 返给 us */
						q->waiting_for_buffers = !q->is_output;


/* query buf 和mmap一起使用 映射到用户空间
   根据设备呢能力 查询buf的属性 size flag number等 之后根据查询到的属性申请内存
*/
ioctl(fd, VIDIOC_QUERYBUF, xx)
	IOCTL_INFO(VIDIOC_QUERYBUF, v4l_querybuf, v4l_print_buffer, INFO_FL_QUEUE | INFO_FL_CLEAR(v4l2_buffer, length)),
		static int v4l_querybuf(const struct v4l2_ioctl_ops *ops, struct file *file, void *fh, void *arg)
			struct v4l2_buffer *p = arg;
			int ret = check_fmt(file, p->type);
			return ret ? ret : ops->vidioc_querybuf(file, fh, p);  /* vb2_ioctl_querybuf */
				vb2_ioctl_querybuf(struct file *file, void *priv, struct v4l2_buffer *p)
					struct video_device *vdev = video_devdata(file);
					return vb2_querybuf(vdev->queue, p);
						struct vb2_buffer *vb;
						vb = q->bufs[b->index];
						vb2_core_querybuf(q, b->index, b);
							call_void_bufop(q, fill_user_buffer, q->bufs[index], pb);
								__fill_v4l2_buffer(struct vb2_buffer *vb, void *pb)


/* queue buf
   应用将消费完成的数据帧内存归还给v4l2 这个地方会绑定ion和plane, 也就是数据帧缓存真正的物理内存
*/
ioctl(fd, VIDIOC_QBUF, xx)
	IOCTL_INFO(VIDIOC_QBUF, v4l_qbuf, v4l_print_buffer, INFO_FL_QUEUE),
		v4l_qbuf(const struct v4l2_ioctl_ops *ops, struct file *file, void *fh, void *arg)
			struct v4l2_buffer *p = arg;
			return ret ? ret : ops->vidioc_qbuf(file, fh, p);   /* vb2_ioctl_qbuf */
				vb2_ioctl_qbuf(struct file *file, void *priv, struct v4l2_buffer *p)
					return vb2_qbuf(vdev->queue, vdev->v4l2_dev->mdev, p);
						ret = __buf_prepare(vb);
							case VB2_MEMORY_DMABUF:
								ret = __prepare_dmabuf(vb); /* pa cookie */
							__vb2_buf_mem_prepare(vb);
						ret = vb2_queue_or_prepare_buf(q, mdev, b, false, &req);
						ret = vb2_core_qbuf(q, b->index, b, req);      /* vdev->queue = q */
							struct vb2_buffer *vb;
							vb = q->bufs[index];
							switch (vb->state) {
							case VB2_BUF_STATE_DEQUEUED:
							case VB2_BUF_STATE_IN_REQUEST:
								if (!vb->prepared) {
									ret = __buf_prepare(vb);
								}
								break;
							default:
								dprintk(q, 1, "invalid buffer state %s\n",
									vb2_state_name(vb->state));
								return -EINVAL;
							}
							orig_state = vb->state;
							list_add_tail(&vb->queued_entry, &q->queued_list);
							q->queued_count++;
							q->waiting_for_buffers = false;
							vb->state = VB2_BUF_STATE_QUEUED;
							if (q->start_streaming_called)  /* VIDIOC_STREAMON */
								__enqueue_in_driver(vb);
									atomic_inc(&q->owned_by_drv_count);
									call_void_vb_qop(vb, buf_queue, vb); /* vid_cap_buf_queue */
										vid_cap_buf_queue(struct vb2_buffer *vb)
											list_add_tail(&buf->list, &dev->vid_cap_active);
											phy_addr = vb2_dma_contig_plane_dma_addr(&vbuf->vb2_buf, 0); /* 这个地方为什么已经可以拿到 pa ? */
											if (dev->vb2_paddr[vbuf->vb2_buf.index]  == 0)
												dfs_oms_initbuf(CHANNEL_ID, vbuf->vb2_buf.index, phy_addr, dev->pix.plane_fmt[0].sizeimage + CAMERA_BUF_OFFSET);
												dev->vb2_paddr[vbuf->vb2_buf.index] =  phy_addr;
											else
												dfs_oms_queuebuf(CHANNEL_ID, vbuf->vb2_buf.index, phy_addr);
							if (q->streaming && !q->start_streaming_called && q->queued_count >= q->min_buffers_needed) {
								ret = vb2_start_streaming(q);
							}


__buf_prepare(struct vb2_buffer *vb)
	struct vb2_queue *q = vb->vb2_queue;
	enum vb2_buffer_state orig_state = vb->state;
	vb->state = VB2_BUF_STATE_PREPARING;
	switch (q->memory) {    /* EnqueueRequest */
	case VB2_MEMORY_MMAP:
		ret = __prepare_mmap(vb);
		break;
	case VB2_MEMORY_USERPTR:
		ret = __prepare_userptr(vb);
			mem_priv = call_ptr_memop(vb, get_userptr, q->alloc_devs[plane] ? : q->dev, planes[plane].m.userptr, planes[plane].length, q->dma_dir);
				vb2_dc_get_userptr(struct device *dev, unsigned long vaddr, unsigned long size, enum dma_data_direction dma_dir) /* vaddr 用户态虚拟地址 */
					struct frame_vector *vec;
					vec = vb2_create_framevec(vaddr, size);
					n_pages = frame_vector_count(vec);
					sgt = kzalloc(sizeof(*sgt), GFP_KERNEL);
					ret = sg_alloc_table_from_pages(sgt, frame_vector_pages(vec), n_pages, offset, size, GFP_KERNEL); /* 最终转换成sgt */
					buf->dma_addr = sg_dma_address(sgt->sgl); /* pa 赋值 */
		break;
	case VB2_MEMORY_DMABUF: /* dfsc type */
		ret = __prepare_dmabuf(vb);
			struct vb2_plane planes[VB2_MAX_PLANES];
			struct vb2_queue *q = vb->vb2_queue;
			ret = call_bufop(vb->vb2_queue, fill_vb2_buffer, vb, planes);
			for (plane = 0; plane < vb->num_planes; ++plane) {
				struct dma_buf *dbuf = dma_buf_get(planes[plane].m.fd);  /* 应用层从ion申请的dmabuf 拿到的句柄 fd  m是一个 union */
					file = fget(fd);
					return file->private_data;
				mem_priv = call_ptr_memop(vb, attach_dmabuf, q->alloc_devs[plane] ? : q->dev,  /* vb2_dc_attach_dmabuf */
						dbuf,               /* dmabuf */
						planes[plane].length,
						q->dma_dir);
				vb->planes[plane].dbuf = dbuf;
				vb->planes[plane].mem_priv = mem_priv;  /* dmabuf attachment */
			}
			for (plane = 0; plane < vb->num_planes; ++plane) {
				ret = call_memop(vb, map_dmabuf, vb->planes[plane].mem_priv);   /* vb2_dc_map_dmabuf dmabuf pa */
					vb2_dc_map_dmabuf(void *mem_priv)
						struct vb2_dc_buf *buf = mem_priv;
						struct sg_table *sgt;
						sgt = dma_buf_map_attachment(buf->db_attach, buf->dma_dir);
						buf->dma_addr = sg_dma_address(sgt->sgl);
							((sg)->dma_address)
						buf->dma_sgt = sgt;
						buf->vaddr = NULL;
				vb->planes[plane].dbuf_mapped = 1;
			}
			for (plane = 0; plane < vb->num_planes; ++plane) {
				vb->planes[plane].bytesused = planes[plane].bytesused;
				vb->planes[plane].length = planes[plane].length;
				vb->planes[plane].m.fd = planes[plane].m.fd;
				vb->planes[plane].data_offset = planes[plane].data_offset;
			}
			ret = call_vb_qop(vb, buf_prepare, vb);
		break;
	default:
		break;
	}
	__vb2_buf_mem_prepare(vb);
		if (vb->need_cache_sync_on_prepare) {
			for (plane = 0; plane < vb->num_planes; ++plane)
				call_void_memop(vb, prepare, vb->planes[plane].mem_priv);  /* vb2_dc_prepare */
		}
	vb->synced = 1;
	vb->prepared = 1;
	vb->state = orig_state;


vb2_dc_prepare(void *buf_priv)
	struct vb2_dc_buf *buf = buf_priv;
	struct sg_table *sgt = buf->dma_sgt;
	dma_sync_sgtable_for_device(buf->dev, sgt, buf->dma_dir);
		dma_sync_sg_for_device(dev, sgt->sgl, sgt->orig_nents, dir);
			if (dma_map_direct(dev, ops))
				dma_direct_sync_sg_for_device(dev, sg, nelems, dir);  /* 处理一致性 */
			else if (ops->sync_sg_for_device)
				ops->sync_sg_for_device(dev, sg, nelems, dir);

/* start camera */
ioctl(fd, VIDIOC_STREAMON, xx)
	IOCTL_INFO(VIDIOC_STREAMON, v4l_streamon, v4l_print_buftype, INFO_FL_PRIO | INFO_FL_QUEUE),
		return ops->vidioc_streamon(file, fh, *(unsigned int *)arg);
			ret = vb2_ioctl_streamon(file, priv, type);
				return vb2_streamon(vdev->queue, i);
					return vb2_core_streamon(q, type);
						ret = vb2_start_streaming(q);
							q->start_streaming_called = 1;
							ret = call_qop(q, start_streaming, q, atomic_read(&q->owned_by_drv_count));
								vid_cap_start_streaming(struct vb2_queue *vq, unsigned count)
									atomic_set(&dev->is_streamon, 1);
									atomic_set(&dev->last_frame_cond, 0);
									dfs_write(dev->ctrl_channel_id, "1", strlen("1"));  /* 通知master streaming on */
						q->streaming = 1;


/* poll */
v4l2_poll(struct file *filp, struct poll_table_struct *poll)
	res = vdev->fops->poll(filp, poll);
		vb2_fop_poll(struct file *file, poll_table *wait)
			res = vb2_poll(vdev->queue, file, wait);
				res = vb2_core_poll(q, file, wait);
					poll_wait(file, &q->done_wq, wait);


/* wake poll */
vb2_buffer_done(struct vb2_buffer *vb, enum vb2_buffer_state state)
	wake_up(&q->done_wq);



/* dqueue buf
   应用层消费v4l2提供的数据帧
*/
ioctl(fd, VIDIOC_DQBUF, xx)
	IOCTL_INFO(VIDIOC_DQBUF, v4l_dqbuf, v4l_print_buffer, INFO_FL_QUEUE),
		return ret ? ret : ops->vidioc_dqbuf(file, fh, p);
			return vb2_dqbuf(vdev->queue, p, file->f_flags & O_NONBLOCK);
				ret = vb2_core_dqbuf(q, NULL, b, nonblocking);
					struct vb2_buffer *vb = NULL;
					ret = __vb2_get_done_vb(q, &vb, pb, nonblocking);
						ret = __vb2_wait_for_done_vb(q, nonblocking);
							if (!list_empty(&q->done_list))
								break;
							if (nonblocking)
								return -EAGAIN;
							q->waiting_in_dqbuf = 1;
							call_void_qop(q, wait_prepare, q);
							ret = wait_event_interruptible(q->done_wq, !list_empty(&q->done_list) || !q->streaming || q->error);
							call_void_qop(q, wait_finish, q);
							q->waiting_in_dqbuf = 0;
						*vb = list_first_entry(&q->done_list, struct vb2_buffer, done_entry);
						list_del(&(*vb)->done_entry);  /* 摘除的vb */
					call_void_vb_qop(vb, buf_finish, vb);
					vb->prepared = 0;
					list_del(&vb->queued_entry);
					q->queued_count--;
					__vb2_dqbuf(vb);
						vb->state = VB2_BUF_STATE_DEQUEUED;
						call_void_bufop(q, init_buffer, vb);  /* __init_vb2_v4l2_buffer */
				b->flags &= ~V4L2_BUF_FLAG_DONE;



/* dfsc 收到数据 */
dfs_rpmsg_recv_cb(struct rpmsg_info* info)
	complex_info = get_complex_info(info);
	dfs_handle_frame(complex_info);
		struct dfs_chnl_info* dfs_chinfo;
		dfs_chinfo = dfs_find_ept_locked(complex_info->channel_id);
		return dfs_oms_handle_frame(complex_info, dfs_chinfo);
			return dfs_oms_dequeuebuf(complex_info, dfs_chinfo);
				getbuf_func = dfs_chinfo->attrs.getbuf_func;
				(void)getbuf_func(frame_addr, complex_info->block_index, frame_size, dfs_chinfo->attrs.priv);
					dfs_data_callback_func(const uint64_t phyaddr, uint32_t index, uint32_t size, void *priv)
						if (fail)
							dfs_oms_queuebuf(CHANNEL_ID, index, dfsc_devs[0]->vb2_paddr[index]);
								init_complex_info(&complex_info, 0, channel_id);
									init_header_info(&info->header_info, MESSAGE_COMPLEX);
									info->message_size = size;
									info->channel_id = channel_id;
								complex_info.oper_id = OPERID_DATASYNC_ACK;
								complex_info.block_index = block_index;
								return dfs_notify(channel_id, (char *)&complex_info, sizeof(complex_info));
									ret = dfs_elink_notify_locked(channel_id, data, size, dfs_chinfo);
										ret = dfs_notify_base((char *)pkt, siengine_rpmsg_packet_size(pkt), dfs_chinfo);
											ret = rpmsg_send(dfs_chinfo->ept, data, size);
						if (success)
							vb2_buffer_done(&vid_cap_buf->vb.vb2_buf, VB2_BUF_STATE_DONE);  /* 加入 done_list 应用可以通过dqueue消费 */
								list_add_tail(&vb->done_entry, &q->done_list);
								switch (state) {
								case VB2_BUF_STATE_QUEUED:
									return;
								default:
									wake_up(&q->done_wq);
									break;
								}



dfs_oms_putbuf(uint64_t channel_id, uint32_t block_index, uint64_t buffer)
	init_complex_info(&complex_info, 0, channel_id);
	complex_info.oper_id = OPERID_DATASYNC_ACK;   /* master dfs 标记buf可用 */
	complex_info.block_index = block_index;
	return dfs_notify(channel_id, (char *)&complex_info, sizeof(complex_info));
		ret = dfs_elink_notify_locked(channel_id, data, size, dfs_chinfo);
			ret = dfs_notify_base((char *)pkt, siengine_rpmsg_packet_size(pkt), dfs_chinfo);
				ret = rpmsg_send(dfs_chinfo->ept, data, size);


vb2_dma_contig_plane_dma_addr(struct vb2_buffer *vb, unsigned int plane_no)
	dma_addr_t *addr = vb2_plane_cookie(vb, plane_no);
		return call_ptr_memop(cookie, vb, vb->planes[plane_no].mem_priv);  /* op = cookie vb2_dc_cookie */
			struct vb2_queue *_q = (vb)->vb2_queue;
			ptr = _q->mem_ops->op ? _q->mem_ops->op(vb, args) : NULL;  /* mem_ops */




/* videobuf2-v4l2.c
   通过 call_bufop 调用
*/
static const struct vb2_buf_ops v4l2_buf_ops = {
	.verify_planes_array	= __verify_planes_array_core,
	.init_buffer		= __init_vb2_v4l2_buffer,
	.fill_user_buffer	= __fill_v4l2_buffer,
	.fill_vb2_buffer	= __fill_vb2_buffer,
	.copy_timestamp		= __copy_timestamp,
};


__fill_vb2_buffer(struct vb2_buffer *vb, struct vb2_plane *planes)
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);
		container_of(vb, struct vb2_v4l2_buffer, vb2_buf)
	for (plane = 0; plane < vb->num_planes; ++plane) {
		if (vb->vb2_queue->memory != VB2_MEMORY_MMAP) {
			planes[plane].m = vbuf->planes[plane].m;
			planes[plane].length = vbuf->planes[plane].length;
		}
		planes[plane].bytesused = vbuf->planes[plane].bytesused;
		planes[plane].data_offset = vbuf->planes[plane].data_offset;
	}


/* frame_buffer.cpp ion 内存申请 */


/* v4l2_wrapper.cpp
   应用下发 ion 给v4l2
*/
int v4l2_wrapper_dfs::EnqueueRequest(std::shared_ptr<default_camera_hal::CaptureRequest> request) {
	v4l2_buffer device_buffer;
	struct v4l2_plane planes[2];
	device_buffer.memory = V4L2_MEMORY_DMABUF;
	device_buffer.type = mDeviceFormat->type();
	device_buffer.index = index;
	device_buffer.m.planes = planes;
	device_buffer.length = 1;
	if (IoctlLocked(VIDIOC_QUERYBUF, &device_buffer) < 0)
	/*/data1/shifan/src-DX114/src/aosp/vendor/siengine/hardware/modules/camera/3_4/arc/frame_buffer.cpp*/
	request_context->camera_buffer->SetDataSizeAndOffsetDfs(requiredSize-CIF_BUFFER_HEADER_LEN, CIF_BUFFER_HEADER_LEN, getDevicePath());
		enum ion_heap_type type;
		ret = ion_query_heap_cnt(mIonFd, &heap_cnt);
		struct ion_heap_data heap_data[heap_cnt];
		ret = ion_query_get_heaps(mIonFd, heap_cnt, heap_data);
		if (0 == mDevicePath.compare("/dev/video0dfsc")) {
			type = (enum ion_heap_type)10;//ION_HEAP_TYPE_CAM_DFSC0;
		} else if (0 == mDevicePath.compare("/dev/video1dfsc")) {
			type = (enum ion_heap_type)11;//ION_HEAP_TYPE_CAM_DFSC1;
		}
		for (i = 0; i < heap_cnt; i++) {
			if (heap_data[i].type == type) {
				heap_mask = 1 << heap_data[i].heap_id;
				break;
			}
		}
		ret = ion_alloc_fd(mIonFd, requiredSize, 0, heap_mask, ION_HEAP_TYPE_DMA, &dmaBuf);
			dmabuf = ion_alloc_dmabuf(len, heap_id_mask, flags);
			fd = dma_buf_fd(dmabuf, O_CLOEXEC);
	planes[0].length = request_context->camera_buffer->GetBufferSize();
	planes[0].bytesused = request_context->camera_buffer->GetDataSize();
	planes[0].m.fd = request_context->camera_buffer->GetDmaBufHandle();
		return ((struct private_handle_t *)mGraphicBuffer)->share_fd;
	if (IoctlLocked(VIDIOC_QBUF, &device_buffer) < 0)
}




/* ion_dfs_heap.c */
ion_add_dfs_heap(void)
	struct ion_dfs_heap *dfs_heap;
	dfs_heap = &dfs_heaps;
	dfs_heap->heap.ops = &ion_rmem_ops;
	dfs_heap->heap.type = ION_HEAP_TYPE_CUSTOM;
	dfs_heap->heap.name = "dfs_camera_rmem";
	ret = ion_device_add_heap(&dfs_heap->heap);
		__ion_device_add_heap(heap, THIS_MODULE)



/* 可能是这个 */
static const struct dma_buf_ops dma_buf_ops = {
	.attach = ion_dma_buf_attach,
	.detach = ion_dma_buf_detatch,
	.map_dma_buf = ion_map_dma_buf,
	.unmap_dma_buf = ion_unmap_dma_buf,
	.release = ion_dma_buf_release,
	.begin_cpu_access = ion_dma_buf_begin_cpu_access,
	.begin_cpu_access_partial = ion_dma_buf_begin_cpu_access_partial,
	.end_cpu_access = ion_dma_buf_end_cpu_access,
	.end_cpu_access_partial = ion_dma_buf_end_cpu_access_partial,
	.mmap = ion_dma_buf_mmap,
	.vmap = ion_dma_buf_vmap,
	.vunmap = ion_dma_buf_vunmap,
	.get_flags = ion_dma_buf_get_flags,
};


ion_map_dma_buf(struct dma_buf_attachment *attachment, enum dma_data_direction direction)
	struct sg_table *table;
	struct ion_dma_buf_attachment *a;
	unsigned long attrs = attachment->dma_map_attrs;
	a = attachment->priv;
	table = a->table;
	if (!dma_map_sg_attrs(attachment->dev, table->sgl, table->nents, direction, attrs))
		ret = __dma_map_sg_attrs(dev, sg, nents, dir, attrs);
			const struct dma_map_ops *ops = get_dma_ops(dev);
			if (dma_map_direct(dev, ops) || arch_dma_map_sg_direct(dev, sg, nents))
				ents = dma_direct_map_sg(dev, sg, nents, dir, attrs);
			else
				ents = ops->map_sg(dev, sg, nents, dir, attrs);


dma_direct_map_sg(struct device *dev, struct scatterlist *sgl, int nents, enum dma_data_direction dir, unsigned long attrs)
	struct scatterlist *sg;
	for_each_sg(sgl, sg, nents, i) {
		sg->dma_address = dma_direct_map_page(dev, sg_page(sg),
				sg->offset, sg->length, dir, attrs);
		if (sg->dma_address == DMA_MAPPING_ERROR)
			goto out_unmap;
		sg_dma_len(sg) = sg->length;
	}