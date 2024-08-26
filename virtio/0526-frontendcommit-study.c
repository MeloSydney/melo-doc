




/* A 前端 应用 提交 ioctl -> commit_work (drm) */
(.unlocked_ioctl	= drm_ioctl,)
drm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
	unsigned int nr = DRM_IOCTL_NR(cmd);
	ioctl = &drm_ioctls[nr]; (drm 定义好的 回调)
	func = ioctl->func;
	drm_ioctl_kernel(filp, func, kdata, ioctl->flags);
		retcode = func(dev, kdata, file_priv);
		(DRM_IOCTL_DEF(DRM_IOCTL_MODE_ATOMIC, drm_mode_atomic_ioctl, DRM_MASTER),)
		drm_mode_atomic_ioctl(struct drm_device *dev, void *data, struct drm_file *file_priv)
			drm_atomic_nonblocking_commit(state);
				config->funcs->atomic_commit(state->dev, state, true);
				(.atomic_commit = drm_atomic_helper_commit,)
				drm_atomic_helper_commit(struct drm_device *dev,...)
					queue_work(system_unbound_wq, &state->commit_work);
					(INIT_WORK(&state->commit_work, commit_work);)
/* A 前端 提交 cursor */
commit_work(struct work_struct *work)
	commit_tail(struct drm_atomic_state *old_state)
		funcs->atomic_commit_tail(old_state);
		(.atomic_commit_tail = vgdev_atomic_commit_tail,)
			drm_atomic_helper_commit_planes(dev, state, 0);
				funcs->atomic_update(plane, old_plane_state);
				(.atomic_update		= virtio_gpu_cursor_plane_update,)
				virtio_gpu_cursor_plane_update(struct drm_plane *plane, struct drm_plane_state *old_state)
					/* 更新cursor数据 */
					output->cursor.pos.x = cpu_to_le32(plane->state->crtc_x);
					output->cursor.pos.y = cpu_to_le32(plane->state->crtc_y);
					virtio_gpu_cursor_ping(vgdev, output);
						/* 1 申请空间
						 * 2 准备数据
						 * 3 入队提交 */
						cur_p = virtio_gpu_alloc_cursor(vgdev, &vbuf);
						memcpy(cur_p, &output->cursor, sizeof(output->cursor));
						virtio_gpu_queue_cursor(vgdev, vbuf);
							/* 1 将vbuf中的数据 转换成sg */
							ret = virtqueue_add_sgs(vq, sgs, outcnt, 0, vbuf, GFP_ATOMIC);
							virtqueue_add(_vq, sgs, total_sg, out_sgs, in_sgs, data, NULL, gfp);
								virtqueue_add_split(_vq, sgs, total_sg, out_sgs, in_sgs, data, ctx, gfp);
							virtqueue_kick(vq);
								virtqueue_notify(vq);
									vq->notify(_vq)
				funcs->atomic_flush(crtc, old_crtc_state);
				(.atomic_flush  = virtio_gpu_crtc_atomic_flush,)
				virtio_gpu_crtc_atomic_flush(struct drm_crtc *crtc, struct drm_crtc_state *old_state)

/* A 前端 提交 ctrl */
commit_work(struct work_struct *work)
	commit_tail(struct drm_atomic_state *old_state)
		funcs->atomic_commit_tail(old_state);
		(.atomic_commit_tail = vgdev_atomic_commit_tail,)
		vgdev_atomic_commit_tail(struct drm_atomic_state *state)
			drm_atomic_helper_commit_planes(dev, state, 0);
				funcs->atomic_update(plane, old_plane_state);
				(.atomic_update		= virtio_gpu_primary_plane_update,)
virtio_gpu_primary_plane_update(struct drm_plane *plane, struct drm_plane_state *old_state)
	virtio_gpu_cmd_resource_flush(vgdev, handle,...)
		virtio_gpu_queue_ctrl_buffer(vgdev, vbuf);
			virtio_gpu_queue_ctrl_buffer_locked(vgdev, vbuf);
				virtqueue_kick(vq);
					if (virtqueue_kick_prepare(vq))
						virtqueue_kick_prepare_split(_vq);
							old = vq->split.avail_idx_shadow - vq->num_added;
							new = vq->split.avail_idx_shadow;
							vq->num_added = 0;
							/* 判断 是否触发kick */
							needs_kick = vring_need_event(virtio16_to_cpu(_vq->vdev, vring_avail_event(&vq->split.vring)), new, old);



