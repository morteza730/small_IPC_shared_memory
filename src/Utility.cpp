#include "./Utility.h"

#if defined(__cplusplus) && defined(USE_EXTERN_C)
extern "C" {
#endif

#include <math.h>

int sgn2f(float var)
{
	return (var > 0) ? 1 : ((var < 0) ? -1 : 0);
}

int nsrFloor2f(float var)
{
	return (var >= 0) ? var : (var - 1);
}

int nsrRound2f(float var)
{
	return (((var - nsrFloor2f(var)) < 0.5) ? nsrFloor2f(var) : nsrFloor2f(var) + 1);
}

int sgn2d(double var)
{
	return (var > 0) ? 1 : ((var < 0) ? -1 : 0);
}

int nsrFloor2d(double var)
{
	return (var >= 0) ? var : (var - 1);
}

int nsrRound2d(double var)
{
	return (((var - nsrFloor2d(var)) < 0.5) ? nsrFloor2d(var) : nsrFloor2d(var) + 1);
}

uint64_t nsrFloor3d(double var)
{
	return (var >= 0) ? var : (var - 1);
}

uint64_t nsrRound3d(double var)
{
	return (((var - nsrFloor3d(var)) < 0.5) ? nsrFloor3d(var) : nsrFloor3d(var) + 1);
}
double discretize(double val, double min, double interval, int down_round_up)
{
	//interval = (max - min)/steps;
	if(interval == 0)
		return val;
	double a = (val - min) / interval;
	int floor_of_a = nsrFloor2d(a);
	if(down_round_up < 0)
		return floor_of_a * interval + min;
	else if(down_round_up == 0)
		return nsrRound2d(a) * interval + min;
	else if(down_round_up > 0)
		return (floor_of_a + 1) * interval + min;
	return -1; //never happens
}

double rescale(double var, double min, double max, double newmin, double newmax)
{
	return ((((newmax) - (newmin)) / ((max) - (min))) * ((var) - (min)) + (newmin));
}

double saturate(double variable, double minimum, double maximum)
{
	return ((variable < minimum) ? minimum : ((variable > maximum) ? maximum : variable));
}

double normalize_angle(double angle)
{
	while(angle > Pi) angle -= 2 * Pi;
	while(angle <= -Pi) angle += 2 * Pi;
	return angle;
}

double normalize_angle_deg(double angle)
{
	while(angle > 180) angle -= 2 * 180;
	while(angle <= -180) angle += 2 * 180;
	return angle;
}

//random/////////////////////////////////////////////////////

//normal random variate generator(box_muller method)
float normrnd(float mean, float _1sigma)
{
	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if(use_last) {		         /* use value from previous call */
		y1 = y2;
		use_last = 0;
	} else {
		do {
			x1 = 2.0 * frand() - 1.0;
			x2 = 2.0 * frand() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while(w >= 1.0);

		w = sqrt((-2.0 * log(w)) / w);
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return(mean + y1 * _1sigma);
}

//Timing/////////////////////////////////////////////////////
double myTime();

/*int time_reached(float period, float phase, float t, float pre_tsim)
{
	float totalPhase;
	if(period < 1e-6)
		return 1;

	totalPhase = (.5 + phase) * period;
	if(nsrRound2f((t - totalPhase) / period + 0.00001) > nsrRound2f((pre_tsim - totalPhase) / period + 0.00001))
		return 1;
	else
		return 0;
}*/

int time_reached(double period, double phase, double t, double pre_tsim)
{
	if(period < 1e-6)
		return 1;

	if(nsrFloor2d(t / period - phase + 0.00001) > nsrFloor2d(pre_tsim / period - phase + 0.00001))
		return 1;

	return 0;
}

#ifdef PERIOD_CHANNEL_NUM
int check_period_passed(uint8_t chan, float interval_s)
{
	static float last_time_s[PERIOD_CHANNEL_NUM] = { -1000000};

	double time_s = myTime();
	if(time_s > last_time_s[chan] + interval_s) {
		last_time_s[chan] = time_s;
		return 1;
	} else
		return 0;
	return -1; //never happens
}
#endif //PERIOD_CHANNEL_NUM

#ifdef FIND_DT_CHANS
float find_dt(int chan)
{
	float dt1;
	static float last_time1[FIND_DT_CHANS] = { -1};
	if(last_time1[chan] >= 0)
		dt1 = myTime() - last_time1[chan];
	else
		dt1 = 0;
	last_time1[chan] = myTime();
	return dt1;
}
#endif //FIND_DT_CHANS

#ifdef TICKRATE_CHANNEL_NUM
float tick_rate(uint8_t chan, int ticks)
{
	static float pre_t[TICKRATE_CHANNEL_NUM] = { -1};
	static float rate[TICKRATE_CHANNEL_NUM] = {0};
	double t, dt, w;

	t = myTime();

	//first time
	if(pre_t[chan] < 0) {
		rate[chan] = 0.;
		pre_t[chan] = t;
		return rate[chan];
	}

	dt = t - pre_t[chan];

	if(dt <= 0) //divide by zero prevention
		dt = 0.01;

	w = tick_rate_chan_weight(chan);
	rate[chan] = w * rate[chan] + (1 - w) * ((float)(ticks) / dt);

	pre_t[chan] = t;
	return rate[chan];
}
#endif //TICKRATE_CHANNEL_NUM

//OS implementation of synchronize_us
#if defined(ANDROID) || defined(__linux__) || defined(__MINGW32__) || defined(__MINGW64__)
#include <unistd.h> //for usleep

#ifdef SYNCHRONIZE_CHANNEL_NUM
//اجرای برنامه را تا اختلاف زمانی مشخص شده، از اجرای قبلی این تابع به تاخیر می اندازد.
int synchronize_us(uint8_t chan, /*long int */int32_t synchronize_time_us)
{
	double time_s;
	double static pre_time_s[SYNCHRONIZE_CHANNEL_NUM] = { -1};
	long int sleep_time_us;

	time_s = myTime();
	if(pre_time_s[chan] == -1)
		pre_time_s[chan] = time_s;

	sleep_time_us = synchronize_time_us - (time_s - pre_time_s[chan]) * 1000000;

	#ifndef MIN_SLEEP_US
	#define MIN_SLEEP_US 0
	#endif
	if(sleep_time_us > MIN_SLEEP_US)
		usleep(sleep_time_us);
	//else
	//	usleep(MIN_SLEEP_US); //minimum sleep time required for program to be responsive

	pre_time_s[chan] = myTime();
	return 0;
}
#endif //SYNCHRONIZE_CHANNEL_NUM

#else //micro implementation of synchronize_us
#include "delay.h"

#ifdef SYNCHRONIZE_CHANNEL_NUM
int32_t synchronize_us(uint8_t chan, long int synchronize_time_us)
{
	static int64_t pre_time_us[SYNCHRONIZE_CHANNEL_NUM] = { -1};
	int64_t time_us, sleep_time_us;
	unsigned short sleep_ms;
	unsigned int sleep_us, sleep_255ms;
	int i;
	WATCH();
	time_us = myTime() * 1000000;
	if(pre_time_us[chan] == -1)
		pre_time_us[chan] = time_us;
	WATCH();
	sleep_time_us = synchronize_time_us - (time_us - pre_time_us[chan]);
	//LOGN ("", "1:%f::::::::::%i::%i::%i::%i\n", myTime(), (int)time_us/1000, (int)pre_time_us[chan]/1000, ((int)time_us - (int)pre_time_us[chan])/1000,(int)sleep_time_us/1000);
	WATCH();
	if(sleep_time_us <= 0)
		sleep_time_us = 0;
	WATCH();
	sleep_255ms = sleep_time_us / 255000;
	sleep_ms = (sleep_time_us % 255000) / 1000;
	sleep_us = sleep_time_us % 1000;
	WATCH();
	for(i = 0; i < sleep_255ms; i++) {
		delay_ms(255);
		WATCH();
	}
	if(sleep_ms > 0) //delay(0) causes lockup
		delay_ms(sleep_ms);
	WATCH();
	if(sleep_us > 0)
		delay_us(sleep_us);
	WATCH();
	//LOGN("", "2:%f\n", myTime());
	pre_time_us[chan] = myTime() * 1000000;
	//a bug was found here after about 1 years!! alhamdollehah
	//saving pre_time should just happen after delay
	WATCH();
	return sleep_time_us;
}
#endif //SYNCHRONIZE_CHANNEL_NUM
#endif

#if !defined(ANDROID) && !defined(__linux__) && !defined(__MINGW32__) && !defined(__MINGW64__)
int32_t wdt_delay_us(int32_t sleep_time_us)
{
	int64_t time_us;
	unsigned short sleep_ms;
	unsigned int sleep_us, sleep_255ms;
	int i;

	sleep_255ms = sleep_time_us / 255000;
	sleep_ms = (sleep_time_us % 255000) / 1000;
	sleep_us = sleep_time_us % 1000;
	WATCH();

	for(i = 0; i < sleep_255ms; i++) {
		delay_ms(255);
		WATCH();
	}

	if(sleep_ms > 0) {//delay(0) causes lockup
		delay_ms(sleep_ms);
		WATCH();
	}

	if(sleep_us > 0) {
		delay_us(sleep_us);
		WATCH();
	}

	return sleep_time_us;
}
#endif

//Tickfinder
double pre_ticktime_s = 0;

void tickfinder(const char* log)
{
	tickfinder2(log, 0.1);
}

void tickfinder2(const char* log, double thresh)
{
	double time_s = myTime();
	if(time_s - pre_ticktime_s > thresh)
		;//LOGE("tickfinder", " %s\n", log);
	pre_ticktime_s = time_s;
}

//Communication////////////////////////////////////////////
#ifdef PACKET_SUCCESS_RATE_CHANNEL_NUM
//making these static causes netlink to lock in recvfrom function!!!
int seq_pre[4];
float packet_success_rate[4] = { -1};
float packet_receive_success_rate(uint8_t chan, int sequence)
{
	int i, all;

	//first time
	if(packet_success_rate[chan] < 0) {
		packet_success_rate[chan] = 1.;
		seq_pre[chan] = sequence;
		return packet_success_rate[chan];
	}

	all = sequence > seq_pre[chan] ? (sequence - seq_pre[chan]) : sequence + 256 - seq_pre[chan]; //turning into (uint8_t) prevent overflow problems

	packet_success_rate[chan] = 0.96 * packet_success_rate[chan] + 0.04 * (1);
	for(i = 0; i < all - 1; i++)
		packet_success_rate[chan] = 0.96 * packet_success_rate[chan] + 0.04 * (0);

	seq_pre[chan] = sequence;

	return packet_success_rate[chan];
}
#endif //PACKET_SUCCESS_RATE_CHANNEL_NUM

/**
 * @brief rc_crc32 is main nsr64 crc generator //1/715 first error rate for 6M files...
 * @param crc:start crc from 0 or previous rc_crc32 if text is continued
 * @cite https://rosettacode.org/wiki/CRC-32#C is crc method
 * @return
 */
uint32_t rc_crc32(uint32_t crc, const char *buf, size_t len)
{
	static uint32_t table[256];
	static int have_table = 0;
	uint32_t rem;
	uint8_t octet;
	int i, j;
	const char *p, *q;

	// This check is not thread safe; there is no mutex.
	if(have_table == 0) {
		/* Calculate CRC table. */
		for(i = 0; i < 256; i++) {
			rem = i;  // remainder from polynomial division
			for(j = 0; j < 8; j++) {
				if(rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = 1;
	}

	crc = ~crc;
	q = buf + len;
	for(p = buf; p < q; p++) {
		octet = *p;  // Cast to unsigned octet.
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}

/*void printf_hex(uint8_t* buf, int len)
{
    int i;
    for(i=0; i<len; i++)
    	RLOG4("", "%x ", buf[i]);
    RLOG4("", "\n");
}

void printf_hex_gcs(uint8_t* buf, int len)
{
    int i;
    for(i=0; i<len; i++)
    	LOGGS("", "%x ", buf[i]);
    if(len>0) LOGGS("", "\n");
}*/

//Other////////////////////////////////////////////////////
uint32_t local_random_generator32_seed = 0x92636456;
void set_random_generator32_seed(uint32_t _seed)
{
	if(_seed != 0)
		local_random_generator32_seed = _seed;
}

uint32_t random_generator32()
{
	local_random_generator32_seed += myTime() * 1000;
	local_random_generator32_seed ^= local_random_generator32_seed << 13;
	local_random_generator32_seed ^= local_random_generator32_seed >> 17;
	local_random_generator32_seed ^= local_random_generator32_seed << 5;
	return local_random_generator32_seed;
}

#if defined(__cplusplus) && defined(USE_EXTERN_C)
}
#endif
