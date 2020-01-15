/*****************************************************************
** �ļ���: user-timer.c
** Copyright (c) 2020 zucker-chen
 
** ������: 
** ��  ��: 
** ��  ��: ��ʱ��
** ��  ��: V1.0

** �޸���:
** ��  ��:
** �޸�����:
** ��  ��: 
******************************************************************/

/*****************************************************************
* ����ͷ�ļ�
******************************************************************/
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include "utimer.h"



/*****************************************************************
* ��ʱ�� ---------------------------------------------------------
******************************************************************/

static void utimer_normal_cb(unsigned long data)
{
    usr_timer_t *t = (usr_timer_t *)data;
    if (t != NULL && t->function != NULL) {
        t->function(NULL);
    }
}

static enum hrtimer_restart utimer_high_cb(struct hrtimer *ht)
{
    usr_timer_t *t = NULL;
    s64 last_expires;
    
    t = container_of(ht, usr_timer_t, timer);
    if (t != NULL && t->function != NULL) {
        last_expires = hrtimer_get_expires_tv64(ht);
        t->function(NULL);
        if (last_expires != hrtimer_get_expires_tv64(ht)) {
            return HRTIMER_RESTART;
        }
    }
    
    return HRTIMER_NORESTART;
}


int utimer_start(usr_timer_t *timer, unsigned long interval)
{
    if(timer == NULL || timer->function == NULL || interval == 0){
        printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    if (timer->type == USER_TIMER_NORMAL) {
        struct timer_list *t = (struct timer_list *)&timer->timer;
        t->expires = jiffies + usecs_to_jiffies(interval) - 1;
        add_timer(t);
        //mod_timer(t, jiffies + usecs_to_jiffies(interval) - 1);
    } else {
        ktime_t kt;
        struct hrtimer *t = (struct hrtimer *)&timer->timer;
        kt = ktime_set(interval/1000000, interval%1000000);
        hrtimer_start(t, kt, HRTIMER_MODE_REL);
    }

    return 0;
}
EXPORT_SYMBOL(utimer_start);

int utimer_restart(usr_timer_t *timer, unsigned long interval)
{
    if(timer == NULL || timer->function == NULL || interval == 0){
        printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    if (timer->type == USER_TIMER_NORMAL) {
        struct timer_list *t = (struct timer_list *)&timer->timer;
        mod_timer(t, jiffies + usecs_to_jiffies(interval) - 1);
    } else {
        ktime_t kt;
        struct hrtimer *t = (struct hrtimer *)&timer->timer;
        kt = ktime_set(interval/1000000, interval%1000000);
        hrtimer_forward_now(t, kt);
    }

    return 0;
}
EXPORT_SYMBOL(utimer_restart);

int utimer_init(usr_timer_t *timer)
{
    if (timer == NULL) {
        printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    if (timer->type == USER_TIMER_NORMAL) {
        struct timer_list *t = (struct timer_list *)&timer->timer;
        init_timer(t);
        t->function = utimer_normal_cb;
        t->data = (unsigned long)timer;
    } else {
        struct hrtimer *t = (struct hrtimer *)&timer->timer;
        hrtimer_init(t, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        t->function = utimer_high_cb;
    }
    
    return 0;
}
EXPORT_SYMBOL(utimer_init);

int utimer_destory(usr_timer_t *timer)
{
    if (timer == NULL) {
        printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    
    if (timer->type == USER_TIMER_NORMAL) {
        struct timer_list *t = (struct timer_list *)&timer->timer;
        del_timer(t);
    } else {
        struct hrtimer *t = (struct hrtimer *)&timer->timer;
        hrtimer_cancel(t);
    }
        
    return 0;
}
EXPORT_SYMBOL(utimer_destory);


static usr_timer_t utimer;
static int utimer_demo_handle(void *param)
{

    printk("%s - - - \n", __FUNCTION__);
    //utimer_restart(&utimer, 1000000);
    
    return 0;
}

static int utimer_demo_init(void)
{
    utimer.type = USER_TIMER_HIGH;
    utimer.function = utimer_demo_handle;
    utimer_init(&utimer);
    
    utimer_start(&utimer, 1000000);

    return 0;
}

static int utimer_demo_deinit(void)
{
    return utimer_destory(&utimer);
}


static int __init utimer_mod_init(void)
{
    printk("%s - hello - \n", __FUNCTION__);
    utimer_demo_init();
    
    return 0;
}

static void __exit utimer_mod_exit(void)
{
    utimer_demo_deinit();
    printk("%s - bye - \n", __FUNCTION__);
}


MODULE_AUTHOR("zucker-chen <timeontheway@163.com>");
MODULE_DESCRIPTION("user timer");
MODULE_LICENSE("GPL");

module_init(utimer_mod_init);
module_exit(utimer_mod_exit);