/* A 前端Guest 提交需求 vring */
virtio_gpu_queue_ctrl_buffer_locked(struct virtio_gpu_device *vgdev, struct virtio_gpu_vbuffer *vbuf)
	ret = virtqueue_add_sgs(vq, sgs, outcnt, incnt, vbuf, GFP_ATOMIC);
		virtqueue_add(_vq, sgs, total_sg, out_sgs, in_sgs, data, NULL, gfp);
			virtqueue_add_split(_vq, sgs, total_sg, out_sgs, in_sgs, data, ctx, gfp); (cursor 直接走到这)
				/* 第一个空闲位置 */
				head = vq->free_head;
				/* 处理sg 将结果保存在desc中 */
				for (sg = sgs[n]; sg; sg = sg_next(sg)){
				dma_addr_t addr = vring_map_one_sg(vq, sg, DMA_FROM_DEVICE);
				desc[i].addr = cpu_to_virtio64(_vq->vdev, addr);
				desc[i].len = cpu_to_virtio32(_vq->vdev, sg->length);}
				/* 将desc中的数据 填充到vring.desc中 */
				vq->split.vring.desc[head].addr = cpu_to_virtio64(_vq->vdev, addr);
				vq->split.vring.desc[head].len = cpu_to_virtio32(_vq->vdev, total_sg * sizeof(struct vring_desc));
				/* 更新free_head 空闲的 就是当前使用的下一个 */
				vq->free_head = virtio16_to_cpu(_vq->vdev, vq->split.vring.desc[head].next);
				vq->vq.num_free -= descs_used;
				/* */
				avail = vq->split.avail_idx_shadow & (vq->split.vring.num - 1);
				vq->split.vring.avail->ring[avail] = cpu_to_virtio16(_vq->vdev, head);
				/* 提交需求后 +=1 */
				vq->split.avail_idx_shadow++;
				vq->split.vring.avail->idx = cpu_to_virtio16(_vq->vdev, vq->split.avail_idx_shadow);
				vq->num_added++;


/* A 前端 提交完成 kick writel 通知后端 */
virtio_gpu_queue_ctrl_buffer_locked(vgdev, vbuf);
	virtqueue_kick(vq);
		virtqueue_notify(vq);
			vq->notify(_vq)
				vp_notify()
					iowrite16(vq->index, (void __iomem *)vq->priv); (VIRTIO_PCI_QUEUE_NOTIFY)


/* G 触发 write操作 -> Host kernel 触发 缺页 -> 交给Qemu -> Qemu接手处理 */



/* B 后端Qemu 被kick   kvm通知Qemu write */
(virtio_pci_config_ops.write = virtio_pci_config_write,)
virtio_pci_config_write(void *opaque, hwaddr addr,...)
	virtio_ioport_write(proxy, addr, val);
		case VIRTIO_PCI_STATUS:
		/* 建立eventfd机制 kvm通知Qemu */
		virtio_pci_start_ioeventfd(proxy);
			virtio_bus_start_ioeventfd(&proxy->bus);
				r = vdc->start_ioeventfd(vdev);
				(vdc->start_ioeventfd = virtio_device_start_ioeventfd_impl;)
				virtio_device_start_ioeventfd_impl(VirtIODevice *vdev)
					event_notifier_set_handler(&vq->host_notifier, virtio_queue_host_notifier_read);
						(virtio_queue_host_notifier_read(EventNotifier *n))
							virtio_queue_notify_vq(vq);
								vq->handle_output(vdev, vq);
								(vdev->vq[i].handle_output = handle_output = virtio_gpu_handle_ctrl_cb)
virtio_gpu_handle_ctrl_cb(VirtIODevice *vdev, VirtQueue *vq)
	qemu_bh_schedule(g->ctrl_bh);
	(g->ctrl_bh = qemu_bh_new(virtio_gpu_ctrl_bh, g);)
		virtio_gpu_ctrl_bh(void *opaque)
			vgc->handle_ctrl(&g->parent_obj.parent_obj, g->ctrl_vq);
			(vgc->handle_ctrl = virtio_gpu_handle_ctrl;)
			virtio_gpu_handle_ctrl(VirtIODevice *vdev, VirtQueue *vq)
				/* 将cmd从virtqueue弹出 加入cmdq队列 */
				cmd = virtqueue_pop(vq, sizeof(struct virtio_gpu_ctrl_command));
				QTAILQ_INSERT_TAIL(&g->cmdq, cmd, next);
virtio_gpu_process_cmdq(g);
	cmd = QTAILQ_FIRST(&g->cmdq);
	vgc->process_cmd(g, cmd);
	(vgc->process_cmd = virtio_gpu_simple_process_cmd;)
	virtio_gpu_simple_process_cmd(VirtIOGPU *g, struct virtio_gpu_ctrl_command *cmd)
		case VIRTIO_GPU_CMD_TRANSFER_TO_HOST_2D:
		virtio_gpu_transfer_to_host_2d(g, cmd); (pixman)
		virtio_gpu_ctrl_response_nodata(g, cmd, cmd->error ? cmd->error : VIRTIO_GPU_RESP_OK_NODATA);
			virtio_gpu_ctrl_response(g, cmd, &resp, sizeof(resp));
				virtqueue_push(cmd->vq, &cmd->elem, s);
					virtqueue_fill(vq, elem, len, 0);
						virtqueue_split_fill(vq, elem, len, idx);
							/* 更新used字段 */
							vring_used_write(vq, &uelem, idx);
					virtqueue_flush(vq, 1);
						vring_used_idx_set(vq, new);
				virtio_notify(VIRTIO_DEVICE(g), cmd->vq);
	vgc->process_cmd(g, cmd);
	(vgc->process_cmd = virtio_gpu_virgl_process_cmd;)
	virtio_gpu_virgl_process_cmd(VirtIOGPU *g, struct virtio_gpu_ctrl_command *cmd)
		case VIRTIO_GPU_CMD_RESOURCE_CREATE_2D:
		virgl_cmd_create_resource_2d(g, cmd);
			struct virtio_gpu_resource_create_2d c2d;
			VIRTIO_GPU_FILL_CMD(c2d);
				/* memcpy (cmd->elem.out_sg -> out) */
				s = iov_to_buf(cmd->elem.out_sg, cmd->elem.out_num, 0, &out, sizeof(out));
			args.handle = c2d.resource_id;
			virgl_renderer_resource_create(&args, NULL, 0);





