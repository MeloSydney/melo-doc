

1 kernel map

vaddr = dma_buf_kmap(dmabuf, 0); /* kernel map */
dma_buf_kunmap(dmabuf, 0, vaddr);
vaddr = dma_buf_vmap(dmabuf);    /* kernel map */
dma_buf_vunmap(dmabuf, 0, vaddr);




2 attachment

{
	attachment = dma_buf_attach(dmabuf, dev);
	table = dma_buf_map_attachment(attachment, DMA_BIDIRECTIONAL);    /* kernel 生成 sg_table 同步 Cache */

	reg_addr = sg_dma_address(table->sgl);
	reg_size = sg_dma_len(table->sgl);
	pr_info("reg_addr = 0x%08x, reg_size = 0x%08x\n", reg_addr, reg_size);

	dma_buf_unmap_attachment(attachment, table, DMA_BIDIRECTIONAL);
	dma_buf_detach(dmabuf, attachment);
}


3 dma_buf_ops

struct dma_buf_ops {
	int (*attach)(struct dma_buf *, struct dma_buf_attachment *);
	void (*detach)(struct dma_buf *, struct dma_buf_attachment *);
	struct sg_table * (*map_dma_buf)(struct dma_buf_attachment *,
					enum dma_data_direction);
	void (*unmap_dma_buf)(struct dma_buf_attachment *,
					struct sg_table *,
					enum dma_data_direction);
}


4 dmabuf framework

dma_buf_map_attachment(struct dma_buf_attachment *attach, enum dma_data_direction direction)
	sg_table = __map_dma_buf(attach, direction);

__map_dma_buf(struct dma_buf_attachment *attach, enum dma_data_direction direction)
	sg_table = attach->dmabuf->ops->map_dma_buf(attach, direction);


5 细说 mmap

https://www.cnblogs.com/huxiao-tee/p/4660352.html

static const struct file_operations dma_buf_fops = {
	.release	= dma_buf_file_release,
	.mmap		= dma_buf_mmap_internal,
	.llseek		= dma_buf_llseek,
	.poll		= dma_buf_poll,
	.unlocked_ioctl	= dma_buf_ioctl,
	.compat_ioctl	= compat_ptr_ioctl,
	.show_fdinfo	= dma_buf_show_fdinfo,
};

dma_buf_mmap_internal(struct file *file, struct vm_area_struct *vma)
	return dmabuf->ops->mmap(dmabuf, vma);

dma_buf_export(const struct dma_buf_export_info *exp_info)
	file = dma_buf_getfile(dmabuf, exp_info->flags);
		file = alloc_file_pseudo(inode, dma_buf_mnt, "dmabuf", flags, &dma_buf_fops);
			struct file *file;
			struct path path;
			d_instantiate(path.dentry, inode);
			file = alloc_file(&path, flags, fops);
				file = alloc_empty_file(flags, current_cred());
					error = init_file(f, flags, cred);
						atomic_long_set(&f->f_count, 1);

6 more times mmap to crash

remap_pfn_range(struct vm_area_struct *vma, unsigned long addr, unsigned long pfn, unsigned long size, pgprot_t prot)
	err = remap_pfn_range_notrack(vma, addr, pfn, size, prot);
		struct mm_struct *mm = vma->vm_mm;
		unsigned long end = addr + PAGE_ALIGN(size);
		pgd = pgd_offset(mm, addr);
		flush_cache_range(vma, addr, end);
		do {
			next = pgd_addr_end(addr, end);
			err = remap_p4d_range(mm, pgd, addr, next, pfn + (addr >> PAGE_SHIFT), prot);
				do {
					next = p4d_addr_end(addr, end);
					err = remap_pud_range(mm, p4d, addr, next, pfn + (addr >> PAGE_SHIFT), prot);
						do {
							next = pud_addr_end(addr, end);
							err = remap_pmd_range(mm, pud, addr, next, pfn + (addr >> PAGE_SHIFT), prot);
								do {
									next = pmd_addr_end(addr, end);
									err = remap_pte_range(mm, pmd, addr, next, pfn + (addr >> PAGE_SHIFT), prot);
										do {
											BUG_ON(!pte_none(*pte));  /* crash ! */
											set_pte_at(mm, addr, pte, pte_mkspecial(pfn_pte(pfn, prot)));
											pfn++;
										} while (pte++, addr += PAGE_SIZE, addr != end);
								} while (pmd++, addr = next, addr != end);
						} while (pud++, addr = next, addr != end);
				} while (p4d++, addr = next, addr != end);
		} while (pgd++, addr = next, addr != end);

mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, dmabuf_fd, 0);


7 kernel dmabuf doc

https://docs.kernel.org/driver-api/dma-buf.html


8 mmap

mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, dmabuf_fd, 0); /* 第一个参数NULL 为什么会分配到同一个va ? */

SYSCALL_DEFINE6(mmap, unsigned long, addr, unsigned long, len,
		unsigned long, prot, unsigned long, flags,
		unsigned long, fd, unsigned long, off)
	return ksys_mmap_pgoff(addr, len, prot, flags, fd, off >> PAGE_SHIFT);
		retval = vm_mmap_pgoff(file, addr, len, prot, flags, pgoff);
			ret = do_mmap(file, addr, len, prot, flag, pgoff, &populate, &uf);
				addr = get_unmapped_area(file, addr, len, pgoff, flags);
					unsigned long error = arch_mmap_check(addr, len, flags);
					get_area = current->mm->get_unmapped_area;
						arch_get_unmapped_area(struct file *filp, unsigned long addr, unsigned long len, unsigned long pgoff, unsigned long flags)
							find_start_end(addr, flags, &begin, &end);
							return vm_unmapped_area(&info);
								addr = unmapped_area(info);
								return addr;
					addr = get_area(file, addr, len, pgoff, flags);
				if (IS_ERR_VALUE(addr))
					return addr;




9 misc mode

misc_register(struct miscdevice *misc)
	misc->this_device = device_create_with_groups(misc_class, misc->parent, dev, misc, misc->groups, "%s", misc->name);
		dev = device_create_groups_vargs(class, parent, devt, drvdata, groups, fmt, vargs);
			retval = device_add(dev);




device_get_devnode(struct device *dev, umode_t *mode, kuid_t *uid, kgid_t *gid, const char **tmp)
	*tmp = dev->class->devnode(dev, mode);


10 request irq

rc = devm_request_irq(dev,
			pdev->irq,
			irq_handler,
			IRQF_SHARED,
			drv_name,
			host);

err = devm_request_threaded_irq(&idev->pdev->dev,
					idev->irq,
					NULL,
					intel_irq,
					IRQF_ONESHOT,
					"bt-host-wake",
					idev);



11 dma_buf_fd

dma_buf_fd(struct dma_buf *dmabuf, int flags)
	fd = get_unused_fd_flags(flags);
		return __get_unused_fd_flags(flags, rlimit(RLIMIT_NOFILE));
			return alloc_fd(0, nofile, flags);

	fd_install(fd, dmabuf->file);



12 f_count

fget(unsigned int fd)
	return __fget(fd, FMODE_PATH);
		return __fget_files(current->files, fd, mask);
			file = __fget_files_rcu(files, fd, mask);
				for (;;) {
					if (unlikely(!atomic_long_inc_not_zero(&file->f_count)))
						continue;
				}



get_file(struct file *f)
	atomic_long_inc(&f->f_count);



#define get_file_rcu(x)
	atomic_long_inc_not_zero(&(x)->f_count)


fput(struct file *file)
	if (atomic_long_dec_and_test(&file->f_count))


__fput_sync(struct file *file)
	if (atomic_long_dec_and_test(&file->f_count))



13 dmabuf fence


显式
userspace 获取 fence fd

隐式
kernelspace 获取 fence fd


dma_fence_signal()  // 完成
dma_fence_wait()    // 查询
dma_fence_begin_signalling()
dma_fence_end_signalling()

ioctl SYNC_START SYNC_END

dma_fence
dma_resv

https://linuxtv.org/downloads/v4l-dvb-internals/device-drivers/ch02s03.html   // fence api

https://www.cnblogs.com/yaongtime/p/14594567.html   // fence demo

https://blog.csdn.net/tugouxp/article/details/131197570  // fence demo

https://xuezitian.github.io/2022/05/28/linux-kernel-dma-buf-analysis.html  // fence dma_resv 区别

dma_buf_ioctl 有fence封装的机制