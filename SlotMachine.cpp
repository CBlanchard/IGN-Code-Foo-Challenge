#include "DarkGDK.h"

//declare variables
int money = 1000, slot1 = 7, slot2 = 7, slot3 = 7, bet, winnings, win = 1, button = 1, mousePOS;
bool inGame = true, invalidBet = false, noBet = false, maxBet = false, regWin = false, restart = false;


void LoadFiles() //Load images and sounds
{
	//loads images
	dbLoadImage("Pictures/slot_machine.jpg", 1);
	dbLoadImage("Pictures/strawberry.jpg", 2);
	dbLoadImage("Pictures/bell.jpg", 3);
	dbLoadImage("Pictures/cherry.jpg", 4);
	dbLoadImage("Pictures/apple.jpg", 5);
	dbLoadImage("Pictures/star.jpg", 6);
	dbLoadImage("Pictures/seven.jpg", 7);
	dbLoadImage("Pictures/win.jpg", 8);
	dbLoadImage("Pictures/lose.jpg", 9);
	dbLoadImage("Pictures/slot_machineBetOne.jpg", 10);
	dbLoadImage("Pictures/slot_machineSpin.jpg", 11);
	dbLoadImage("Pictures/slot_machineBetMax.jpg", 12);

	//load music
	dbLoadMusic("Audio/casino.mid", 1);

	//load SFX
	dbLoadMusic("Audio/BEEPARCA.wav", 2);
	dbLoadMusic("Audio/BEEPMETA.wav", 3);
	dbLoadMusic("Audio/SIREN4.wav", 4);
	dbLoadMusic("Audio/lose.wav", 5);
	dbLoadMusic("Audio/tada2.wav", 6);
	dbLoadMusic("Audio/buyitem.wav", 7);
}

void DrawScreen() // set up the main screen and changes according to actions by player
{
	//background and three slots
	dbPasteImage(button,0,0);
	dbPasteImage(slot1, 87,130);
	dbPasteImage(slot2, 255,130);
	dbPasteImage(slot3, 423,130);

	//Text
	dbText(85,445,"2 of a Kind = 1x   2 7's = 2x");
	dbText(85,460,"3 of a kind = 3x   3 7's = 5x");
	dbText(185,75,dbStr(money));
	dbCenterText(385,340,dbStr(bet));
	dbCenterText(525,340,dbStr(winnings));
	if(invalidBet == true)
		dbCenterText(110,340, "Not enough money!");
	else if(noBet == true)
		dbCenterText(110,340, "No Bet Placed!");
	else if(maxBet == true)
		dbCenterText(110,340, "Max Bet Reached!");
	else if(money < 5 && bet == 0)
	{
		dbCenterText(114,340, "Out of Cash.  Game Over...");
		dbCenterText(319,75,"Press a Key to Restart");
	}
	else
		dbCenterText(110,340,"1 bet = $5 / Max Bet = 50");

	//win or lose display
	if (win == 1)
		dbPasteImage(8,235,317);
	else if (win == 2)
		dbPasteImage(9,235,317);
}

void WheelSpin() //Display spinning wheel animaton
{
	dbStopMusic(4);
	dbStopMusic(5);
	dbStopMusic(6);
	button = 1;
	money -= (bet * 5);
	winnings = 0;
	win = 0;
	dbLoopMusic(2);

	//spins 3 slots
	for(int x = 0; x < 75; x++)
	{
		slot1 = dbRND(5) + 2;
		slot2 = dbRND(5) + 2;
		slot3 = dbRND(5) + 2;
		dbCLS();
		DrawScreen();
		dbWait(1);
	}
	dbPlayMusic(3);

	//spins 2 slots
	for(int x = 0; x < 75; x++)
	{
		slot2 = dbRND(5) + 2;
		slot3 = dbRND(5) + 2;
		dbCLS();
		DrawScreen();
		dbWait(1);
	}
	dbStopMusic(3);
	dbPlayMusic(3);

	//spins 1 slot
	for(int x = 0; x < 100; x++)
	{
		slot3 = dbRND(5) + 2;
		dbCLS();
		DrawScreen();
		dbWait(1);
	}
	dbStopMusic(3);
	dbStopMusic(2);
	dbPlayMusic(3);
}

void CalcSpin() //Calculate winnings
{
	if(slot1 == slot2 || slot2 == slot3 || slot1 == slot3)
	{
		winnings = bet * 5;
		regWin = true;
		if(slot1 == 7 && slot2 == 7 || slot2 == 7 && slot3 == 7 || slot1 == 7 && slot3 == 7)
			winnings = 2 * (bet * 5);
	}
	if(slot1 == slot2 && slot2 == slot3)
	{
		winnings = 3 * (bet * 5);
		regWin = true;
		if (slot1 == 7 && slot2 == 7 && slot3 == 7)
		{
			winnings = 5 * (bet * 5);
			dbPlayMusic(4);
			regWin = false;
		}
	}
	money += winnings;
	bet = 0;
	if(winnings > 0)
		win = 1;
	else
		win = 2;
	dbStopMusic(3);
	if (winnings == 0)
		dbPlayMusic(5);
	if (regWin)
		dbPlayMusic(6);
}

void mousePOSCheck() //does action depending on where player clicked
{
	switch(mousePOS)
	{
		case 1: //spin button
			button = 11;
			dbCLS();
			DrawScreen();
			dbWait(100);
			if(bet > 0)
			{
				WheelSpin();
				CalcSpin();
			}
			else
				noBet = true;
			break;
		case 2: //bet one button
			button = 10;
			dbCLS();
			DrawScreen();
			for(int x = 0; x < 10; x++)
				dbWait(1);
			if(money >= ((bet + 1) * 5) && bet < 50)
			{
				bet += 1;
				dbPlayMusic(7);
			}
			else
			{
				if (money < ((bet + 1) * 5))
					invalidBet = true;
				if (bet == 50)
					maxBet = true;
			}
			break;
		case 3: //bet max button
			button = 12;
			dbCLS();
			DrawScreen();
			for(int x = 0; x < 10; x++)
				dbWait(1);
			if (bet == 50)
				maxBet = true;
			if(money >= 250)
			{
				bet = 50;
				dbPlayMusic(7);
			}
			else
				invalidBet = true;
			break;
		case 4: //exit button
			if(dbMouseX() > 16 && dbMouseX() < 79)
				inGame = false;
			break;
		default:
			break;
	}
	mousePOS = 0;
}


void DarkGDK() //main program
{
	dbMaximizeWindow();
	dbSetTextFont("Arial");
	dbSetTextSize(18);
	dbSetTextToBold();
	dbRandomize(dbTimer());
	LoadFiles();
	dbLoopMusic(1);
	while(inGame)
	{
		button = 1;
		dbCLS();
		DrawScreen();
		if(dbMouseClick()) //if player clicks mouse
		{
			noBet = false;
			invalidBet = false;
			maxBet = false;
			regWin = false;
			dbStopMusic(7);
			if(dbMouseY() > 382 && dbMouseY() < 427)
			{
				if(dbMouseX() > 255 && dbMouseX() < 369)
					mousePOS = 1;
				if(dbMouseX() > 46 && dbMouseX() < 159)
					mousePOS = 2;
				if(dbMouseX() > 471 && dbMouseX() < 583)
					mousePOS = 3;
			}
			if(dbMouseY() > 447 && dbMouseY() < 479)
					mousePOS = 4;
			mousePOSCheck();
		}
		if(money < 5)
		{
			restart = true;
			dbCLS();
			DrawScreen();
			dbWaitKey();
			money = 1000;
		}
		dbWait(1);
	}
	
}