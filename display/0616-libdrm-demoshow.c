
/* Single Framebuffer */
struct buffer_object {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t handle;
	uint32_t size;
	uint8_t *vaddr;
	uint32_t fb_id;
};
struct buffer_object buf;

main(int argc, char **argv)
	fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
	/* 获取 res 其中包含 connector encoder crtc */
	res = drmModeGetResources(fd);
	conn_id = res->connectors[0];
	conn = drmModeGetConnector(fd, conn_id);
	modeset_create_fb(fd, &buf);
		drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create);
		drmModeAddFB(fd, bo->width, bo->height, 24, 32, bo->pitch, bo->handle, &bo->fb_id);
		drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &map);
		/* 内存映射 清空内存 */
		bo->vaddr = mmap(0, create.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, map.offset);
		memset(bo->vaddr, 0xff, bo->size);
	crtc_id = res->crtcs[0];
	drmModeSetCrtc(fd, crtc_id, buf.fb_id, 0, 0, &conn_id, 1, &conn->modes[0]);
	modeset_destroy_fb(fd, &buf);
		drmModeRmFB(fd, bo->fb_id);
		munmap(bo->vaddr, bo->size);
		drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy);
	drmModeFreeConnector(conn);
	drmModeFreeResources(res);



/* Double Framebuffer */
struct buffer_object {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t handle;
	uint32_t size;
	uint32_t *vaddr;
	uint32_t fb_id;
};
struct buffer_object buf[2];

main(int argc, char **argv)
	fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
	/* 获取 res 其中包含 connector encoder crtc */
	res = drmModeGetResources(fd);
	conn_id = res->connectors[0];
	conn = drmModeGetConnector(fd, conn_id);
	buf[0].width = conn->modes[0].hdisplay;
	buf[0].height = conn->modes[0].vdisplay;
	buf[1].width = conn->modes[0].hdisplay;
	buf[1].height = conn->modes[0].vdisplay;
	/* 创建 双buf */
	modeset_create_fb(fd, &buf[0], 0xff0000);
	modeset_create_fb(fd, &buf[1], 0x0000ff);
		/* 内存映射 清空内存 设置颜色 */
		drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create);
		drmModeAddFB(fd, bo->width, bo->height, 24, 32, bo->pitch, bo->handle, &bo->fb_id);
		drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &map);
		bo->vaddr = mmap(0, create.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, map.offset);
		memset(bo->vaddr, 0xff, bo->size);
		for (i = 0; i < (bo->size / 4); i++)
			bo->vaddr[i] = color;
	crtc_id = res->crtcs[0];
	drmModeSetCrtc(fd, crtc_id, buf.fb_id, 0, 0, &conn_id, 1, &conn->modes[0]);
	modeset_destroy_fb(fd, &buf[1]);
	modeset_destroy_fb(fd, &buf[0]);
	drmModeFreeConnector(conn);
	drmModeFreeResources(res);


/* Page Flip */
struct buffer_object {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t handle;
	uint32_t size;
	uint32_t *vaddr;
	uint32_t fb_id;
};
struct buffer_object buf[2];

main(int argc, char **argv)
	/* 注册信号 ctrl+c 结束 */
	signal(SIGINT, sigint_handler);
	fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
	/* 获取 res 其中包含 connector encoder crtc */
	res = drmModeGetResources(fd);
	crtc_id = res->crtcs[0];
	conn_id = res->connectors[0];
	/* 1 获取 connector
	 * 2 获取 connector的物理大小 */
	conn = drmModeGetConnector(fd, conn_id);
	buf[0].width = conn->modes[0].hdisplay;
	buf[0].height = conn->modes[0].vdisplay;
	buf[1].width = conn->modes[0].hdisplay;
	buf[1].height = conn->modes[0].vdisplay;
	/* 创建 双buf */
	modeset_create_fb(fd, &buf[0], 0xff0000);
	modeset_create_fb(fd, &buf[1], 0x0000ff);
	drmModeSetCrtc(fd, crtc_id, buf.fb_id, 0, 0, &conn_id, 1, &conn->modes[0]);
	/* 1 设置 ev handler
	 * 2 调用 drmHandleEvent 循环 等待vblank事件 触发回调handler */
	drmModePageFlip(fd, crtc_id, buf[0].fb_id, DRM_MODE_PAGE_FLIP_EVENT, &crtc_id);
	ev.version = DRM_EVENT_CONTEXT_VERSION;
	ev.page_flip_handler = modeset_page_flip_handler;
	(modeset_page_flip_handler(int fd, uint32_t frame, uint32_t sec, uint32_t usec, void *data))
		drmModePageFlip(fd, crtc_id, buf[i].fb_id, DRM_MODE_PAGE_FLIP_EVENT, data);
		usleep(500000);
	while (!terminate) {
		drmHandleEvent(fd, &ev);
	}
	modeset_destroy_fb(fd, &buf[1]);
	modeset_destroy_fb(fd, &buf[0]);
	drmModeFreeConnector(conn);
	drmModeFreeResources(res);