/* B 后端 消费 cmd 细节 ?? */
virtio_gpu_handle_ctrl(VirtIODevice *vdev, VirtQueue *vq)
	cmd = virtqueue_pop(vq, sizeof(struct virtio_gpu_ctrl_command));
		virtqueue_packed_pop(vq, sz);
			vq->last_avail_idx += elem->ndescs;
			vq->inuse += elem->ndescs;
			vq->shadow_avail_idx = vq->last_avail_idx;
		virtqueue_split_pop(vq, sz);
			hwaddr addr[VIRTQUEUE_MAX_SIZE];
			struct iovec iov[VIRTQUEUE_MAX_SIZE];
			VRingDesc desc;
			/* (后端device负责更新 last_avail_idx) */
			if (!virtqueue_get_head(vq, vq->last_avail_idx++, &head))
				*head = vring_avail_ring(vq, idx % vq->vring.num);
					/* 后端 消费avail */
					return le16toh(vq->vring.avail->ring[i]);
			/* 更新 avail_event */
			if (virtio_vdev_has_feature(vdev, VIRTIO_RING_F_EVENT_IDX))
				vring_set_avail_event(vq, vq->last_avail_idx);
					avail = (uint16_t *)&vq->vring.used->ring[vq->vring.num];
					*avail = htole16(val);
			/* 填充 desc */
			vring_split_desc_read(vdev, &desc, desc_cache, i);
				address_space_read_cached(cache, i * sizeof(VRingDesc), desc, sizeof(VRingDesc));
					memcpy(buf, cache->ptr + addr, len);
			virtqueue_map_desc(vdev, &out_num, addr, iov, VIRTQUEUE_MAX_SIZE, false, desc.addr, desc.len);
				iov[num_sg].iov_len = len;
				addr[num_sg] = pa;
			elem = virtqueue_alloc_element(sz, out_num, in_num);
			elem->index = head;
			elem->out_addr[i] = addr[i];
			elem->out_sg[i] = iov[i];
			vq->inuse++;






/* B后端 Qemu 通知kvm 建立 ioeventfd */
virtio_pci_config_write(void *opaque, hwaddr addr, int64_t val, unsigned size)
	virtio_ioport_write(void *opaque, uint32_t addr, uint32_t val)
		virtio_pci_start_ioeventfd(VirtIOPCIProxy *proxy)
			virtio_bus_start_ioeventfd(VirtioBusState *bus)
				r = vdc->start_ioeventfd(vdev);
				(vdc->start_ioeventfd = virtio_device_start_ioeventfd_impl;)
				virtio_device_start_ioeventfd_impl(VirtIODevice *vdev)
					event_notifier_set_handler(&vq->host_notifier, virtio_queue_host_notifier_read);
					memory_region_transaction_commit();
address_space_update_ioeventfds(AddressSpace *as)
	address_space_add_del_ioeventfds(as, ioeventfds, ioeventfd_nb, as->ioeventfds, as->ioeventfd_nb);
		MEMORY_LISTENER_CALL(as, eventfd_add, Reverse, &section, fd->match_data, fd->data, fd->e);
		(s->memory_listener.listener.eventfd_add = kvm_mem_ioeventfd_add;)
		kvm_mem_ioeventfd_add(MemoryListener *listener,..)
			kvm_set_ioeventfd_mmio(fd, section->offset_within_address_space,..)
				ret = kvm_vm_ioctl(kvm_state, KVM_IOEVENTFD, &iofd);
					ret = ioctl(s->vmfd, type, arg);
					(.unlocked_ioctl = kvm_vm_ioctl,) (kvm 陷入内核态! kvm接手处理)
kvm_vm_ioctl(kvm_state, KVM_IOEVENTFD, &iofd); (kvm kernel)
	case KVM_IOEVENTFD:
	r = kvm_ioeventfd(kvm, &data); (fs的eventfd机制)
		kvm_assign_ioeventfd(kvm, args);
			bus_idx = ioeventfd_bus_from_flags(args->flags);
			ret = kvm_assign_ioeventfd_idx(kvm, bus_idx, args);
				struct _ioeventfd *p;
				eventfd = eventfd_ctx_fdget(args->fd);
				p = kzalloc(sizeof(*p), GFP_KERNEL_ACCOUNT);
				/* 落在 addr ~ addr+len 范围内的会触发 ioeventfd_ops */
				p->addr    = args->addr;
				p->length  = args->len;
				p->bus_idx = bus_idx;
				p->eventfd = eventfd;
				kvm_iodevice_init(&p->dev, &ioeventfd_ops);
				(.write      = ioeventfd_write,)
				ret = kvm_io_bus_register_dev(kvm, bus_idx, p->addr, p->length, &p->dev);
				list_add_tail(&p->list, &kvm->ioeventfds);







