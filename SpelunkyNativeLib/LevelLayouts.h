#pragma once
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
 000000X000000\n\
 00000XXX00000\n\
 0000XXXXX0000\n\
 000XXXXXXX000\n\
 00XXXXXXXXX00\n\
 0XXXXXXXXXXX0\n\
 XXXXXXXXXXXXX";

const string dropRecieverRight1 = 
"XXXXX000XXXXX\n\
 XXXXX00000000\n\
 XXXXXX0000000\n\
 XXXXXXX000000\n\
 X0000XXX00000\n\
 X000XXXXX0000\n\
 X00XXXXXXX000\n\
 XPXXXXXXXXX00\n\
 XXXXXXXXXXXX0\n\
 XXXXXXXXXXXXX";

const string hallwayDrop1 = 
"XXXXXXXXXXXXX\n\
 0000000M00000\n\
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
 0XXXXXPXXXXX0\n\
 XXXXXXXXXXXXX";

const string rightRecieverDrop1= 
"XXXXXXXXXXXXX\n\
 X00000B000000\n\
 X000000000000\n\
 X000000000000\n\
 X000000000000\n\
 X000000000000\n\
 X000000000000\n\
 X0XXX000XXX00\n\
 XXXX00000XXX0\n\
 XXXXX000XXXXX";

/*
const string startingPlatform1 = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000900\n\
 000000Q000000\n\
 0000RXXX00000\n\
 0000XXXXX0000\n\
 000XXXXXXX000\n\
 00XXXXXXXXX00\n\
 0XXXXXXXXXXX0\n\
 XXXXXXXXXXXXX";
*/

const string startingPlatform1 = 
"XXXXXXXXXXXXX\n\
 000000Q000000\n\
 000000X000000\n\
 0000000000000\n\
 0000000000000\n\
 0000000000000\n\
 0090000000G00\n\
 0000000000000\n\
 0000000000000\n\
 XXXXXXXXXXXXX";

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

#define rightRecieverDropLength 1
const string rightRecieverDrop[rightRecieverDropLength] = { rightRecieverDrop1 };

#define dropRecieverLength 2
const string dropRecievers[dropRecieverLength] = { dropRecieverRight1,dropRecieverBi1 };

#define startingPlatformsLength 1
const string startingPlatforms[startingPlatformsLength] = { startingPlatform1 };

#define endingPlatformsLength 1
const string endingPlatforms[endingPlatformsLength] = { endingPlatform1 };

#define hallwayLength 1
const string hallway[hallwayLength] = { hallway1 };

#define hallwayDropLength 1
const string hallwayDrop[hallwayDropLength] = { hallwayDrop1};


//0 Nothing
//X Block
//S Snake
//W Spike
//R Rock
//P PrizeBox
//r SmallRopePile
//K LargeBombBox
//k SmallBombBox
//$ LargeGoldPile
//B Bat
//M Spider
//Q entrance/exit
//O door button
//9 godolmec
