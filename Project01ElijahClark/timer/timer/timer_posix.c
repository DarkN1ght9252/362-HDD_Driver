#include <sys/signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define COUNTDOWN_VALUE 10

#define TIMEOUT_INTERVAL 2
#define TIMEOUT_DELAY 5

timer_t gTimerid;
int count = COUNTDOWN_VALUE;

void start_timer(void)
{
    struct itimerspec value;

    value.it_value.tv_sec = TIMEOUT_DELAY;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = TIMEOUT_INTERVAL;
    value.it_interval.tv_nsec = 0;

    timer_create (CLOCK_REALTIME, NULL, &gTimerid);

    timer_settime (gTimerid, 0, &value, NULL);

}

void stop_timer(void)
{
    struct itimerspec value;

    value.it_value.tv_sec = 0;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;

    timer_settime (gTimerid, 0, &value, NULL);
}

void timer_callback(int sig)
{
    printf("Caught timer signal: %d ... !!\n", sig);
    struct timeval ts;
    time_t tm;

    time(&tm); // man 3 time
    printf("Time: %s", ctime(&tm)); // man ctime

    gettimeofday(&ts, NULL); // man gettimeofday
    printf("Time: %ld.%06ld secs.\n\n", (long)ts.tv_sec, (long)ts.tv_usec);
    count--;
}

int main(int ac, char **av)
{
    (void) signal(SIGALRM, timer_callback);
    start_timer();
    while (count >= 0);
}
