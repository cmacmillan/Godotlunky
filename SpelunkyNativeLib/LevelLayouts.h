#pragma once
#include "Common.h"
#include <string>

#define metaBlockWidth 13
#define metaBlockHeight 10
#define numMetaBlocksWidth 5
#define numMetaBlocksHeight 5
#define numMetaBlocks (numMetaBlocksHeight*numMetaBlocksHeight)

const string dropRecieverBi1 = 
"XXXXX000XXXXX\n\
 0000000000000\n\
 0000000000000\n\
 0000002000000\n\
 0000022200000\n\
 00002XXX20000\n\
 0002XXXXX2000\n\
 002XXXXXXX200\n\
 02XXXXXXXXX20\n\
 XXXXXXXXXXXXX";

/*
const string dropRecieverBi1 = 
"XXXXX000XXXXX\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 XXXXXXXXXXXXX";
 */

//spikes
const string dropRecieverBi2 = 
"XXXXX000XXXXX\n\
 0000000000000\n\
 00000XXX00000\n\
 000000000000\n\
 0077000007770\n\
 02XX00200XXX0\n\
 0000000000000\n\
 0000000000000\n\
 7777777777777\n\
 XXXXXXXXXXXXX";

const string dropRecieverBi3 = 
"XXXXX000XXXXX\n\
 222XX000XX222\n\
 0022200022200\n\
 002220000X200\n\
 00222000X220\n\
 0022200022200\n\
 0002200022200\n\
 0000000000002\n\
 X00000020000X\n\
 XXXXXXXXXXXXX";

const string dropRecieverBi4 = 
"XXXXX000XXXXX\n\
 2222X000000X0\n\
 0002XX2XX20X0\n\
 0000000000000\n\
 00000000X2200\n\
 0000000000000\n\
 00002X2000000\n\
 0000000000000\n\
 XX00777777700\n\
 XXXXXXXXXXXXX";

const string dropRecieverRight1 = 
"XXXXX000XXXXX\n\
 XXXXX00000000\n\
 XXXX220000000\n\
 XXXX222000000\n\
 0000022X00000\n\
 0000XXXX20000\n\
 000XXXXXXX000\n\
 00XXXXXXXX200\n\
 XXXXXXXXXXXX0\n\
 XXXXXXXXXXXXX";

const string hallwayDrop1 = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 00X0000000X00\n\
 0XXX00000XXX0\n\
 XXXXX000XXXXX";

const string hallway1 = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 00XSSS0SSSX00\n\
 0XXXXXWXXXXX0\n\
 XXXXXXXXXXXXX";

//updown
 const string hallway2 = 
"XXXXXXXXXXXXX\n\
 000000X000000\n\
 000000X000000\n\
 00X0002000000\n\
 0X20002000X00\n\
 00XX00X000X00\n\
 0XXX00000XXX0\n\
 XXXXX0000XXX0\n\
 XXXXX777XXXXX\n\
 XXXXXXXXXXXXX";

//roofprize
 const string hallway3 = 
"XXXXXXXXXXXXX\n\
 0002X2P2X0000\n\
 0002X2XX2X000\n\
 0002X2X000000\n\
 000X0X0020000\n\
 000XX00222000\n\
 000X000222000\n\
 0000000000000\n\
 00777777770X0\n\
 XXXXXXXXXXXXX";

//pit
const string hallway4 = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000000\n\
 =0XX00000XX0=\n\
 00XX0220XX000\n\
 00XX00000XXX0\n\
 00XXWWWWWXX00\n\
 0XXXXXXXXXX00\n\
 XXXXXXPXXXXXX\n\
 XXXXXXXXXXXXX";

//vertical bar
const string hallway5 = 
"XXXXXXXXXXXXX\n\
 000000000XX00\n\
 0000000000000\n\
 0000000000X00\n\
 000000000XXX0\n\
 0000000X0XXXX\n\
 00000XXX0XX00\n\
 00XX00000XXXX\n\
 0XXX72007XX00\n\
 XXXXXXXXXXXXX";

//maze
const string hallway6 = 
"XXXXXXXXXXXXX\n\
 0XX0X0000XXXX\n\
 220000X200022\n\
 0XX00X22X000X\n\
 0000200000000\n\
 00XX0000XXXX0\n\
 02200000X0000\n\
 0XXXXX00X0XX0\n\
 000002X000XXX\n\
 XXXXXXXXXXXXX";

//platforms
const string hallway7 = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0XX0000000000\n\
 00000000000XX\n\
 0000020000000\n\
 0020X2XX00200\n\
 0XX00020002X0\n\
 0000000000000\n\
 WWWWWWWWWWWWW\n\
 XXXXXXXXXXXXX";

//challenge
const string hallway8 = 
"XXXXXXXXXXXXX\n\
 X00000000XXXX\n\
 X000000000pXX\n\
 00000000XXXXX\n\
 XX000000000XX\n\
 XWWWWWWWWWWXX\n\
 0XXXXXXXXXXX0\n\
 0000000000000\n\
 0000000000000\n\
 XXXXXXXXXXXXX";

