
/* gbm from louCQ */
main()
	struct gbm_import_fd_modifier_data data = {0};
	struct gbm_bo *bo = NULL;
	drmModeFB2Ptr fb2;
	void *ptr = NULL;

	int fd = open("/dev/dri/card0", O_RDWR);
	struct gbm_device *gbd = gbm_create_device(fd);
	fb = get_fb(fd);
		/* 获取资源 */
		drmSetClientCap(drm_fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
		res = drmModeGetResources(drm_fd);
		/* 1 获取crtc
		 * 2 获取FB */
		for(i = res->count_crtcs;i >= 0;--i)
		crtc = drmModeGetCrtc(drm_fd, res->crtcs[i]);
		fb = drmModeGetFB2(drm_fd, crtc->buffer_id);
	data.width = fb->width;
	data.height = fb->height;
	data.stride = fb->pitch;
	data.format = GBM_FORMAT_XRGB8888;
	/* 导出dmafd  data.fd
	 * 1 fd  打开的drm设备
	 * 2 handle dmabuf的handle
	 * 3 flag
	 * 4 dmabuf描述符 */
	drmPrimeHandleToFD(fd, fb->handle, O_RDONLY, &data.fd);
	drmPrimeHandleToFD(fd, fb->handle, O_RDONLY, &data.fd);

	bo = gbm_bo_import(gbd, GBM_BO_IMPORT_FD_MODIFIER, &data, 0);
	ptr = gbm_bo_map(bo, 0, 0, fb2->width, fb2->height, GBM_BO_TRANSFER_READ_WRITE, &stride, &map_data);
	file = fopen(fileName, "w+");
	fwrite(ptr, 1, stride * fb->height, file);
	fclose(file);
	gbm_bo_unmap(bo, map_data);


/* gbm-modifier from louCQ */
main()
	struct gbm_import_fd_modifier_data data = {0};
	drmModeFB2Ptr fb2;

	int fd = open("/dev/dri/card0", O_RDWR);
	fb2 = get_fb(fd);
	data.width = fb2->width;
	data.height = fb2->height;
	data.format = fb2->pixel_format;
	data.num_fds = count;
	data.modifier = fb2->modifier;
	memcpy(data.strides, fb2->pitches, count * sizeof(uint32_t));
	memcpy(data.offsets, fb2->offsets, count * sizeof(uint32_t));
	for (i = 0; i < count; i++)
		drmPrimeHandleToFD(fd, fb2->handles[0], O_RDONLY, &data.fds[i]);
	struct gbm_device *gbd = gbm_create_device(fd);
	bo = gbm_bo_import(gbd, GBM_BO_IMPORT_FD_MODIFIER, &data, 0);
	ptr = gbm_bo_map(bo, 0, 0, fb2->width, fb2->height, GBM_BO_TRANSFER_READ_WRITE, &stride, &map_data);
	file = fopen(fileName, "w+");
	fwrite(ptr, 1, stride * fb2->height, file);
	fclose(file);
	gbm_bo_unmap(bo, map_data);

XDG_RUNTIME_DIR=/run/user/1000


/* modetest 如何调整分辨率 分析 */
main(int argc, char **argv)
	dev.fd = drmOpen(module, device);

	dev.resources = get_resources(&dev);
		drmSetClientCap(dev->fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
		res->res = drmModeGetResources(dev->fd);
		get_resource(res, res, crtc, Crtc);
			drmModeGetCrtc();
		get_resource(res, res, encoder, Encoder);
			drmModeGetEncoder();
		get_resource(res, res, connector, Connector);
			drmModeGeConnector();
		get_resource(res, res, fb, FB);
			drmModeGetFB();
		get_properties(res, res, crtc, CRTC);
			drmModeObjectGetProperties(dev->fd, obj->type->crtc_id, DRM_MODE_OBJECT_CRTC);
			drmModeGetProperty(dev->fd, obj->props->props[j]);
		get_properties(res, res, connector, CONNECTOR);
		res->plane_res = drmModeGetPlaneResources(dev->fd);
		get_resource(res, plane_res, plane, Plane);
		get_properties(res, plane_res, plane, PLANE);

	dump_resource(&dev, encoders);
	dump_resource(&dev, connectors);
	dump_resource(&dev, crtcs);
	dump_resource(&dev, planes);
	dump_resource(&dev, framebuffers);
	ret = drmGetCap(dev.fd, DRM_CAP_DUMB_BUFFER, &cap);
	/* 静态图片显示 */
	set_mode(&dev, pipe_args, count);
		bo = bo_create(dev->fd, pipes[0].fourcc, dev->mode.width, dev->mode.height, handles, pitches, offsets, PATTERN_SMPTE);
		ret = drmModeAddFB2(dev->fd, dev->mode.width, dev->mode.height, pipes[0].fourcc, handles, pitches, offsets, &fb_id, 0);
		ret = drmModeSetCrtc(dev->fd, pipe->crtc->crtc->crtc_id, fb_id, x, 0, pipe->con_ids, pipe->num_cons, pipe->mode);
		drmModeDirtyFB(dev->fd, fb_id, NULL, 0);

	/* 图片闪烁 */
	test_page_flip(&dev, pipe_args, count);


