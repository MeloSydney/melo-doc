
// TODO - [ext2 格式结构]

* Disk
	* MBR
	* Partation 1
	* Partation 2
	* Partation 3
	* Partation 4

* Partation
	BootBlock
	Ext2 fs


* Ext2 fs
	Block Group 0
	Block Group 1
	Block Group 2


* Block Group
	Super Block
	GDT
	Block map
	inode map
	inode Table
	Data Blocks


* inode
	inode number
	file type
	permission
	link count
	uid
	gid
	size
	pointer