// #include "types.h"
// #include "stat.h"
// #include "user.h"
// // #include "defs.h"
// #include "param.h"
// #include "x86.h"
// #include "memlayout.h"
// #include "mmu.h"
// #include "proc.h"
// #include "spinlock.h"
// void initlock(struct spinlock *lk, char *name)
// {
//     lk->name = name;
//     lk->locked = 0;
//     lk->cpu = 0;
// }
// struct thread_spinlock
// {
//     uint locked; // Is the lock held?

//     // For debugging:
//     struct cpu *cpu; // The cpu holding the lock.
//     uint pcs[10];    // The call stack (an array of program counters)
//                      // that locked the lock.
// };

// void thread_spin_init(struct thread_spinlock *lk)
// {
//     lk->locked = 0;
//     lk->cpu = 0;
// }
// void thread_spin_lock(struct thread_spinlock *lk)
// {
//     pushcli(); // disable interrupts to avoid deadlock.
//     if (holding(lk))
//         panic("acquire");

//     // The xchg is atomic.
//     while (xchg(&lk->locked, 1) != 0)
//         ;

//     // Tell the C compiler and the processor to not move loads or stores
//     // past this point, to ensure that the critical section's memory
//     // references happen after the lock is acquired.
//     __sync_synchronize();

//     // Record info about lock acquisition for debugging.
//     lk->cpu = mycpu();
//     getcallerpcs(&lk, lk->pcs);
// }
// // Acquire the lock.
// // Loops (spins) until the lock is acquired.
// // Holding a lock for a long time may cause
// // other CPUs to waste time spinning to acquire it.
// void acquire(struct spinlock *lk)
// {
//     pushcli(); // disable interrupts to avoid deadlock.
//     if (holding(lk))
//         panic("acquire");

//     // The xchg is atomic.
//     while (xchg(&lk->locked, 1) != 0)
//         ;

//     // Tell the C compiler and the processor to not move loads or stores
//     // past this point, to ensure that the critical section's memory
//     // references happen after the lock is acquired.
//     __sync_synchronize();

//     // Record info about lock acquisition for debugging.
//     lk->cpu = mycpu();
//     getcallerpcs(&lk, lk->pcs);
// }

// // Release the lock.
// void release(struct spinlock *lk)
// {
//     if (!holding(lk))
//         panic("release");

//     lk->pcs[0] = 0;
//     lk->cpu = 0;

//     // Tell the C compiler and the processor to not move loads or stores
//     // past this point, to ensure that all the stores in the critical
//     // section are visible to other cores before the lock is released.
//     // Both the C compiler and the hardware may re-order loads and
//     // stores; __sync_synchronize() tells them both not to.
//     __sync_synchronize();

//     // Release the lock, equivalent to lk->locked = 0.
//     // This code can't use a C assignment, since it might
//     // not be atomic. A real OS would use C atomics here.
//     asm volatile("movl $0, %0"
//                  : "+m"(lk->locked)
//                  :);

//     popcli();
// }

// // Record the current call stack in pcs[] by following the %ebp chain.
// void getcallerpcs(void *v, uint pcs[])
// {
//     uint *ebp;
//     int i;

//     ebp = (uint *)v - 2;
//     for (i = 0; i < 10; i++)
//     {
//         if (ebp == 0 || ebp < (uint *)KERNBASE || ebp == (uint *)0xffffffff)
//             break;
//         pcs[i] = ebp[1];      // saved %eip
//         ebp = (uint *)ebp[0]; // saved %ebp
//     }
//     for (; i < 10; i++)
//         pcs[i] = 0;
// }

// // Check whether this cpu is holding the lock.
// int holding(struct spinlock *lock)
// {
//     int r;
//     pushcli();
//     r = lock->locked && lock->cpu == mycpu();
//     popcli();
//     return r;
// }

// // Pushcli/popcli are like cli/sti except that they are matched:
// // it takes two popcli to undo two pushcli.  Also, if interrupts
// // are off, then pushcli, popcli leaves them off.

