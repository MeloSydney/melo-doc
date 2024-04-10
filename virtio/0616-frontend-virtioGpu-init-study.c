/* 前端 virtgpu 初始化 */
virtio_gpu_probe(struct virtio_device *vdev)
	dev = drm_dev_alloc(&driver, &vdev->dev);
		ret = drm_dev_init(dev, driver, parent);
	vdev->priv = dev;
	ret = virtio_gpu_init(dev);
		virtio_gpu_modeset_init(vgdev);
			vgdev_output_init(vgdev, i);
				/* 注册component:
				 * 1 plane
				 * 2 crtc
				 * 3 connector
				 * 4 encoder */
				primary = virtio_gpu_plane_init(vgdev, DRM_PLANE_TYPE_PRIMARY, index);
				cursor = virtio_gpu_plane_init(vgdev, DRM_PLANE_TYPE_CURSOR, index);
				drm_crtc_init_with_planes(dev, crtc, primary, cursor, &virtio_gpu_crtc_funcs, NULL);
				drm_crtc_helper_add(crtc, &virtio_gpu_crtc_helper_funcs);
				drm_connector_init(dev, connector, &virtio_gpu_connector_funcs, DRM_MODE_CONNECTOR_VIRTUAL);
				drm_connector_helper_add(connector, &virtio_gpu_conn_helper_funcs);
				drm_encoder_init(dev, encoder, &virtio_gpu_enc_funcs, DRM_MODE_ENCODER_VIRTUAL, NULL);
				drm_encoder_helper_add(encoder, &virtio_gpu_enc_helper_funcs);
				drm_connector_attach_encoder(connector, encoder);
				drm_connector_register(connector);
	ret = drm_dev_register(dev, 0);
	drm_fbdev_generic_setup(vdev->priv, 32);



/* fb 截图 gbGrab */
main(int argc, char **argv)
	get_framebufferdata(device, &fb_varinfo, &fb_fixedinfo, verbose);
		fd=open(device, O_RDONLY)
		/* 获取 varinfo 和 fixedinfo */
		if (ioctl(fd, FBIOGET_VSCREENINFO, fb_varinfo_p) != 0)
		if (ioctl(fd, FBIOGET_FSCREENINFO, fb_fixedinfo) != 0)
	read_framebuffer(infile, buf_size, buf_p, skip_bytes);
		read(fd, buf_p, bytes)
	convert_and_write(buf_p, outfile, (unsigned int) width, (unsigned int) height, line_length, bitdepth, interlace, png_compression);
		switch(bits)
		case 32:
			convert8888to32(width, height, line_length, inbuffer, outbuffer);
		write_PNG(outbuffer, filename, width, height, interlace, compression);
			outfile = fopen(filename, "wb");
			/* png 处理 ... */
			...
			(void) fclose(outfile);