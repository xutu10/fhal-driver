/*
 * Copyright (c) 2016 Andreas Werner <kernel@andy89.org>
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, 
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */
#ifndef TIMER_H_
#define TIMER_H_
#include <stdbool.h>
#include <stdint.h>
#include <hal.h>
/**
 * \defgroup TIMER Timer Subsystem
 * \ingroup HAL
 * \code
 * #include <timer.h>
 * \endcode
 * 
 * This is the Timer Subsystem for controlling Timer of a SOC. 
 * \{
 */
/**
 * Private Structure of Timer driver
 */
struct timer;
/**
 * Generic Timer Interface
 * All Driver shall implement this interface 
 * Thread save access is not supported 
 */
struct timer_generic {
	/**
	 * true = is init
	 * false = is not init
	 */
	bool init;
#ifdef CONFIG_INSTANCE_NAME
	/**
	 * Name of Driver Instance for Debugging 
	 */
	const char *name;
#endif
#ifdef CONFIG_TIMER_MULTI
	/**
	 * Ops of driver in Multi mode
	 */
	const struct timer_ops *ops;
#endif
};
#ifdef CONFIG_TIMER_MULTI
/**
 * Function of timer driver in Multi Driver implementation mode 
 */
struct timer_ops {
	struct timer *(*timer_init)(uint32_t index, uint32_t prescaler, uint64_t basetime, int64_t adjust);
	int32_t (*timer_deinit)(struct timer *timer);

	int32_t (*timer_setOverflowCallback)(struct timer *timer, bool (*callback)(struct timer *timer, void *data), void * data);
	int32_t (*timer_start)(struct timer *timer);
	int32_t (*timer_stop)(struct timer *timer);
	int32_t (*timer_oneshot)(struct timer *timer, uint64_t us);
	int32_t (*timer_periodic)(struct timer *timer, uint64_t us);
	uint64_t (*timer_getTime)(struct timer *timer);
};
#endif
#ifndef CONFIG_TIMER_MULTI
/**
 * Init Timer
 * 
 * \param index Index of timer 
 * \param prescaler Prescaler of Timer 0 if only get Timer Instants timer must initialized 
 * \param basetime Base time of adjustment for example 20ms
 * \param adjust Diff to basetime for example 10us
 * \return Timer instance NULL on Error or not init if prescaler = 0
 */
struct timer *timer_init(uint32_t index, uint32_t prescaler, uint64_t basetime, int64_t adjust);
/**
 * Deinit Timer
 * \param timer Timer instance
 * \return -1 on error 0 on ok
 */
int32_t timer_deinit(struct timer *timer);
/**
 * Set Overflow Callback
 * \param timer Timer instance
 * \param callback Callback used if Timer Overflow
 * \param data Data passed to Callback
 * \return -1 on error 0 on ok
 */
int32_t timer_setOverflowCallback(struct timer *timer, bool (*callback)(struct timer *timer, void *data), void * data);
/**
 * Start timer
 * \param timer Timer instance
 * \return -1 on error 0 on ok
 */
int32_t timer_start(struct timer *timer);
/**
 * Stop timer
 * \param timer Timer instance
 * \return -1 on error 0 on ok
 */
int32_t timer_stop(struct timer *timer);
/**
 * Set timer in oneshot mode, stop after us
 * \param timer Timer instance
 * \param us Timerout
 * \return -1 on error 0 on ok
 */
int32_t timer_oneshot(struct timer *timer, uint64_t us);
/**
 * Set timer in periodic mode
 * \param timer Timer instance
 * \param us Timerout
 * \return -1 on error 0 on ok
 */
int32_t timer_periodic(struct timer *timer, uint64_t us);
/**
 * Get Time
 * \param timer Timer instance
 * \return time
 */
uint64_t timer_getTime(struct timer *timer);
#else
inline struct timer *timer_init(uint32_t index, uint32_t prescaler, uint64_t basetime, int64_t adjust) {
	HAL_DEFINE_GLOBAL_ARRAY(timer);
	struct timer_generic *timer = (struct timer_generic *)HAL_GET_DEV(timer, index);
	if (timer == NULL) {
		return NULL;
	}
	return timer->ops->timer_init(index, prescaler, basetime, adjust);
}
inline int32_t timer_deinit(struct timer *timer) {
	struct timer_generic *t = (struct timer_generic *) timer;
	return t->ops->timer_deinit(timer);
}

inline int32_t timer_setOverflowCallback(struct timer *timer, bool (*callback)(struct timer *timer, void *data), void * data) {
	struct timer_generic *t = (struct timer_generic *) timer;
	return t->ops->timer_setOverflowCallback(timer, callback, data);
}
inline int32_t timer_start(struct timer *timer) {
	struct timer_generic *t = (struct timer_generic *) timer;
	return t->ops->timer_start(timer);
}
inline int32_t timer_stop(struct timer *timer) {
	struct timer_generic *t = (struct timer_generic *) timer;
	return t->ops->timer_stop(timer);
}
inline int32_t timer_oneshot(struct timer *timer, uint64_t us) {
	struct timer_generic *t = (struct timer_generic *) timer;
	return t->ops->timer_oneshot(timer, us);
}
inline int32_t timer_periodic(struct timer *timer, uint64_t us) {
	struct timer_generic *t = (struct timer_generic *) timer;
	return t->ops->timer_periodic(timer, us);
}
inline uint64_t timer_getTime(struct timer *timer) {
	struct timer_generic *t = (struct timer_generic *) timer;
	return t->ops->timer_getTime(timer);
}
#endif
/**\}*/
#endif