// // void pushcli(void)
// // {
// //     int eflags;

// //     eflags = readeflags();
// //     cli();
// //     if (mycpu()->ncli == 0)
// //         mycpu()->intena = eflags & FL_IF;
// //     mycpu()->ncli += 1;
// // }

// // void popcli(void)
// // {
// //     if (readeflags() & FL_IF)
// //         panic("popcli - interruptible");
// //     if (--mycpu()->ncli < 0)
// //         panic("popcli");
// //     if (mycpu()->ncli == 0 && mycpu()->intena)
// //         sti();
// // }
#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
// copied spinlock
struct thread_spinlock
{
    uint locked; // Is the lock held?

    // For debugging
    char *name; // Name of lock.
};
struct mutex_lock
{
    uint locked;
};
// volatile int total_balance = 0;
struct thread_spinlock lock;
struct mutex_lock m_lock;
struct balance
{
    char name[32];
    int amount;
};

// volatile unsigned int delay (unsigned int d) {
//    unsigned int i;
//    for (i = 0; i < d; i++) {
//        __asm volatile( "nop" ::: );
//    }

//    return i;
// }

volatile int total_balance = 0;
volatile unsigned int delay(unsigned int d)
{
    unsigned int i;
    for (i = 0; i < d; i++)
    {
        __asm volatile("nop" ::
                           :);
    }
    return i;
}


void thread_initlock(struct thread_spinlock*,char*);
void thread_spin_lock(struct thread_spinlock *);
void thread_spin_unlock(struct thread_spinlock *);

void mutex_initlock(struct mutex_lock *);
void mutex_lock(struct mutex_lock *);
void mutex_unlock(struct mutex_lock*);


void do_work(void *arg)
{
    int i;
    int old;
    struct balance *b = (struct balance *)arg;
    thread_spin_lock(&lock);
    // mutex_lock(&m_lock);
    printf(1, "Starting do_work: s:%s\n", b->name);
    // mutex_unlock(&m_lock);

    thread_spin_unlock(&lock);

    for (i = 0; i < b->amount; i++)
    {
        thread_spin_lock(&lock);
        // mutex_lock(&m_lock);
        old = total_balance;
        delay(100000);
        total_balance = old + 1;
        // printf(3,"%d %d %s",old,total_balance,b->name);
        // mutex_unlock(&m_lock);
        thread_spin_unlock(&lock);
    }
    printf(1, "Done s:%x\n", b->name);
    thread_exit();
    return;
}

int main(int argc, char *argv[])
{
    struct balance b1 = {"b1", 3200};
    struct balance b2 = {"b2", 2800};
    void *s1, *s2;
    int t1, t2, r1, r2;
    s1 = malloc(4096);
    s2 = malloc(4096);
    t1 = thread_create(do_work, (void *)&b1, s1);
    t2 = thread_create(do_work, (void *)&b2, s2);
    r1 = thread_join();
    r2 = thread_join();
    printf(1, "Threads finished: (%d):%d, (%d):%d, shared balance:%d\n",
           t1, r1, t2, r2, total_balance);
    exit();
}
void thread_initlock(struct thread_spinlock *lk, char *name)
{
    lk->name = name;
    lk->locked = 0;
}

void thread_spin_lock(struct thread_spinlock *lk)
{

    // The xchg is atomic.
    while (xchg(&lk->locked, 1) != 0)
        ;
    __sync_synchronize();
}


void thread_spin_unlock(struct thread_spinlock *lk)
{

    __sync_synchronize();

    asm volatile("movl $0, %0"
                 : "+m"(lk->locked)
                 :);
}

void mutex_initlock(struct mutex_lock *lk)
{
    lk->locked = 0;
}

void mutex_lock(struct mutex_lock *lk)
{
    while (xchg(&lk->locked, 1) != 0)
        sleep(1);
    __sync_synchronize();
}


void mutex_unlock(struct mutex_lock *lk)
{
    __sync_synchronize();

    asm volatile("movl $0, %0"
                 : "+m"(lk->locked)
                 :);
}