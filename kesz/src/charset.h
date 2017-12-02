#include <stdatomic.h>
#include <stdbool.h>

void ascii_2_unicode(char * ascii,unsigned short * uni);
void ascii_2_unicode_a(char * ascii,atomic_ushort * uni);
void utf8_2_unicode_a(char * utf8,atomic_ushort * uni);
void latin2_2_unicode_a(unsigned char *latin2, atomic_ushort *uni);
void win1250_2_unicode_a(unsigned char *win1250,atomic_ushort *uni);
void win1252_2_unicode_a(unsigned char *win1252,atomic_ushort *uni); 
void unicode_2_unicode_a(unsigned short *uni_nona,atomic_ushort *uni); 
void char_2_char_a(char * c,atomic_char *ca);
bool cmp_a_ushort(atomic_ushort *s1,atomic_ushort *s2);
void digit_2_uni(int d,unsigned short *uni);