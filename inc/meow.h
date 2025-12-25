#ifndef MEOW_H
#define MEOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/*
* args:
* buffer - pointer to buffer where to meow
* randInt - pointer to the function that returns random uint16_t in [a; b]*
* randFloat - pointer to the function thath returns random float int [a; b]
*
* there is no buffer overflow check here
* give the buffer of sufficient length, you are responsible
* in the default config, a single call to generateMeowString takes up 15 bytes of buffer space in the worst case
*/
void generateMeowString(char* buffer, uint16_t (*randInt)(uint16_t, uint16_t));

#ifdef __cplusplus
}
#endif

#endif