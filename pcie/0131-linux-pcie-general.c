


//TODO - kernel pcie API

// 获取bar 0的信息 //
pio_start = pci_resource_start (pdev, 0);
pio_end = pci_resource_end (pdev, 0);
pio_flags = pci_resource_flags (pdev, 0);
pio_len = pci_resource_len (pdev, 0);
ioaddr = ioport_map(pio_start, pio_len);   ioport映射

// 获取bar 1的信息 //
mmio_start = pci_resource_start (pdev, 1);
mmio_end = pci_resource_end (pdev, 1);
mmio_flags = pci_resource_flags (pdev, 1);
mmio_len = pci_resource_len (pdev, 1);
