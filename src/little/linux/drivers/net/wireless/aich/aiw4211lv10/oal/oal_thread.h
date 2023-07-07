/*
 * Copyright (c) 2020-2022 Chengdu Aich Technology Co.,Ltd.All rights reserved.
 * Description: oal_thread
 * Author: CompanyName
 * Create: 2021-08-04
 */

#ifndef __OAL_LINUX_THREAD_H__
#define __OAL_LINUX_THREAD_H__

/* 头文件包含 */
#include <asm/atomic.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <asm/param.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include "oal_util.h"
#include "oam_ext_if.h"

/* new fix: */
#if (_PRE_KERVER != _PRE_KERVER_4D9)
#include <linux/sched/types.h>
#include <linux/sched/signal.h>
#endif

#if (_PRE_KERVER == _PRE_KERVER_5D10)
#include <uapi/linux/sched/types.h>
#define OAL_SCHED_NORMAL      0
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 宏定义 */
#define oal_kthread_should_stop   kthread_should_stop
#define oal_schedule              schedule

#define OAL_CURRENT         current

#define OAL_SCHED_FIFO      1
#define OAL_SCHED_RR        2
#define NOT_BIND_CPU        (-1)

typedef  struct task_struct oal_kthread_stru;
typedef  int (*oal_thread_func)(void *);

/* 结构体定义 */
typedef struct _kthread_param_ {
    td_u32         ul_stacksize;
    td_s32           l_prio;
    td_s32           l_policy;
    td_s32           l_cpuid;
    td_s32           l_nice;
} oal_kthread_param_stru;

/* inline 函数定义 */
static inline oal_kthread_stru *oal_kthread_create(char *pc_thread_name, oal_thread_func pf_threadfn,
    void *p_data, const oal_kthread_param_stru  *thread_param)
{
    oal_kthread_stru *kthread = NULL;
    struct sched_param       st_sched_param;
    oal_bug_on(!thread_param);

    kthread = kthread_create(pf_threadfn, p_data, "%s", pc_thread_name);
    if (IS_ERR_OR_NULL(kthread)) {
        oam_error_log1("failed to run theread:%s", pc_thread_name);
        return NULL;
    }

    st_sched_param.sched_priority = thread_param->l_prio;
/* new fix: */
#if (_PRE_KERVER == _PRE_KERVER_5D10)
    if (thread_param->l_policy == OAL_SCHED_NORMAL)
        sched_set_normal(kthread, thread_param->l_prio);
    else
        sched_set_fifo(kthread);
#else
    td_s32 uw_ret;
    uw_ret = sched_setscheduler(kthread, thread_param->l_policy, &st_sched_param);
    if (oal_unlikely(uw_ret)) {
        oam_error_log3("%s sched_setscheduler failed! ret =%d, prio=%d", \
            pc_thread_name, uw_ret, thread_param->l_prio);
    }
#endif
    if (thread_param->l_cpuid >= 0) {   /* cpuid为负数时无效 */
        kthread_bind(kthread, thread_param->l_cpuid);
    } else {
        oam_error_log0("did not bind cpu");
    }
    wake_up_process(kthread);
    return kthread;
}

static inline td_u32 oal_set_thread_property(oal_kthread_stru *pst_thread, oal_kthread_param_stru *thread_param)
{
    struct sched_param       st_sched_param;
    oal_bug_on(!pst_thread);
    oal_bug_on(!thread_param);

    st_sched_param.sched_priority = thread_param->l_prio;
    oam_error_log1("set thread scheduler policy %d", thread_param->l_policy);
/* new fix: */
#if (_PRE_KERVER == _PRE_KERVER_5D10)
    if (thread_param->l_policy == OAL_SCHED_NORMAL)
        sched_set_normal(pst_thread, thread_param->l_prio);
    else
        sched_set_fifo(pst_thread);
#else
    if (sched_setscheduler(pst_thread, thread_param->l_policy, &st_sched_param)) {
        oam_error_log1("[Error]set scheduler failed! %d", thread_param->l_policy);
        return -OAL_EFAIL;
    }
#endif
    if (thread_param->l_policy != SCHED_FIFO && thread_param->l_policy != SCHED_RR) {
        oam_error_log1("set thread scheduler nice %d", thread_param->l_nice);
        set_user_nice(pst_thread, thread_param->l_nice);
    }
    return EXT_SUCCESS;
}

static inline td_void oal_kthread_stop(oal_kthread_stru *pst_thread)
{
    oal_bug_on(!pst_thread);

    send_sig(SIGTERM, pst_thread, 1);
    kthread_stop(pst_thread);
}

static inline char* oal_get_current_task_name(td_void)
{
    return current->comm;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
