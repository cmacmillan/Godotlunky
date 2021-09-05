#pragma once
#include <string>

#define metaBlockWidth 13
#define metaBlockHeight 10
#define numMetaBlocksWidth 4
#define numMetaBlocksHeight 4
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
 0XXXXXPXXXXX0\n\
 XXXXXXXXXXXXX";

const string rightRecieverDrop1= 
"XXXXXXXXXXXXX\n\
 X000000000000\n\
 X000000000000\n\
 X000000000000\n\
 X000000000000\n\
 X000000000000\n\
 X000000000000\n\
 X0XXX000XXX00\n\
 XXXX00000XXX0\n\
 XXXXX000XXXXX";

const string startingPlatform1 = 
"XXXXXXXXXXXXX\n\
 0000000000000\n\
 0000000000000\n\
 000000X000000\n\
 00000XXX00000\n\
 0000XXXXX0000\n\
 000XXXXXXX000\n\
 00XXXXXXXXX00\n\
 0XXXXXXXXXXX0\n\
 XXXXXXXXXXXXX";

const string rightRecieverDrop[1] = { rightRecieverDrop1 };

const string dropRecievers[2] = { dropRecieverRight1,dropRecieverBi1 };

const string startingPlatforms[1] = { startingPlatform1 };

const string hallway[1] = { hallway1 };

const string hallwayDrop[1] = { hallwayDrop1};


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
