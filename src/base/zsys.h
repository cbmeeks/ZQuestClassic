//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  zsys.h
//
//  System functions, etc.
//
//--------------------------------------------------------

#if defined(ALLEGRO_WINDOWS)
#include <conio.h>
#endif

#ifndef _ZSYS_H_
#define _ZSYS_H_

#include "base/zdefs.h"
#include "jwin.h"
#include "zsyssimple.h"

#include <functional>

#define FILENAME8_3   0
#define FILENAME8__   1
#define FILENAMEALL   2

extern int32_t jwin_pal[jcMAX];

extern bool is_large;
extern void large_dialog(DIALOG *d);

extern volatile int32_t dclick_status, dclick_time;

char *time_str_short(dword time);
char *time_str_short2(dword time);
char *time_str_med(dword time);
char *time_str_long(dword time);

void extract_name(char const* path,char *name,int32_t type);
void temp_name(char temporaryname[]);
char *zc_make_relative_filename(char *dest, const char *path, const char *filename, int32_t size);
void chop_path(char *path);
int32_t  used_switch(int32_t argc,char *argv[],const char *s);
bool isinRect(int32_t x,int32_t y,int32_t rx1,int32_t ry1,int32_t rx2,int32_t ry2);

extern char zeldapwd[8];
extern char zquestpwd[8];
extern char datapwd[8];
extern char zcheat[4][8];

void resolve_password(char *pwd);

bool decode_007(byte *buf, dword size, dword key, word check1, word check2, int32_t method);
void encode_007(byte *buf, dword size, dword key, word *check1, word *check2, int32_t method);
int32_t encode_file_007(const char *srcfile, const char *destfile, int32_t key, const char *header, int32_t method);
int32_t decode_file_007(const char *srcfile, const char *destfile, const char *header, int32_t method, bool packed, const char *password);
void copy_file(const char *src, const char *dest);

int32_t  get_bit(byte const* bitstr,int32_t bit);
void set_bit(byte *bitstr,int32_t bit,byte val);
bool toggle_bit(byte *bitstr,int32_t bit);

int32_t  get_bitl(int32_t bitstr,int32_t bit);
void set_bitl(int32_t bitstr,int32_t bit,byte val);

[[noreturn]] void Z_error_fatal(const char *format,...);
void Z_error(const char *format,...);
void Z_title(const char *format,...);
void set_should_zprint_cb(std::function<bool()> cb);
void zprint(const char * const format,...);
void zprint2(const char * const format,...);

int32_t anim_3_4(int32_t clk, int32_t speed);

int32_t bound(int32_t &x,int32_t low,int32_t high);

enum {ssfmtBMP, ssfmtGIF, ssfmtJPG, ssfmtPNG, ssfmtPCX, ssfmtTGA, ssfmtMAX };
extern char *snapshotformat_str[ssfmtMAX][2];
extern const char *snapshotformatlist(int32_t index, int32_t *list_size);


/*0=normal, 1=jwin*/
void box_save_x();
void box_load_x();
void box_pause();
void dither_rect(BITMAP *bmp, PALETTE *pal, int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                 int32_t src_color1, int32_t src_color2, byte dest_color1,
                 byte dest_color2);
void dclick_check(void);
void lock_dclick_function();

void textout_shadow_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_center_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_right_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_u_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_center_u_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_right_u_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_o_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_center_o_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_right_o_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_plus_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_center_plus_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_right_plus_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_x_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_center_x_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadow_right_x_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t shadow, int32_t bg);
void textout_shadowed_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_center_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_right_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_u_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_center_u_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_right_u_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_o_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_center_o_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_right_o_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);



void textout_shadowed_plus_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_center_plus_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_right_plus_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_x_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_center_x_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);
void textout_shadowed_right_x_ex(BITMAP *bmp, const FONT *f, const char *s, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg);




void textprintf_shadow_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_center_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_right_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_u_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_center_u_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_right_u_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_o_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_center_o_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_right_o_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);





void textprintf_shadow_plus_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_center_plus_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_right_plus_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_x_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_center_x_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadow_right_x_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t shadow, int32_t bg, const char *format, ...);



void textprintf_shadowed_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_center_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_right_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_u_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_center_u_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_right_u_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_o_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_center_o_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_right_o_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);


void textprintf_shadowed_plus_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_center_plus_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_right_plus_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_x_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_center_x_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);
void textprintf_shadowed_right_x_ex(BITMAP *bmp, const FONT *f, int32_t x, int32_t y, int32_t color, int32_t shadow, int32_t bg, const char *format, ...);

void copy_dialog(DIALOG **to, DIALOG *from);
void free_dialog(DIALOG **dlg);

void safe_rect(BITMAP *bmp, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t color);

int32_t gcd(int32_t a, int32_t b);
int32_t lcm(int32_t a, int32_t b);

size_t count_digits(int32_t n);

void quit_game();

int32_t zc_trace_handler(const char *);
void zc_trace_clear();

void sane_destroy_bitmap(BITMAP **bmp);

#endif                                                      // _ZSYS_H_