/* plane Test */
struct buffer_object {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t handle;
	uint32_t size;
	uint32_t *vaddr;
	uint32_t fb_id;
};
struct buffer_object buf;

main(int argc, char **argv)
	fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
	/* 获取 res 其中包含 connector encoder crtc */
	res = drmModeGetResources(fd);
	crtc_id = res->crtcs[0];
	conn_id = res->connectors[0];
	/* 设置指定的cap 为指定的值
	 * DRM_CLIENT_CAP_UNIVERSAL_PLANES 会暴露所有的layer overlay primary cursor */
	drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
	/* 通过getPlaneResources 获取plane资源 */
	plane_res = drmModeGetPlaneResources(fd);
	plane_id = plane_res->planes[0];
	conn = drmModeGetConnector(fd, conn_id);
	buf.width = conn->modes[0].hdisplay;
	buf.height = conn->modes[0].vdisplay;
	/* 创建 buf */
	modeset_create_fb(fd, &buf);
	drmModeSetCrtc(fd, crtc_id, buf.fb_id, 0, 0, &conn_id, 1, &conn->modes[0]);
	/* crop the rect from framebuffer(100, 150) to crtc(50, 50) */
	drmModeSetPlane(fd, plane_id, crtc_id, buf.fb_id, 0,
			50, 50, 320, 320,
			100 << 16, 150 << 16, 320 << 16, 320 << 16);
	modeset_destroy_fb(fd, &buf);
	drmModeFreeConnector(conn);
	drmModeFreeResources(res);



/* atomic-crtc */
struct buffer_object {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t handle;
	uint32_t size;
	uint32_t *vaddr;
	uint32_t fb_id;
};
struct buffer_object buf;

main(int argc, char **argv)
	fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
	/* 获取 res 其中包含 connector encoder crtc */
	res = drmModeGetResources(fd);
	crtc_id = res->crtcs[0];
	conn_id = res->connectors[0];
	drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
	/* 获取plane */
	plane_res = drmModeGetPlaneResources(fd);
	plane_id = plane_res->planes[0];
	conn = drmModeGetConnector(fd, conn_id);
	buf.width = conn->modes[0].hdisplay;
	buf.height = conn->modes[0].vdisplay;
	/* 创建 buf */
	modeset_create_fb(fd, &buf);
	/* DRM_CLIENT_CAP_ATOMIC 会暴露所有的atomic属性给用户空间 */
	drmSetClientCap(fd, DRM_CLIENT_CAP_ATOMIC, 1);
	/* 1 获取 connector属性集合
	 * 2 获取 crtc id 属性 */
	props = drmModeObjectGetProperties(fd, conn_id,	DRM_MODE_OBJECT_CONNECTOR);
	property_crtc_id = get_property_id(fd, props, "CRTC_ID");
	drmModeFreeObjectProperties(props);
	/* 1 获取 crtc属性集合
	 * 2 获取 active mode 属性 */
	props = drmModeObjectGetProperties(fd, crtc_id, DRM_MODE_OBJECT_CRTC);
	property_active = get_property_id(fd, props, "ACTIVE");
	property_mode_id = get_property_id(fd, props, "MODE_ID");
	drmModeFreeObjectProperties(props);
	/* create blob to store current mode, and retun the blob id */
	drmModeCreatePropertyBlob(fd, &conn->modes[0], sizeof(conn->modes[0]), &blob_id);
	/* start modeseting */
	req = drmModeAtomicAlloc();
	drmModeAtomicAddProperty(req, crtc_id, property_active, 1);
	drmModeAtomicAddProperty(req, crtc_id, property_mode_id, blob_id);
	drmModeAtomicAddProperty(req, conn_id, property_crtc_id, crtc_id);
	drmModeAtomicCommit(fd, req, DRM_MODE_ATOMIC_ALLOW_MODESET, NULL);
	getchar();
	drmModeSetPlane(fd, plane_id, crtc_id, buf.fb_id, 0,
			50, 50, 320, 320,
			0, 0, 320 << 16, 320 << 16);
	drmModeAtomicFree(req);
	drmModeFreeConnector(conn);
	drmModeFreeResources(res);





