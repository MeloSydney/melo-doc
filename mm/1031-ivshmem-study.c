



//TODO - e371 ivshmem 初始化

static struct pci_driver uio_ikv_shmem_driver = {
	.name = "uio_ikvshmem",
	.id_table = ikvsm_id,
	.probe = probe,
	.remove = remove,
};


struct uio_ivshmem_dev {
	struct uio_info info;
	struct kvs_config_head cfg;
	struct pci_dev *pdev;
	struct uio_msix_info msix_info;
};

struct kvs_config_head {
	struct kvs_cap_head head;
	union { struct kvs_config_type0 t0; } type;
} __attribute__((packed));

struct kvs_cap_head {
	//PCI CAP VINDOR ID: must 9
	uint8_t capid;
	uint8_t next;
	uint8_t type;
	uint8_t name[KVS_REGION_NAME_LEN];
} __attribute__((packed));


/**
 * struct uio_info - UIO device capabilities
 * @uio_dev:		the UIO device this info belongs to
 * @name:		device name
 * @version:		device driver version
 * @mem:		list of mappable memory regions, size==0 for end of list
 * @port:		list of port regions, size==0 for end of list
 * @irq:		interrupt number or UIO_IRQ_CUSTOM
 * @irq_flags:		flags for request_irq()
 * @priv:		optional private data
 * @handler:		the device's irq handler
 * @mmap:		mmap operation for this uio device
 * @open:		open operation for this uio device
 * @release:		release operation for this uio device
 * @irqcontrol:		disable/enable irqs when 0/1 is written to /dev/uioX
 */
struct uio_info {
	struct uio_device	*uio_dev;
	const char		*name;
	const char		*version;
	struct uio_mem		mem[MAX_UIO_MAPS];	/* #define MAX_UIO_MAPS	5 */
	struct uio_port		port[MAX_UIO_PORT_REGIONS];
	long			irq;
	unsigned long		irq_flags;
	void			*priv;
	irqreturn_t (*handler)(int irq, struct uio_info *dev_info);
	int (*mmap)(struct uio_info *info, struct vm_area_struct *vma);
	int (*open)(struct uio_info *info, struct inode *inode);
	int (*release)(struct uio_info *info, struct inode *inode);
	int (*irqcontrol)(struct uio_info *info, s32 irq_on);
#ifdef CONFIG_PCI_MSI
	int (*ioctl)(struct uio_info *info, unsigned int cmd,
			unsigned long arg);
#endif
};

struct uio_mem {
	const char		*name;
	phys_addr_t		addr;
	unsigned long		offs;
	resource_size_t		size;
	int			memtype;
	void __iomem		*internal_addr;
	struct uio_map		*map;
};

probe(struct pci_dev *pdev, const struct pci_device_id *id)
	if (IS_SHMEM_PCI_KV(pdev))
		return kv_probe(pdev, id);
			rc = ikvshmem_get_cfg(pdev, &kvsd->cfg);
			reg_size = pci_resource_len(pdev, REGISTER_BAR);
			kvsd->regs = pci_iomap(pdev, REGISTER_BAR, reg_size);
			kvsd->shm_phys = pci_resource_start(pdev, SHARED_MEMORY_BAR);
			kvsd->shm_size = pci_resource_len(pdev, SHARED_MEMORY_BAR);
			kvsd->shm_map = memremap(kvsd->shm_phys, kvsd->shm_size, MEMREMAP_WB);
	else
		return iv_probe(pdev, id);
			struct uio_ivshmem_dev *gdev;
			gdev = devm_kzalloc(&pdev->dev, sizeof(struct uio_ivshmem_dev), GFP_KERNEL);
			if (ikvshmem_get_cfg(pdev, &gdev->cfg))		/* 获取 pci 配置 */
			gdev->info.name = gdev->cfg.head.name;		/* 获取 name */
			gdev->info.fix_name = true;
			gdev->info.ioctl = uio_msi_ioctl;
			gdev->pdev = pdev;
			uiomem = &gdev->info.mem[0];
			for (i = 0; i < MAX_UIO_MAPS; ++i)
				struct resource *r = &pdev->resource[i];
				if (i == 2)				/* 针对 bar2 ?? */
					uiomem->memtype = UIO_MEM_IOVA;
				else
					uiomem->memtype = UIO_MEM_PHYS;
				uiomem->addr = r->start & PAGE_MASK;
				uiomem->offs = r->start & ~PAGE_MASK;
				uiomem->size = (uiomem->offs + resource_size(r) + PAGE_SIZE - 1) & PAGE_MASK;
				uiomem->name = r->name;
				++uiomem;
			err = devm_uio_register_device(&pdev->dev, &gdev->info);
				__devm_uio_register_device(THIS_MODULE, parent, info)
					struct uio_info **ptr;
					ptr = devres_alloc(devm_uio_unregister_device, sizeof(*ptr), GFP_KERNEL);
					ret = __uio_register_device(owner, parent, info);
						if (info->fix_name)
							ret = dev_set_name(&idev->dev, "uio_%s", info->name);
						else
							ret = dev_set_name(&idev->dev, "uio%d", idev->minor);
						ret = uio_dev_add_attributes(idev);



