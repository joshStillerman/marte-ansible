#include <unistd.h>
#include <stdint.h>
#include <ev.h>
#include <iostream>
#ifdef NDEBUG
#undef NDEBUG
#include <mdsobjects.h>
#endif
//#include "VL6180XSet.h"
#include <sched.h>
#include <sys/resource.h>
#define MDSPLUS_EVENT "BAGEL_HEIGHTS"
//static struct VL6180XSet *tofs = NULL;
static double start_time;
static void
run_on_isolated_cpus(const uint8_t *cpus, uint8_t ncpus)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    for (int i=0; i<ncpus; i++)
        CPU_SET(cpus[i], &set);
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        perror("Couldn't set affinity with sched_setaffinity");
    if (setpriority(PRIO_PROCESS, getpid(), -20) == -1)
        perror("Couldn't raise priority to realtime.");
}
static void send_data(double now, float *samples, uint8_t nsamp)
{
    uint64_t inow;
    float    fnow;
    int dims[] = {nsamp};
    int shot = 1; // dummy "shot" info
    inow = (uint64_t)(now * 1000);
    fnow = (float)(now - start_time);
    // data = new MDSplus::Uint8Array(raw, 4);
    MDSplus::EventStream::send(shot, MDSPLUS_EVENT, false, 1, (void *)&fnow, 
                                1, dims, samples);
    // MDSplus::EventStream::send(shot, "H_BAGEL_ABST", true, 1, (void *)&inow, 4, samples);
}
static void send_bogus_data(double now, uint8_t nsamp)
{
    uint64_t inow;
    float fnow;
    int shot = 1; // dummy "shot" info
    float samples[] = {0.,1.,2.,3.};
    int dims[] = {nsamp};
    inow = (uint64_t)(now * 1000);
    fnow = (float)(now - start_time);
    MDSplus::EventStream::send(shot, "BOGUS_DATA", false, 1, (void *)&fnow, 
                                1, dims, samples);
}
static int counter=0;

static void 
periodic_task(struct ev_loop *loop, ev_timer *w, int revents)
{
    // lets get the data.. then start a new data gather.. then send the data
    float now;
    float samples[4];
    uint8_t raw[4];
    uint64_t nowi;
    // MDSplus::Uint8Array *data;
//    VL6180XSet_Read(tofs, raw);
    for (int i=0; i<4; i++) {
        samples[i] = counter++;
    }
    send_data(ev_now(loop), samples, 4);
    usleep(1500);
//    VL6180XSet_Start(tofs);
}
#define TOF_I2C_CHAN 3
#define TOF_NUM 4
const uint8_t TOF_GPIOS[] = {16, 17, 25, 27};
int main(int argc, char *argv[])
{
    struct ev_loop *loop = ev_default_loop (0);
    ev_timer mytimer;
    const uint8_t cpus[] = {2,3};
    run_on_isolated_cpus(cpus, sizeof(cpus));
//    tofs = VL6180XSet_Setup(TOF_I2C_CHAN, TOF_NUM, TOF_GPIOS);
    usleep(2000);
//    VL6180XSet_Start(tofs);
    usleep(6000);
    ev_now_update(loop);
    start_time = ev_now(loop);
    send_bogus_data(ev_now(loop), 4);
    ev_timer_init(&mytimer, periodic_task, 0.015, 0.010);
    ev_timer_start(loop, &mytimer);
    ev_run (loop, 0);
}
