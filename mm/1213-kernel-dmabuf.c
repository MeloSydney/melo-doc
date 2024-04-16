

//TODO - kernel map

vaddr = dma_buf_kmap(dmabuf, 0); /* kernel map */
dma_buf_kunmap(dmabuf, 0, vaddr);
vaddr = dma_buf_vmap(dmabuf);    /* kernel map */
dma_buf_vunmap(dmabuf, 0, vaddr);




//TODO - attachment

{
	attachment = dma_buf_attach(dmabuf, dev);
	table = dma_buf_map_attachment(attachment, DMA_BIDIRECTIONAL);    /* kernel 生成 sg_table 同步 Cache */

	reg_addr = sg_dma_address(table->sgl);
	reg_size = sg_dma_len(table->sgl);
	pr_info("reg_addr = 0x%08x, reg_size = 0x%08x\n", reg_addr, reg_size);

	dma_buf_unmap_attachment(attachment, table, DMA_BIDIRECTIONAL);
	dma_buf_detach(dmabuf, attachment);
}


//TODO - dma_buf_ops

struct dma_buf_ops {
  bool cache_sgt_mapping;
	int (*attach)(struct dma_buf *, struct dma_buf_attachment *);
	void (*detach)(struct dma_buf *, struct dma_buf_attachment *);
	struct sg_table * (*map_dma_buf)(struct dma_buf_attachment *, enum dma_data_direction);
	void (*unmap_dma_buf)(struct dma_buf_attachment *,struct sg_table *, enum dma_data_direction);
	void (*release)(struct dma_buf *);
	int (*begin_cpu_access)(struct dma_buf *, enum dma_data_direction);
	int (*end_cpu_access)(struct dma_buf *, enum dma_data_direction);
	int (*mmap)(struct dma_buf *, struct vm_area_struct *vma);
	void *(*vmap)(struct dma_buf *);
	void (*vunmap)(struct dma_buf *, void *vaddr);
};


1 map_dma_buf/unmap_dma_buf exporter提供dma_buf，ops内必须实现
2 mmap 用于用户态user访问1中提供的dma buf
3 vunmap/vmap 用于内核态importer访问1中提供的dma buf，两者区别在于物理地址是否连续
4 begin_cpu_access/end_cpu_access 考虑cache一致性时使用
5 attach/detach 设备绑定
6 release 当无设备引用时释放

//TODO - dmabuf framework

dma_buf_map_attachment(struct dma_buf_attachment *attach, enum dma_data_direction direction)
	sg_table = __map_dma_buf(attach, direction);

__map_dma_buf(struct dma_buf_attachment *attach, enum dma_data_direction direction)
	sg_table = attach->dmabuf->ops->map_dma_buf(attach, direction);


//TODO - 细说 mmap

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

//TODO - more times mmap to crash

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


//TODO - kernel dmabuf doc

https://docs.kernel.org/driver-api/dma-buf.html


//TODO - mmap

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




//TODO - misc mode

misc_register(struct miscdevice *misc)
	misc->this_device = device_create_with_groups(misc_class, misc->parent, dev, misc, misc->groups, "%s", misc->name);
		dev = device_create_groups_vargs(class, parent, devt, drvdata, groups, fmt, vargs);
			retval = device_add(dev);




device_get_devnode(struct device *dev, umode_t *mode, kuid_t *uid, kgid_t *gid, const char **tmp)
	*tmp = dev->class->devnode(dev, mode);


//TODO - request irq

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



//TODO - dma_buf_fd

dma_buf_fd(struct dma_buf *dmabuf, int flags)
	fd = get_unused_fd_flags(flags);
		return __get_unused_fd_flags(flags, rlimit(RLIMIT_NOFILE));
			return alloc_fd(0, nofile, flags);

	fd_install(fd, dmabuf->file);



//TODO - f_count

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



//TODO - dmabuf fence

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


dma_buf_export(const struct dma_buf_export_info *exp_info)
        struct dma_buf *dmabuf;
        struct file *file;
        file = dma_buf_getfile(exp_info->size, exp_info->flags);
                struct inode *inode = alloc_anon_inode(dma_buf_mnt->mnt_sb);
                        static const struct address_space_operations anon_aops = {
                                .dirty_folio	= noop_dirty_folio,
                        };
                        struct inode *inode = new_inode_pseudo(s);
                        inode->i_ino = get_next_ino();  /* atomic_t */
                        inode->i_mapping->a_ops = &anon_aops;
                        inode->i_state = I_DIRTY;
                        inode->i_mode = S_IRUSR | S_IWUSR;
                        inode->i_uid = current_fsuid();
                        inode->i_gid = current_fsgid();
                        inode->i_flags |= S_PRIVATE;
                        simple_inode_init_ts(inode);
                                struct timespec64 ts = inode_set_ctime_current(inode);
                                inode_set_atime_to_ts(inode, ts);
                                inode_set_mtime_to_ts(inode, ts);
                struct file *file;
                inode->i_size = size;
                file = alloc_file_pseudo(inode, dma_buf_mnt, "dmabuf", flags, &dma_buf_fops);
                return file;
        dmabuf = kzalloc(alloc_size, GFP_KERNEL);
        dmabuf->priv = exp_info->priv;
        dmabuf->ops = exp_info->ops;
        dmabuf->size = exp_info->size;
        dmabuf->exp_name = exp_info->exp_name;
        dmabuf->owner = exp_info->owner;
        file->private_data = dmabuf;
        file->f_path.dentry->d_fsdata = dmabuf;
        dmabuf->file = file;
        list_add(&dmabuf->list_node, &db_list.head);  /* static struct dma_buf_list db_list; dma-buf.c */
        return dmabuf;