drivers/misc/driver/cma_shmm_service.c

cma_shmm_service_init(void)
	ret = platform_device_register(&cma_shmm_device);
	ret = platform_driver_register(&cma_shmm_driver);


static struct platform_driver cma_shmm_driver = {
    .driver = {
        .name = "cma_shmm_service",
    },
    .probe = cma_shmm_service_probe,
    .remove = cma_shmm_service_remove,
};


cma_shmm_service_probe(struct platform_device *pdev)
	init_status = cma_shmm_areas_init();
		cma = cma_find_by_name(CMA_SHM_NAME);
		cma_share_dev->cma_area = cma;
		ret = dma_heap_add_cma(cma_share_dev);
	init_status = shmm_use_statics_init_sysfs();




cma 如何初始化 ?


cma_init_reserved_mem(phys_addr_t base, phys_addr_t size,
				 unsigned int order_per_bit,
				 const char *name,
				 struct cma **res_cma)


cma_declare_contiguous_nid(phys_addr_t base,
			phys_addr_t size, phys_addr_t limit,
			phys_addr_t alignment, unsigned int order_per_bit,
			bool fixed, const char *name, struct cma **res_cma,
			int nid)




//TODO - dmabuf exporter demo

https://github.com/hexiaolong2008/sample-code/tree/master/dma-buf/04


static long exporter_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int fd = dma_buf_fd(dmabuf_exported, O_CLOEXEC);	/* 有 exporter 就可以导出dmabuf fd */
	if (copy_to_user((int __user *)arg, &fd, sizeof(fd)))
		return -EFAULT;
	return 0;
}

static struct dma_buf *exporter_alloc_page(void)		/* 如何制作一个 exporter */
{
	DEFINE_DMA_BUF_EXPORT_INFO(exp_info);
	struct dma_buf *dmabuf;
	void *vaddr;

	vaddr = kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (!vaddr)
		return NULL;

	exp_info.ops = &exp_dmabuf_ops;
	exp_info.size = PAGE_SIZE;
	exp_info.flags = O_CLOEXEC;
	exp_info.priv = vaddr;

	dmabuf = dma_buf_export(&exp_info);
	if (IS_ERR(dmabuf)) {
		kfree(vaddr);
		return NULL;
	}

	sprintf(vaddr, "hello world!");

	return dmabuf;
}

?? 有一段物理地址 phy_base phy_size 如何构造一个 exporter


celadon_kernel/drivers/misc/fastrpc.c




//TODO - kernel mmap remap_pfn_range

http://blog.chinaunix.net/uid-20754930-id-1877658.html

/data1/shifan/src-g4release.kernel/kernel/drivers/dma-buf/heaps/system_heap.c

https://linux-kernel-labs.github.io/refs/heads/master/labs/memory_mapping.html
SetPageReserved 不允许 swap out
ClearPageReserved

//TODO - kernel mmap 调用栈分析

https://www.sobyte.net/post/2022-03/mmap/


//TODO - io_remap_pfn_range

https://unix.stackexchange.com/questions/643077/how-can-my-pci-device-driver-remap-pci-memory-to-userspace


//TODO - kernel mmap pci bar into us

https://www.cnblogs.com/dream397/p/13563978.html



//TODO - kvshmem doorbell irq

/* id */
kvshmem_send_irq(void *handle, enum kv_shmem_vmid id, enum kv_shmem_irq chn)
	struct kv_shmem_device *kvsd = (struct kv_shmem_device *)handle;
	writel((id << 16) | chn, kvsd->regs + DOORBELL);


