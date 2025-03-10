#include "SymbolDefs.h"

ItemclassSymbols ItemclassSymbols::singleton = ItemclassSymbols();

static AccessorTable itemclassTable[] =
{
	//name,                       tag,            rettype,   var,               funcFlags,  params,optparams
	{ "getFamily",                  0,         ZTID_FLOAT,   ITEMCLASSFAMILY,           0,  { ZTID_ITEMCLASS },{} },
	{ "setFamily",                  0,          ZTID_VOID,   ITEMCLASSFAMILY,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getType",                    0,         ZTID_FLOAT,   ITEMCLASSFAMILY,           0,  { ZTID_ITEMCLASS },{} },
	{ "setType",                    0,          ZTID_VOID,   ITEMCLASSFAMILY,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getLevel",                   0,         ZTID_FLOAT,   ITEMCLASSFAMTYPE,          0,  { ZTID_ITEMCLASS },{} },
	{ "setLevel",                   0,          ZTID_VOID,   ITEMCLASSFAMTYPE,          0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getAmount",                  0,         ZTID_FLOAT,   ITEMCLASSAMOUNT,           0,  { ZTID_ITEMCLASS },{} },
	{ "setAmount",                  0,          ZTID_VOID,   ITEMCLASSAMOUNT,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getMax",                     0,         ZTID_FLOAT,   ITEMCLASSMAX,              0,  { ZTID_ITEMCLASS },{} },
	{ "setMax",                     0,          ZTID_VOID,   ITEMCLASSMAX,              0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getMaxIncrement",            0,         ZTID_FLOAT,   ITEMCLASSSETMAX,           0,  { ZTID_ITEMCLASS },{} },
	{ "setMaxIncrement",            0,          ZTID_VOID,   ITEMCLASSSETMAX,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getKeep",                    0,          ZTID_BOOL,   ITEMCLASSSETGAME,          0,  { ZTID_ITEMCLASS },{} },
	{ "setKeep",                    0,          ZTID_VOID,   ITEMCLASSSETGAME,          0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getEquipmentItem",           0,          ZTID_BOOL,   ITEMCLASSSETGAME,          0,  { ZTID_ITEMCLASS },{} },
	{ "setEquipmentItem",           0,          ZTID_VOID,   ITEMCLASSSETGAME,          0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getCounter",                 0,         ZTID_FLOAT,   ITEMCLASSCOUNTER,          0,  { ZTID_ITEMCLASS },{} },
	{ "setCounter",                 0,          ZTID_VOID,   ITEMCLASSCOUNTER,          0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getUseSound",                0,         ZTID_FLOAT,   ITEMCLASSUSESOUND,         0,  { ZTID_ITEMCLASS },{} },
	{ "setUseSound",                0,          ZTID_VOID,   ITEMCLASSUSESOUND,         0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getUseSound2",               0,         ZTID_FLOAT,   IDATAUSESOUND2,            0,  { ZTID_ITEMCLASS },{} },
	{ "setUseSound2",               0,          ZTID_VOID,   IDATAUSESOUND2,            0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getID",                      0,         ZTID_FLOAT,   ITEMCLASSID,               0,  { ZTID_ITEMCLASS },{} },
	
	{ "getPower",                   0,         ZTID_FLOAT,   ITEMCLASSPOWER,            0,  { ZTID_ITEMCLASS },{} },
	{ "setPower",                   0,          ZTID_VOID,   ITEMCLASSPOWER,            0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getDamage",                  0,         ZTID_FLOAT,   ITEMCLASSPOWER,            0,  { ZTID_ITEMCLASS },{} },
	{ "setDamage",                  0,          ZTID_VOID,   ITEMCLASSPOWER,            0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getInitD[]",                 0,       ZTID_UNTYPED,   ITEMCLASSINITDD,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "setInitD[]",                 0,          ZTID_VOID,   ITEMCLASSINITDD,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "GetName",                    0,          ZTID_VOID,   -1,                   FL_INL,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "RunScript",                  0,          ZTID_VOID,   -1,                   FL_INL,  { ZTID_ITEMCLASS, ZTID_FLOAT },{ 10000 } },
	
	{ "getModifier",                0,         ZTID_FLOAT,   IDATALTM,                  0,  { ZTID_ITEMCLASS },{} },
	{ "setModifier",                0,          ZTID_VOID,   IDATALTM,                  0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getTileMod",                 0,         ZTID_FLOAT,   IDATALTM,                  0,  { ZTID_ITEMCLASS },{} },
	{ "setTileMod",                 0,          ZTID_VOID,   IDATALTM,                  0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getScript",                  0,         ZTID_FLOAT,   IDATASCRIPT,               0,  { ZTID_ITEMCLASS },{} },
	{ "setScript",                  0,          ZTID_VOID,   IDATASCRIPT,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getPScript",                 0,         ZTID_FLOAT,   IDATAPSCRIPT,              0,  { ZTID_ITEMCLASS },{} },
	{ "setPScript",                 0,          ZTID_VOID,   IDATAPSCRIPT,              0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getPString",                 0,         ZTID_FLOAT,   IDATAPSTRING,              0,  { ZTID_ITEMCLASS },{} },
	{ "setPString",                 0,          ZTID_VOID,   IDATAPSTRING,              0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getPickupString",            0,         ZTID_FLOAT,   IDATAPSTRING,              0,  { ZTID_ITEMCLASS },{} },
	{ "setPickupString",            0,          ZTID_VOID,   IDATAPSTRING,              0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getPickupStringFlags",       0,         ZTID_FLOAT,   IDATAPFLAGS,               0,  { ZTID_ITEMCLASS },{} },
	{ "setPickupStringFlags",       0,          ZTID_VOID,   IDATAPFLAGS,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getMagicCost",               0,         ZTID_FLOAT,   IDATAMAGCOST,              0,  { ZTID_ITEMCLASS },{} },
	{ "setMagicCost",               0,          ZTID_VOID,   IDATAMAGCOST,              0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getCost",                    0,         ZTID_FLOAT,   IDATAMAGCOST,              0,  { ZTID_ITEMCLASS },{} },
	{ "setCost",                    0,          ZTID_VOID,   IDATAMAGCOST,              0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getCost2",                   0,         ZTID_FLOAT,   IDATACOST2,                0,  { ZTID_ITEMCLASS },{} },
	{ "setCost2",                   0,          ZTID_VOID,   IDATACOST2,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getCostCounter",             0,         ZTID_FLOAT,   IDATACOSTCOUNTER,          0,  { ZTID_ITEMCLASS },{} },
	{ "setCostCounter",             0,          ZTID_VOID,   IDATACOSTCOUNTER,          0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getCostCounter2",            0,         ZTID_FLOAT,   IDATACOSTCOUNTER2,         0,  { ZTID_ITEMCLASS },{} },
	{ "setCostCounter2",            0,          ZTID_VOID,   IDATACOSTCOUNTER2,         0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getMinHearts",               0,         ZTID_FLOAT,   IDATAMINHEARTS,            0,  { ZTID_ITEMCLASS },{} },
	{ "setMinHearts",               0,          ZTID_VOID,   IDATAMINHEARTS,            0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getTile",                    0,         ZTID_FLOAT,   IDATATILE,                 0,  { ZTID_ITEMCLASS },{} },
	{ "setTile",                    0,          ZTID_VOID,   IDATATILE,                 0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getFlash",                   0,         ZTID_FLOAT,   IDATAMISC,                 0,  { ZTID_ITEMCLASS },{} },
	{ "setFlash",                   0,          ZTID_VOID,   IDATAMISC,                 0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getCSet",                    0,         ZTID_FLOAT,   IDATACSET,                 0,  { ZTID_ITEMCLASS },{} },
	{ "setCSet",                    0,          ZTID_VOID,   IDATACSET,                 0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
//	{ "getFrame",                   0,         ZTID_FLOAT,   IDATAFRAME,                0,  { ZTID_ITEMCLASS },{} },
//	{ "setFrame",                   0,          ZTID_VOID,   IDATAFRAME,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getAFrames",                 0,         ZTID_FLOAT,   IDATAFRAMES,               0,  { ZTID_ITEMCLASS },{} },
	{ "setAFrames",                 0,          ZTID_VOID,   IDATAFRAMES,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getASpeed",                  0,         ZTID_FLOAT,   IDATAASPEED,               0,  { ZTID_ITEMCLASS },{} },
	{ "setASpeed",                  0,          ZTID_VOID,   IDATAASPEED,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getDelay",                   0,         ZTID_FLOAT,   IDATADELAY,                0,  { ZTID_ITEMCLASS },{} },
	{ "setDelay",                   0,          ZTID_VOID,   IDATADELAY,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getCombine",                 0,          ZTID_BOOL,   IDATACOMBINE,              0,  { ZTID_ITEMCLASS },{} },
	{ "setCombine",                 0,          ZTID_VOID,   IDATACOMBINE,              0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getDowngrade",               0,          ZTID_BOOL,   IDATADOWNGRADE,            0,  { ZTID_ITEMCLASS },{} },
	{ "setDowngrade",               0,          ZTID_VOID,   IDATADOWNGRADE,            0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getKeepOld",                 0,          ZTID_BOOL,   IDATAKEEPOLD,              0,  { ZTID_ITEMCLASS },{} },
	{ "setKeepOld",                 0,          ZTID_VOID,   IDATAKEEPOLD,              0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getRupeeCost",               0,          ZTID_BOOL,   IDATARUPEECOST,            0,  { ZTID_ITEMCLASS },{} },
	{ "setRupeeCost",               0,          ZTID_VOID,   IDATARUPEECOST,            0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getEdible",                  0,          ZTID_BOOL,   IDATAEDIBLE,               0,  { ZTID_ITEMCLASS },{} },
	{ "setEdible",                  0,          ZTID_VOID,   IDATAEDIBLE,               0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getUnused",                  0,          ZTID_BOOL,   IDATAFLAGUNUSED,           0,  { ZTID_ITEMCLASS },{} },
	{ "setUnused",                  0,          ZTID_VOID,   IDATAFLAGUNUSED,           0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getGainLower",               0,          ZTID_BOOL,   IDATAGAINLOWER,            0,  { ZTID_ITEMCLASS },{} },
	{ "setGainLower",               0,          ZTID_VOID,   IDATAGAINLOWER,            0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getAttributes[]",            0,       ZTID_UNTYPED,   IDATAATTRIB,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "setAttributes[]",            0,          ZTID_VOID,   IDATAATTRIB,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getMisc[]",                  0,       ZTID_UNTYPED,   IDATAATTRIB,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "setMisc[]",                  0,          ZTID_VOID,   IDATAATTRIB,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getFlags[]",                 0,          ZTID_BOOL,   IDATAFLAGS,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "setFlags[]",                 0,          ZTID_VOID,   IDATAFLAGS,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT, ZTID_BOOL },{} },
	
	{ "getSprites[]",               0,         ZTID_FLOAT,   IDATASPRITE,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "setSprites[]",               0,          ZTID_VOID,   IDATASPRITE,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT, ZTID_FLOAT },{} },
	
	{ "getWeapon",                  0,         ZTID_FLOAT,   IDATAUSEWPN,               0,  { ZTID_ITEMCLASS },{} },
	{ "setWeapon",                  0,          ZTID_VOID,   IDATAUSEWPN,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getDefense",                 0,         ZTID_FLOAT,   IDATAUSEDEF,               0,  { ZTID_ITEMCLASS },{} },
	{ "setDefense",                 0,          ZTID_VOID,   IDATAUSEDEF,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getDefence",                 0,         ZTID_FLOAT,   IDATAUSEDEF,               0,  { ZTID_ITEMCLASS },{} },
	{ "setDefence",                 0,          ZTID_VOID,   IDATAUSEDEF,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getRange",                   0,         ZTID_FLOAT,   IDATAWRANGE,               0,  { ZTID_ITEMCLASS },{} },
	{ "setRange",                   0,          ZTID_VOID,   IDATAWRANGE,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getDuration",                0,         ZTID_FLOAT,   IDATADURATION,             0,  { ZTID_ITEMCLASS },{} },
	{ "setDuration",                0,          ZTID_VOID,   IDATADURATION,             0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	
	
//	{ "getMovement[]",              0,         ZTID_FLOAT,   IDATAUSEMVT,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
//	{ "setMovement[]",              0,          ZTID_VOID,   IDATAUSEMVT,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT, ZTID_FLOAT },{} },
	
	{ "getWeaponInitD[]",           0,       ZTID_UNTYPED,   IDATAWPNINITD,             0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "setWeaponInitD[]",           0,          ZTID_VOID,   IDATAWPNINITD,             0,  { ZTID_ITEMCLASS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getWeaponMisc[]",            0,       ZTID_UNTYPED,   IDATAMISCD,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "setWeaponMisc[]",            0,          ZTID_VOID,   IDATAMISCD,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT, ZTID_UNTYPED },{} },
	
	{ "getDuplicates",              0,         ZTID_FLOAT,   IDATADUPLICATES,           0,  { ZTID_ITEMCLASS },{} },
	{ "setDuplicates",              0,          ZTID_VOID,   IDATADUPLICATES,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getDrawLayer",               0,         ZTID_FLOAT,   IDATADRAWLAYER,            0,  { ZTID_ITEMCLASS },{} },
	{ "setDrawLayer",               0,          ZTID_VOID,   IDATADRAWLAYER,            0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getCollectFlags",            0,         ZTID_FLOAT,   IDATACOLLECTFLAGS,         0,  { ZTID_ITEMCLASS },{} },
	{ "setCollectFlags",            0,          ZTID_VOID,   IDATACOLLECTFLAGS,         0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getWeaponScript",            0,         ZTID_FLOAT,   IDATAWEAPONSCRIPT,         0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponScript",            0,          ZTID_VOID,   IDATAWEAPONSCRIPT,         0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getWeaponHitXOffset",        0,         ZTID_FLOAT,   IDATAWEAPHXOFS,            0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponHitXOffset",        0,          ZTID_VOID,   IDATAWEAPHXOFS,            0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getWeaponHitYOffset",        0,         ZTID_FLOAT,   IDATAWEAPHYOFS,            0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponHitYOffset",        0,          ZTID_VOID,   IDATAWEAPHYOFS,            0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getWeaponHitWidth",          0,         ZTID_FLOAT,   IDATAWEAPHXSZ,             0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponHitWidth",          0,          ZTID_VOID,   IDATAWEAPHXSZ,             0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getWeaponHitHeight",         0,         ZTID_FLOAT,   IDATAWEAPHYSZ,             0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponHitHeight",         0,          ZTID_VOID,   IDATAWEAPHYSZ,             0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getWeaponHitZHeight",        0,         ZTID_FLOAT,   IDATAWEAPHZSZ,             0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponHitZHeight",        0,          ZTID_VOID,   IDATAWEAPHZSZ,             0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getWeaponDrawXOffset",       0,         ZTID_FLOAT,   IDATAWEAPXOFS,             0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponDrawXOffset",       0,          ZTID_VOID,   IDATAWEAPXOFS,             0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getWeaponDrawYOffset",       0,         ZTID_FLOAT,   IDATAWEAPYOFS,             0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponDrawYOffset",       0,          ZTID_VOID,   IDATAWEAPYOFS,             0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getWeaponDrawZOffset",       0,         ZTID_FLOAT,   IDATAWEAPZOFS,             0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponDrawZOffset",       0,          ZTID_VOID,   IDATAWEAPZOFS,             0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getMagicCostTimer",          0,         ZTID_FLOAT,   IDATAMAGICTIMER,           0,  { ZTID_ITEMCLASS },{} },
	{ "setMagicCostTimer",          0,          ZTID_VOID,   IDATAMAGICTIMER,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getCostTimer",               0,         ZTID_FLOAT,   IDATAMAGICTIMER,           0,  { ZTID_ITEMCLASS },{} },
	{ "setCostTimer",               0,          ZTID_VOID,   IDATAMAGICTIMER,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getCostTimer2",              0,         ZTID_FLOAT,   IDATAMAGICTIMER2,          0,  { ZTID_ITEMCLASS },{} },
	{ "setCostTimer2",              0,          ZTID_VOID,   IDATAMAGICTIMER2,          0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	
	
	{ "getHitXOffset",              0,         ZTID_FLOAT,   IDATAHXOFS,                0,  { ZTID_ITEMCLASS },{} },
	{ "setHitXOffset",              0,          ZTID_VOID,   IDATAHXOFS,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getHitYOffset",              0,         ZTID_FLOAT,   IDATAHYOFS,                0,  { ZTID_ITEMCLASS },{} },
	{ "setHitYOffset",              0,          ZTID_VOID,   IDATAHYOFS,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getDrawXOffset",             0,         ZTID_FLOAT,   IDATADXOFS,                0,  { ZTID_ITEMCLASS },{} },
	{ "setDrawXOffset",             0,          ZTID_VOID,   IDATADXOFS,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getDrawYOffset",             0,         ZTID_FLOAT,   IDATADYOFS,                0,  { ZTID_ITEMCLASS },{} },
	{ "setDrawYOffset",             0,          ZTID_VOID,   IDATADYOFS,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getHitWidth",                0,         ZTID_FLOAT,   IDATAHXSZ,                 0,  { ZTID_ITEMCLASS },{} },
	{ "setHitWidth",                0,          ZTID_VOID,   IDATAHXSZ,                 0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getHitHeight",               0,         ZTID_FLOAT,   IDATAHYSZ,                 0,  { ZTID_ITEMCLASS },{} },
	{ "setHitHeight",               0,          ZTID_VOID,   IDATAHYSZ,                 0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getHitZHeight",              0,         ZTID_FLOAT,   IDATAHZSZ,                 0,  { ZTID_ITEMCLASS },{} },
	{ "setHitZHeight",              0,          ZTID_VOID,   IDATAHZSZ,                 0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getTileWidth",               0,         ZTID_FLOAT,   IDATATILEW,                0,  { ZTID_ITEMCLASS },{} },
	{ "setTileWidth",               0,          ZTID_VOID,   IDATATILEW,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getTileHeight",              0,         ZTID_FLOAT,   IDATATILEH,                0,  { ZTID_ITEMCLASS },{} },
	{ "setTileHeight",              0,          ZTID_VOID,   IDATATILEH,                0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getPickup",                  0,         ZTID_FLOAT,   IDATAPICKUP,               0,  { ZTID_ITEMCLASS },{} },
	{ "setPickup",                  0,          ZTID_VOID,   IDATAPICKUP,               0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getOverrideFlags",           0,         ZTID_FLOAT,   IDATAOVERRIDEFL,           0,  { ZTID_ITEMCLASS },{} },
	{ "setOverrideFlags",           0,          ZTID_VOID,   IDATAOVERRIDEFL,           0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
//	Weapon-forwarded Variables
	
	{ "getWeaponTileWidth",         0,         ZTID_FLOAT,   IDATATILEWWEAP,            0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponTileWidth",         0,          ZTID_VOID,   IDATATILEWWEAP,            0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	{ "getWeaponTileHeight",        0,         ZTID_FLOAT,   IDATATILEHWEAP,            0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponTileHeight",        0,          ZTID_VOID,   IDATATILEHWEAP,            0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getWeaponOverrideFlags",     0,         ZTID_FLOAT,   IDATAOVERRIDEFLWEAP,       0,  { ZTID_ITEMCLASS },{} },
	{ "setWeaponOverrideFlags",     0,          ZTID_VOID,   IDATAOVERRIDEFLWEAP,       0,  { ZTID_ITEMCLASS, ZTID_FLOAT },{} },
	
	{ "getValidate",                0,          ZTID_BOOL,   IDATAVALIDATE,             0,  { ZTID_ITEMCLASS },{} },
	{ "setValidate",                0,          ZTID_VOID,   IDATAVALIDATE,             0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	{ "getValidate2",               0,          ZTID_BOOL,   IDATAVALIDATE2,            0,  { ZTID_ITEMCLASS },{} },
	{ "setValidate2",               0,          ZTID_VOID,   IDATAVALIDATE2,            0,  { ZTID_ITEMCLASS, ZTID_BOOL },{} },
	
	{ "",                           0,          ZTID_VOID,   -1,                        0,  {},{} }
};

ItemclassSymbols::ItemclassSymbols()
{
	table = itemclassTable;
	refVar = REFITEMCLASS;
}

void ItemclassSymbols::generateCode()
{
	//void GetName(itemclass, int32_t)
	{
		Function* function = getFunction("GetName");
		int32_t label = function->getLabel();
		vector<shared_ptr<Opcode>> code;
		//pop off the param
		addOpcode2 (code, new OPopRegister(new VarArgument(EXP1)));
		LABELBACK(label);
		//pop pointer, and ignore it
		POPREF();
		addOpcode2 (code, new OGetItemName(new VarArgument(EXP1)));
		RETURN();
		function->giveCode(code);
	}
	//void RunScript(itemclass)
	{
		Function* function = getFunction("RunScript");
		int32_t label = function->getLabel();
		vector<shared_ptr<Opcode>> code;
		//pop off the param
		addOpcode2 (code, new OPopRegister(new VarArgument(EXP1)));
		LABELBACK(label);
		//pop pointer, and ignore it
		POPREF();
		addOpcode2 (code, new ORunItemScript(new VarArgument(EXP1)));
		RETURN();
		function->giveCode(code);
	}
	
}

