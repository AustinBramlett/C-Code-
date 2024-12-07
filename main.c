//main.c

#include "gba.h"

void sync()
{
	while(REG_DISPLAY_VCOUNT >= 160);
	while(REG_DISPLAY_VCOUNT < 160);
}

void drawRect(struct Rect r, uint16 color)
{
	for(int i = 0; i < r.h; i++)
	{
		for(int j = 0; j < r.w; j++)
		{
			SCREEN_BUFFER[SCREEN_WIDTH * (i + r.y) + (j + r.x)] = color;
		}
	}
}

void init7Seg()
{
	//Initialize my rectangles into the correct spots
	a.w = 16;
	a.h = 4;
	f.w = 4;
	f.h = 16;
	d = g = a;
	b = c = e = f;
	
	a.x = d.x = e.x = f.x = g.x = SCREEN_WIDTH/2;
	b.x = c.x = a.x + a.w;
	
	a.y = b.y = f.y = 0;
	c.y = e.y = g.y = b.y + b.h - a.h;
	d.y = g.y + b.h - a.h;
}

void draw7Seg(uint16 color, uint8 value)
{
	//ex: value = 0111, 0111 & 0001 = 0001
	uint8 z = value & 1;
	uint8 y = value & 2;
	uint8 x = value & 4;
	uint8 w = value & 8;
	
	//z = (value & 1) ? 1 : 0;
	
	//Boolean expression we got from the kmap for segment A
	if(w || y || (x && z) || (!x && !z))
		drawRect(a, color);
	
	if(!x || (!y && !z) || (y && z))
		drawRect(b, color);

	if(x || !y || z)
		drawRect(c, color);

    if(w || (y && !z) || (!x && !z) || (x && z &&!y)||(y && z && !x))
	    drawRect(d, color);

	if ((y && !z) || (!x && !z))
		drawRect(e,color);

	if (w || (!y&&!z) || (x&&!y) || (x && y && !z))
		drawRect(f,color);

    if (w || (y && !z) || (x && !y) ||(y && z && !x))
        drawRect(g,color);
	/*
	drawRect(d, color);
	drawRect(e, color);
	drawRect(f, color);
	drawRect(g, color);
	*/
}

int main()
{
	REG_DISPLAY = VIDEO_MODE | BG_MODE;
	
	init7Seg();
	
	uint8 counter = 0;
	uint8 down = 0;
	
	ball.x = 120;
	ball.y = 80;
	ball.w = 8;
	ball.h = 8;
	
	uint16 ballColor = 0xe700;
	
	int ballSpeedX = 1;
	int ballSpeedY = 1;
	
	prevBall = ball;
	
	while(1)
	{
		sync();
		
		//This is erasing the previous frame
		draw7Seg(0x0000, 8);
		drawRect(prevBall, 0x0000);
		
		if(!(REG_DISPLAY_INPUT & A) && !down)
		{
			//counter = (counter > 9) ? 0:counter + 1;
			counter++;
			if(counter > 9)
				counter = 0;
			down = 1;
		}
		else if((REG_DISPLAY_INPUT & A))
		{
			down = 0;
		}
		
		ball.x += ballSpeedX;
		ball.y += ballSpeedY;
		
		if(ball.y + ball.h >= SCREEN_HEIGHT || ball.y <= 0)
		{
			ballSpeedY = -ballSpeedY;
		}
		
		if(ball.x + ball.w >= SCREEN_WIDTH || ball.x <= 0)
		{
			ballSpeedX = -ballSpeedX;
		}
		
		prevBall = ball;
		//This is drawing the current frame
		drawRect(ball, ballColor);
		draw7Seg(0x0e30, counter);

	}
	
	return 0;
}