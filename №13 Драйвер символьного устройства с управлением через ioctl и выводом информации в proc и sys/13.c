#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "custom_device"
#define CLASS_NAME "custom"
#define BUF_SIZE 1024
#define IOCTL_SET_STATE _IOW('c', 1, int)
#define IOCTL_GET_STATE _IOR('c', 2, int)

static int major;
static struct class *custom_class = NULL;
static struct device *custom_device = NULL;
static struct cdev custom_cdev;
static char *device_buffer;
static int device_state = 0;
static struct mutex device_mutex;
static struct proc_dir_entry *proc_file;

static int dev_open(struct inode *inodep, struct file *filep) {
    mutex_lock(&device_mutex);
    pr_info("%s: Device opened\n", DEVICE_NAME);
    mutex_unlock(&device_mutex);
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    pr_info("%s: Device closed\n", DEVICE_NAME);
    return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int ret;
    mutex_lock(&device_mutex);
    ret = simple_read_from_buffer(buffer, len, offset, device_buffer, BUF_SIZE);
    mutex_unlock(&device_mutex);
    return ret;
}

static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    int ret;
    mutex_lock(&device_mutex);
    ret = simple_write_to_buffer(device_buffer, BUF_SIZE, offset, buffer, len);
    mutex_unlock(&device_mutex);
    return ret;
}

static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    mutex_lock(&device_mutex);
    switch (cmd) {
        case IOCTL_SET_STATE:
            if (copy_from_user(&device_state, (int __user *)arg, sizeof(device_state))) {
                mutex_unlock(&device_mutex);
                return -EFAULT;
            }
            break;
        case IOCTL_GET_STATE:
            if (copy_to_user((int __user *)arg, &device_state, sizeof(device_state))) {
                mutex_unlock(&device_mutex);
                return -EFAULT;
            }
            break;
        default:
            mutex_unlock(&device_mutex);
            return -EINVAL;
    }
    mutex_unlock(&device_mutex);
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
    .unlocked_ioctl = dev_ioctl,
};

static ssize_t proc_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos) {
    char buf[100];
    int len = snprintf(buf, sizeof(buf), "Device state: %d\n", device_state);
    return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
};

static ssize_t sysfs_show(struct device *dev, struct device_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", device_state);
}

static ssize_t sysfs_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%d", &device_state);
    return count;
}

device_attr_t device_attribute = __ATTR(state, 0660, sysfs_show, sysfs_store);

static int __init custom_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) return major;

    custom_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(custom_class)) return PTR_ERR(custom_class);

    custom_device = device_create(custom_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(custom_device)) return PTR_ERR(custom_device);

    device_buffer = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!device_buffer) return -ENOMEM;
    mutex_init(&device_mutex);

    cdev_init(&custom_cdev, &fops);
    cdev_add(&custom_cdev, MKDEV(major, 0), 1);

    proc_file = proc_create(DEVICE_NAME "_info", 0666, NULL, &proc_fops);
    if (!proc_file) return -ENOMEM;

    device_create_file(custom_device, &device_attribute);
    pr_info("%s: Module loaded\n", DEVICE_NAME);
    return 0;
}

static void __exit custom_exit(void) {
    device_remove_file(custom_device, &device_attribute);
    proc_remove(proc_file);
    cdev_del(&custom_cdev);
    device_destroy(custom_class, MKDEV(major, 0));
    class_destroy(custom_class);
    unregister_chrdev(major, DEVICE_NAME);
    kfree(device_buffer);
    pr_info("%s: Module unloaded\n", DEVICE_NAME);
}

module_init(custom_init);
module_exit(custom_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SweetBear");
MODULE_DESCRIPTION("Custom character device driver with proc, sysfs, and ioctl support");