kv_probe(struct pci_dev *pdev, const struct pci_device_id *id)
	reg_size = pci_resource_len(pdev, REGISTER_BAR);
	kvsd->regs = pci_iomap(pdev, REGISTER_BAR, reg_size);


struct kv_test {
	char *name;
	void *handle;
	struct kv_shmem_resource res;
	//just for test
	int irqs[8];
};

enum kv_shmem_irq {
	KV_SHMEM_IRQ_CHN0, /* 0 */
	KV_SHMEM_IRQ_CHN1,
	KV_SHMEM_IRQ_CHN2,
	KV_SHMEM_IRQ_CHN3,
	KV_SHMEM_IRQ_CHN4,
	KV_SHMEM_IRQ_CHN5,
	KV_SHMEM_IRQ_CHN6,
	KV_SHMEM_IRQ_CHN7  /* 7 */
};

static struct kv_test g_kv_test = {
	.irqs[0] = KV_SHMEM_IRQ_CHN0,
	.irqs[1] = KV_SHMEM_IRQ_CHN1,
	.irqs[2] = KV_SHMEM_IRQ_CHN2,
	.irqs[3] = KV_SHMEM_IRQ_CHN3,
	.irqs[4] = KV_SHMEM_IRQ_CHN4,
	.irqs[5] = KV_SHMEM_IRQ_CHN5,
	.irqs[6] = KV_SHMEM_IRQ_CHN6,
	.irqs[7] = KV_SHMEM_IRQ_CHN7
};

kv_probe(struct pci_dev *pdev, const struct pci_device_id *id)
	kvsd = devm_kzalloc(&pdev->dev, sizeof(*kvsd), GFP_KERNEL);
	kvshmem_init_irq(struct kv_shmem_device *kvsd)
		kvsd->msix_entries = kzalloc(nvecs * sizeof(struct msix_entry), GFP_KERNEL);
		ret = pci_enable_msix_exact(kvsd->pdev, kvsd->msix_entries, nvecs);
		kvsd->msix_enabled = true;

kv_test_init(void)
	g_kv_test.handle = kvshmem_get_handle(kvname);
	for (i = 0; i < 8; i++) {
		rc = kvshmem_request_irq(g_kv_test.handle, g_kv_test.irqs[i], kv_test_interrupt, &g_kv_test.irqs[i]);
			struct kv_shmem_device *kvsd = (struct kv_shmem_device *)handle;
			return kvshmem_do_request_irq(kvsd, irq, handler, data);
				if (kvsd->msix_enabled && !kvsd->requested_irqs[irq]) {
					rc = request_irq(kvsd->msix_entries[irq].vector, handler, IRQF_TRIGGER_NONE, kvsd->cfg.head.name, data);
					kvsd->requested_irqs[irq] = true;
					return 0;
				}
		if (rc) {
			pr_info("kvtest %s %d irq error:%d\n", __func__, __LINE__, rc);
			goto resource_err;
		}
	}

irqreturn_t kv_test_interrupt(int irq, void *data) /* isr */


kvshmem_get_handle(char *name)
	list_for_each(pos, &kvs_devs_list) {
		if (strncmp(kvsd->cfg.head.name, name, KVS_REGION_NAME_LEN) == 0)
			find = true;
			kvsd->user_ref = 1;
			break;
	}


//TODO - cp shm_dev.c mem

1. shm_dev.c 没有使用 dmabuf 建议改造
2. shm_dev.c shm_block_info 数据结构不再适用

struct shm_block_info {
    char key[64];
    int index;
    int mode;
    int srcsize;
    int dstsize;
    int srcmode;
    int dstmode;
    u64 srcaddr;
    u64 dstaddr;
    void *kernel_virtual_addr;
    int dma_write_state;
    struct dma_buf *dma_buf;

    bool remote_addr_sent;
    bool remote_sync_sent;
    struct scatter_data *scatter_data;
    struct dma_chan *chan;
};


struct shm_dev_private_data {
	void *kernel_virtual_addr;
	struct shm_block_info smb_info;
};

