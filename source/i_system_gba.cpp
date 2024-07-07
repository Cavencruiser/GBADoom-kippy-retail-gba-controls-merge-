#include <stdarg.h>
#include <stdio.h>
#include <cstring>

#ifdef GBA

extern "C"
{
    #include "doomdef.h"
    #include "doomtype.h"
    #include "d_main.h"
    #include "d_event.h"

    #include "global_data.h"

    #include "tables.h"
}

#include "i_system_e32.h"

#include "lprintf.h"

#include <tonc.h>
#include <maxmod.h>

#define VID_PAGE1 MEM_VRAM
#define VID_PAGE2 (MEM_VRAM + 0xA000)

//**************************************************************************************


//*******************************************************************************
//VBlank handler.
//*******************************************************************************

__attribute__((target("arm")))
void VBlankCallback()
{
    mmVBlank();
    mmFrame();

    // Acknowledge interrupt manually.
    REG_IFBIOS |= IRQ_VBLANK;
    REG_IF = IRQ_VBLANK;
}


void I_InitScreen_e32()
{
    // Speed up IRQs by using the VBlank callback as an IRQ directly.
    irq_init(VBlankCallback);
    irq_enable(II_VBLANK);

    // Set gamepak wait states and prefetch.
    REG_WAITCNT = 0x46DA;

    // TODO: TTE init

    REG_TM2CNT_L= 65535-1872;     // 1872 ticks = 1/35 secs
    REG_TM2CNT_H = TM_FREQ_256 | TM_ENABLE;       // we're using the 256 cycle timer

    // cascade into tm3
    REG_TM3CNT_H = TM_CASCADE | TM_ENABLE;
}

//**************************************************************************************

void I_BlitScreenBmp_e32()
{

}

//**************************************************************************************

void I_StartWServEvents_e32()
{

}

//**************************************************************************************

void I_PollWServEvents_e32()
{
    key_poll();

    u16 key_down = key_curr_state() & (~key_prev_state());

    event_t ev;

    if(key_down)
    {
        ev.type = ev_keydown;

        if(key_down & KEY_UP)
        {
            ev.data1 = KEYD_UP;
            D_PostEvent(&ev);
        }
        else if(key_down & KEY_DOWN)
        {
            ev.data1 = KEYD_DOWN;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_LEFT)
        {
            ev.data1 = KEYD_LEFT;
            D_PostEvent(&ev);
        }
        else if(key_down & KEY_RIGHT)
        {
            ev.data1 = KEYD_RIGHT;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_SELECT)
        {
            ev.data1 = KEYD_SELECT;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_START)
        {
            ev.data1 = KEYD_START;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_A)
        {
            ev.data1 = KEYD_A;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_B)
        {
            ev.data1 = KEYD_B;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_L)
        {
            ev.data1 = KEYD_L;
            D_PostEvent(&ev);
        }

        if(key_down & KEY_R)
        {
            ev.data1 = KEYD_R;
            D_PostEvent(&ev);
        }
    }

    u16 key_up = key_prev_state() & (~key_curr_state());

    if(key_up)
    {
        ev.type = ev_keyup;

        if(key_up & KEY_UP)
        {
            ev.data1 = KEYD_UP;
            D_PostEvent(&ev);
        }
        else if(key_up & KEY_DOWN)
        {
            ev.data1 = KEYD_DOWN;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_LEFT)
        {
            ev.data1 = KEYD_LEFT;
            D_PostEvent(&ev);
        }
        else if(key_up & KEY_RIGHT)
        {
            ev.data1 = KEYD_RIGHT;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_SELECT)
        {
            ev.data1 = KEYD_SELECT;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_START)
        {
            ev.data1 = KEYD_START;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_A)
        {
            ev.data1 = KEYD_A;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_B)
        {
            ev.data1 = KEYD_B;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_L)
        {
            ev.data1 = KEYD_L;
            D_PostEvent(&ev);
        }

        if(key_up & KEY_R)
        {
            ev.data1 = KEYD_R;
            D_PostEvent(&ev);
        }
    }
}

//**************************************************************************************

void I_ClearWindow_e32()
{

}

//**************************************************************************************

unsigned short* I_GetBackBuffer()
{
    if(REG_DISPCNT & DCNT_PAGE)
        return (unsigned short*)VID_PAGE1;

    return (unsigned short*)VID_PAGE2;
}

//**************************************************************************************

unsigned short* I_GetFrontBuffer()
{
    if(REG_DISPCNT & DCNT_PAGE)
        return (unsigned short*)VID_PAGE2;

    return (unsigned short*)VID_PAGE1;
}

//**************************************************************************************

void I_CreateWindow_e32()
{

    //Bit5 = unlocked vram at h-blank.
    REG_DISPCNT = DCNT_MODE4 | DCNT_BG2 | DCNT_OAM_HBL;

    unsigned short* bb = I_GetBackBuffer();

    memset(bb, 0, 240*160);

    I_FinishUpdate_e32(NULL, NULL, 0, 0);

    bb = I_GetBackBuffer();

    memset(bb, 0, 240*160);

    I_FinishUpdate_e32(NULL, NULL, 0, 0);

}

//**************************************************************************************

void I_CreateBackBuffer_e32()
{
    I_CreateWindow_e32();
}

//**************************************************************************************

void I_FinishUpdate_e32(const byte* srcBuffer, const byte* pallete, const unsigned int width, const unsigned int height)
{
    REG_DISPCNT ^= DCNT_PAGE;
}

//**************************************************************************************

void I_SetPallete_e32(const byte* pallete)
{
    unsigned short* pal_ram = (unsigned short*)0x5000000;

    for(int i = 0; i< 256; i++)
    {
        unsigned int r = *pallete++;
        unsigned int g = *pallete++;
        unsigned int b = *pallete++;

        pal_ram[i] = RGB15(r >> 3, g >> 3, b >> 3);
    }
}

//**************************************************************************************

int I_GetVideoWidth_e32()
{
    return 120;
}

//**************************************************************************************

int I_GetVideoHeight_e32()
{
    return 160;
}



//**************************************************************************************

void I_ProcessKeyEvents()
{
    I_PollWServEvents_e32();
}

//**************************************************************************************

#define MAX_MESSAGE_SIZE 1024

__attribute__((noreturn))
void I_Error (const char *error, ...)
{
    // TODO: TTE init

    char msg[MAX_MESSAGE_SIZE];

    va_list v;
    va_start(v, error);

    vsnprintf(msg, MAX_MESSAGE_SIZE, error, v);

    va_end(v);

    // TODO: printf("%s", msg);

    while(true)
        VBlankIntrWait();
}

extern "C"
{

void __assert_func_stub(const char *file, int line, const char *fnct, const char *msg)
{
    I_Error(msg);
}

}

//**************************************************************************************

void I_Quit_e32()
{

}

//**************************************************************************************

#endif
