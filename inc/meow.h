#ifndef MEOW_H
#define MEOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/*
* args:
* buffer - pointer to buffer where to meow
* randInt - pointer to the function that returns random uint16_t in [a; b]*
* randFloat - pointer to the function thath returns random float int [a; b]
*/
void generateMeowString(char* buffer, uint16_t (*randInt)(uint16_t, uint16_t));

#ifdef __cplusplus
}
#endif

#endif