/* A 前端 中断 处理 被 后端 通知 */
vp_interrupt(int irq, void *opaque)
	vp_vring_interrupt(int irq, void *opaque)
		vring_interrupt(irq, info->vq)
			vq->vq.callback(&vq->vq);
			(callbacks[] = {virtio_gpu_ctrl_ack, virtio_gpu_cursor_ack};)
			(virtio_gpu_ctrl_ack(struct virtqueue *vq))
				schedule_work(&vgdev->ctrlq.dequeue_work);
				(virtio_gpu_dequeue_ctrl_func(struct work_struct *work))
					while (!virtqueue_enable_cb(vgdev->ctrlq.vq));
						last_used_idx = virtqueue_enable_cb_prepare(_vq);
							/* 前端Guest 更新 used_event */
							vring_used_event(&vq->split.vring) = cpu_to_virtio16(_vq->vdev, last_used_idx = vq->last_used_idx);
					list_for_each_entry_safe(entry, tmp, &reclaim_list, list)
						entry->resp_cb(vgdev, entry);
						(vbuf->resp_cb = resp_cb;)
					wake_up(&vgdev->ctrlq.ack_queue);
				(virtio_gpu_dequeue_cursor_func(struct work_struct *work))
					while (!virtqueue_enable_cb(vgdev->cursorq.vq));
						last_used_idx = virtqueue_enable_cb_prepare(_vq);
							/* 前端Guest 更新 used_event */
							vring_used_event(&vq->split.vring) = cpu_to_virtio16(_vq->vdev, last_used_idx = vq->last_used_idx);
					wake_up(&vgdev->cursorq.ack_queue);





/* A 前端 初始化 应答 dequeue */
virtio_gpu_init(struct drm_device *dev)
	(static vq_callback_t *callbacks[] = { virtio_gpu_ctrl_ack, virtio_gpu_cursor_ack };)
	(ret = virtio_find_vqs(vgdev->vdev, 2, vqs, callbacks, names, NULL);)
		(vdev->config->find_vqs(vdev, nvqs, vqs, callbacks, names, NULL, desc);)
		(virtio_gpu_ctrl_ack(struct virtqueue *vq))
			schedule_work(&vgdev->ctrlq.dequeue_work);
	virtio_gpu_init_vq(&vgdev->ctrlq, virtio_gpu_dequeue_ctrl_func);
		INIT_WORK(&vgvq->dequeue_work, work_func);




/* A 前端Guest 初始化 vq (中断 vq) (vp_find_vqs)
 * 1 申请 分配 vq
 * 2 绑定 中断处理函数
 * 3 通知 host
 */
virtio_gpu_probe(struct virtio_device *vdev)
	ret = virtio_gpu_init(dev);
		callbacks[] = { virtio_gpu_ctrl_ack, virtio_gpu_cursor_ack }
		ret = virtio_find_vqs(vgdev->vdev, 2, vqs, callbacks, names, NULL);
			return vdev->config->find_vqs(vdev, nvqs, vqs, callbacks, names, NULL, desc);
			(.find_vqs	= vp_find_vqs,)
			vp_find_vqs(struct virtio_device *vdev, unsigned nvqs,...)
				/* 3种情况  1 msix  2 msi  3 legacy */
				vp_find_vqs_intx(struct virtio_device *vdev, unsigned nvqs, struct virtqueue *vqs[], vq_callback_t *callbacks[],..)
					vp_dev->vqs = kcalloc(nvqs, sizeof(*vp_dev->vqs), GFP_KERNEL);
					err = request_irq(vp_dev->pci_dev->irq, vp_interrupt, IRQF_SHARED, dev_name(&vdev->dev), vp_dev);
					vqs[i] = vp_setup_vq(vdev, queue_idx++, callbacks[i], names[i], ctx ? ctx[i] : false, VIRTIO_MSI_NO_VECTOR);
						vq = vp_dev->setup_vq(vp_dev, info, index, callback, name, ctx, msix_vec);
						(vp_dev->setup_vq = setup_vq;) (virtio_pci_legacy.c)
setup_vq(struct virtio_pci_device *vp_dev,...)
	iowrite16(index, vp_dev->ioaddr + VIRTIO_PCI_QUEUE_SEL);
	num = ioread16(vp_dev->ioaddr + VIRTIO_PCI_QUEUE_NUM);
	vring_create_virtqueue(index, num, VIRTIO_PCI_VRING_ALIGN, &vp_dev->vdev, true, false, ctx, vp_notify, callback, name)
		vring_create_virtqueue_split(index, num, vring_align,...)
			/* 分配 queue !! */
			queue = vring_alloc_queue(vdev, vring_size(num, vring_align), &dma_addr, GFP_KERNEL|__GFP_NOWARN|__GFP_ZERO);
			vring_init(&vring, num, queue, vring_align);
				vr->num = num;
				vr->desc = p;
				vr->avail = p + num*sizeof(struct vring_desc);
				vr->used = (void *)(((uintptr_t)&vr->avail->ring[num] + sizeof(__virtio16) + align-1) & ~(align - 1));
			vq = __vring_new_virtqueue(index, vring, vdev, weak_barriers, context, notify, callback, name);
				vq = kmalloc(sizeof(*vq), GFP_KERNEL);
				vq->vq.callback = callback;
				vq->vq.num_free = vring.num;
				list_add_tail(&vq->vq.list, &vdev->vqs);
			to_vvq(vq)->split.queue_dma_addr = dma_addr;
	/* 通过写 PCI IO 空间 VIRTIO_PCI_QUEUE_PFN 来告知 Host ，Guest 的 virtqueue 的 GPA 地址；
	 * Host 收到了 GPA，然后转换成 Host 的虚拟地址 */
	q_pfn = virtqueue_get_desc_addr(vq) >> VIRTIO_PCI_QUEUE_ADDR_SHIFT;
		return vq->split.queue_dma_addr;
		(return vq->packed.ring_dma_addr;)
	iowrite32(q_pfn, vp_dev->ioaddr + VIRTIO_PCI_QUEUE_PFN);
	vq->priv = (void __force *)vp_dev->ioaddr + VIRTIO_PCI_QUEUE_NOTIFY;