const string sideRoom1 = 
"XXXXX222XXXXX\n\
 2200000000000\n\
 XXX0000000000\n\
 XXX00000XXXX0\n\
 XXX00000XXXX0\n\
 XXX000XXXX000\n\
 XXX000XXXX000\n\
 XXXX0000XX000\n\
 000000000XXX0\n\
 XXXXXX222XXXX";

const string sideRoom2 = 
"XXXXX222XXXXX\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 00XXXXXXX0000\n\
 0000000XXX000\n\
 00000000XXX00\n\
 00000000XXXX0\n\
 XXXXX222XXXXX";

const string sideRoom3 = 
"XXXXX222XXXXX\n\
 0000000000002\n\
 XXXXXXXXX0022\n\
 XXX00000000XX\n\
 XXX000000X0XX\n\
 XXX000000X0XX\n\
 XXXXXXXXX00XX\n\
 0000000000002\n\
 XXXXX000XXXXX\n\
 XXXXX222XXXXX";

const string sideRoom4 = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 X00000000000X\n\
 XX000000000XX\n\
 XXX0000000XXX\n\
 XXXX2000XXXX\n\
 XXXXX222XXXXX";

const string sidePrizeRoom =
"XXXXXXXXXXXXX\n\
 XXXXXXXXXXXXX\n\
 XXX0000000XXX\n\
 XXX00W0W00XXX\n\
 XXX00XpX00XXX\n\
 XXX00XXX00XXX\n\
 XXX0000000XXX\n\
 XXXWWWWWWWXXX\n\
 XXXXXXXXXXXXX\n\
 XXXXXXXXXXXXX";

const string sideRoom6 = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 00000XXXX0000\n\
 0X00000000000\n\
 0XX00000000XX\n\
 0000XXX000000\n\
 000000000XX00\n\
 00XXX00000000\n\
 0000000000000\n\
 00000XXX00000";

const string sideRoom7 = 
"XXXXXXXXXXXXX\n\
 0XXXXXXXXXXX0\n\
 00XXXXXXXXX00\n\
 000XXXXXXX000\n\
 0000022222000\n\
 0002022220000\n\
 000XX7777200\n\
 00XXXXXXXXX0\n\
 0XXXXXXXXXXX0\n\
 XXXXXXXXXXXXX";

const string sideRoomX = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 XXXXXXXXXXXXX";

const string rightRecieverDrop1= 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 00XXX000XXX00\n\
 XXXX00000XXX0\n\
 XXXXX000XXXXX";

const string startingPlatform1 = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000000\n\
 000000Q000000\n\
 00000XXX00000\n\
 00002XXX20000\n\
 000XXXXX2X000\n\
 002XXXXXXX200\n\
 0XXXXXXXXXXX0\n\
 XXXXXXXXXXXXX";

/*
const string startingPlatform1 = 
"XXXXXXXXXXXXX\n\
 000000Q000000\n\
 000000X000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 XXXXXXXXXXXXX";
*/

const string endingPlatform1= 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 000000Q0O0000\n\
 000XXXXXXX000\n\
 00XXXXXXXXX00\n\
 0XXXXXXXXXXX0\n\
 XXXXXXXXXXXXX";

const string finalBossLevel =
"000000XXXXX000000000000000XXXX00XXX00000000000000000000000XX0000000\n\
 0000000XXX0000000000000000XPX0000X000000000000000000000000XX0000000\n\
 00000000X00000000000000000XXXX0000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 0000000000000000000000000000000090000000000000000000000000000000000\n\
 0000000000000000000000000000000000000000000000000000000000000000000\n\
 00000000000000rk0000000000000000000000000000000000000000O0000000000\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\
";

#define rightRecieverDropLength 1
const string rightRecieverDrop[rightRecieverDropLength] = { rightRecieverDrop1 };

#define dropRecieverLength 5
const string dropRecievers[dropRecieverLength] = { dropRecieverRight1,dropRecieverBi1,dropRecieverBi2,dropRecieverBi3,dropRecieverBi4 };

#define startingPlatformsLength 1
const string startingPlatforms[startingPlatformsLength] = { startingPlatform1 };

#define endingPlatformsLength 1
const string endingPlatforms[endingPlatformsLength] = { endingPlatform1 };

#define hallwayLength 8
const string hallway[hallwayLength] = { hallway1,hallway2,hallway3,hallway4,hallway5,hallway6,hallway7,hallway8 };

#define sideRoomLength 6
const string sideRooms[sideRoomLength] = { sideRoom1,sideRoom2,sideRoom3,sideRoom4,sideRoom6,sideRoom7};

#define hallwayDropLength 1
const string hallwayDrop[hallwayDropLength] = { hallwayDrop1};


//0 Nothing
//X Block
//S Snake
//W Spike
//R Rock
//G Shotgun
//P PrizeBox (or nothing chance)
//p PrizeBox (100%)
//r SmallRopePile
//K LargeBombBox
//k SmallBombBox
//$ LargeGoldPile
//B Bat
//M Spider
//Q entrance/exit
//O door button
//9 godolmec
//= rope
//7 50% spike block
//2 50% block
//T roof item w/chance of nothing
//g chance of snake
