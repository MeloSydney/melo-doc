


main()
	initEcarxSPServer();
		gReader = std::make_shared<DfsReader>(nRendFrame, onDfsStop, messageCallback);
		gReader->init();
	wlcontext = (WaylandContextStruct*)calloc(1, sizeof(WaylandContextStruct)); /* 分配 waylandcontext */
	bkgnd_settings = get_bkgnd_settings();
		option = getenv("IVI_CLIENT_SURFACE_ID");
		bkgnd_settings->surface_id = strtol(option, &end, 0);
		bkgnd_settings->surface_width = 1920;
		bkgnd_settings->surface_height = 720;
		bkgnd_settings->surface_stride = bkgnd_settings->surface_width * 4; /* 7680 */
	if(init_wayland_context(wlcontext))
		wlcontext->wl_display = wl_display_connect(NULL); /* 连接wayland server */
	wlcontext->wl_registry = wl_display_get_registry(wlcontext->wl_display); /* 获取wayland提供的全局变量和接口 */
	if(create_bkgnd_surface(wlcontext))
	wl_display_roundtrip(wlcontext->wl_display);
	draw_bkgnd_surface(wlcontext, NULL, 0);