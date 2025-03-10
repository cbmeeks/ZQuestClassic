#include "SymbolDefs.h"

CombosPtrSymbols CombosPtrSymbols::singleton = CombosPtrSymbols();

static AccessorTable CombosTable[] =
{
	//name,                       tag,            rettype,   var,               funcFlags,  params,optparams
	//Only as 'this->' in combo scripts
	{ "getX",                       0,         ZTID_FLOAT,   COMBOXR,                   0,  { ZTID_COMBOS },{} },
	{ "getY",                       0,         ZTID_FLOAT,   COMBOYR,                   0,  { ZTID_COMBOS },{} },
	{ "getPos",                     0,         ZTID_FLOAT,   COMBOPOSR,                 0,  { ZTID_COMBOS },{} },
	{ "getLayer",                   0,         ZTID_FLOAT,   COMBOLAYERR,               0,  { ZTID_COMBOS },{} },
	//
	{ "getInitD[]",                 0,       ZTID_UNTYPED,   COMBODATAINITD,            0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setInitD[]",                 0,          ZTID_VOID,   COMBODATAINITD,            0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getID",                      0,         ZTID_FLOAT,   COMBODATAID,               0,  { ZTID_COMBOS },{} },
	{ "getOriginalTile",            0,         ZTID_FLOAT,   COMBODOTILE,               0,  { ZTID_COMBOS },{} },
	{ "setOriginalTile",            0,          ZTID_VOID,   COMBODOTILE,               0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getFrame",                   0,         ZTID_FLOAT,   COMBODFRAME,               0,  { ZTID_COMBOS },{} },
	{ "setFrame",                   0,          ZTID_VOID,   COMBODFRAME,               0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getAClk",                    0,         ZTID_FLOAT,   COMBODACLK,                0,  { ZTID_COMBOS },{} },
	{ "setAClk",                    0,          ZTID_VOID,   COMBODACLK,                0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTile",                    0,         ZTID_FLOAT,   COMBODTILE,                0,  { ZTID_COMBOS },{} },
	{ "setTile",                    0,          ZTID_VOID,   COMBODTILE,                0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getScript",                  0,         ZTID_FLOAT,   COMBODATASCRIPT,           0,  { ZTID_COMBOS },{} },
	{ "setScript",                  0,          ZTID_VOID,   COMBODATASCRIPT,           0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getASpeed",                  0,         ZTID_FLOAT,   COMBODASPEED,              0,  { ZTID_COMBOS },{} },
	{ "setASpeed",                  0,          ZTID_VOID,   COMBODASPEED,              0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getFlip",                    0,         ZTID_FLOAT,   COMBODFLIP,                0,  { ZTID_COMBOS },{} },
	{ "setFlip",                    0,          ZTID_VOID,   COMBODFLIP,                0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getWalk",                    0,         ZTID_FLOAT,   COMBODWALK,                0,  { ZTID_COMBOS },{} },
	{ "setWalk",                    0,          ZTID_VOID,   COMBODWALK,                0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getEffect",                  0,         ZTID_FLOAT,   COMBODEFFECT,              0,  { ZTID_COMBOS },{} },
	{ "setEffect",                  0,          ZTID_VOID,   COMBODEFFECT,              0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getType",                    0,         ZTID_FLOAT,   COMBODTYPE,                0,  { ZTID_COMBOS },{} },
	{ "setType",                    0,          ZTID_VOID,   COMBODTYPE,                0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getCSet",                    0,         ZTID_FLOAT,   COMBODCSET,                0,  { ZTID_COMBOS },{} },
	{ "setCSet",                    0,          ZTID_VOID,   COMBODCSET,                0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getCSet2",                   0,         ZTID_FLOAT,   COMBODCSET,                0,  { ZTID_COMBOS },{} },
	{ "setCSet2",                   0,          ZTID_VOID,   COMBODCSET,                0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getCSet2Flags",              0,         ZTID_FLOAT,   COMBODCSET2FLAGS,          0,  { ZTID_COMBOS },{} },
	{ "setCSet2Flags",              0,          ZTID_VOID,   COMBODCSET2FLAGS,          0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getFrames",                  0,         ZTID_FLOAT,   COMBODFRAMES,              0,  { ZTID_COMBOS },{} },
	{ "setFrames",                  0,          ZTID_VOID,   COMBODFRAMES,              0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getNextData",                0,         ZTID_FLOAT,   COMBODNEXTD,               0,  { ZTID_COMBOS },{} },
	{ "setNextData",                0,          ZTID_VOID,   COMBODNEXTD,               0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getNextCSet",                0,         ZTID_FLOAT,   COMBODNEXTC,               0,  { ZTID_COMBOS },{} },
	{ "setNextCSet",                0,          ZTID_VOID,   COMBODNEXTC,               0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getFlag",                    0,         ZTID_FLOAT,   COMBODFLAG,                0,  { ZTID_COMBOS },{} },
	{ "setFlag",                    0,          ZTID_VOID,   COMBODFLAG,                0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getSkipAnimX",               0,         ZTID_FLOAT,   COMBODSKIPANIM,            0,  { ZTID_COMBOS },{} },
	{ "setSkipAnimX",               0,          ZTID_VOID,   COMBODSKIPANIM,            0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getSkipAnimY",               0,         ZTID_FLOAT,   COMBODAKIMANIMY,           0,  { ZTID_COMBOS },{} },
	{ "setSkipAnimY",               0,          ZTID_VOID,   COMBODAKIMANIMY,           0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getAnimFlags",               0,         ZTID_FLOAT,   COMBODANIMFLAGS,           0,  { ZTID_COMBOS },{} },
	{ "setAnimFlags",               0,          ZTID_VOID,   COMBODANIMFLAGS,           0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getAttributes[]",            0,         ZTID_FLOAT,   COMBODATTRIBUTES,          0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setAttributes[]",            0,          ZTID_VOID,   COMBODATTRIBUTES,          0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getAttribytes[]",            0,         ZTID_FLOAT,   COMBODATTRIBYTES,          0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setAttribytes[]",            0,          ZTID_VOID,   COMBODATTRIBYTES,          0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getAttrishorts[]",           0,         ZTID_FLOAT,   COMBODATTRISHORTS,         0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setAttrishorts[]",           0,          ZTID_VOID,   COMBODATTRISHORTS,         0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getTrigFlags[]",             0,          ZTID_BOOL,   COMBODTRIGGERFLAGS2,       0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setTrigFlags[]",             0,          ZTID_VOID,   COMBODTRIGGERFLAGS2,       0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_BOOL },{} },
	{ "getTriggerButton[]",         0,          ZTID_BOOL,   COMBODTRIGGERBUTTON,       0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setTriggerButton[]",         0,          ZTID_VOID,   COMBODTRIGGERBUTTON,       0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_BOOL },{} },
	{ "getFlags[]",                 0,          ZTID_BOOL,   COMBODUSRFLAGARR,          0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setFlags[]",                 0,          ZTID_VOID,   COMBODUSRFLAGARR,          0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_BOOL },{} },
	{ "getGenFlags[]",              0,          ZTID_BOOL,   COMBODGENFLAGARR,          0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setGenFlags[]",              0,          ZTID_VOID,   COMBODGENFLAGARR,          0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_BOOL },{} },
	{ "getTriggerLevel",            0,         ZTID_FLOAT,   COMBODTRIGGERLEVEL,        0,  { ZTID_COMBOS },{} },
	{ "setTriggerLevel",            0,          ZTID_VOID,   COMBODTRIGGERLEVEL,        0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTriggerItem",             0,         ZTID_FLOAT,   COMBODTRIGGERITEM,         0,  { ZTID_COMBOS },{} },
	{ "setTriggerItem",             0,          ZTID_VOID,   COMBODTRIGGERITEM,         0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTriggerTimer",            0,         ZTID_FLOAT,   COMBODTRIGGERTIMER,        0,  { ZTID_COMBOS },{} },
	{ "setTriggerTimer",            0,          ZTID_VOID,   COMBODTRIGGERTIMER,        0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTriggerSFX",              0,         ZTID_FLOAT,   COMBODTRIGGERSFX,          0,  { ZTID_COMBOS },{} },
	{ "setTriggerSFX",              0,          ZTID_VOID,   COMBODTRIGGERSFX,          0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTriggerChange",           0,         ZTID_FLOAT,   COMBODTRIGGERCHANGECMB,    0,  { ZTID_COMBOS },{} },
	{ "setTriggerChange",           0,          ZTID_VOID,   COMBODTRIGGERCHANGECMB,    0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTriggerProximity",        0,         ZTID_FLOAT,   COMBODTRIGGERPROX,         0,  { ZTID_COMBOS },{} },
	{ "setTriggerProximity",        0,          ZTID_VOID,   COMBODTRIGGERPROX,         0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTriggerLightBeam",        0,         ZTID_FLOAT,   COMBODTRIGGERLIGHTBEAM,    0,  { ZTID_COMBOS },{} },
	{ "setTriggerLightBeam",        0,          ZTID_VOID,   COMBODTRIGGERLIGHTBEAM,    0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTriggerCounter",          0,         ZTID_FLOAT,   COMBODTRIGGERCTR,          0,  { ZTID_COMBOS },{} },
	{ "setTriggerCounter",          0,          ZTID_VOID,   COMBODTRIGGERCTR,          0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTriggerCtrAmount",        0,         ZTID_FLOAT,   COMBODTRIGGERCTRAMNT,      0,  { ZTID_COMBOS },{} },
	{ "setTriggerCtrAmount",        0,          ZTID_VOID,   COMBODTRIGGERCTRAMNT,      0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTrigCooldown",            0,         ZTID_FLOAT,   COMBODTRIGGERCOOLDOWN,     0,  { ZTID_COMBOS },{} },
	{ "setTrigCooldown",            0,          ZTID_VOID,   COMBODTRIGGERCOOLDOWN,     0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTrigCopycat",             0,         ZTID_FLOAT,   COMBODTRIGGERCOPYCAT,      0,  { ZTID_COMBOS },{} },
	{ "setTrigCopycat",             0,          ZTID_VOID,   COMBODTRIGGERCOPYCAT,      0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTrigSpawnItemPickup",     0,         ZTID_FLOAT,   COMBODTRIGITEMPICKUP,      0,  { ZTID_COMBOS },{} },
	{ "setTrigSpawnItemPickup",     0,          ZTID_VOID,   COMBODTRIGITEMPICKUP,      0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTrigExState",             0,         ZTID_FLOAT,   COMBODTRIGEXSTATE,         0,  { ZTID_COMBOS },{} },
	{ "setTrigExState",             0,          ZTID_VOID,   COMBODTRIGEXSTATE,         0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTrigSpawnEnemy",          0,         ZTID_FLOAT,   COMBODTRIGSPAWNENEMY,      0,  { ZTID_COMBOS },{} },
	{ "setTrigSpawnEnemy",          0,          ZTID_VOID,   COMBODTRIGSPAWNENEMY,      0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTrigSpawnItem",           0,         ZTID_FLOAT,   COMBODTRIGSPAWNITEM,       0,  { ZTID_COMBOS },{} },
	{ "setTrigSpawnItem",           0,          ZTID_VOID,   COMBODTRIGSPAWNITEM,       0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTrigCSetChange",          0,         ZTID_FLOAT,   COMBODTRIGCSETCHANGE,      0,  { ZTID_COMBOS },{} },
	{ "setTrigCSetChange",          0,          ZTID_VOID,   COMBODTRIGCSETCHANGE,      0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftGFXCombo",            0,         ZTID_FLOAT,   COMBODLIFTGFXCOMBO,        0,  { ZTID_COMBOS },{} },
	{ "setLiftGFXCombo",            0,          ZTID_VOID,   COMBODLIFTGFXCOMBO,        0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftGFXCSet",             0,         ZTID_FLOAT,   COMBODLIFTGFXCCSET,        0,  { ZTID_COMBOS },{} },
	{ "setLiftGFXCSet",             0,          ZTID_VOID,   COMBODLIFTGFXCCSET,        0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftUnderCombo",          0,         ZTID_FLOAT,   COMBODLIFTUNDERCMB,        0,  { ZTID_COMBOS },{} },
	{ "setLiftUnderCombo",          0,          ZTID_VOID,   COMBODLIFTUNDERCMB,        0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftUnderCSet",           0,         ZTID_FLOAT,   COMBODLIFTUNDERCS,         0,  { ZTID_COMBOS },{} },
	{ "setLiftUnderCSet",           0,          ZTID_VOID,   COMBODLIFTUNDERCS,         0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftDamage",              0,         ZTID_FLOAT,   COMBODLIFTDAMAGE,          0,  { ZTID_COMBOS },{} },
	{ "setLiftDamage",              0,          ZTID_VOID,   COMBODLIFTDAMAGE,          0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftLevel",               0,         ZTID_FLOAT,   COMBODLIFTLEVEL,           0,  { ZTID_COMBOS },{} },
	{ "setLiftLevel",               0,          ZTID_VOID,   COMBODLIFTLEVEL,           0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftItem",                0,         ZTID_FLOAT,   COMBODLIFTITEM,            0,  { ZTID_COMBOS },{} },
	{ "setLiftItem",                0,          ZTID_VOID,   COMBODLIFTITEM,            0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftFlags[]",             0,          ZTID_BOOL,   COMBODLIFTFLAGS,           0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setLiftFlags[]",             0,          ZTID_VOID,   COMBODLIFTFLAGS,           0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_BOOL },{} },
	{ "getLiftGFXType",             0,         ZTID_FLOAT,   COMBODLIFTGFXTYPE,         0,  { ZTID_COMBOS },{} },
	{ "setLiftGFXType",             0,          ZTID_VOID,   COMBODLIFTGFXTYPE,         0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftGFXSprite",           0,         ZTID_FLOAT,   COMBODLIFTGFXSPRITE,       0,  { ZTID_COMBOS },{} },
	{ "setLiftGFXSprite",           0,          ZTID_VOID,   COMBODLIFTGFXSPRITE,       0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftSFX",                 0,         ZTID_FLOAT,   COMBODLIFTSFX,             0,  { ZTID_COMBOS },{} },
	{ "setLiftSFX",                 0,          ZTID_VOID,   COMBODLIFTSFX,             0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftBreakSprite",         0,         ZTID_FLOAT,   COMBODLIFTBREAKSPRITE,     0,  { ZTID_COMBOS },{} },
	{ "setLiftBreakSprite",         0,          ZTID_VOID,   COMBODLIFTBREAKSPRITE,     0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftBreakSFX",            0,         ZTID_FLOAT,   COMBODLIFTBREAKSFX,        0,  { ZTID_COMBOS },{} },
	{ "setLiftBreakSFX",            0,          ZTID_VOID,   COMBODLIFTBREAKSFX,        0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftHeight",              0,         ZTID_FLOAT,   COMBODLIFTHEIGHT,          0,  { ZTID_COMBOS },{} },
	{ "setLiftHeight",              0,          ZTID_VOID,   COMBODLIFTHEIGHT,          0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getLiftTime",                0,         ZTID_FLOAT,   COMBODLIFTTIME,            0,  { ZTID_COMBOS },{} },
	{ "setLiftTime",                0,          ZTID_VOID,   COMBODLIFTTIME,            0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	
	//Undocumented intentionally
	{ "_getPosX",                   0,         ZTID_FLOAT,   COMBOXR,                   0,  { ZTID_COMBOS },{} },
	{ "_getPosY",                   0,         ZTID_FLOAT,   COMBOYR,                   0,  { ZTID_COMBOS },{} },
	{ "_getPos",                    0,         ZTID_FLOAT,   COMBOPOSR,                 0,  { ZTID_COMBOS },{} },
	{ "_getLayer",                  0,         ZTID_FLOAT,   COMBOLAYERR,               0,  { ZTID_COMBOS },{} },
	{ "getSkipAnim",                0,         ZTID_FLOAT,   COMBODSKIPANIM,            0,  { ZTID_COMBOS },{} },
	{ "setSkipAnim",                0,          ZTID_VOID,   COMBODSKIPANIM,            0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getUserFlags",               0,         ZTID_FLOAT,   COMBODUSRFLAGS,            0,  { ZTID_COMBOS },{} },
	{ "setUserFlags",               0,          ZTID_VOID,   COMBODUSRFLAGS,            0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getExpansion[]",             0,         ZTID_FLOAT,   COMBODEXPANSION,           0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setExpansion[]",             0,          ZTID_VOID,   COMBODEXPANSION,           0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getNextTimer",               0,         ZTID_FLOAT,   COMBODNEXTTIMER,           0,  { ZTID_COMBOS },{} },
	{ "setNextTimer",               0,          ZTID_VOID,   COMBODNEXTTIMER,           0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "getTriggerFlags[]",          0,         ZTID_FLOAT,   COMBODTRIGGERFLAGS,        0,  { ZTID_COMBOS, ZTID_FLOAT },{} },
	{ "setTriggerFlags[]",          0,          ZTID_VOID,   COMBODTRIGGERFLAGS,        0,  { ZTID_COMBOS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	
	{ "",                           0,          ZTID_VOID,   -1,                        0,  {},{} }
};

CombosPtrSymbols::CombosPtrSymbols()
{
	table = CombosTable;
	refVar = REFCOMBODATA; //NUL;
}

void CombosPtrSymbols::generateCode()
{}