/* A 前端Guest vring申请大小计算
 * 1 num个desc
 * 2 3个u16   (avail->flags  avail->idx  ??)
 * 3 num个u16 (avail->ring[])
 * 3 3个u16   (used->flags   used->idx   ??)
 * 4 num个used_elem */
vring_size(num, vring_align)
	return ((sizeof(struct vring_desc) * num
		+ sizeof(__virtio16) * (3 + num)
		+ align - 1) & ~(align - 1))
		+ sizeof(__virtio16) * 3
		+ sizeof(struct vring_used_elem) * num;



/* A 前端Guest 初始化 vq (中断 vq) (vm_find_vqs)
 * 1 申请 分配 vq
 * 2 绑定 中断处理函数
 * 3 申请desc空间 并告知host
 */
virtio_gpu_probe(struct virtio_device *vdev)
	dev = drm_dev_alloc(&driver, &vdev->dev);
	ret = virtio_gpu_init(dev);
		callbacks[] = { virtio_gpu_ctrl_ack, virtio_gpu_cursor_ack }
		vgdev = kzalloc(sizeof(struct virtio_gpu_device), GFP_KERNEL);
		ret = virtio_find_vqs(vgdev->vdev, 2, vqs, callbacks, names, NULL);
		vgdev->ctrlq.vq = vqs[0];
		vgdev->cursorq.vq = vqs[1];
			return vdev->config->find_vqs(vdev, nvqs, vqs, callbacks, names, NULL, desc);
			(.find_vqs	= vm_find_vqs,)
			vm_find_vqs(struct virtio_device *vdev, unsigned nvqs, struct virtqueue *vqs[], vq_callback_t *callbacks[],..)
				int irq = platform_get_irq(vm_dev->pdev, 0);
				err = request_irq(irq, vm_interrupt, IRQF_SHARED, dev_name(&vdev->dev), vm_dev);
				vqs[i] = vm_setup_vq(vdev, queue_idx++, callbacks[i], names[i], tx ? ctx[i] : false);
/* num = desc 数量 Host在初始化的时候会写入 是否支持3d 设置不同的desc长度 */
num = readl(vm_dev->base + VIRTIO_MMIO_QUEUE_NUM_MAX);
vq = vring_create_virtqueue(index, num, VIRTIO_MMIO_VRING_ALIGN, vdev, true, true, ctx, vm_notify, callback, name);
	/* 为vring 申请空间
		* queue    为vring的va
		* dma_addr 为vring的pa
		*/
	queue = vring_alloc_queue(vdev, vring_size(num, vring_align), &dma_addr,..)
	vring_create_virtqueue_split(index, num, vring_align,..)
		vring_init(&vring, num, queue, vring_align);
			vr->num = num;
			vr->desc = p; (p=vring_alloc_queue)
			vr->avail = p + num*sizeof(struct vring_desc);
			vr->used = (void *)(((uintptr_t)&vr->avail->ring[num] + sizeof(__virtio16) + align-1) & ~(align - 1));
		__vring_new_virtqueue(unsigned int index, struct vring vring,..)
			vq->vq.num_free = vring.num;
			vq->vq.index = index;
			vq->last_used_idx = 0;
			vq->split.vring = vring;
			vq->split.avail_flags_shadow = 0;
			vq->split.avail_idx_shadow = 0;
		to_vvq(vq)->split.queue_dma_addr = dma_addr;
/* 告诉host queue的长度 */
writel(virtqueue_get_vring_size(vq), vm_dev->base + VIRTIO_MMIO_QUEUE_NUM);
/* 告诉host desc的Gpa地址 */
u64 q_pfn = virtqueue_get_desc_addr(vq) >> PAGE_SHIFT;
	return vq->split.queue_dma_addr;
writel(q_pfn, vm_dev->base + VIRTIO_MMIO_QUEUE_PFN);





/* B 后端Qemu 响应上述writel
 * 只能携带64Byte数据 就是value */
virtio_mmio_write(void *opaque, hwaddr offset, uint64_t value, unsigned size)
	case VIRTIO_MMIO_QUEUE_PFN:
	virtio_queue_set_addr(vdev, vdev->queue_sel, value << proxy->guest_page_shift);
		vdev->vq[n].vring.desc = addr;
		virtio_queue_update_rings(vdev, n);
			VRing *vring = &vdev->vq[n].vring;
			vring->avail = vring->desc + vring->num * sizeof(VRingDesc);
			vring->used = vring_align(vring->avail + offsetof(VRingAvail, ring[vring->num]), vring->align);




