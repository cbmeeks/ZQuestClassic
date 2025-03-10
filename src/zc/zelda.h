	//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  zelda.h
//
//  Definitions, function prototypes, etc. for zelda.cc
//
//--------------------------------------------------------

#ifndef _ZELDA_H_
#define _ZELDA_H_

/*********************************/
/********** Definitions **********/
/*********************************/

#include <vector>
#include <map>
#include "base/zdefs.h"
#include "base/zc_array.h"
#include "zc_sys.h"
#include "zeldadat.h"
#include "sfx.h"
#include "zcmusic.h"
#include "jwin.h"
#include "gamedata.h"
#include "base/zsys.h"
#include "script_drawing.h"
#include "zfix.h"
#include "base/util.h"
#include "base/fonts.h"
#include "replay.h"

class sprite;

int32_t isFullScreen();
int32_t onFullscreen();
extern bool zqtesting_mode;
extern bool use_testingst_start;

#if DEVLEVEL > 0
extern bool dev_logging;
extern bool dev_debug;
extern bool dev_timestmp;
#endif

#define  MAXMIDIS     ZC_MIDI_COUNT+MAXCUSTOMTUNES

#define MAX_IDLE      72000                                 // 20 minutes
#define MAX_ACTIVE    72000                                 // 20 minutes

// saved games
#define MAXSAVES      (standalone_mode?1:15) // It's constant enough... :p

#define mDOOR_UP         0x0001                               // only dungeons use this
#define mDOOR_DOWN       0x0002                               //        ''
#define mDOOR_LEFT       0x0004                               //        ''
#define mDOOR_RIGHT      0x0008                               //        ''

#define mITEM            0x0010                               // item (main screen)
#define mSPECIALITEM           0x0020                               // special item (underground)
#define mNEVERRET        0x0040                               // enemy never returns
#define mTMPNORET        0x0080                               // enemies don't return until you leave the dungeon

#define mLOCKBLOCK       0x0100                               // if the lockblock on the screen has been triggered
#define mBOSSLOCKBLOCK   0x0200                               // if the bosslockblock on the screen has been triggered
#define mCHEST           0x0400                               // if the unlocked check on this screen has been opened
#define mLOCKEDCHEST     0x0800                               // if the locked chest on this screen has been opened

#define mBOSSCHEST       0x1000                               // if the boss chest on this screen has been opened
#define mSECRET          0x2000                               // only overworld and caves use this
#define mVISITED         0x4000                               // only overworld uses this
#define mLIGHTBEAM       0x8000                               // light beam triggers completed

#define mMAXIND          16

/*********************************/
/******** Enums & Structs ********/
/*********************************/

enum { qQUIT=1, qRESET, qEXIT, qGAMEOVER, qCONT, qSAVE, qSAVECONT, qWON, qRELOAD, qERROR, qINCQST, qLAST };

// "special" walk flags
enum
{
    spw_none, spw_door, spw_clipright, spw_floater, spw_trap, spw_halfstep,
    spw_water, spw_wizzrobe, spw_clipbottomright
};

// death & fade defs
enum
{
    fade_none, fade_flicker, fade_invisible, fade_flash_die,
    fade_blue_poof
};

/*********************************/
/*********** Procedures **********/
/*********************************/

/*

  // aglogo.cc
  int32_t  aglogo(BITMAP* frame);


  // title.cc
  void update_game_icons();

  // zc_sys.cc
  void color_layer(RGB *src,RGB *dest,char r,char g,char b,char pos,int32_t from,int32_t to);
  void go();
  void comeback();
  void waitvsync(bool fast);
  int32_t  input_idle(bool checkmouse);
  int32_t  after_time();
  void hit_close_button();
  */

void port250QuestRules();

bool get_debug();
void set_debug(bool d);

void Z_eventlog(const char *format, ...);
void Z_scripterrlog(const char * const format, ...);
void zprint(const char * const format, ...);
void zprint2(const char * const format, ...);

