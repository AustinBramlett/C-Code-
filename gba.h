//gba.h

#define SCREEN_WIDTH		240
#define SCREEN_HEIGHT		160

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;

#define MEMIO				0x04000000
#define VRAM				0x06000000

#define VIDEO_MODE			0x0003
#define BG_MODE				0x0400

#define REG_DISPLAY			(*(volatile uint32 *) (MEMIO))
#define REG_DISPLAY_VCOUNT	(*(volatile uint32 *) (MEMIO + 0x0006))
#define REG_DISPLAY_INPUT	(*(volatile uint32 *) (MEMIO + 0x0130))

#define A		1
#define B		2
#define SELECT	4
#define START	8
#define RIGHT	16
#define LEFT	32
#define UP		64
#define DOWN	128
#define R 		256
#define L 		512

#define SCREEN_BUFFER	((volatile uint16 *) VRAM)

struct Rect {
	int x, y, w, h;
}
 a,b,c,d,e,f,g,a2,b2,c2,d2,e2,f2,g2,
player1,prevPlayer1,ball,prevBall,player2,prevPlayer2;