/* B 后端Qemu gpu vq 初始化 */
virtio_gpu_class_init(ObjectClass *klass, void *data)
	(vdc->realize = virtio_gpu_device_realize;)
	virtio_gpu_device_realize(DeviceState *qdev, Error **errp)
		(if (!virtio_gpu_base_device_realize(qdev, virtio_gpu_handle_ctrl_cb, virtio_gpu_handle_cursor_cb, errp)))
		virtio_gpu_base_device_realize(DeviceState *qdev,...)
			virtio_init(VIRTIO_DEVICE(g), "virtio-gpu", VIRTIO_ID_GPU,...)
				vdev->vq = g_new0(VirtQueue, VIRTIO_QUEUE_MAX);
			virtio_add_queue(vdev, 256, ctrl_cb);
			virtio_add_queue(vdev, 16, cursor_cb);
				vdev->vq[i].vring.num = queue_size; (queue_size=256)
				vdev->vq[i].handle_output = handle_output;
				vdev->vq[i].used_elems = g_new0(VirtQueueElement, queue_size);
	/* 两个vq = cursor + ctrl 以及 两个vq 对应的处理cb */
	g->ctrl_vq = virtio_get_queue(vdev, 0);
	g->cursor_vq = virtio_get_queue(vdev, 1);
	g->ctrl_bh = qemu_bh_new(virtio_gpu_ctrl_bh, g);
	g->cursor_bh = qemu_bh_new(virtio_gpu_cursor_bh, g);
	QTAILQ_INIT(&g->reslist);
	QTAILQ_INIT(&g->cmdq);



/* B 后端Qemu gpu vq 初始化 */
virtio_gpu_gl_class_init(ObjectClass *klass, void *data)
	(vdc->realize = virtio_gpu_gl_device_realize;)
	virtio_gpu_gl_device_realize(DeviceState *qdev, Error **errp)
		g->parent_obj.conf.flags |= (1 << VIRTIO_GPU_FLAG_VIRGL_ENABLED);
		virtio_gpu_device_realize(DeviceState *qdev, Error **errp)
			virtio_gpu_base_device_realize(qdev, virtio_gpu_handle_ctrl_cb, virtio_gpu_handle_cursor_cb, errp)
				virtio_init(VIRTIO_DEVICE(g), "virtio-gpu", VIRTIO_ID_GPU, sizeof(struct virtio_gpu_config));
					vdev->vq = g_new0(VirtQueue, VIRTIO_QUEUE_MAX);
				/* 支持3d 256
				 * 不支持3d 64
				 */
				virtio_add_queue(vdev, 256, ctrl_cb);
				virtio_add_queue(vdev, 16, cursor_cb);
					vdev->vq[i].vring.num = queue_size;
					vdev->vq[i].used_elems = g_new0(VirtQueueElement, queue_size);
					vdev->vq[i].handle_output = handle_output = virtio_gpu_handle_ctrl_cb;



/* B 后端Qemu pci vq 初始化 */
virtio_pci_bus_class_init(ObjectClass *klass, void *data)
	(k->device_plugged = virtio_pci_device_plugged;)
	virtio_pci_device_plugged(DeviceState *d, Error **errp)
		memory_region_init_io(&proxy->bar, OBJECT(proxy), &virtio_pci_config_ops, proxy, "virtio-pci", size); (MemoryRegion)
		(mr->ops = virtio_pci_config_ops;)
		(virtio_pci_config_ops.write = virtio_pci_config_write,)
		virtio_pci_config_write(void *opaque, hwaddr addr, uint64_t val,..) (addr前端写入地址)(val前端写入值)
			virtio_ioport_write(proxy, addr, val);
				case VIRTIO_PCI_QUEUE_PFN:
				pa = (hwaddr)val << VIRTIO_PCI_QUEUE_ADDR_SHIFT;
				virtio_queue_set_addr(vdev, vdev->queue_sel, pa);
					vdev->vq[n].vring.desc = addr;
					virtio_queue_update_rings(vdev, n);
						vring->avail = vring->desc + vring->num * sizeof(VRingDesc);
						vring->used = vring_align(vring->avail + offsetof(VRingAvail, ring[vring->num]), vring->align);


type_register_static(&virtio_device_info);
	(virtio_device_info.class_init = virtio_device_class_init,)
	virtio_device_class_init(ObjectClass *klass, void *data)
		(dc->realize = virtio_device_realize;)
		virtio_device_realize(DeviceState *dev, Error **errp)
			virtio_bus_device_plugged(VirtIODevice *vdev, Error **errp)
				klass->device_plugged(qbus->parent, &local_err);