shm_dev_rpmsg_endpoint_callback(struct rpmsg_device *rpdev, void *data, int len, void *priv, u32 addr)
	switch (sd_sr_data->operate) {
	case SHM_DEV_RPMSG_GET:
		add_info = (struct shm_block_list_info *)kmalloc(sizeof(struct shm_block_list_info), GFP_KERNEL);
		memcpy(&add_info->smb_info,  &sd_sr_data->smb_info, sizeof(struct shm_block_info));
		atomic_set(&add_info->refcount, 1);
		add_shm_block_list_info(add_info);is_wake_up_ioctl = 1;
		wake_up(&g_rpmsg_waitq_ioctl);
		break;
	case SHM_DEV_RPMSG_FREE:
		list_info = find_shm_block_list_info(sd_sr_data->smb_info.key);
		if (list_info) {
			atomic_dec(&list_info->refcount);
			if (atomic_read(&list_info->refcount) == 0) {
				ret = shmm_free_phy_addr(&sd_sr_data->smb_info);
				del_shm_block_list_info(list_info);
			}
		}
	}


//TODO - cp shm_dev.c mmap

static const struct file_operations shm_dev_fops =
{
	.open = shm_dev_open,
	.unlocked_ioctl = shm_dev_ioctl,
	.mmap = shm_dev_mmap,
	.release = shm_dev_release,
};


shm_dev_mmap(struct file *filp, struct vm_area_struct *vma)
	struct shm_dev_private_data *smd_pdata = filp->private_data;
	size_t size = vma->vm_end - vma->vm_start;
	phys_addr_t offset = smd_pdata->smb_info.addr;
	vma->vm_pgoff = offset >> PAGE_SHIFT;		/* pfn */
	vma->vm_ops = &mmap_mem_ops;
	if (remap_pfn_range(vma,
				vma->vm_start, //virtual addr start
				vma->vm_pgoff, //phys addr
				size,
				vma->vm_page_prot))


//TODO - pci_dev.c

struct cpvt_vm_pci_dev_config sos_pci_devs[CONFIG_MAX_PCI_DEV_NUM] = {
	//chnlnotify
	IVSHMEM_PVM_CONFIG(0, 0x00U, 0x1aU, 0x00U, IVSHMEM_FEATURE_FIX_USER),
	//chnlivshmem
	IVSHMEM_PVM_CONFIG(1, 0x00U, 0x1aU, 0x01U, IVSHMEM_FEATURE_FIX_KERNEL),
	//ivnotify
	IVSHMEM_PVM_CONFIG(2, 0x00U, 0x1bU, 0x00U, IVSHMEM_FEATURE_FIX_USER),
	//camera_data
	IVSHMEM_PVM_CONFIG(3, 0x00U, 0x1cU, 0x00U, IVSHMEM_FEATURE_FIX_USER),
	//camera_cmd
	IVSHMEM_PVM_CONFIG(4, 0x00U, 0x1cU, 0x01U, IVSHMEM_FEATURE_FIX_KERNEL),
	//uart
	IVSHMEM_PVM_CONFIG(5, 0x00U, 0x1cU, 0x02U, IVSHMEM_FEATURE_FIX_USER),
};

struct cpvt_vm_pci_dev_config vm2_pci_devs[VM2_CONFIG_PCI_DEV_NUM] = {
	IVSHMEM_GVM_CONFIG(0, IVSHMEM_FEATURE_USER),
	IVSHMEM_GVM_CONFIG(1, IVSHMEM_FEATURE_KERNEL),
	IVSHMEM_GVM_CONFIG(3, IVSHMEM_FEATURE_KERNEL),
	IVSHMEM_GVM_CONFIG(4, IVSHMEM_FEATURE_KERNEL),
	IVSHMEM_GVM_CONFIG(5, IVSHMEM_FEATURE_KERNEL),
};


//TODO - vm_configurations.c

struct cpvt_vm_config {
	enum cpvt_vm_load_order load_order;		/* specify the load order of VM */
	char name[MAX_VM_NAME_LEN];				/* VM name identifier */
	uint8_t reserved[2];
	uint8_t severity;				/* severity of the VM */
	uint64_t cpu_affinity;				/* The set bits represent the pCPUs the vCPUs of
							 * the VM may run on.
							 */
	uint64_t guest_flags;				/* VM flags that we want to configure for guest
							 * Now we have two flags:
							 *	GUEST_FLAG_SECURE_WORLD_ENABLED
							 *	GUEST_FLAG_LAPIC_PASSTHROUGH
							 * We could add more guest flags in future;
							 */
	uint32_t vm_prio;				/* The priority for VM vCPU scheduling */
	uint16_t companion_vm_id;			/* The companion VM id for this VM */
	struct cpvt_vm_mem_config memory;		/* memory configuration of VM */
	struct epc_section epc;				/* EPC memory configuration of VM */
	uint16_t pci_dev_num;				/* indicate how many PCI devices in VM */
	struct cpvt_vm_pci_dev_config *pci_devs;	/* point to PCI devices BDF list */
	struct cpvt_vm_os_config os_config;		/* OS information the VM */
	struct cpvt_vm_acpi_config acpi_config;		/* ACPI config for the VM */

