#Next Make CPW 2017 Simulator




#===========================================================================
#======= NOT TO EDIT =======================================================
#===========================================================================

#run "python3 -m pip install pygame" in terminal if pygame not found
import pygame, time, sys
from pygame.locals import *

pygame.init()
screen = pygame.display.set_mode((550, 400))
pygame.display.set_caption('Pygame Keyboard Test')
pygame.mouse.set_visible(0)


WHITE = 255,255,255
GREEN = 0,255,0
BLACK = 0,0,0
BLUE  = 0,0,255
RED   = 255,0,0

#1 if key pressed, 0 if not pressed
key = [0,0,0,0,0,0,0,0];
key_W = 0;
key_A = 1;
key_S = 2;
key_D = 3;
key_Up = 4;
key_Down = 5;
key_Left = 6;
key_Right = 7;

#The screen is a 7x5 array,
#this is the only item which should be modified
screenBuffer = [[0,0,0,0,0],
 [0,0,0,0,0],
 [0,0,0,0,0],
 [0,0,0,0,0],
 [0,0,0,0,0],
 [0,0,0,0,0],
 [0,0,0,0,0]];


def clearBuffer():
	global screenBuffer;
	for x in range(7):
		for y in range(5):
			screenBuffer[x][y] = 0;

dispCounter = 0;
def simulatorThings():
	global dispCounter;

	def drawScreen():
		global screenBuffer;
		for x in range(7):
			for y in range(5):
				if(screenBuffer[x][y] == 0):
					pygame.draw.circle(screen, RED, (50+75*x,350-75*y), 30, 3) #filled
				else:
					pygame.draw.circle(screen, RED, (50+75*x,350-75*y), 30, 0) #filled

	for event in pygame.event.get():  #praxis stuff
		if event.type==pygame.QUIT:
			sys.exit()
		elif event.type == KEYDOWN and event.key == K_ESCAPE:
			pygame.display.quit()
			sys.exit(0)
			
			
	#update key presses
	keys = pygame.key.get_pressed()  #checking pressed keys
	key[key_Up] = keys[pygame.K_UP];
	key[key_Down] = keys[pygame.K_DOWN];
	key[key_Right] = keys[pygame.K_RIGHT];
	key[key_Left] = keys[pygame.K_LEFT];
		
	key[key_W] = keys[pygame.K_w];
	key[key_S] = keys[pygame.K_s];
	key[key_D] = keys[pygame.K_d];
	key[key_A] = keys[pygame.K_a];
	
	if(dispCounter>=20):
		drawScreen();
		pygame.display.flip();
		dispCounter = 0;
	else:
		dispCounter = dispCounter + 1;
		
	
	#pygame.time.delay(1)
	
	screen.fill(BLACK);
	
	time.sleep(0.001);

#===========================================================================







#===========================================================================
#======= Test Programs =====================================================
#===========================================================================

#Test programs defined here
def keyTest():
	global screenBuffer;
	
	clearBuffer();
	
	screenBuffer[5][2] = key[key_Up];
	screenBuffer[5][0] = key[key_Down];
	screenBuffer[6][1] = key[key_Right];
	screenBuffer[4][1] = key[key_Left];
	
	screenBuffer[1][2] = key[key_W];
	screenBuffer[1][0] = key[key_S];
	screenBuffer[2][1] = key[key_D];
	screenBuffer[0][1] = key[key_A];

charX = 0;
charY = 0;
timeout = 0;
def characterMove():
	global charX, charY, timeout

	clearBuffer();

	if(timeout>=100):
		if(key[key_W]):
			charY = charY + 1;
			timeout = 0;
		if(key[key_A]):
			charX = charX - 1;
			timeout = 0;
		if(key[key_S]):
			charY = charY - 1;
			timeout = 0;
		if(key[key_D]):
			charX = charX + 1;
			timeout = 0;
	else:
		timeout = timeout + 1;
		
	#check bounds
	if(charX<0):
		charX = 0;
	if(charX>6):
		charX = 6;
	if(charY<0):
		charY = 0;
	if(charY>4):
		charY = 4;
		
	#output character dot
	screenBuffer[charX][charY] = 1;

#===========================================================================






#===========================================================================
#======= Write your own Programs! ==========================================
#===========================================================================

#Note: your code will run every 1ms, no other timing mechanisms are allowed,
# so you cannot use time.sleep(). Instead make a counter variable and count
# time in milliseconds

#define game global variables here
yourVar = 0;
#define program here
def yourProgram():
	global yourVar;
	#modify the screenbuffer to draw things on the screen
	screenBuffer[0][0] = yourVar;
	#read from the keys pressed
	if(key[key_W]):
		yourVar = 1 - yourVar;


#===========================================================================






#===========================================================================
#======= Run the code ======================================================
#===========================================================================

#loop runs every 1ms
while True:
	
	keyTest();
	#characterMove();
	#yourProgram();
	
	#needed for simulation to run
	simulatorThings();

#===========================================================================