/* B 后端 Qemu 收到MMIO异常 并处理 */
(.class_init = x86_cpu_common_class_init,)
x86_cpu_common_class_init(ObjectClass *oc, void *data)
	device_class_set_parent_realize(dc, x86_cpu_realizefn, &xcc->parent_realize);
		x86_cpu_realizefn(DeviceState *dev, Error **errp)
			qemu_init_vcpu(CPUState *cpu)
			cpus_accel->create_vcpu_thread(cpu);
			(ops->create_vcpu_thread = kvm_start_vcpu_thread;)
			qemu_thread_create(cpu->thread, thread_name, kvm_vcpu_thread_fn, cpu, QEMU_THREAD_JOINABLE);
				kvm_vcpu_thread_fn(void *arg)
					kvm_cpu_exec(CPUState *cpu)
						case KVM_EXIT_MMIO:
						address_space_rw(&address_space_memory,..)
							address_space_write(as, addr, attrs, buf, len);
								result = flatview_write(fv, addr, attrs, buf, len);
									flatview_write_continue(fv, addr, attrs, buf, len, addr1, l, mr);
										result |= memory_region_dispatch_write(mr, addr1, val, size_memop(l), attrs);
											access_with_adjusted_size(addr, &data, size, memory_region_write_accessor, ..)
												mr->ops->write(mr->opaque, addr, tmp, size);
												(MemoryRegionOps notify_ops = { .write = virtio_pci_notify_write, })
												virtio_pci_notify_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
													vq->handle_output(vdev, vq);
													(virtio_gpu_base_device_realize(qdev, virtio_gpu_handle_ctrl_cb,..))
													virtio_gpu_handle_ctrl_cb(VirtIODevice *vdev, VirtQueue *vq)






/* A Host Kvm ioeventfd 处理 ept misconfig 找到 被注册的 eventfd */
do_syscall_64(unsigned long nr, struct pt_regs *regs)
	regs->ax = sys_call_table[nr](regs); (x86 syscall Table)
		...
		SYSCALL_DEFINE3(ioctl, unsigned int, fd, unsigned int, cmd, unsigned long, arg)
			ksys_ioctl(fd, cmd, arg);
				error = do_vfs_ioctl(f.file, fd, cmd, arg);
					error = vfs_ioctl(filp, cmd, arg);
						error = filp->f_op->unlocked_ioctl(filp, cmd, arg);
						(.unlocked_ioctl = kvm_vcpu_ioctl,)
						kvm_vcpu_ioctl(struct file *filp, unsigned int ioctl, unsigned long arg)
							r = kvm_arch_vcpu_ioctl_run(vcpu, vcpu->run);
								r = vcpu_run(vcpu);
									for (;;)
									r = vcpu_enter_guest(vcpu);
										kvm_x86_ops->run(vcpu);
										(.run = vmx_vcpu_run,) (VMExit 时走到这个地方)
										r = kvm_x86_ops->handle_exit(vcpu);
										(.handle_exit = vmx_handle_exit,)
vmx_handle_exit(struct kvm_vcpu *vcpu)
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u32 exit_reason = vmx->exit_reason;
	kvm_vmx_exit_handlers[exit_reason](vcpu);  ([EXIT_REASON_EPT_MISCONFIG] = handle_ept_misconfig,)
		handle_ept_misconfig(struct kvm_vcpu *vcpu)
			kvm_mmu_page_fault(vcpu, gpa, PFERR_RSVD_MASK, NULL, 0);
				x86_emulate_instruction(vcpu, cr2_or_gpa, emulation_type, insn, insn_len);
					r = x86_emulate_insn(ctxt);
						rc = writeback(ctxt, &ctxt->src);
							segmented_write(ctxt, op->addr.mem, &op->val, op->bytes);
								ctxt->ops->write_emulated(ctxt, linear, data, size, &ctxt->exception);
								(.write_emulated      = emulator_write_emulated,)
								emulator_write_emulated(struct x86_emulate_ctxt *ctxt,..)
									emulator_read_write(ctxt, addr, (void *)val, bytes, exception, &write_emultor);
										emulator_read_write_onepage(addr, val, now, exception, vcpu, ops);
											handled = ops->read_write_mmio(vcpu, gpa, bytes, val);
											(.read_write_mmio = write_mmio,)

write_mmio(struct kvm_vcpu *vcpu, gpa_t gpa, int bytes, void *val)
	vcpu_mmio_write(vcpu, gpa, bytes, val);
		kvm_io_bus_write(vcpu, KVM_MMIO_BUS, addr, n, v)
			r = __kvm_io_bus_write(vcpu, bus, &range, val);
				kvm_iodevice_write(vcpu, bus->range[idx].dev, range->addr, range->len, val)
					dev->ops->write ? dev->ops->write(vcpu, dev, addr, l, v)
					(.write      = ioeventfd_write,)
					ioeventfd_write(struct kvm_vcpu *vcpu, struct kvm_io_device *this, gpa_t addr,..)
						eventfd_signal(p->eventfd, 1);







 qemu-system-x86-3036    [000] ....  9879.113162: ioeventfd_write <-__kvm_io_bus_write
 qemu-system-x86-3036    [000] ....  9879.113166: <stack trace>
 => ioeventfd_write
 => __kvm_io_bus_write
 => kvm_io_bus_write
 => write_mmio
 => emulator_read_write_onepage
 => emulator_read_write.isra.0
 => emulator_write_emulated
 => segmented_write
 => writeback
 => x86_emulate_insn
 => x86_emulate_instruction
 => kvm_mmu_page_fault
 => kvm_handle_page_fault
 => handle_exception_nmi
 => __vmx_handle_exit
 => vmx_handle_exit
 => vcpu_enter_guest
 => kvm_arch_vcpu_ioctl_run
 => kvm_vcpu_ioctl
 => __x64_sys_ioctl
 => do_syscall_64
 => entry_SYSCALL_64_after_hwframe





