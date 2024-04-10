
//TODO - dmabuf demo ks

#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/types.h>
#include <asm/ioctl.h>
#include <asm/fcntl.h>
#include <linux/uaccess.h>
#include <linux/dma-fence.h>
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/sync_file.h>
#include <linux/fs.h>

#define DMA_FENCE_IN_CMD		_IOWR('f', 0, int)
#define DMA_FENCE_OUT_CMD		_IOWR('f', 1, int)
#define DMA_FENCE_SIGNAL_CMD	_IO('f', 2)

static int in_fence_fd = -1;
static int out_fence_fd = -1;

struct dma_fence *out_fence = NULL;
struct dma_fence_cb cb;

static DEFINE_SPINLOCK(fence_lock);

static void dma_fence_cb(struct dma_fence *f, struct dma_fence_cb *cb)
{
	printk("dma-fence callback !");
}

static const char *dma_fence_get_name(struct dma_fence *fence)
{
	return "dma-fence-example";
}

static const struct dma_fence_ops fence_ops = {
	.get_driver_name = dma_fence_get_name,
	.get_timeline_name = dma_fence_get_name,
};

static long fence_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct sync_file *sync_file;
	struct dma_fence *in_fence;

	switch (cmd) {
	case(DMA_FENCE_SIGNAL_CMD):

		if (out_fence) {
			printk("Signal Fence\n");
			dma_fence_signal(out_fence);
		}

		break;

	case(DMA_FENCE_IN_CMD):

		if (copy_from_user(&in_fence_fd, (void __user *)arg, sizeof(int)) != 0)
			return -EFAULT;

		printk("Get in-fence from fd = %d\n", in_fence_fd);
		in_fence = sync_file_get_fence(in_fence_fd);

		if (!in_fence)
			return -EINVAL;

		/* add a callback func */
		dma_fence_add_callback(in_fence, &cb, dma_fence_cb);

		printk("Waiting in-fence to be signaled, process is blocking ...\n");
		dma_fence_wait(in_fence, true);
		printk("in-fence signaled, process exit\n");

		dma_fence_put(in_fence);

		break;

	case(DMA_FENCE_OUT_CMD):

		if (!out_fence)
			return -EINVAL;

		sync_file = sync_file_create(out_fence);
		out_fence_fd = get_unused_fd_flags(O_CLOEXEC);
		fd_install(out_fence_fd, sync_file->file);

		if (copy_to_user((void __user *)arg, &out_fence_fd, sizeof(int)) != 0)
			return -EFAULT;

		printk("Created an out-fence fd = %d\n", out_fence_fd);

		dma_fence_put(out_fence);
		break;

	default:
		printk("bad cmd\n");
		break;
	}

	return 0;
}

static struct dma_fence *create_fence(void)
{
	struct dma_fence *fence;

	fence = kzalloc(sizeof(*fence), GFP_KERNEL);
	if (!fence)
		return NULL;

	dma_fence_init(fence, &fence_ops, &fence_lock, 0, 0);

	return fence;
}

static int fence_open(struct inode *inode, struct file *filp)
{
	/* create an new fence */
	out_fence = create_fence();
	if (!out_fence)
		return -ENOMEM;

	return 0;
}

static int fence_close(struct inode *inode, struct file *filp)
{
	dma_fence_put(out_fence);
	return 0;
}

static struct file_operations fence_fops = {
	.owner	= THIS_MODULE,
	.unlocked_ioctl = fence_ioctl,
	.open = fence_open,
	.release = fence_close,
};

static struct miscdevice mdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dma-fence",
	.fops = &fence_fops,
};

static int __init dma_fence_demo_init(void)
{
	return misc_register(&mdev);
}

static void __exit dma_fence_demo_unint(void)
{
	misc_deregister(&mdev);
}

module_init(dma_fence_demo_init);
module_exit(dma_fence_demo_unint);

MODULE_AUTHOR("Yaong");
MODULE_LICENSE("GPL v2");


//TODO - dmabuf demo us

#include "stdio.h"
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <poll.h>

#define DMA_FENCE_IN_CMD		_IOWR('f', 0, int)
#define DMA_FENCE_OUT_CMD		_IOWR('f', 1, int)
#define DMA_FENCE_SIGNAL_CMD	_IO('f', 2)

// #define BLOCKING_IN_KERNEL

int fd = -1;

static inline int sync_wait(int fd, int timeout)
{
	struct pollfd fds = {0};
	int ret;

	fds.fd = fd;
	fds.events = POLLIN;

	do {
		ret = poll(&fds, 1, timeout);
		if (ret > 0) {
			if (fds.revents & (POLLERR | POLLNVAL)) {
				errno = EINVAL;
				return -1;
			}
			return 0;
		} else if (ret == 0) {
			errno = ETIME;
			return -1;
		}
	} while (ret == -1 && (errno == EINTR || errno == EAGAIN));

	return ret;
}

static void * signal_pthread(void *arg)
{
	sleep(1);

	if (ioctl(fd, DMA_FENCE_SIGNAL_CMD) < 0) {
		perror("get out fence fd fail\n");
	}

	return NULL;
}

int main(void)
{

	int out_fence_fd;
	pthread_t tidp;

	fd = open("/dev/dma-fence", O_RDWR | O_NONBLOCK, 0);
	if (-1 == fd) {
		printf("Cannot open dma-fence dev\n");
		exit(1);
	}

	if(ioctl(fd, DMA_FENCE_OUT_CMD, &out_fence_fd) < 0) {
		perror("get out fence fd fail\n");
		close(fd);
		return -1;
	}

	printf("Get an out-fence fd = %d\n", out_fence_fd);

	if ((pthread_create(&tidp, NULL, signal_pthread, NULL)) == -1) {
		printf("create error!\n");
		close(out_fence_fd);
		close(fd);
		return -1;
	}

#ifdef BLOCKING_IN_KERNEL
	printf("Waiting out-fence to be signaled on KERNEL side ...\n");
	if(ioctl(fd, DMA_FENCE_IN_CMD, &out_fence_fd) < 0) {
		perror("get out fence fd fail\n");
		close(out_fence_fd);
		close(fd);
		return -1;
	}
#else
	printf("Waiting out-fence to be signaled on USER side ...\n");
	sync_wait(fd, -1);
#endif

	printf("out-fence is signaled\n");

	if (pthread_join(tidp, NULL)) {
		printf("thread is not exit...\n");
		return -1;
	}

	close(out_fence_fd);
	close(fd);

	return 0;
}



