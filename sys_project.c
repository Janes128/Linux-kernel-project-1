/*
 *Introuduction:
 *      This system call will take a pid as parameter, and get the process virtual address,
 *and physical address which match the pid.
 */
#ifndef __LINUX_PROJECT
#define __LINUX_PROJECT
 
#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm-i386/page.h>
#include <asm-i386/pgtable.h>
 
 
asmlinkage long sys_project(unsigned long pid1, unsigned long pid2){
        struct vm_area_struct *vm;      /* loop counter to loop through all vm section */
        struct page *page;
        unsigned long vm_address;       /* the virtual address */
        struct task_struct *p;          /* the process descriptor */
        unsigned long pfn;
        unsigned short flag = 0;       
        /*the total page frame is the current system. Do not modified this value. */   
        extern unsigned long max_low_pfn;      
        printk("total page frame : %lx \n", max_low_pfn);
        printk("host memory size is about\t%lu MB\n", pages_to_mb(max_low_pfn));
        /*
         * loop through the whole process list.
         * I think it is the same as the find_task_by_pid().
         */    
        for_each_process(p){           
                if (p->pid == pid1){
                        flag =1;       
                        break;
                }              
        }      
        /* it's time to get the virtual address and physical address of the process */
        if (flag){
                /* check if NULL pointer */
                if(p->mm == NULL || p->mm->pgd == NULL || p->mm->mmap == NULL){
                        printk("ptr is NULL\n");
                        goto out;
                }
                printk("vm section for process1 pid:%u:\n",pid1);
                /* loop through vm interval */
                for ( vm = p->mm->mmap; vm != NULL; vm = vm->vm_next){
                        /* print vm section start and end */
                        printk("from 0x%08lx~0x%08lx\n",vm->vm_start,vm->vm_end);
                        printk("page frame of the interval : \n");
                        for (   vm_address = vm->vm_start;
                                vm_address < vm->vm_end;
                                vm_address += 0x1000)
                        {
                                page = follow_page(vm, vm_address, 0);
                                if ( page == NULL) continue;
                                pfn = page_to_pfn(page);
                                printk("0x%x ",pfn);
                        }
                        printk("\n");
                }              
        }      
/*if some error happened just jumps to here*/  
out:   
        return 0;
 
}
 
#endif
