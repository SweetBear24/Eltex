#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
#include <linux/spinlock.h>

#define PROC_FILENAME "task_monitor"

struct task_info {
    pid_t pid;
    char comm[TASK_COMM_LEN];
    long state;
    struct rcu_head rcu;
};

static struct task_info *task_list = NULL;
static spinlock_t task_list_lock;
static struct proc_dir_entry *proc_entry;

/* Обновление списка задач */
static void update_task_list(void) {
    struct task_struct *task;
    struct task_info *new_list, *old_list;

    /* Выделение памяти для нового списка */
    new_list = kmalloc(sizeof(struct task_info) * 128, GFP_KERNEL);
    if (!new_list) return;

    int count = 0;
    rcu_read_lock();
    for_each_process(task) {
        if (count >= 128) break;
        new_list[count].pid = task->pid;
        get_task_comm(new_list[count].comm, task);
        new_list[count].state = task->state;
        count++;
    }
    rcu_read_unlock();

    /* Защита обновления с помощью спинлока */
    spin_lock(&task_list_lock);
    old_list = task_list;
    task_list = new_list;
    spin_unlock(&task_list_lock);

    /* Удаление старого списка после окончания всех RCU-операций */
    if (old_list)
        kfree_rcu(old_list, rcu);
}

/* Функция вывода в /proc/task_monitor */
static int task_monitor_show(struct seq_file *m, void *v) {
    struct task_info *info;
    
    rcu_read_lock();
    info = rcu_dereference(task_list);
    if (info) {
        for (int i = 0; i < 128 && info[i].pid != 0; i++)
            seq_printf(m, "PID: %d | Name: %s | State: %ld\n",
                       info[i].pid, info[i].comm, info[i].state);
    }
    rcu_read_unlock();
    return 0;
}

/* Открытие /proc/task_monitor */
static int task_monitor_open(struct inode *inode, struct file *file) {
    return single_open(file, task_monitor_show, NULL);
}

/* Определение операций для /proc */
static const struct proc_ops proc_fops = {
    .proc_open    = task_monitor_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

/* Инициализация модуля */
static int __init task_monitor_init(void) {
    spin_lock_init(&task_list_lock);
    proc_entry = proc_create(PROC_FILENAME, 0, NULL, &proc_fops);
    if (!proc_entry)
        return -ENOMEM;

    update_task_list();
    printk(KERN_INFO "task_monitor: Модуль загружен.\n");
    return 0;
}

/* Выгрузка модуля */
static void __exit task_monitor_exit(void) {
    remove_proc_entry(PROC_FILENAME, NULL);
    
    spin_lock(&task_list_lock);
    if (task_list)
        kfree_rcu(task_list, rcu);
    spin_unlock(&task_list_lock);

    printk(KERN_INFO "task_monitor: Модуль выгружен.\n");
}

module_init(task_monitor_init);
module_exit(task_monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SweetBear");
MODULE_DESCRIPTION("Мониторинг задач с RCU и спинлоками");