	/*
	 * below are variable length members (per build).
	 * Service VM can get the vm_configs[] array through hypercall, but Service VM may not
	 * need to parse these members.
	 */

	uint16_t num_pclosids; /* This defines the number of elements in the array pointed to by pclosids */
	/* pclosids: a pointer to an array of physical CLOSIDs (pCLOSIDs)) that is defined in vm_configurations.c
	 * by vmconfig,
	 * applicable only if CONFIG_RDT_ENABLED is defined on CAT capable platforms.
	 * The number of elements in the array must be equal to the value given by num_pclosids
	 */
	uint16_t *pclosids;

	/* max_type_pcbm (type: l2 or l3) specifies the allocated portion of physical cache
	 * for the VM and is a contiguous capacity bitmask (CBM) starting at bit position low
	 * (the lowest assigned physical cache way) and ending at position high
	 * (the highest assigned physical cache way, inclusive).
	 * As CBM only allows contiguous '1' combinations, so max_type_pcbm essentially
	 * is a bitmask that selects/covers all the physical cache ways assigned to the VM.
	 */
	uint32_t max_l2_pcbm;
	uint32_t max_l3_pcbm;

	struct vuart_config vuart[MAX_VUART_NUM_PER_VM];/* vuart configuration for VM */

	bool pt_tpm2;
	struct cpvt_mmiodev mmiodevs[MAX_MMIO_DEV_NUM];

	bool pt_p2sb_bar; /* whether to passthru p2sb bridge to pre-launched VM or not */

	uint16_t pt_intx_num; /* number of pt_intx_config entries pointed by pt_intx */
	struct pt_intx_config *pt_intx; /* stores the base address of struct pt_intx_config array */
} __aligned(8);


struct cpvt_vm_config vm_configs[CONFIG_MAX_VM_NUM] = {
	{
		/* Static configured VM0 */
		CONFIG_SERVICE_VM,
		.name = "CPVT_VM0",
		/* Allow Service VM to reboot the system since it is the highest priority VM. */
		.vm_prio = PRIO_LOW,
		.companion_vm_id = 65535U,
		.guest_flags = (GUEST_FLAG_STATIC_VM),
		.cpu_affinity = SERVICE_VM_CONFIG_CPU_AFFINITY,
		.os_config =
			{
				.name = "",
				.kernel_type = KERNEL_BZIMAGE,
				.kernel_mod_tag = "Linux_bzImage",
				.ramdisk_mod_tag = "",
				.bootargs = SERVICE_VM_OS_BOOTARGS,
			},
		.vuart[0] =
			{
				.type = VUART_LEGACY_PIO,
				.addr.port_base = 0x3F8U,
				.irq = 4U,
			},
		.pci_dev_num = 0U,
		.pci_devs = sos_pci_devs,
	},
};


//TODO - ivshmem.c
hv init a ivshmem pcie device

create_ivshmem_vdev(struct cpvt_vm *vm, struct cpvt_vdev *dev)
	for (i = 0U; i < vm_config->pci_dev_num; i++) {
		dev_config = &vm_config->pci_devs[i];
		struct ivshmem_shm_region *region = find_shm_region(dev_config->shm_region_name);
		dev_config->vbdf.value = (uint16_t) dev->slot;
		dev_config->vbar_base[IVSHMEM_MMIO_BAR] = (uint64_t) dev->io_addr[IVSHMEM_MMIO_BAR];
		ivshare_info("bar%d, addr:0x%lx", \
				IVSHMEM_MMIO_BAR, \
				dev_config->vbar_base[IVSHMEM_MMIO_BAR]);
		dev_config->vbar_base[IVSHMEM_MSIX_BAR] = (uint64_t) dev->io_addr[IVSHMEM_MSIX_BAR];
		ivshare_info("bar%d, addr:0x%lx", \
				IVSHMEM_MSIX_BAR, \
				dev_config->vbar_base[IVSHMEM_MSIX_BAR]);
		dev_config->vbar_base[IVSHMEM_SHM_BAR] = (uint64_t) dev->io_addr[IVSHMEM_SHM_BAR];
		dev_config->vbar_base[IVSHMEM_SHM_BAR] |= ((uint64_t) dev->io_addr[IVSHMEM_SHM_BAR + 1U]) << 32U;
		ivshare_info("bar%d, addr:0x%lx", \
				IVSHMEM_SHM_BAR, \
				dev_config->vbar_base[IVSHMEM_SHM_BAR]);
		(void) vpci_init_vdev(&vm->vpci, dev_config, NULL);
	}


