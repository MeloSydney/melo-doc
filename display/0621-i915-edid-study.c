

/* i915 ioctls */
i915_ioctls

/* i915 加载固件 */
i915_init(void)
	return pci_register_driver(&i915_pci_driver);
	(i915_pci_driver.probe = i915_pci_probe,)
	i915_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
		i915_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
			i915_driver_modeset_probe(struct drm_device *dev)
				intel_csr_ucode_init(struct drm_i915_private *dev_priv)
					INIT_WORK(&dev_priv->csr.work, csr_load_work_fn);
					schedule_work(&dev_priv->csr.work);
					(csr_load_work_fn(struct work_struct *work))
						(i915 0000:00:02.0: [drm] Finished loading DMC firmware i915/kbl_dmc_ver1_04.bin (v1.4))


/* i915 加载 额外固件 */
DRM_IOCTL_DEF(DRM_IOCTL_MODE_GETCONNECTOR, drm_mode_getconnector, 0),
	drm_mode_getconnector(struct drm_device *dev, void *data, struct drm_file *file_priv)
		connector->funcs->fill_modes(connector, dev->mode_config.max_width, dev->mode_config.max_height);
		drm_helper_probe_single_connector_modes(struct drm_connector *connector, uint32_t maxX, uint32_t maxY)
			drm_add_override_edid_modes(struct drm_connector *connector)
				drm_get_override_edid(struct drm_connector *connector)
					drm_load_edid_firmware(struct drm_connector *connector)
						edid_load(struct drm_connector *connector, const char *name, const char *connector_name)
							([    2.857098] [drm] Got external EDID base block and 0 extensions from "edid/1280x800R.bin" for connector "DP-1")
				drm_connector_update_edid_property(connector, override);
					drm_add_display_info(connector, edid);
						drm_reset_display_info(connector);
				num_modes = drm_add_edid_modes(connector, override);