/* B 后端Qemu 处理完任务 执行response */
virtio_gpu_ctrl_response_nodata
	virtio_gpu_ctrl_response(g, cmd, &resp, sizeof(resp));
		virtqueue_push(cmd->vq, &cmd->elem, s);
			virtqueue_fill(vq, elem, len, 0);
				virtqueue_unmap_sg(vq, elem, len);
			virtqueue_flush(vq, 1);
				virtqueue_split_flush(vq, count);
					old = vq->used_idx;
					new = old + count;
					vq->inuse -= count;
					vring_used_idx_set(vq, new);
						vq->used_idx = val;
		/* B 后端Qemu 发送虚拟中断 通知前端Guest   由前端 vp_interrupt 处理 */
		virtio_notify(VIRTIO_DEVICE(g), cmd->vq);
			virtio_irq(vq);
				virtio_notify_vector(vq->vdev, vq->vector);
					k->notify(qbus->parent, vector);





/* 前端Guest 和 后端Qemu avail_event used_event */
#define vring_used_event(vr) ((vr)->avail->ring[(vr)->num])
#define vring_avail_event(vr) (*(__virtio16 *)&(vr)->used->ring[(vr)->num])




/* A 前端 释放free_num */
virtqueue_get_buf(struct virtqueue *_vq, unsigned int *len)
	virtqueue_get_buf_ctx(_vq, len, NULL);
		virtqueue_get_buf_ctx_split(_vq, len, ctx);
			detach_buf_split(vq, i, ctx);
				vq->vq.num_free++;


/* A 前端virtqueue 数据结构 */
struct vring_virtqueue {
	struct virtqueue vq;
		struct virtqueue {
			unsigned int index;
			unsigned int num_free;
			(vq->vq.num_free -= descs_used; virtqueue_add) (descs_used = 本次 virtqueue_add 使用了多少descTable)
			void *priv;
		};
	/* Head of free buffer list. */
	unsigned int free_head;
	(vq->free_head = virtio16_to_cpu(_vq->vdev, vq->split.vring.desc[head].next); virtqueue_add)
	(head = vq->free_head;)
	(当前空闲的 descTable)
	/* Number we've added since last sync. */
	unsigned int num_added;
	(virtqueue_add时+1   virtqueue_kick 会清零)

	/* Last used index we've seen. */
	u16 last_used_idx;

	union {
		/* Available for split ring */
		struct {
			/* Actual memory layout for this queue. */
			struct vring vring;
				struct vring {
					unsigned int num;
					struct vring_desc *desc;
					struct vring_avail *avail;
						struct vring_avail {
							__virtio16 idx;
							(先更新 avail_idx_shadow 再更新 avail->idx)
						};
					struct vring_used *used;
				};
			/* Last written value to avail->idx in guest byte order. */
			u16 avail_idx_shadow;
			(vq->split.avail_idx_shadow++;   virtqueue_add)
			(前端每提交一个需求  +1)
			(下一次Guest添加Buffer后   将Buffer头部的二级索引放到avail.ring[]的第几个entry)
			(Guest一次添加Buffer 可能消耗多个 descTable 但是 该值只+1)

			/* Per-descriptor state. */
			struct vring_desc_state_split *desc_state;

			/* DMA address and size information */
			dma_addr_t queue_dma_addr;
			size_t queue_size_in_bytes;
		} split;
	};
};




/* B 后端virtqueue 数据结构 */
struct VirtQueue {
	VRing vring;
		typedef struct VRing {
		unsigned int num;
		(vring中有num个buffer   队列深度)
		unsigned int num_default;
		unsigned int align;
		hwaddr desc;
		hwaddr avail;
		hwaddr used;
		VRingMemoryRegionCaches *caches;
		} VRing;
	VirtQueueElement *used_elems;

	/* Next head to pop */
	uint16_t last_avail_idx;
	(vq->last_avail_idx += elem->ndescs; virtqueue_packed_pop)
	(每次映射一个elem   ndescs+=1   last_avail_idx的增量 标志着消费了多少命令   Host实际工作的位置)

	/* Last avail_idx read from VQ. */
	uint16_t shadow_avail_idx;
	(vq->shadow_avail_idx = vq->last_avail_idx;   virtqueue_packed_pop)
	(last_avail_idx的备份)
	(Host从vq中取出Guest设置的 avail_idx_shadow)

	uint16_t used_idx;
	(old = vq->used_idx;
	 new = old + count;
	 vq->used_idx = val;   virtqueue_split_flush)
	(后端每push一个cmd used_idx+=1   count通常为1   每次调用virtqueue_push时 输入count=1)

	/* Last used index value we have signalled on */
	uint16_t signalled_used;

	/* Last used index value we have signalled on */
	bool signalled_used_valid;

	unsigned int inuse;
};




//TODO - eventfd & irqfd


eventfd

1 允许 Qemu 通知 Host os
2 Host os处理后续IO


irqfd

1 允许 Qemu 通知 Guest os
2 Guest os 处理中断