/* atomic-plane */
struct buffer_object {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t handle;
	uint32_t size;
	uint32_t *vaddr;
	uint32_t fb_id;
};
struct buffer_object buf;

main(int argc, char **argv)
	fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
	/* 获取 res 其中包含 connector encoder crtc */
	res = drmModeGetResources(fd);
	crtc_id = res->crtcs[0];
	conn_id = res->connectors[0];
	drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
	/* 1 获取plane
	 * 2 获取connector
	 * 3 创建 fb */
	plane_res = drmModeGetPlaneResources(fd);
	plane_id = plane_res->planes[0];
	conn = drmModeGetConnector(fd, conn_id);
	buf.width = conn->modes[0].hdisplay;
	buf.height = conn->modes[0].vdisplay;
	modeset_create_fb(fd, &buf);
	/* DRM_CLIENT_CAP_ATOMIC 会暴露所有的atomic属性给用户空间 */
	drmSetClientCap(fd, DRM_CLIENT_CAP_ATOMIC, 1);
	/* 1 获取 connector属性集合
	 * 2 获取 crtc id 属性 */
	props = drmModeObjectGetProperties(fd, conn_id,	DRM_MODE_OBJECT_CONNECTOR);
	property_crtc_id = get_property_id(fd, props, "CRTC_ID");
	drmModeFreeObjectProperties(props);
	/* 1 获取 crtc属性集合
	 * 2 获取 active mode 属性 */
	props = drmModeObjectGetProperties(fd, crtc_id, DRM_MODE_OBJECT_CRTC);
	property_active = get_property_id(fd, props, "ACTIVE");
	property_mode_id = get_property_id(fd, props, "MODE_ID");
	drmModeFreeObjectProperties(props);
	/* create blob to store current mode, and retun the blob id */
	drmModeCreatePropertyBlob(fd, &conn->modes[0], sizeof(conn->modes[0]), &blob_id);
	/* start modeseting */
	req = drmModeAtomicAlloc();
	drmModeAtomicAddProperty(req, crtc_id, property_active, 1);
	drmModeAtomicAddProperty(req, crtc_id, property_mode_id, blob_id);
	drmModeAtomicAddProperty(req, conn_id, property_crtc_id, crtc_id);
	drmModeAtomicCommit(fd, req, DRM_MODE_ATOMIC_ALLOW_MODESET, NULL);
	drmModeAtomicFree(req);
	getchar();
	/* get plane properties */
	props = drmModeObjectGetProperties(fd, plane_id, DRM_MODE_OBJECT_PLANE);
	property_crtc_id = get_property_id(fd, props, "CRTC_ID");
	property_fb_id = get_property_id(fd, props, "FB_ID");
	property_crtc_x = get_property_id(fd, props, "CRTC_X");
	property_crtc_y = get_property_id(fd, props, "CRTC_Y");
	property_crtc_w = get_property_id(fd, props, "CRTC_W");
	property_crtc_h = get_property_id(fd, props, "CRTC_H");
	property_src_x = get_property_id(fd, props, "SRC_X");
	property_src_y = get_property_id(fd, props, "SRC_Y");
	property_src_w = get_property_id(fd, props, "SRC_W");
	property_src_h = get_property_id(fd, props, "SRC_H");
	drmModeFreeObjectProperties(props);
	/* atomic plane update
	 * same with:
	 * drmModeSetPlane(fd, plane_id, crtc_id, buf.fb_id, 0,
			50, 50, 320, 320,
			0, 0, 320 << 16, 320 << 16); */
	req = drmModeAtomicAlloc();
	drmModeAtomicAddProperty(req, plane_id, property_crtc_id, crtc_id);
	drmModeAtomicAddProperty(req, plane_id, property_fb_id, buf.fb_id);
	drmModeAtomicAddProperty(req, plane_id, property_crtc_x, 50);
	drmModeAtomicAddProperty(req, plane_id, property_crtc_y, 50);
	drmModeAtomicAddProperty(req, plane_id, property_crtc_w, 320);
	drmModeAtomicAddProperty(req, plane_id, property_crtc_h, 320);
	drmModeAtomicAddProperty(req, plane_id, property_src_x, 0);
	drmModeAtomicAddProperty(req, plane_id, property_src_y, 0);
	drmModeAtomicAddProperty(req, plane_id, property_src_w, 320 << 16);
	drmModeAtomicAddProperty(req, plane_id, property_src_h, 320 << 16);
	drmModeAtomicCommit(fd, req, 0, NULL);
	drmModeAtomicFree(req);
