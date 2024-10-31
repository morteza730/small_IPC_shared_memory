/**
 * @file ماکرو ها و سرتیتر توابع چند منظوره
 * این فایل ماکرو ها و سرتیتر توابع همه منظوره ای را مخصوص سیستم عامل در بر دارد: ا
 * @version 3.0
 * @since 1391
 */

#ifndef __NSRUTILITY_H__
#define __NSRUTILITY_H__

#include <stdint.h>
#include <stdbool.h> //for bool in c

//#include "./nsrPlatform.h"
#if defined(__cplusplus) && defined(USE_EXTERN_C)
extern "C" {
#endif

//math/////////////////////////////////////////////////////
#ifndef Pi
#define Pi 3.141592653589793
//#define pi M_PI //conflicts with qt
#endif

#define EQUALS(variable, value, thresh)	\
	((((variable) - (value)) <= thresh && ((value) - (variable)) <= thresh)?1:0)

#define nsrMin(a,b) (a>b?b:a)
#define nsrMax(a,b) (a>b?a:b)

#define nsrFloor(var) \
	((int)((var>=0)?var:(var-1)))
#define nsrRound(var) \
	(((var - nsrFloor(var)) < 0.5)?nsrFloor(var):nsrFloor(var)+1)

int sgn2f(float var);
int nsrFloor2f(float var);
int nsrRound2f(float var);

int sgn2d(double var);
int nsrFloor2d(double var);
int nsrRound2d(double var);
uint64_t nsrFloor3d(double var);
uint64_t nsrRound3d(double var);

//down_round_up<0: floor, down_round_up=0 round, down_round_up>0 ceil
double discretize(double val, double min, double interval, int down_round_up);
double rescale(double var, double min, double max, double newmin, double newmax);
double saturate(double variable, double minimum, double maximum);

//also check min & max
#define saturate2(variable, minimum, maximum) \
	saturate(variable, nsrMin(minimum, maximum), nsrMax(minimum, maximum))

double normalize_angle(double angle);
double normalize_angle_deg(double angle);

inline double pow2(double var) {return var * var;}
inline double pow3(double var) {return var * var * var;}

//Random number generation/////////////////////////////////
#include <stdlib.h>

//set seed with srand(unsigned int seed)

//uniform distribution
#define frand() ((double) rand() / (RAND_MAX+1.0))
#define RAND(min, max) ((min) + (float)rand()/(RAND_MAX) * ((max)-(min)))

//normal distribution
float normrnd(float mean, float _1sigma);

void set_random_generator32_seed(uint32_t _seed);
uint32_t random_generator32();

//Bitwise operations///////////////////////////////////////
//note: shifting by more than word size(32 bits for arm) is undefined, should put cast to uint64_t
//note2: shifting does -not- change inputs

#define _ones(bits) (((uint64_t)1 << (bits)) - 1) //_ones(3):=0b111  //verified

#define getBit(byte,bit) ((byte >> bit)  & 0x01)
#define getBitRange2(byte, lowerbit, higherbit) (((byte) & _ones(higherbit+1)) >> (lowerbit)) //nearly verified

#define setBit(byte,bit,val) byte = ((val)==1?((0x01 << bit) | byte):~((0x01 << bit) | ~byte))
#define setBit64(byte,bit,val) byte = ((val)==1?(((uint64_t)0x01 << bit) | byte):~(((uint64_t)0x01 << bit) | ~byte))
//also cuts input val to fit in higher-lower bit range, verified
#define setBitRange2(byte, lowerbit, higherbit, val) \
	byte = ((val & _ones(higherbit-lowerbit+1)) << lowerbit) \
		   | (byte & ~(_ones(higherbit+1) ^ _ones(lowerbit))) //wipe previous setBitRange part

/** Convert 16-bit value */
#define SWAP16(x) (\
				   (((uint16_t)(x) & 0xff00) >> 8) | \
				   (((uint16_t)(x) & 0x00ff) << 8))
/** Convert 32-bit value */
#define SWAP32(x) (\
				   (((uint32_t)(x) & 0xff000000) >> 24) | \
				   (((uint32_t)(x) & 0x00ff0000) >> 8)  | \
				   (((uint32_t)(x) & 0x0000ff00) << 8)  | \
				   (((uint32_t)(x) & 0x000000ff) << 24))

//Timing///////////////////////////////////////////////////
//int time_reached(float period, float phase, float t, float pre_tsim); //phase c [0., 1.]
int time_reached(double period, double phase, double t, double pre_tsim);

#if !defined(ANDROID) && !defined(__linux__)
int32_t wdt_delay_us(int32_t sleep_time_us);
#endif

#ifdef PERIOD_CHANNEL_NUM
int check_period_passed(uint8_t chan, float interval_s);
#endif //PERIOD_CHANNEL_NUM

#ifdef FIND_DT_CHANS
float find_dt(int chan);
#endif //FIND_DT_CHANS

double myTime();

static inline double calc_dt(double time_s = -1e10) //Use only once in a single file
{
	static int inited = 0;
    static double last_time_s = 0;
	
	if(time_s < -1e9) {
		time_s = myTime();
	}

	if(inited == 0) {
		last_time_s = time_s;
		inited = 1;
		return 0;
	}

    float dt = time_s - last_time_s;
    last_time_s = time_s;
    return dt;
}

#ifdef TICKRATE_CHANNEL_NUM
float tick_rate(uint8_t chan, int ticks);
#endif //TICKRATE_CHANNEL_NUM

#define SYNCHRONIZE_CHANNEL_NUM 10
#ifdef SYNCHRONIZE_CHANNEL_NUM
int32_t synchronize_us(uint8_t chan, int32_t synchronize_time_us);
#endif //SYNCHRONIZE_CHANNEL_NUM

//Tickfinder
void tickfinder(const char* log);
void tickfinder2(const char* log, double thresh);

//Communication////////////////////////////////////////////
//to prevent error as 4 chans was harcoded in previous programs
#ifndef PACKET_SUCCESS_RATE_CHANNEL_NUM
#define PACKET_SUCCESS_RATE_CHANNEL_NUM 4
#endif

#ifdef PACKET_SUCCESS_RATE_CHANNEL_NUM
float packet_receive_success_rate(uint8_t chan, int sequence);
#endif //PACKET_SUCCESS_RATE_CHANNEL_NUM

uint32_t rc_crc32(uint32_t crc, const char *buf, size_t len);
/*void printf_hex(uint8_t* buf, int len);
void printf_hex_gcs(uint8_t* buf, int len);*/

#if defined(__cplusplus) && defined(USE_EXTERN_C)
}
#endif

#endif /* __NSRUTILITY_H__ */
