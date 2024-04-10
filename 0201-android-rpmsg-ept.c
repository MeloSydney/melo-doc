

rpmsg_chrdev_probe(struct rpmsg_device *rpdev)
	cdev_init(&ctrldev->cdev, &rpmsg_ctrldev_fops);
	ret = cdev_device_add(&ctrldev->cdev, &ctrldev->dev);
		rc = cdev_add(cdev, dev->devt, 1);
		rc = device_add(dev);
			kobject_uevent(&dev->kobj, KOBJ_ADD); /* 通知 ueventd */


/* 處理 uevent */
DeviceHandler::HandleUevent(const Uevent& uevent)
	HandleDevice(uevent.action, devpath, block, uevent.major, uevent.minor, links);
		DeviceHandler::HandleDevice(const std::string& action,
						const std::string& devpath,
						bool block,
						int major, int minor,
						const std::vector<std::string>& links)
			MakeDevice(devpath, block, major, minor, links);
				DeviceHandler::MakeDevice(const std::string& path,
								bool block,
								int major,
								int minor,
								const std::vector<std::string>& links)
					if (mknod(path.c_str(), mode, dev) && (errno == EEXIST) && !secontext.empty())

/* mknod 系统调用 */
SYSCALL_DEFINE4(mknodat, int, dfd, const char __user *, filename, umode_t, mode, unsigned int, dev)
	do_mknodat(int dfd, const char __user *filename, umode_t mode, unsigned int dev)
		switch (mode & S_IFMT)
		case 0: case S_IFREG:
			error = vfs_create(path.dentry->d_inode,dentry,mode,true);
			error = dir->i_op->create(dir, dentry, mode, want_excl);
			break;
		case S_IFCHR: case S_IFBLK:
			error = vfs_mknod(path.dentry->d_inode,dentry,mode, new_decode_dev(dev));
				error = dir->i_op->mknod(dir, dentry, mode, dev); /* ramfs_mknod */
			break;
		done_path_create(&path, dentry);
