/*
 * fonts.h
 *
 *  Created on: Jun 26, 2024
 *      Author: jeanh
 */

#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
#define FONT1 Arial_Narrow8x12
#define FONT2 Arial_Narrow10x13
#define FONT3 Arial_Narrow12x16
#define FONT4 Arial_Narrow15x19

    extern const uint8_t Arial_Narrow8x12[];
    extern const uint8_t Arial_Narrow10x13[];
    extern const uint8_t Arial_Narrow12x16[];
    extern const uint8_t Arial_Narrow15x19[];

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // __FONTS_H__
