#ifndef _PC_GBI_H_
#define _PC_GBI_H_

#include <stdint.h>
#include <stddef.h>

// Basic GBI Types for PC (64-bit safe)
// We use 2x uintptr_t to ensure pointers fit in w1.
typedef struct {
    uintptr_t w0;
    uintptr_t w1;
} Gwords;

typedef union {
    Gwords words;
    long long int force_structure_alignment;
} Gfx;

// Helper Macros
#define _SHIFTL(v, s, w) ((((unsigned long)(v)) & ((0x01UL << (w)) - 1)) << (s))
#define _SHIFTR(v, s, w) ((((unsigned long)(v)) >> (s)) & ((0x01UL << (w)) - 1))

// OpCodes (F3DEX_GBI_2 compatible where possible)
#define	G_SPNOOP		0x00
#define	G_VTX			0x01
#define	G_TRI1			0x05
#define	G_TRI2			0x06
#define	G_DL			0xde
#define	G_ENDDL			0xdf
#define	G_SETTIMG		0xfd
#define	G_SETOTHERMODE_L	0xe2
#define	G_SETOTHERMODE_H	0xe3
#define G_LOADBLOCK     0xf3

// Macros adapted for PC Runtime generation

// DMA (Address based) - Safe for 64-bit pointers
#define	gDma1p(pkt, c, s, l, p)						\
{									\
	Gfx *_g = (Gfx *)(pkt);						\
	_g->words.w0 = (_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) |	\
			_SHIFTL((l), 0, 16));				\
	_g->words.w1 = (uintptr_t)(s);				\
}

// Static DMA - Safe for 64-bit pointers
#define	gsDma1p(c, s, l, p)						\
{									\
	(_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | 			\
	 _SHIFTL((l), 0, 16)), 						\
        (uintptr_t)(s)						\
}

// Vertex
// Note: sizeof(Vtx) on PC might match N64 if packed correctly, but strict N64 sizeof is 16 bytes.
// Vtx in ultra64.h is union with long long, so it should be 16 bytes aligned.
#define	gSPVertex(pkt, v, n, v0) \
        gDma1p(pkt, G_VTX, v, sizeof(Vtx)*(n),((n)-1)<<4|(v0))

#define	gsSPVertex(v, n, v0) \
        gsDma1p(G_VTX, v, sizeof(Vtx)*(n), ((n)-1)<<4|(v0))

// Triangle
#define gSP1Triangle(pkt, v0, v1, v2, flag)				\
{									\
	Gfx *_g = (Gfx *)(pkt);						\
	_g->words.w0 = _SHIFTL(G_TRI1, 24, 8);				\
	_g->words.w1 = _SHIFTL((v0)*2,16,8)|_SHIFTL((v1)*2,8,8)|_SHIFTL((v2)*2,0,8); \
}

#define gsSP1Triangle(v0, v1, v2, flag)					\
{									\
	_SHIFTL(G_TRI1, 24, 8),						\
	_SHIFTL((v0)*2,16,8)|_SHIFTL((v1)*2,8,8)|_SHIFTL((v2)*2,0,8)	\
}

// End DL
#define gSPEndDisplayList(pkt)						\
{									\
	Gfx *_g = (Gfx *)(pkt);						\
	_g->words.w0 = _SHIFTL(G_ENDDL, 24, 8);				\
	_g->words.w1 = 0;						\
}

#define gsSPEndDisplayList()						\
{									\
	_SHIFTL(G_ENDDL, 24, 8), 0					\
}

// Texture
#define gsDPSetTextureImage(fmt, siz, width, img) \
    { (_SHIFTL(G_SETTIMG, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width)-1, 0, 12)), (uintptr_t)(img) }

#define gDPSetTextureImage(pkt, fmt, siz, width, img) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->words.w0 = (_SHIFTL(G_SETTIMG, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width)-1, 0, 12)); \
      _g->words.w1 = (uintptr_t)(img); }

#endif // _PC_GBI_H_