dma_buf_fd(struct dma_buf *dmabuf, int flags)
        fd = get_unused_fd_flags(flags);
                return __get_unused_fd_flags(flags, rlimit(RLIMIT_NOFILE));
                        return alloc_fd(0, nofile, flags);
                                struct fdtable *fdt;
                                fdt = files_fdtable(files);
                                if (flags & O_CLOEXEC)
                                        __set_close_on_exec(fd, fdt);
        fd_install(fd, dmabuf->file);
        return fd;


ksys_mmap_pgoff(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags, unsigned long fd, unsigned long pgoff)
        struct file *file = NULL;
        if (!(flags & MAP_ANONYMOUS)) {
        file = fget(fd);  /* f_count */
                return __fget(fd, FMODE_PATH);
                        return __fget_files(current->files, fd, mask);
                                file = __fget_files_rcu(files, fd, mask);
                                        struct file *file;
                                        struct fdtable *fdt = rcu_dereference_raw(files->fdt);
                                        fdentry = fdt->fd + array_index_nospec(fd, fdt->max_fds);
                                        file = rcu_dereference_raw(*fdentry);
        }
        retval = vm_mmap_pgoff(file, addr, len, prot, flags, pgoff);
                ret = security_mmap_file(file, prot, flag);
                if (!ret) {
                        ret = do_mmap(file, addr, len, prot, flag, pgoff, &populate, &uf);
                }
        if (file)
                fput(file);  /* f_count */
        return retval;


do_mmap(struct file *file, unsigned long addr, unsigned long len,
                        unsigned long prot, unsigned long flags, unsigned long pgoff,
                        unsigned long *populate, struct list_head *uf)
        addr = get_unmapped_area(file, addr, len, pgoff, flags);
        if (file) {
                struct inode *inode = file_inode(file);
        }
        mmap_region(struct file *file, unsigned long addr, unsigned long len, vm_flags_t vm_flags, unsigned long pgoff, struct list_head *uf)
                struct mm_struct *mm = current->mm;
                struct vm_area_struct *vma = NULL;
                vma = vm_area_alloc(mm);
                vma->vm_start = addr;
                vma->vm_end = end;
                vma->vm_flags = vm_flags;
                vma->vm_page_prot = vm_get_page_prot(vm_flags);
                vma->vm_pgoff = pgoff;
                if (file)
                        vma->vm_file = get_file(file);  /* f_count */
                        error = call_mmap(file, vma);  /* return file->f_op->mmap(file, vma); */
                if (vma->vm_file)
                        i_mmap_lock_write(vma->vm_file->f_mapping);
                mm->map_count++;
                file = vma->vm_file;


SYSCALL_DEFINE1(close, unsigned int, fd)
        int retval = close_fd(fd);
                file = pick_file(files, fd);
                return filp_close(file, files);
                        fput(filp);
                                if (atomic_long_dec_and_test(&file->f_count))
                                        struct task_struct *task = current;
                                        if (likely(!in_interrupt() && !(task->flags & PF_KTHREAD)))
                                                init_task_work(&file->f_rcuhead, ____fput);
                                                        ____fput(struct callback_head *work)
                                                                __fput(container_of(work, struct file, f_rcuhead));
                                                                        if (file->f_op->release)
                                                                                file->f_op->release(inode, file);
                                                                                fops_put(file->f_op);
                                                                                        do { if (fops) module_put((fops)->owner); } while(0)
                                                                                put_pid(file->f_owner.pid);
                                                                                put_file_access(file);
                                                                                dput(dentry);
                                                                                        dentry = dentry_kill(dentry);
                                                                                                struct inode *inode = dentry->d_inode;
                                                                                                __dentry_kill(dentry);
                                                                                                        dentry->d_op->d_release(dentry);
                                                                                                                dma_buf_release(struct dentry *dentry)
                                                                                                                        dmabuf->ops->release(dmabuf);
                                                                                mntput(mnt);
                                                                                file_free(file);
                                                if (!task_work_add(task, &file->f_rcuhead, TWA_RESUME))
                                                        return;


__alloc_file(int flags, const struct cred *cred)
        atomic_long_set(&f->f_count, 1);


dma_buf_poll(struct file *file, poll_table *poll)
        get_file(dmabuf->file);


get_dma_buf(struct dma_buf *dmabuf)
        get_file(dmabuf->file);


SYSCALL_DEFINE2(munmap, unsigned long, addr, size_t, len)
        return vm_munmap(addr, len);
                return __vm_munmap(start, len, false);
                        struct mm_struct *mm = current->mm;
                        LIST_HEAD(uf);
                        ret = do_mas_munmap(&mas, mm, start, len, &uf, downgrade);
                                struct vm_area_struct *vma;
                                end = start + PAGE_ALIGN(len);
                                arch_unmap(mm, start, end);
                                vma = mas_find(mas, end - 1);
                                return do_mas_align_munmap(mas, vma, mm, start, end, uf, downgrade);
                                        struct vm_area_struct *prev, *next = NULL;