//TODO - vpci.c

vpci_init_vdev(struct acrn_vpci *vpci, struct acrn_vm_pci_dev_config *dev_config, struct pci_vdev *parent_pf_vdev)
	struct pci_vdev *vdev = &vpci->pci_vdevs[vpci->pci_vdev_cnt];
	vpci->pci_vdev_cnt++;
	vdev->vpci = vpci;
	vdev->bdf.value = dev_config->vbdf.value;
	vdev->pdev = dev_config->pdev;
	vdev->pci_dev_config = dev_config;
	vdev->phyfun = parent_pf_vdev;

	hlist_add_head(&vdev->link, &vpci->vdevs_hlist_heads[hash64(dev_config->vbdf.value, VDEV_LIST_HASHBITS)]);
	if (dev_config->vdev_ops != NULL) {
		vdev->vdev_ops = dev_config->vdev_ops;
	} else {
		vdev->vdev_ops = &pci_pt_dev_ops;
		ASSERT(dev_config->emu_type == PCI_DEV_TYPE_PTDEV,
			"Only PCI_DEV_TYPE_PTDEV could not configure vdev_ops");
		ASSERT(dev_config->pdev != NULL, "PCI PTDev is not present on platform!");
	}

	vdev->vdev_ops->init_vdev(vdev);
		init_ivshmem_vdev(struct pci_vdev *vdev)   /* hypervisor 配置 config space */
			create_ivshmem_device(vdev);
				vdev->priv_data = &ivshmem_dev[i];   /* 选一个空的slot */
			pci_vdev_write_vcfg(vdev, PCIR_VENDOR, 2U, IVSHMEM_VENDOR_ID);							/* vendor id */
			pci_vdev_write_vcfg(vdev, PCIR_DEVICE, 2U, IVSHMEM_DEVICE_ID);							/* device id */
			pci_vdev_write_vcfg(vdev, PCIR_REVID,  1U, IVSHMEM_REV | vdev->pci_dev_config->funcs);				/* revision id */
			pci_vdev_write_vcfg(vdev, PCIR_CLASS,  1U, IVSHMEM_CLASS);							/* class code */
			pci_vdev_write_vcfg(vdev, PCIR_HDRTYPE, 1U, PCIM_HDRTYPE_NORMAL | ((vdev->bdf.bits.f == 0U) ? PCIM_MFDEV : 0U));/* header type */
			region = find_shm_region(vdev->pci_dev_config->shm_region_name);
			if (region) {
				// add pci capability for kv share mem
				memset(&cfg, 0, sizeof(cfg));
				cfg.head.capid = PCI_CAP_VENDOR_ID;
				cfg.head.type = 0;
				cfg.type.t0.vmid = (char)vpci2vm(vdev->vpci)->vm_id;
				// sharemem vmids, fixme later
				cfg.type.t0.vm_low = 0x0;
				cfg.type.t0.vm_high = 0x0;

				//skip the strings hv:/
				strncpy_s((char *)cfg.head.name, KVS_REGION_NAME_LEN, vdev->pci_dev_config->shm_region_name + 4, KVS_REGION_NAME_LEN);
				vpci_add_capability(vdev, (uint8_t *)&cfg, sizeof(cfg));
					if ((sts & PCIM_STATUS_CAPPRESENT) == 0U) {
						pci_vdev_write_vcfg(vdev, PCIR_CAP_PTR, 1U, capoff);
						pci_vdev_write_vcfg(vdev, PCIR_STATUS, 2U, sts|PCIM_STATUS_CAPPRESENT);
					} else {
						pci_vdev_write_vcfg(vdev, vdev->prev_capoff + 1U, 1U, capoff);
					}
					(void)memcpy_s((void *)&vdev->cfgdata.data_8[capoff], caplen, (void *)capdata, caplen);
					pci_vdev_write_vcfg(vdev, capoff + 1U, 1U, 0U);
					vdev->prev_capoff = capoff;
					vdev->free_capoff = capoff + reallen;
					ret = capoff;
			}
			add_vmsix_capability(vdev, MAX_IVSHMEM_MSIX_TBL_ENTRY_NUM, IVSHMEM_MSIX_BAR);
			vdev->nr_bars = 4U;
			init_ivshmem_bar(vdev, IVSHMEM_MMIO_BAR);
			init_ivshmem_bar(vdev, IVSHMEM_MSIX_BAR);
			init_ivshmem_bar(vdev, IVSHMEM_SHM_BAR);
			ivshmem_server_bind_peer(vdev);
			vdev->user = vdev;
	return vdev;



