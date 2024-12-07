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
	

}

void init7Seg2(){
	a2.w = 16;
    a2.h = 4;
    f2.w = 4;
    f2.h = 16;
    d2 = g2 = a2;
    b2 = c2 = e2 = f2;
   
    a2.x = d2.x = e2.x = f2.x = g2.x = SCREEN_WIDTH/2/2;
    b2.x = c2.x = a2.x + a2.w;
   
    a2.y = b2.y = f2.y = 0;
    c2.y = e2.y = g2.y = b2.y + b2.h - a2.h;
    d2.y = g2.y + b2.h - a2.h;

}

void draw7Seg2(uint16 color, uint8 value)
{
	//ex: value = 0111, 0111 & 0001 = 0001
	uint8 z = value & 1;
	uint8 y = value & 2;
	uint8 x = value & 4;
	uint8 w = value & 8;

	if(w || y || (x && z) || (!x && !z))
		drawRect(a2, color);
	
	if(!x || (!y && !z) || (y && z))
		drawRect(b2, color);

	if(x || !y || z)
		drawRect(c2, color);

    if(w || (y && !z) || (!x && !z) || (x && z &&!y)||(y && z && !x))
	    drawRect(d2, color);

	if ((y && !z) || (!x && !z))
		drawRect(e2,color);

	if (w || (!y&&!z) || (x&&!y) || (x && y && !z))
		drawRect(f2,color);

    if (w || (y && !z) || (x && !y) ||(y && z && !x))
        drawRect(g2,color);
	
}

int main()
{
	REG_DISPLAY = VIDEO_MODE | BG_MODE;
	
	init7Seg();
    init7Seg2();

uint8 counter = 0;
uint8 down = 0;

	
ball.x = 120;
	ball.y = 80;
	ball.w = 8;
	ball.h = 8;
	

player1.x = 0;
player1.y = 0;
player1.w = 8;
player1.h= 32;

player2.x = SCREEN_WIDTH - 8;
player2.y = 0;
player2.w = 8;
player2.h= 32;

uint16 ballColor = 0xe700;
  uint16 playerColor = 0x7f00;

  int ballSpeedX = 1;
  int ballSpeedY = 1;

  int playerSpeed = 2;

  prevBall = ball;
  prevPlayer1 = player1;

  prevPlayer2 = player2;

  uint8  score2 = 0;
  
	
	while(1)
	{
		sync();
		
		//This is erasing the previous frame
		draw7Seg(0x0000, 8);
		draw7Seg2(0x0000, 8);
		drawRect(prevBall, 0x0000);
		drawRect(prevPlayer1,0x0000 );
        drawRect(prevPlayer2,0x0000 );

        
	
    //player 1 controls
    if (!(REG_DISPLAY_INPUT & DOWN )) {
		player1.y += playerSpeed;
	}
    if (!(REG_DISPLAY_INPUT & UP)) {
		player1.y -= playerSpeed;
	}

	if (player1.y < 0) player1.y = 0;
    if (player1.y + player1.h > SCREEN_HEIGHT) player1.y = SCREEN_HEIGHT - player1.h;

    // player 2 controls
    if (!(REG_DISPLAY_INPUT & L )) {
		player2.y += playerSpeed;
	}
    if (!(REG_DISPLAY_INPUT & R)) {
		player2.y -= playerSpeed;
	}

	if (player2.y < 0) player2.y = 0;
    if (player2.y + player2.h > SCREEN_HEIGHT) player2.y = SCREEN_HEIGHT - player2.h;


    if (ball.x == player1.x + player1.w && ball.y < player1.y + player1.h && ball.y + ball.h > player1.y){
		ballSpeedX = -ballSpeedX;
	}
    if (ball.x + ball.w == player2.x && ball.y < player2.y + player2.h && ball.y + ball.h > player2.y) {
		ballSpeedX = -ballSpeedX;
	}

    //Ball movement
    ball.x += ballSpeedX;
    ball.y += ballSpeedY;
  
    if (ball.y + ball.h >= SCREEN_HEIGHT || ball.y <= 0){
		ballSpeedY = -ballSpeedY;
	}

    if (ball.x + ball.w >= SCREEN_WIDTH) {
    score2++;
			if(score2 > 9)
				score2 = 0;
	ball.x = SCREEN_WIDTH / 2;
	ball.y = SCREEN_WIDTH / 2;
	ballSpeedX = -1; 
    }
    if (ball.x <= 0) {
	counter++;
			if(counter > 9)
				counter = 0;
	ball.x = SCREEN_WIDTH / 2;
	ball.y = SCREEN_WIDTH / 2;
    ballSpeedX = 1;
    }

     //draw frame
	 drawRect(ball, ballColor);
     drawRect(player1, playerColor);
	 drawRect(player2, playerColor);
	 draw7Seg(0x0e30, counter);
	 draw7Seg2(0x0e30, score2);

 
     prevBall = ball;
     prevPlayer1 = player1;
     prevPlayer2 = player2;
    

	}
	return 0;
}