//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  zc_alleg.h
//
//  For debugging...
//
//--------------------------------------------------------

#ifndef _ZC_ALLEG_H_
#define _ZC_ALLEG_H_

#define DEBUGMODE
#define ALLEGRO_NO_COMPATIBILITY
#include "base/allegro_wrapper.h"
#include <allegro/internal/aintern.h>
#include <a5alleg.h>
#include "base/alleg_compat.h"
#include "zfix.h"

#ifdef __cplusplus
INLINE fix abs(fix f)
{
    fix t;
    
    if(f < 0)
    {
        t.v = -f.v;
    }
    else
    {
        t.v = f.v;
    }
    
    return t;
}
#endif

#ifdef ALLEGRO_WINDOWS
#include <winalleg.h>
#define YIELD() Sleep(10)
#else
#if defined(ALLEGRO_UNIX)||defined(ALLEGRO_LINUX)||defined(ALLEGRO_MACOSX)
#include <sys/time.h>
INLINE void YIELD(void)
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1;
    select(0, NULL, NULL, NULL, &tv);
}
#else
#define YIELD() yield_timeslice()
#endif
#endif

#if !defined(ALLEGRO_MACOSX)
#define KEY_ZC_LCONTROL KEY_LCONTROL
#define KEY_ZC_RCONTROL KEY_RCONTROL
#else
#define KEY_ZC_LCONTROL KEY_COMMAND
#define KEY_ZC_RCONTROL KEY_COMMAND

#endif

#endif