//TODO - pci_dev.c

#define IVSHMEM_GVM_CONFIG(index, fun) \
	{ \
		.emu_type = PCI_DEV_TYPE_HVEMUL, \
		.vdev_ops = &vpci_ivshmem_ops, \
		.vbdf.value = UNASSIGNED_VBDF, \
		.funcs = fun, \
		.shm_region_name = IVSHMEM_SHM_REGION_##index, \
	}


//TODO - ivshmem.c

const struct pci_vdev_ops vpci_ivshmem_ops = {
	.init_vdev	= init_ivshmem_vdev,
	.deinit_vdev	= deinit_ivshmem_vdev,
	.write_vdev_cfg	= write_ivshmem_vdev_cfg,
	.read_vdev_cfg	= read_ivshmem_vdev_cfg,
};


//TODO - vdev.c

void pci_vdev_write_vcfg(struct pci_vdev *vdev, uint32_t offset, uint32_t bytes, uint32_t val)
			/* 设备                 寄存器偏移         写入长度         写入值 */
{
	switch (bytes) {
	case 1U:
		vdev->cfgdata.data_8[offset] = (uint8_t)val;
		break;
	case 2U:
		vdev->cfgdata.data_16[offset >> 1U] = (uint16_t)val;
		break;
	default:
		vdev->cfgdata.data_32[offset >> 2U] = val;
		break;
	}
}


//TODO - ivshmem.c

init_ivshmem_bar(struct pci_vdev *vdev, uint32_t bar_idx)
	struct pci_vbar *vbar;
	uint64_t addr, mask, size = 0UL;
	addr = dev_config->vbar_base[bar_idx];
	vbar = &vdev->vbars[bar_idx];
	vbar->bar_type.bits = addr;
	if (bar_idx == IVSHMEM_SHM_BAR) {
		struct ivshmem_shm_region *region = find_shm_region(dev_config->shm_region_name);
		if (region != NULL) {
			size = region->size;
			vbar->base_hpa = region->hpa;
		}
	} else if (bar_idx == IVSHMEM_MSIX_BAR) {
		size = VMSIX_ENTRY_TABLE_PBA_BAR_SIZE;
	} else if (bar_idx == IVSHMEM_MMIO_BAR) {
		size = IVSHMEM_MMIO_BAR_SIZE;
	}


//TODO - hv-land && dm-land

https://projectacrn.github.io/2.7/developer-guides/hld/ivshmem-hld.html#ivshmem-hld


//TODO - msi inject init

create_vm(uint16_t vm_id, uint64_t pcpu_bitmap, struct cpvt_vm_config *vm_config, struct cpvt_vm **rtn_vm)
	status = init_vpci(vm);
	register_mmio_emulation_handler(vm, vpci_mmio_cfg_access, vm->vpci.pci_mmcfg.address, vm->vpci.pci_mmcfg.address + get_pci_mmcfg_size(&vm->vpci.pci_mmcfg) - 1, &vm->vpci, false);

vpci_mmio_cfg_access(struct io_request *io_req, void *private_data)
	ret = vpci_write_cfg(vpci, bdf, reg_num, (uint32_t)mmio->size, (uint32_t)mmio->value);
		pci_pdev_write_cfg(bdf, offset, bytes, val);
			cpvt_pci_cfg_ops->pci_write_cfg(bdf, offset, bytes, val);


static const struct pci_cfg_ops pci_mmcfg_cfg_ops = {
	.pci_read_cfg = pci_mmcfg_read_cfg,
	.pci_write_cfg = pci_mmcfg_write_cfg,
};