// zelda.cc
void addLwpn(int32_t x,int32_t y,int32_t z,int32_t id,int32_t type,int32_t power,int32_t dir, int32_t parentid);
void addLwpnEx(int32_t x,int32_t y,int32_t z,int32_t id,int32_t type,int32_t power,int32_t dir, int32_t parentitem, int32_t parentid, byte script_gen);
void ALLOFF(bool messagesToo = true, bool decorationsToo = true, bool force = false);
void centerHero();
zfix  HeroX();
zfix  HeroY();
zfix  HeroZ();
zfix  HeroFakeZ();
int32_t  HeroHClk();
int32_t  HeroNayrusLoveShieldClk();
int32_t  HeroHoverClk();
int32_t  HeroSwordClk();
int32_t  HeroItemClk();
int32_t  HeroAction();
int32_t  HeroCharged();
byte HeroGetDontDraw();
void setSwordClk(int32_t newclk);
void setItemClk(int32_t newclk);
int32_t  HeroLStep();
void HeroCheckItems(int32_t index = -1);
zfix  HeroModifiedX();
zfix  HeroModifiedY();
zfix  GuyX(int32_t j);
zfix  GuyY(int32_t j);
int32_t  GuyID(int32_t j);
int32_t  GuyMisc(int32_t j);
void StunGuy(int32_t j,int32_t stun);
bool  GuySuperman(int32_t j);
int32_t  GuyCount();
int32_t  HeroDir();
void add_grenade(int32_t wx, int32_t wy, int32_t wz, int32_t size, int32_t parentid);
zfix distance(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
bool getClock();
void setClock(bool state);
void CatchBrang();;
int32_t HeroAction();

extern int32_t DMapEditorLastMaptileUsed;

void do_dcounters();
void game_loop();

void clearmsgnext(int32_t str);
void donewmsg(int32_t str);
void msg_bg(MsgStr const& msg);
void msg_prt();
void blit_msgstr_bg(BITMAP* dest, int32_t x, int32_t y, int32_t dx, int32_t dy, int32_t w, int32_t h);
void blit_msgstr_fg(BITMAP* dest, int32_t x, int32_t y, int32_t dx, int32_t dy, int32_t w, int32_t h);
void blit_msgstr_prt(BITMAP* dest, int32_t x, int32_t y, int32_t dx, int32_t dy, int32_t w, int32_t h);
void dismissmsg();
void dointro();
void init_dmap();
int32_t  init_game();
int32_t  cont_game();
void restart_level();
int32_t  load_quest(gamedata *g, bool report=true, byte printmetadata = 0);
//int32_t  init_palnames();

int32_t get_currdmap();
int32_t get_dlevel();
int32_t get_currscr();
int32_t get_currmap();
int32_t get_homescr();
int32_t get_bmaps(int32_t si);
bool no_subscreen();
bool is_zquest();
bool screenIsScrolling();
//void quit_game();
int32_t d_timer_proc(int32_t msg, DIALOG *d, int32_t c);

/*ZScript
const int32_t TINT_NONE = 0;
const int32_t TINT_GREY = 0;
const int32_t TINT_RED = 0;
const int32_t TINT_GREEN = 0;
const int32_t TINT_BLUE = 0;
const int32_t TINT_VIOLET = 0;
const int32_t TINT_TEAL = 0;
const int32_t TINT_AMBER = 0;
const int32_t TINT_CYAN = 0;
const int32_t TINT_MODE_UNIFORM = 0;
const int32_t TINT_MODE_DISTRIBUTED = 10;

Graphics->Tint(mode)
Graphics->Tint(TINT_MODE_DISTRIBUTED+TINT_VIOLET)
*/
//2.54
extern int16_t lastMonoPreset;
extern int16_t lastCustomTint[4];

//extern byte __isZQuest;

void setMonochromatic(int32_t mode); //GFX are monochrome. 
void setMonochrome(bool state); //GFX are monochrome. 
bool isMonochrome();
bool isUserTinted();
void isUserTinted(bool state);
void addColour(int32_t radd, int32_t gadd, int32_t badd, int32_t base);
void shiftColour(int32_t rshift, int32_t gshift, int32_t bshift, int32_t base);
void doClearTint();
void restoreTint();
void restoreMonoPreset();
void refreshTints();

void doGFXMonohue(int32_t _r, int32_t _g, int32_t _b, bool m);
void doTint(int32_t _r, int32_t _g, int32_t _b);

void runDrunkRNG();
void load_replay_file_deferred(ReplayMode mode, std::string replay_file);
void zc_game_srand(int seed, zc_randgen* rng = nullptr);

//Save screen settings. 
enum { 	SAVESC_BACKGROUND, 		SAVESC_TEXT, 			SAVESC_USETILE, 	
	SAVESC_CURSOR_CSET, 		SAVESC_CUR_SOUND,  		SAVESC_TEXT_CONTINUE_COLOUR, 
	SAVESC_TEXT_SAVE_COLOUR, 	SAVESC_TEXT_RETRY_COLOUR, 	SAVESC_TEXT_CONTINUE_FLASH, 
	SAVESC_TEXT_SAVE_FLASH, 	SAVESC_TEXT_RETRY_FLASH,	SAVESC_MIDI,
	SAVESC_CUR_FLIP, 		    SAVESC_TEXT_DONTSAVE_COLOUR, 	SAVESC_TEXT_SAVEQUIT_COLOUR, 
	SAVESC_TEXT_SAVE2_COLOUR, 	SAVESC_TEXT_QUIT_COLOUR, 	SAVESC_TEXT_DONTSAVE_FLASH,
	SAVESC_TEXT_SAVEQUIT_FLASH,	SAVESC_TEXT_SAVE2_FLASH, 	SAVESC_TEXT_QUIT_FLASH,
	SAVESC_EXTRA1, 			SAVESC_EXTRA2,			SAVESC_EXTRA3,			
	SAVESC_LAST	};

extern int32_t SaveScreenSettings[24]; //BG, Text, Cursor CSet, MIDI
//Save Screen text. 
enum { SAVESC_CONTINUE, SAVESC_SAVE, SAVESC_RETRY, SAVESC_DONTSAVE, SAVESC_SAVEQUIT, SAVESC_SAVE2, SAVESC_QUIT, SAVESC_END };
extern char SaveScreenText[7][32]; //(char *) "CONTINUE", (char *) "SAVE", (char*) "RETRY" , 
					//DON'T SAVE, SAVE AND QUIT, SAVE, QUIT
extern void SetSaveScreenSetting(int32_t indx, int32_t value);
extern void ChangeSubscreenText(int32_t index, const char *f);
INLINE void sfx(int32_t index)
{
    sfx(index,128,false);
}
INLINE void sfx(int32_t index,int32_t pan)
{
	sfx(index,vbound(pan, 0, 255) ,false);
}

bool isSideViewGravity(int32_t t = 0);
bool isSideViewHero(int32_t t = 0);

//INLINE void SCRFIX() { putpixel(screen,0,0,getpixel(screen,0,0)); }

//Script Clearing
void initZScriptGlobalScript(int32_t ID);

void update_hw_screen(bool force);

/**********************************/
/******** Global Variables ********/
/**********************************/


extern ZCMUSIC *zcmusic;

extern int32_t colordepth;
extern int32_t db;
extern int32_t detail_int[10];                                  //temporary holder for things you want to detail
extern zinitdata  zinit;
extern int32_t lens_hint_item[MAXITEMS][2];                     //aclk, aframe
extern int32_t lens_hint_weapon[MAXWPNS][5];                    //aclk, aframe, dir, x, y
extern int32_t strike_hint_counter;
extern int32_t strike_hint_timer;
extern int32_t strike_hint;
extern signed char pause_in_background_menu_init;

extern RGB_MAP rgb_table;
extern COLOR_MAP trans_table, trans_table2;
extern BITMAP   *framebuf, *menu_bmp, *gui_bmp, *scrollbuf, *tmp_bmp, *tmp_scr, *screen2,
                *msg_txt_bmp_buf, *msg_portrait_display_buf, *msg_txt_display_buf, *msg_bg_display_buf, *msg_bg_bmp_buf,
				*msg_menu_bmp_buf, *msg_portrait_bmp_buf, *pricesdisplaybuf, *tb_page[3],
				*temp_buf, *temp_buf2, *prim_bmp,
				*script_menu_buf, *f6_menu_buf;
extern BITMAP   *darkscr_bmp_curscr, *darkscr_bmp_scrollscr,
                *darkscr_bmp_curscr_trans, *darkscr_bmp_scrollscr_trans;
extern BITMAP *lightbeam_bmp;
extern BITMAP *zcmouse[4];
extern DATAFILE *datafile, *sfxdata, *fontsdata, *mididata;
extern SAMPLE   wav_refill;
//extern FONT custom_fonts[MAXFONTS];
extern PALETTE  RAMpal;
extern byte     *colordata;
//extern byte     *tilebuf;
extern itemdata *itemsbuf;
extern wpndata  *wpnsbuf;
extern comboclass *combo_class_buf;
extern guydata  *guysbuf;
extern item_drop_object    item_drop_sets[MAXITEMDROPSETS];
extern ZCHEATS  zcheats;
extern byte     use_cheats;
extern byte     use_tiles;
extern char     palnames[MAXLEVELS][17];

extern bool is_large;

extern bool standalone_mode;
extern char *standalone_quest;
extern bool skip_title;
extern bool disable_save_to_disk;

extern int32_t draw_screen_clip_rect_x1; //Used by the ending, bu could be used to change the drawn screen size. 
extern int32_t draw_screen_clip_rect_x2;
extern int32_t draw_screen_clip_rect_y1;
extern int32_t draw_screen_clip_rect_y2;

/*
extern tiledata *newtilebuf, *grabtilebuf;
extern newcombo *combobuf;
extern word animated_combo_table[MAXCOMBOS][2];             //[0]=position in act2, [1]=original tile
extern word animated_combo_table4[MAXCOMBOS][2];            //[0]=combo, [1]=clock
extern word animated_combos;
extern word animated_combo_table2[MAXCOMBOS][2];             //[0]=position in act2, [1]=original tile
extern word animated_combo_table24[MAXCOMBOS][2];            //[0]=combo, [1]=clock
extern word animated_combos2;
extern bool blank_tile_table[NEWMAXTILES];                  //keeps track of blank tiles
extern bool blank_tile_quarters_table[NEWMAXTILES*4];       //keeps track of blank tiles
*/
extern bool ewind_restart;
extern word     msgclk, msgstr, enqueued_str, msgpos, msgptr, msg_count, msgcolour, msgspeed,msg_w,
       msg_h,
       msg_count,
       msgorig,
       msg_xpos,
       msg_ypos,
       cursor_x,
       cursor_y;
extern int16_t msg_margins[4];
extern int32_t prt_tile;
extern byte msgstr_layer;
extern byte prt_cset, prt_x, prt_y, prt_tw, prt_th, msg_shdtype, msg_shdcol;
extern bool msg_onscreen, msg_active,msgspace;
extern FONT	*msgfont;
extern word     door_combo_set_count;
extern word     introclk, intropos, dmapmsgclk, linkedmsgclk;
extern int16_t    lensclk;
extern int32_t     lensid;
extern int32_t    Bpos;
extern byte screengrid[22];
extern byte screengrid_layer[2][22];
extern byte ffcgrid[MAXFFCS/8];
extern volatile int32_t logic_counter;
#ifdef _SCRIPT_COUNTER
extern volatile int32_t script_counter;
#endif
extern bool halt;
extern bool screenscrolling;
extern int32_t jwin_pal[jcMAX];
extern int32_t gui_colorset;
extern int32_t fullscreen;
extern byte frame_rest_suggest, forceExit, zc_vsync;
extern byte zc_color_depth;
extern byte use_win32_proc, zasm_debugger, zscript_debugger; //windows only

#ifdef _SCRIPT_COUNTER
void update_script_counter();
#endif

extern PALETTE tempbombpal;
extern bool usebombpal;

extern int32_t slot_arg, slot_arg2;
extern char *SAVE_FILE;

extern int32_t homescr,currscr,frame,currmap,dlevel,warpscr,worldscr,scrolling_scr,scrolling_map;
extern int32_t newscr_clk,opendoors,currdmap,fadeclk,currgame,listpos;
extern int32_t lastentrance,lastentrance_dmap, prices[3],loadside, Bwpn, Awpn, Xwpn, Ywpn;
extern int32_t digi_volume,midi_volume,sfx_volume,emusic_volume,currmidi,hasitem,whistleclk,pan_style;
extern bool analog_movement;
extern int32_t joystick_index,Akey,Bkey,Skey,Lkey,Rkey,Pkey,Exkey1,Exkey2,Exkey3,Exkey4,Abtn,Bbtn,Sbtn,Mbtn,Lbtn,Rbtn,Pbtn,Exbtn1,Exbtn2,Exbtn3,Exbtn4,Quit;
extern uint32_t GameFlags;
extern int32_t js_stick_1_x_stick, js_stick_1_x_axis, js_stick_1_x_offset;
extern int32_t js_stick_1_y_stick, js_stick_1_y_axis, js_stick_1_y_offset;
extern int32_t js_stick_2_x_stick, js_stick_2_x_axis, js_stick_2_x_offset;
extern int32_t js_stick_2_y_stick, js_stick_2_y_axis, js_stick_2_y_offset;
extern int32_t DUkey, DDkey, DLkey, DRkey, DUbtn, DDbtn, DLbtn, DRbtn, ss_after, ss_speed, ss_density, ss_enable;
extern int32_t hs_startx, hs_starty, hs_xdist, hs_ydist, clockclk, clock_zoras[eMAXGUYS];
extern int32_t swordhearts[4], currcset, currspal6, currspal14, gfc, gfc2, pitx, pity, refill_what, refill_why;
extern int32_t heart_beep_timer, new_enemy_tile_start, nets, magicitem, nayruitem, title_version;
extern int32_t magiccastclk, castx, casty, quakeclk, wavy, df_x, df_y, nl1_x, nl1_y, nl2_x, nl2_y, magicdrainclk, conveyclk, memrequested;
extern byte newconveyorclk;
extern dword fps_secs;
extern float avgfps;

extern bool cheats_execute_goto, cheats_execute_light;
extern bool blockmoving;
extern bool Throttlefps, MenuOpen, ClickToFreeze, Paused, Saving, Advance, ShowFPS, Showpal, Playing, FrameSkip, TransLayers, clearConsoleOnLoad, disableClickToFreeze, SaveDragResize, DragAspect, SaveWinPos;
extern int32_t LastWidth, LastHeight;
extern bool refreshpal,blockpath,__debug,loaded_guys,freeze_guys;
extern bool loaded_enemies,drawguys,details,debug_enabled,watch;
extern bool down_control_states[controls::btnLast];
extern bool F12,F11,F5,keyI, keyQ;
extern bool SystemKeys,NESquit,volkeys,useCD,boughtsomething;
extern bool fixed_door, darkroom,naturaldark,BSZ;            //,NEWSUBSCR;
extern bool hookshot_used, hookshot_frozen, pull_hero, hs_fix, hs_switcher, cheat_superman, gofast, checkhero;
extern bool ewind_restart, didpit, heart_beep, pausenow, castnext;
extern bool add_df1asparkle, add_df1bsparkle, add_nl1asparkle, add_nl1bsparkle, add_nl2asparkle, add_nl2bsparkle;
extern bool is_on_conveyor, activated_timed_warp;
extern int32_t hooked_combopos, switchhook_cost_item;
extern int32_t is_conveyor_stunned;
extern uint16_t hooked_layerbits;
extern int32_t hooked_undercombos[14];
extern solid_object* switching_object;

extern byte COOLSCROLL;

extern int32_t SnapshotFormat, NameEntryMode;

extern int32_t add_asparkle, add_bsparkle;

extern bool show_layer_0, show_layer_1, show_layer_2, show_layer_3, show_layer_4, show_layer_5, show_layer_6, show_layer_over, show_layer_push, show_sprites, show_ffcs, show_hitboxes, show_walkflags, show_ff_scripts, show_effectflags;

extern int32_t    cheat_goto_dmap, cheat_goto_screen;
extern char   cheat_goto_dmap_str[4];
extern char   cheat_goto_screen_str[3];
extern int16_t  visited[6];
extern byte   guygrid[176];
extern byte   guygridffc[MAXFFCS];
extern mapscr tmpscr[2];
extern mapscr tmpscr2[6];
extern mapscr tmpscr3[6];
extern char   sig_str[44];
extern script_data *ffscripts[NUMSCRIPTFFC];
extern script_data *itemscripts[NUMSCRIPTITEM];
extern script_data *globalscripts[NUMSCRIPTGLOBAL];
extern script_data *genericscripts[NUMSCRIPTSGENERIC];
extern script_data *guyscripts[NUMSCRIPTGUYS];
extern script_data *wpnscripts[NUMSCRIPTWEAPONS];
extern script_data *lwpnscripts[NUMSCRIPTWEAPONS];
extern script_data *ewpnscripts[NUMSCRIPTWEAPONS];
extern script_data *playerscripts[NUMSCRIPTPLAYER];
extern script_data *screenscripts[NUMSCRIPTSCREEN];
extern script_data *dmapscripts[NUMSCRIPTSDMAP];
extern script_data *itemspritescripts[NUMSCRIPTSITEMSPRITE];
extern script_data *comboscripts[NUMSCRIPTSCOMBODATA];

extern SAMPLE customsfxdata[WAV_COUNT];
extern int32_t sfxdat;

struct ScriptOwner
{
	ScriptOwner();
	byte scriptType;
	uint32_t ownerUID;
	bool specOwned;
	bool specCleared;
	void clear();
};

#define NUM_ZSCRIPT_ARRAYS	4096
typedef ZCArray<int32_t> ZScriptArray;
extern ZScriptArray localRAM[NUM_ZSCRIPT_ARRAYS];
extern std::map<int32_t,ZScriptArray> objectRAM;
extern ScriptOwner arrayOwner[NUM_ZSCRIPT_ARRAYS];

dword getNumGlobalArrays();

extern int32_t  resx,resy,scrx,scry;
extern int32_t window_width, window_height;

extern bool toogam;
extern bool ignoreSideview;

extern int32_t cheat;                                           // 0 = none; 1,2,3,4 = cheat level
extern int32_t maxcheat;

extern int32_t  mouse_down;                                     // used to hold the last reading of 'gui_mouse_b()' status
extern int32_t idle_count, active_count;
extern char *qstpath;
extern char *qstdir;
extern gamedata *saves;
extern gamedata *game;

extern std::string load_qstpath;
extern char header_version_nul_term[10];

extern volatile int32_t lastfps;
extern volatile int32_t framecnt;
extern void throttleFPS();

// quest file data
extern zquestheader QHeader;
extern byte                quest_rules[QUESTRULES_NEW_SIZE];
extern byte                extra_rules[EXTRARULES_SIZE];
extern byte                midi_flags[MIDIFLAGS_SIZE];
extern byte                music_flags[MUSICFLAGS_SIZE];
extern word                map_count;
extern MsgStr              *MsgStrings;
extern int32_t				   msg_strings_size;
extern DoorComboSet        *DoorComboSets;
extern dmap                *DMaps;
extern miscQdata           QMisc;
extern std::vector<mapscr> TheMaps;
extern zcmap               *ZCMaps;
extern byte                *quest_file;

/**********************************/
/*********** Misc Data ************/
/**********************************/

extern const char startguy[8];
extern const char gambledat[12*6];
extern const byte stx[4][9];
extern const byte sty[4][9];
extern const byte ten_rupies_x[10];
extern const byte ten_rupies_y[10];
extern zctune tunes[MAXMIDIS];
//extern zcmidi_ tunes[MAXMIDIS];
//extern emusic enhancedMusic[MAXMUSIC];

#endif

/*** end of zelda.h ***/

