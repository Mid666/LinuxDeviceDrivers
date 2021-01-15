/* Midhun Nair  
 *
 * Create a new proc entry under /proc/proc_test/test. When you access
 * this file you must get the pid, tgid, cmd and kernel­stack address of every
 * process in the system.
 *
 * cat /proc/process/all_proc_info  --> should display pid,tgid,cmd,kernel stack address of every process.
 * 
 * Date: 30/10/2020
 *
 */

#include<linux/proc_fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/seq_file.h>
#include<linux/sched.h>
#include<linux/sched/signal.h>
#include<linux/sched/task.h>

/*struct declarations*/
static struct proc_dir_entry *entry1=NULL;	
static struct proc_dir_entry *entry=NULL;
struct task_struct *proc_desc;
struct task_struct *next_proc_task;
struct task_struct *current_proc_task;
struct task_struct *task;
struct thread_info *curr_info;
struct mm_struct *proc_mm;

/*accessing init pd*/
struct task_struct *init_proc_task = &init_task;

/*****************seq_operations******************/
static void *all_proc_info_start(struct seq_file *seq, loff_t *pos)
{
	loff_t off=0;
	pr_alert("in seq_start\n")
		
	for_each_process(task)
	{
		if(*pos == off++)
			return task;
	}
	return NULL;
}

static int all_proc_info_show(struct seq_file *file, void *v)
{
	curr_info = current_thread_info();
	proc_desc = ((struct task_struct *)v);
	proc_mm = (struct mm_struct *)proc_desc->mm;

	seq_printf(file, "|[cmd:%s]", proc_desc->comm);
	seq_printf(file, "--[pid:%d]", proc_desc->pid);
	seq_printf(file, "--[tgid:%d]", proc_desc->tgid);
	seq_printf(file, "--[kstack:0x%px]\n\n", proc_desc->stack);
	//seq_printf(file, "--[Kstack:0x%lx]\n\n", proc_desc->stack); //yields same result as %px, proc_desc->stack
	//seq_printf(file, "--[current_thread_info:0x%px\n\n]",curr_info); //Address of Pd of currently scheduled process
	/*for user space stack address*/
	//seq_printf(file, "--[user stack: 0x%lx]\n\n", proc_mm->start_stack); 
	//should print start_stack field of mm_struct but KILLED	

	return 0;
}

static void *all_proc_info_next(struct seq_file *seq, void *v, loff_t *pos)
{	
	current_proc_task =((struct task_struct *)v);
	next_proc_task = next_task(current_proc_task);

	++*pos;

	if(next_proc_task != init_proc_task)     //since pds are maintained in circular list
		return next_proc_task;

	pr_alert("reached final list of processes\n");
	return NULL;
}

static void all_proc_info_stop(struct seq_file *seq, void *v)
{
	//Nothing to free!
	pr_alert("in seq_stop\n");
}

static struct seq_operations all_proc_info_open = 
{
	.start  = all_proc_info_start,
	.next   = all_proc_info_next,
	.stop   = all_proc_info_stop,
	.show   = all_proc_info_show,
};

static int proc_info_open(struct inode *inode, struct  file *file)
{
	return seq_open(file,&all_proc_info_open);
}

static const struct file_operations proc_file_ops = 
{
	.owner   = THIS_MODULE,
	.open    = proc_info_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
};

static int __init all_proc_info_init(void)
{
	pr_alert("in entry point\n");
	entry1 = proc_mkdir("process", NULL);		//subdirectory under /proc
	entry = proc_create("all_proc_info", 0444, entry1, &proc_file_ops);		//created file obj
	if(entry == NULL)
		return -ENOMEM;

	return 0;
}

static void all_proc_info_exit(void)
{
	pr_alert("goodbye\n");
	remove_proc_entry("all_proc_info",entry1);
	remove_proc_entry("process", NULL);
}

module_init(all_proc_info_init);
module_exit(all_proc_info_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Midhun Nair");

