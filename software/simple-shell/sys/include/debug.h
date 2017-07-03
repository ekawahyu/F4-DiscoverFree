/*
 * debug.h
 *
 * Created on: Jun 20, 2012
 *     Author: Ekawahyu Susilo
 *
 * Copyright (c) 2012, Chongqing Aisenke Electronic Technology Co., Ltd.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the copyright holder.
 * 
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

//#define DEBUG_SYSTEM 1
//#define DEBUG_DEVICE 1

/* Non-OS system requires to define DEBUGMSG */
#if !defined DEBUGMSG
#define DEBUGMSG(mode,...)				\
	do{									\
		if(mode) printf(__VA_ARGS__);	\
	}while(0);
#endif

/* Provide switches to turn on/off debug message to stdout */
#if defined (DEBUG_NETWORK)||(DEBUG_SYSTEM)
#define DEBUGMSG_NORMAL				0
#define DEBUGMSG_REDUCED			1
#define DEBUGMSG_MAXCOUNT			64
#define DEBUGMSG_VAR(icounter)		static unsigned char icounter=0;
#define DEBUGMSG_RESET(icounter)	icounter=0;
#define DEBUGMSG_MESSAGE(mode,icounter,...)									\
	do{																		\
		if(mode) icounter++;												\
		if(icounter==1||icounter>=DEBUGMSG_MAXCOUNT||mode==DEBUGMSG_NORMAL)	\
		{																	\
			DEBUGMSG(1,__VA_ARGS__)											\
			icounter=1;														\
		}																	\
	}while(0);
#define DEBUGFN_CALL(x)				x;
#else
#define DEBUGMSG_VAR(icounter)
#define DEBUGMSG_RESET(icounter)
#define DEBUGMSG_MESSAGE(mode,icounter,...)
#define DEBUGFN_CALL(x)
#endif

#ifdef DEBUG_NETWORK
#define DEBUGMSG_NETWORK(mode,icounter,...)		DEBUGMSG_MESSAGE(mode,icounter,__VA_ARGS__)
#define DEBUGFN_NETWORK(x)						DEBUGFN_CALL(x);
#else
#define DEBUGMSG_NETWORK(mode,icounter,...)
#define DEBUGFN_NETWORK(x)
#endif

#ifdef DEBUG_SYSTEM
#define DEBUGMSG_SYSTEM(mode,icounter,...)		DEBUGMSG_MESSAGE(mode,icounter,__VA_ARGS__)
#define DEBUGFN_SYSTEM(x)						DEBUGFN_CALL(x);
#else
#define DEBUGMSG_SYSTEM(mode,icounter,...)
#define DEBUGFN_SYSTEM(x)
#endif

#ifdef DEBUG_DEVICE
#define DEBUGMSG_DEVICE(mode,icounter,...)		DEBUGMSG_MESSAGE(mode,icounter,__VA_ARGS__)
#define DEBUGFN_DEVICE(x)						DEBUGFN_CALL(x);
#else
#define DEBUGMSG_DEVICE(mode,icounter,...)
#define DEBUGFN_DEVICE(x)
#endif

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H_ */