pci_mmcfg_write_cfg(union pci_bdf bdf, uint32_t offset, uint32_t bytes, uint32_t val)
	switch (bytes) {
	case 1U:
		mmio_write8((uint8_t)val, hva);
		break;
	case 2U:
		mmio_write16((uint16_t)val, hva);
		break;
	default:
		mmio_write32(val, hva);
		break;
	}


//TODO - msi inject handle

write_ivshmem_vdev_cfg(struct pci_vdev *vdev, uint32_t offset, uint32_t bytes, uint32_t val)
	if (vbar_access(vdev, offset))
		ivshmem_vbar_map(struct pci_vdev *vdev, uint32_t idx)
			register_mmio_emulation_handler(vm, ivshmem_mmio_handler, vbar->base_gpa, (vbar->base_gpa + vbar->size - IVSHMEM_MMIO_BAR_REG_WIDTH), vdev, false);
				mmio_node = alloc_mmio_node(vm->vm_id);
				mmio_node->hold_lock = hold_lock;
				mmio_node->read_write = read_write;   /* ivshmem_mmio_handler */
				mmio_node->range_start= start;
				mmio_node->range_end = end;
				mmio_node->handler_private_data = handler_private_data;


ivshmem_mmio_handler(struct io_request *io_req, void *data)
	union ivshmem_doorbell doorbell;
	struct pci_vdev *vdev = (struct pci_vdev *) data;
	if (offset == IVSHMEM_DOORBELL_REG) {
		doorbell.val = mmio->value;
		ivshmem_server_notify_peer(ivs_dev, doorbell.reg.peer_id, doorbell.reg.vector_index);
			struct msix_table_entry *entry;
			entry = &(dest_ivs_dev->pcidev->msix.table_entries[vector_index]);
			avic_inject_msi(dest_vm, entry->addr, entry->data);  /* (struct cpvt_vm *vm, uint64_t addr, uint64_t msg) */
				union msi_addr_reg address;
				union msi_data_reg data;
				address.full = addr;
				data.full = (uint32_t) msg;
				irq.dest_id = address.bits.dest_field;
				irq.vector = data.bits.vector;
				irq.dest_mode = (address.bits.dest_mode ? APIC_DESTMODE_LOG : APIC_DESTMODE_PHY);
				irq.trig_mode = data.bits.trigger_mode;
				irq.delivery_mode = data.bits.delivery_mode << 8U;
				irq.level = 1U;
				irq.shorthand = APIC_DEST_NOSHORT;
				irq.msi_redir_hint = address.bits.rh;
				cpvt_irq_delivery_to_apic(vm, NULL, &irq);
					cpvt_apic_set_irq(vcpu, irq);
						__apic_accept_irq(vcpu, irq->delivery_mode, irq->vector, irq->level, irq->trig_mode);   /* lapic */
							switch(delivery_mode)
							case APIC_DELMODE_FIXED:
								bitmap32_set_lock(VEC_POS(vector), (void *)vcpu->arch.avic.backing_page + AVIC_TMR + REG_POS(vector));
								svm_deliver_interrupt(vcpu, delivery_mode, trig_mode, vector);
									bitmap32_set_lock(VEC_POS(vector), (void *)vcpu->arch.avic.backing_page + AVIC_IRR + REG_POS(vector));
									svm_complete_interrupt_delivery(vcpu, delivery_mode, trig_mode, vector);
	}



vcpu_thread(struct thread_object *obj)
	ret = svm_vmexit_handler(vcpu);
		struct vmcb *vmcb = (struct vmcb *) vcpu->arch.vmcb;
		switch (vmcb->exitcode)
		default:
			ret = svm_dispatch_table[vmcb->exitcode](vcpu);  /* 根据 exit_reason 找到handler */



handle_vm_npf(struct cpvt_vcpu *vcpu)   /* VMEXIT_NPF nested page fault */
	struct io_request *io_req = &vcpu->req;
	io_req->io_type = CPVT_IOREQ_TYPE_MMIO;
	status = emulate_io(vcpu, io_req);
	switch (io_req->io_type)
	case CPVT_IOREQ_TYPE_MMIO:
		status = hv_emulate_mmio(vcpu, io_req);
			if (mmio_rb_lookup(&(vcpu->vm->emul_mmio_rb_root), address, &mmio_handler) == 0)
					hold_lock = mmio_handler->hold_lock;
					read_write = mmio_handler->read_write;
			status = read_write(io_req, handler_private_data); /* ivshmem_mmio_handler */

