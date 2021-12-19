#include "itemeditor.h"
#include "info.h"
#include "alert.h"
#include "../zsys.h"
#include <gui/builder.h>

void reset_itembuf(itemdata *item, int32_t id);
extern zquestheader header;
extern bool saved;
extern char *item_string[];
extern itemdata *itemsbuf;
extern zcmodule moduledata;
static bool _reset_default;
static itemdata reset_ref;
static std::string reset_name;
void call_item_editor(int32_t index)
{
	_reset_default = false;
	ItemEditorDialog(index).show();
	while(_reset_default)
	{
		_reset_default = false;
		reset_itembuf(&reset_ref, index);
		ItemEditorDialog(reset_ref, reset_name.c_str(), index).show();
	}
}

static const GUI::ListData PFlagTypeList
{
	{ "Disabled", 0 },
	{ "Set To These", 1 },
	{ "Add These", 2 },
	{ "Subtract These", 3 },
	{ "Limit To These", 4 }
};

static const GUI::ListData WeapMoveTypeList
{
	{ "None", 0 },
	{ "Line", 1 },
	{ "Sine Wave", 2 },
	{ "Cosine", 3 },
	{ "Circular", 4 },
	{ "Arc", 5 },
	{ "Pattern A", 6 },
	{ "Pattern B", 7 },
	{ "Pattern C", 8 },
	{ "Pattern D", 9 },
	{ "Pattern E", 10 },
	{ "Pattern F", 11 }
};

//Sets the Item Editor Field Names
ItemNameInfo inameinf[itype_max];

ItemNameInfo defInfo =
{
	-1,
	"Power:",
	{
		"Attributes[0]:", "Attributes[1]:", "Attributes[2]:",
		"Attributes[3]:", "Attributes[4]:","Attributes[5]:","Attributes[6]:",
		"Attributes[7]:","Attributes[8]:","Attributes[9]:"
	},
	{
		"Flags[0]", "Flags[1]", "Flags[2]", "Flags[3]", "Flags[4]", "Flags[5]",
		"Flags[6]", "Flags[7]", "Flags[8]", "Flags[9]", "Flags[10]", "Flags[11]",
		"Flags[12]", "Flags[13]", "Flags[14]","Constant Script"
	},
	{
		"Sprites[0]:", "Sprites[1]:","Sprites[2]:","Sprites[3]:","Sprites[4]:",
		"Sprites[5]:","Sprites[6]:","Sprites[7]:","Sprites[8]:","Sprites[9]:"
	},
	{ "UseSound", "UseSound2" }
};

void loadinfo(ItemNameInfo * inf, itemdata const& ref)
{
	inf->clear();
	inf->iclass = ref.family;
	#define _SET(mem, str, helpstr) \
	do{ \
		inf->mem = str; \
		inf->h_##mem = helpstr; \
	}while(false)
	#define FLAG(val) (ref.flags & ITEM_FLAG##val)
	switch(ref.family)
	{
		case itype_fairy:
		{
			_SET(misc[2], "Step Speed:", "The movement speed of the fairy, in 100ths of pixel/frame");
			_SET(misc[3], "Bottle Fill:", "What bottle type to fill an empty bottle with if caught using a Bug Net");
			_SET(flag[0], "Life is Percent", "HP Regained is a percentage out of max HP");
			_SET(flag[1], "Magic is Percent", "MP Regained is a percentage out of max MP");
			if(FLAG(1))
				_SET(misc[0], "% HP Regained:", "Percentage of max life restored when collected.");
			else
				_SET(misc[0], "HP Regained:", "Life (in points) restored when collected.");
			if(FLAG(2))
				_SET(misc[1], "% MP Regained:", "Percentage of max magic restored when collected.");
			else
				_SET(misc[1], "MP Regained:", "Magic (in points) restored when collected.");
			_SET(actionsnd[0], "Item Drop Sound:", "Plays SFX when dropped");
			break;
		}
		case itype_triforcepiece:
		{
			_SET(misc[0], "Cutscene MIDI:", "If non-zero, overrides the cutscene midi");
			_SET(misc[1], "Cutscene Type (0-1):", "If >0, uses the 'big triforce' cutscene style");
			_SET(misc[2], "Second Collect Sound:", "A second sound to play on pickup, played only if flag 'Play Second SFX' is checked.");
			_SET(misc[3], "Custom Cutscene Duration", "If nonzero, overrides the duration of the cutscene, in frames");
			_SET(misc[4], "Custom Refill Frame", "If non-zero, changes the timing of the triforce's life refill.");
			_SET(flag[0], "Side Warp Out", "Warp out using sidewarp A upon completion");
			_SET(flag[2], "Removes Sword Jinxes", "Heal sword jinxes on pickup");
			_SET(flag[3], "Removes Item Jinxes", "Heal item jinxes on pickup");
			_SET(flag[8], "Don't Dismiss Messages", "If not checked, the cutscene clears screen strings");
			_SET(flag[9], "Cutscene Interrupts Action Script", "The action script, if running on collection, is paused for the duration of the cutscene.");
			_SET(flag[10], "Don't Affect Music", "If not checked, music is stopped for the cutscene");
			_SET(flag[11], "No Cutscene", "Skip the cutscene animation");
			_SET(flag[12], "Run Action Script on Collection", "Item action script runs when it is collected");
			_SET(flag[13], "Play Second SFX", "Plays the 'Second Collect Sound' SFX on pickup");
			_SET(flag[14], "Don't Play MIDI", "Don't play a cutscene midi if checked");
			_SET(actionsnd[0], "Collect Sound:", "First sound played on pickup");
			break;
		}
		case itype_shield:
		{
			_SET(misc[0], "Block Flags:", "(Rock=1, Arrow=2, BRang=4, Fireball=8, Sword=16, Magic=32, Flame=64, Script=128, Fireball2=256, Lightbeam=512)\n"
				"Sum all of the values you want to apply. Weapons and lightbeams with their flags set will be blocked from in front of the player.");
			_SET(misc[1], "Reflect Flags:", "(Rock=1, Fireball=8, Sword=16, Magic=32, Script=128, Fireball2=256, Lightbeam=512)\n"
				"Sum all of the values you want to apply. Weapons and lightbeams with their flags set will be reflected from in front of the player.\n"
				"Weapons are only reflected if their value is in both 'Block Flags' and 'Reflect Flags'.");
			_SET(actionsnd[0], "De/Reflection Sound:", "Plays when the shield successfully blocks or reflects a weapon");
			break;
		}
		case itype_agony:
		{
			_SET(power, "Sensitivity:", "The radius in which flags are checked for");
			_SET(misc[0], "Vibration Frequency", "0-58, higher = faster vibrations");
			break;
		}
		case itype_wealthmedal:
		{
			_SET(misc[0], "Discount Amount:", "If 'Discount is Percent' is checked, this is the percentage of the price that will still be charged.\n"
				"else, this many rupees are ADDED to the price (use negative to subtract)");
			_SET(flag[0], "Discount Is Percent", "Makes the discount percentage-based");
			break;
		}
		case itype_cbyrna: //!TODO Help Text
		{
			inf->power = "Beam Damage:";
			inf->misc[0] = "Beam Speed Divisor:";
			inf->misc[1] = "Orbit Radius:";
			inf->misc[2] = "Number Of Beams:";
			inf->misc[3] = "Melee Damage:";
			inf->misc[9] = "Slash Sound:";
			inf->flag[0] = "Penetrates Enemies";
			inf->flag[1] = "Invincible Player";
			inf->flag[2] = "No Melee Attack";
			inf->flag[3] = "Can Slash";
			inf->flag[5] = "Melee Weapons Use Magic Cost";
			inf->flag[6] = "Doesn't Pick Up Dropped Items";
			inf->flag[7] = "Flip Right-Facing Slash";
			inf->wpn[0] = "Stab Sprite:";
			inf->wpn[1] = "Slash Sprite:";
			inf->wpn[2] = "Beam Sprite:";
			inf->wpn[3] = "Sparkle Sprite:";
			inf->wpn[4] = "Damaging Sparkle Sprite:";
			inf->actionsnd[0] = "Orbit Sound:";
			break;
		}
		case itype_whispring:
		{
			_SET(power, "Jinx Divisor:", "Amount to divide jinx duration by.\n"
				"If 'Perm. Jinx Are Temp' is checked, perm jinxes last for 150/[divisor] frames."
				" If divisor is 0, player is immune to affected jinxes.\n"
				"When picked up, regardless of flags, perm jinxes matching the Jinx Type become 150-frame temp jinxes.");
			_SET(misc[0], "Jinx Type:", "1 = sword, 2 = item, 3 = both");
			_SET(flag[0], "Perm. Jinx Are Temp.", "Perm jinxes inflicted instead are temp for 150/[divisor] frames");
			break;
		}
		case itype_ladder:
		{
			_SET(power, "Four-Way:", "If >0, the player can exit the ladder in all 4 directions.\n"
				"If ==0, the player can only exit parallel to the direction they entered from.");
			_SET(flag[0], "Can Ladder Over Pitfalls", "Can be used to cross Pitfall combos");
			break;
		}
		case itype_magickey:
		{
			_SET(power, "Dungeon Level:", "What dungeon level to grant infinite keys for");
			_SET(flag[0], "Lesser D. Levels Also", "If checked, lower levels are also affected");
			break;
		}
		case itype_wallet:
		{
			_SET(power, "Infinite Supply:", "If >0, grants infinite money");
			_SET(misc[0], "Increase Amount:", "How much money regenerates over time from having this");
			_SET(misc[1], "Delay Duration:", "How often money regenerates from having this, in frames");
			break;
		}
		case itype_quiver:
		{
			_SET(power, "Infinite Supply:", "If >0, grants infinite arrows");
			_SET(misc[0], "Increase Amount:", "How much arrows regenerates over time from having this");
			_SET(misc[1], "Delay Duration:", "How often arrows regenerates from having this, in frames");
			break;
		}
		case itype_bombbag:
		{
			_SET(power, "Infinite Supply:", "If >0, grants infinite bombs");
			_SET(misc[0], "Increase Amount:", "How much bombs regenerates over time from having this");
			_SET(misc[1], "Delay Duration:", "How often bombs regenerates from having this, in frames");
			_SET(flag[0], "Super Bombs Also", "Also Regenerates S. Bombs");
			break;
		}
		case itype_quakescroll2: //!TODO Help Text
		{
			inf->power = "Damage Multiplier:";
			inf->misc[0] = "Stun Duration:";
			inf->misc[1] = "Stun Radius:";
			inf->actionsnd[0] = "Quake Sound:";
			break;
		}
		case itype_quakescroll: //!TODO Help Text
		{
			inf->power = "Damage Multiplier:";
			inf->misc[0] = "Stun Duration:";
			inf->misc[1] = "Stun Radius:";
			inf->actionsnd[0] = "Quake Sound:";
			break;
		}
		case itype_perilscroll: //!TODO Help Text
		{
			inf->misc[0] = "Maximum Hearts:";
			break;
		}
		case itype_spinscroll2: //!TODO Help Text
		{
			inf->power = "Damage Multiplier:";
			inf->misc[0] = "Number of Spins:";
			inf->actionsnd[0] = "Spinning Sound:";
			break;
		}
		case itype_spinscroll: //!TODO Help Text
		{
			inf->power = "Damage Multiplier:";
			inf->misc[0] = "Number of Spins:";
			inf->actionsnd[0] = "Spinning Sound:";
			break;
		}
		case itype_clock:
		{
			_SET(misc[0], "Duration:", "How long the invincibility lasts, in frames. 0 = infinite.");
			break;
		}
		case itype_magicring:
		{
			_SET(power, "Infinite Magic:", "If >0, grants infinite magic");
			_SET(misc[0], "Increase Amount:", "How much magic regenerates over time from having this");
			_SET(misc[1], "Delay Duration:", "How often magic regenerates from having this, in frames");
			break;
		}
		case itype_heartring:
		{
			_SET(misc[0], "Increase Amount:", "How much life regenerates over time from having this");
			_SET(misc[1], "Delay Duration:", "How often life regenerates from having this, in frames");
			break;
		}
		case itype_chargering: //!TODO Help Text
		{
			inf->misc[0] = "Charging Duration:";
			inf->misc[1] = "Magic C. Duration:";
			break;
		}
		case itype_hoverboots: //!TODO Help Text
		{
			inf->misc[0] = "Hover Duration:";
			inf->flag[0] = "Timer only resets on landing";
			inf->wpn[0] = "Halo Sprite:";
			inf->actionsnd[0] = "Hovering Sound:";
			break;
		}
		case itype_rocs: //!TODO Help Text
		{
			inf->power = "Jump Power:";
			inf->misc[0] = "Extra Jumps:";
			inf->flag[0] = "Jump is Power/100";
			inf->actionsnd[0] = "Jumping Sound:";
			break;
		}
		case itype_sbomb: //!TODO Help Text
		{
			inf->power = "Damage:";
			inf->misc[0] = "Fuse Duration (0 = Remote):";
			inf->misc[1] = "Max. On Screen:";
			inf->misc[2] = "Damage to Player:";
			inf->flag[1] = "Explosion Hurts Player";
			inf->wpn[0] = "Bomb Sprite:";
			inf->wpn[1] = "Explosion Sprite:";
			inf->actionsnd[0] = "Explosion Sound:";
			break;
		}
		case itype_bomb: //!TODO Help Text
		{
			inf->power = "Damage:";
			inf->misc[0] = "Fuse Duration (0 = Remote):";
			inf->misc[1] = "Max. On Screen:";
			inf->misc[2] = "Damage to Player:";
			inf->flag[0] = "Use 1.92 Timing";
			inf->flag[1] = "Explosion Hurts Player";
			inf->wpn[0] = "Bomb Sprite:";
			inf->wpn[1] = "Explosion Sprite:";
			inf->actionsnd[0] = "Explosion Sound:";
			break;
		}
		case itype_nayruslove: //!TODO Help Text
		{
			inf->misc[0] = "Duration:";
			inf->flag[0] = "Rocket Flickers";
			inf->flag[1] = "Translucent Rocket";
			inf->flag[2] = "Translucent Shield";
			inf->flag[3] = "Shield Flickers";
			inf->wpn[0] = "Left Rocket Sprite:";
			inf->wpn[1] = "L. Rocket Return Sprite:";
			inf->wpn[2] = "L. Rocket Sparkle Sprite:";
			inf->wpn[3] = "L. Return Sparkle Sprite:";
			inf->wpn[4] = "Shield Sprite (2x2, Over):";
			inf->wpn[5] = "Right Rocket Sprite:";
			inf->wpn[6] = "R. Rocket Return Sprite:";
			inf->wpn[7] = "R. Rocket Sparkle Sprite:";
			inf->wpn[8] = "R. Return Sparkle Sprite:";
			inf->wpn[9] = "Shield Sprite (2x2, Under):";
			inf->actionsnd[0] = "Shield Sound:";
			break;
		}
		case itype_faroreswind: //!TODO Help Text
		{
			inf->misc[0] = "Warp Animation (0-2):";
			inf->actionsnd[0] = "Wind Sound:";
			break;
		}
		case itype_dinsfire: //!TODO Help Text
		{
			inf->power = "Damage:";
			inf->misc[0] = "Number of Flames:";
			inf->misc[1] = "Circle Width:";
			inf->flag[1] = "Don't Provide Light";
			inf->flag[2] = "Falls in Sideview";
			inf->flag[4] = "Temporary Light";
			inf->wpn[0] = "Rocket Up Sprite:";
			inf->wpn[1] = "Rocket Down Sprite:";
			inf->wpn[2] = "R. Up Sparkle Sprite:";
			inf->wpn[3] = "R. Down Sparkle Sprite:";
			inf->wpn[4] = "Flame Sprite:";
			inf->actionsnd[0] = "Ring Sound:";
			break;
		}
		case itype_hammer: //!TODO Help Text
		{
			inf->power = "Damage:";
			inf->flag[2] = "No Melee Attack";
			inf->wpn[0] = "Hammer Sprite:";
			inf->wpn[1] = "Smack Sprite:";
			inf->actionsnd[0] = "Pound Sound:";
			inf->flag[6] = "Doesn't Pick Up Dropped Items";
			break;
		}
		case itype_lens: //!TODO Help Text
		{
			inf->misc[0] = "Lens Width:";
			inf->flag[0] = "Show Hints";
			inf->flag[1] = "Hide Secret Combos";
			inf->flag[2] = "No X-Ray for Items";
			inf->flag[3] = "Show Raft Paths";
			inf->flag[4] = "Show Invis. Enemies";
			inf->actionsnd[0] = "Activation Sound:";
			break;
		}
		case itype_hookshot:
		{
			inf->power = "Damage:";
			_SET(misc[0], "Chain Length:", "Max length, in tiles, of the hookshot chain");
			_SET(misc[1], "Chain Links:", "Max number of chain link sprites to display (less = a more stretched chain)");
			_SET(flag[0], "No Handle Damage", "The handle does not collide with enemies");
			_SET(flag[1], "Allow Diagonal", "The hookshot can be fired diagonally. (EXPERIMENTAL)");
			_SET(flag[3], "Pick Up Anything", "Any collided item will be grabbed, not just dropped items");
			_SET(flag[4], "Drags Items", "Collected items are dragged towards the player");
			_SET(flag[5], "Hits Enemy Projectiles", "If checked, the weapon can collide with projectiles, either blocking or reflecting them.");
			_SET(flag[6], "Picks Up Keys", "Will pick up Key type items");
			
			inf->wpn[0] = "Tip Sprite:";
			inf->wpn[1] = "Chain Sprite (H):";
			inf->wpn[2] = "Chain Sprite (V):";
			inf->wpn[3] = "Handle Sprite:";
			inf->wpn[4] = "Diagonal Tip Sprite:";
			inf->wpn[5] = "Diagonal Handle Sprite:";
			inf->wpn[6] = "Diagonal Chain Sprite:";
			_SET(actionsnd[0], "Firing Sound:", "Sound plays while the hookshot extends");
			if(FLAG(6))
			{
				_SET(misc[2], "Block Flags:",
					"(Rock=1, Arrow=2, BRang=4, Fireball=8, Sword=16, Magic=32, Flame=64, Script=128, Fireball2=256)\n"
					"Sum all of the values you want to apply. Weapons with their flags set will be blocked by the weapon.");
				_SET(misc[3], "Reflect Flags:",
					"(Rock=1, Fireball=8, Sword=16, Magic=32, Script=128, Fireball2=256)\n"
					"Sum all of the values you want to apply. Weapons with their flags set will be reflected by the weapon.\n"
					"Weapons are only reflected if their value is in both 'Block Flags' and 'Reflect Flags'.");
			}
			break;
		}
		case itype_switchhook:
		{
			inf->power = "Damage:";
			_SET(misc[0], "Chain Length:", "Max length, in tiles, of the switchhook chain");
			_SET(misc[1], "Chain Links:", "Max number of chain link sprites to display (less = a more stretched chain)");
			_SET(misc[4], "Switch FX Type:", "The visual effect to use when switching."
				"\n0 = Poof, 1 = Flicker, 2 = Rise/Fall");
			_SET(flag[0], "No Handle Damage", "The handle does not collide with enemies");
			_SET(flag[1], "Allow Diagonal", "The switchhook can be fired diagonally. (EXPERIMENTAL)");
			_SET(flag[3], "Pick Up Anything", "Any collided item will be grabbed, not just dropped items");
			_SET(flag[5], "Hits Enemy Projectiles", "If checked, the weapon can collide with projectiles, either blocking or reflecting them.");
			_SET(flag[6], "Picks Up Keys", "Will pick up Key type items");
			_SET(flag[7], "Cost on Swap", "The cost of the item is only paid upon swapping successfully.");
			_SET(flag[8], "Swaps Items", "Items that would be collected are instead swapped with the player.");
			if(!FLAG(9))
				_SET(flag[4], "Drags Items", "Collected items are dragged towards the player");
			
			inf->wpn[0] = "Tip Sprite:";
			inf->wpn[1] = "Chain Sprite (H):";
			inf->wpn[2] = "Chain Sprite (V):";
			inf->wpn[3] = "Handle Sprite:";
			inf->wpn[4] = "Diagonal Tip Sprite:";
			inf->wpn[5] = "Diagonal Handle Sprite:";
			inf->wpn[6] = "Diagonal Chain Sprite:";
			_SET(actionsnd[0], "Firing Sound:", "Sound plays while the hookshot extends");
			_SET(actionsnd[1], "Switch Sound:", "Sound plays when a switch occurs");
			if(FLAG(6))
			{
				_SET(misc[2], "Block Flags:",
					"(Rock=1, Arrow=2, BRang=4, Fireball=8, Sword=16, Magic=32, Flame=64, Script=128, Fireball2=256)\n"
					"Sum all of the values you want to apply. Weapons with their flags set will be blocked by the weapon.");
				_SET(misc[3], "Reflect Flags:",
					"(Rock=1, Fireball=8, Sword=16, Magic=32, Script=128, Fireball2=256)\n"
					"Sum all of the values you want to apply. Weapons with their flags set will be reflected by the weapon.\n"
					"Weapons are only reflected if their value is in both 'Block Flags' and 'Reflect Flags'.");
			}
			break;
		}
		case itype_itmbundle:
		{
			for(auto q = 0; q < 10; ++q)
				_SET(misc[q], "Item " + std::to_string(q+1) + ":", "If > -1, an item ID to 'collect' when you collect this item bundle.");
			_SET(flag[0], "Run Pickup Scripts", "Run the collect script of bundled items.");
			break;
		}
		case itype_progressive_itm:
		{
			for(auto q = 0; q < 10; ++q)
				_SET(misc[q], "Item " + std::to_string(q+1) + ":", "If > -1, an item ID in the progressive order.");
			break;
		}
		case itype_boots:
		{
			_SET(power, "Protection Power:", "Protects against damage combos that deal up to 16*power points of damage.");
			_SET(flag[0], "Not Solid Combos", "Does not protect against solid damage combos");
			_SET(flag[1], "Heavy", "Some step combo types are only triggered if the player has boots with this flag checked.");
			break;
		}
		case itype_bracelet:
		{
			_SET(power, "Push Combo Level:", "If 1 or higher, can push 'Heavy' blocks. If 2 or higher, can push 'Very Heavy' blocks.");
			_SET(flag[0], "Limited Per Screen", "Can only use a set number of times per screen");
			if(FLAG(1))
			{
				_SET(misc[2], "Uses Per Screen", "How many times this bracelet can be used per screen. Minimum 1.");
			}
			break;
		}
		case itype_book: //!TODO Help Text
		{
			inf->power = "M. Damage:";
			inf->misc[0] = "W. Damage:";
			inf->misc[1] = "Wand Sound";
			inf->misc[2] = "Special Step";
			inf->misc[3] = "Fire Damage";
			inf->flag[0] = "Fire Magic";
			inf->flag[1] = "Override Wand Damage";
			inf->flag[2] = "Fire Doesn't Hurt Player";
			inf->flag[3] = "Override Wand SFX";
			inf->flag[4] = "Temporary Light";
			inf->flag[5] = "Replace Wand Weapon";
			inf->wpn[0] = "Magic Sprite:";
			inf->wpn[1] = "Projectile Sprite:";
			inf->actionsnd[0] = "Firing Sound:";
			break;
		}
		case itype_ring:
		{
			_SET(misc[0], "Player Sprite Pal:", "The Sprite Palette row to load into CSet 6");
			inf->flag[0] = "Affects Damage Combos";
			inf->flag[1] = "Percentage Multiplier";
			if(FLAG(2))
				inf->power = "Damage % Mult:";
			else
				inf->power = "Damage Divisor:";
			break;
		}
		case itype_wand: //!TODO Help Text
		{
			inf->power = "Damage:";
			inf->misc[0] = "M. Damage";
			inf->misc[1] = "W. Type:";
			inf->misc[2] = "W. Speed:";
			inf->misc[3] = "W. Range:";
			inf->misc[4] = "Move Effect:";
			inf->misc[5] = "Mvt Arg1:";
			inf->misc[6] = "Mvt Arg2:";
			inf->misc[7] = "No. of Clones:";
			inf->misc[8] = "Clone Pattern:";
			inf->misc[9] = "Slash Sound:";
			inf->flag[0] = "Allow Magic w/o Book";
			inf->flag[1] = "Wand Moves";
			inf->flag[2] = "No Melee Attack";
			inf->flag[3] = "Can Slash";
			inf->flag[4] = "No Melee Cooldown";
			inf->flag[5] = "Melee Weapons Use Magic Cost";
			inf->flag[6] = "Doesn't Pick Up Dropped Items";
			inf->flag[7] = "Flip Right-Facing Slash";
			inf->flag[8] = "Light Torches";
			inf->wpn[0] = "Stab Sprite:";
			inf->wpn[1] = "Slash Sprite:";
			inf->wpn[2] = "Projectile Sprite:";
			inf->wpn[3] = "Projectile Misc:";
			inf->actionsnd[0] = "Firing Sound:";
			break;
		}
		case itype_bait: //!TODO Help Text
		{
			inf->misc[0] = "Duration:";
			inf->misc[1] = "Bait Range (0 = infinite)";
			inf->flag[0] = "Repels enemies";
			inf->wpn[0] = "Bait Sprite:";
			inf->actionsnd[0] = "Baiting Sound:";
			break;
		}
		case itype_potion:
		{
			_SET(flag[0], "Life is Percent", "HP Regained is a percentage out of max HP");
			_SET(flag[1], "Magic is Percent", "MP Regained is a percentage out of max MP");
			inf->flag[2] = "Removes Sword Jinxes";
			inf->flag[3] = "Removes Item Jinxes";
			
			if(FLAG(1))
				_SET(misc[0], "% HP Regained:", "Percentage of max life restored when collected.");
			else
				_SET(misc[0], "HP Regained:", "Life (in points) restored when collected.");
			if(FLAG(2))
				_SET(misc[1], "% MP Regained:", "Percentage of max magic restored when collected.");
			else
				_SET(misc[1], "MP Regained:", "Magic (in points) restored when collected.");
			
			break;
		}
		case itype_whistle: //!TODO Help Text
		{
			inf->misc[0] = "Whirlwind Direction:";
			inf->misc[1] = "Warp Ring:";
			inf->misc[4] = "Weapon Damage";
			inf->flag[0] = "One W.Wind Per Scr.";
			inf->flag[1] = "Has Damage";
			inf->flag[2] = "Whirlwinds Reflect off Prism/Mirror Combos";
			inf->wpn[0] = "Whirlwind Sprite:";
			inf->actionsnd[0] = "Music Sound:";
			break;
		}
		case itype_candle:
		{
			inf->power = "Damage:";
			_SET(misc[1], "Max Fires On Screen", "If < 1, defaults to 2");
			_SET(flag[0], "Limited Per Screen", "Can only use a set number of times per screen");
			_SET(flag[1], "Don't Provide Light", "Does not light up dark rooms");
			inf->flag[2] = "Fire Doesn't Hurt Player";
			_SET(flag[3], "Can Slash", "The candle slashes instead of stabs");
			inf->flag[7] = "Flip Right-Facing Slash";
			_SET(wpn[0], "Stab Sprite:", "The sprite for the candle stabbing");
			_SET(wpn[1], "Slash Sprite:", "The sprite for the candle slashing");
			_SET(wpn[2], "Fire Sprite:", "The sprite for the candle fire");
			inf->actionsnd[0] = "Firing Sound:";
			if(FLAG(1))
			{
				_SET(misc[2], "Uses Per Screen", "How many times this candle can be used per screen. Minimum 1.");
			}
			if(!FLAG(2))
			{
				_SET(flag[4], "Temporary Light", "In Old-style Dark Rooms, only keeps the room lit while it exists.");
			}
			if(FLAG(3))
			{
				_SET(misc[0], "Damage to Player:", "Damage value to be used for self-damage");
			}
			break;
		}
		case itype_arrow: //!TODO Help Text
		{
			inf->power = "Damage:";
			inf->misc[0] = "Duration (0 = Infinite):";
			inf->flag[0] = "Penetrate Enemies";
			inf->flag[1] = "Allow Item Pickup";
			inf->flag[3] = "Pick Up Anything";
			inf->wpn[0] = "Arrow Sprite:";
			inf->wpn[1] = "Sparkle Sprite:";
			inf->wpn[2] = "Damaging Sparkle Sprite:";
			inf->actionsnd[0] = "Firing Sound:";
			inf->flag[6] = "Picks Up Keys";
			break;
		}
		case itype_brang: //!TODO Help Text
		{
			inf->power = "Damage:";
			inf->misc[0] = "Range (0 = Infinite):";
			inf->misc[2] = "Block Flags:";
			inf->misc[3] = "Reflect Flags:";
			inf->flag[0] = "Corrected Animation";
			inf->flag[1] = "Directional Sprites";
			inf->flag[2] = "Do Not Return";
			inf->flag[3] = "Pick Up Anything";
			inf->flag[4] = "Drags Items";
			inf->flag[5] = "Reflects Enemy Projectiles";
			inf->flag[6] = "Picks Up Keys";
			inf->flag[7] = "Triggers 'Fire(Any)'";
			inf->wpn[0] = "Boomerang Sprite:";
			inf->wpn[1] = "Sparkle Sprite:";
			inf->wpn[2] = "Damaging Sparkle Sprite:";
			inf->actionsnd[0] = "Spinning Sound:";
			break;
		}
		case itype_sword: //!TODO Help Text
		{
			inf->power = "Damage:";
			inf->misc[0] = "Beam Hearts:";
			inf->misc[1] = "Beam Damage:";
			inf->flag[0] = "Beam Hearts Is Percent";
			inf->flag[1] = "Beam Damage Is Percent";
			inf->flag[2] = "Beam Penetrates Enemies";
			inf->flag[3] = "Can Slash";
			inf->flag[4] = "No Melee Cooldown";
			inf->flag[5] = "Melee Weapons Use Magic Cost";
			inf->flag[6] = "Doesn't Pick Up Dropped Items";
			inf->flag[7] = "Flip Right-Facing Slash";
			inf->flag[8] = "Sword Beams Reflect off Prism/Mirror Combos";
			inf->flag[9] = "Walk slowly while charging";
			inf->wpn[0] = "Stab Sprite:";
			inf->wpn[1] = "Slash Sprite:";
			inf->wpn[2] = "Beam Sprite:";
			inf->actionsnd[0] = "Slash/Stab Sound";
			break;
		}
		case itype_whimsicalring:
		{
			_SET(power, "Damage Bonus:", "How much damage to add to strikes from the Sword, Wand, and Hammer."
				"\nThis is added before Attack Rings take effect, and also before the hero damage multiplier.");
			_SET(misc[0], "Chance (1 in n):", "There is a '1 in n' chance of the bonus damage being added (min 1)");
			_SET(actionsnd[0], "Whimsy Sound:", "This sound plays when the bonus damage is successfully applied.");
			break;
		}
		case itype_perilring:
		{
			_SET(misc[0], "Maximum Hearts:", "Takes effect when below this number of hearts");
			inf->flag[1] = "Percentage Multiplier";
			if(FLAG(2))
				inf->power = "Damage % Mult:";
			else
				inf->power = "Damage Divisor:";
			break;
		}
		case itype_stompboots:
		{
			inf->power = "Damage:";
			break;
		}
		case itype_bow:
		{
			_SET(power, "Arrow Speed Mod:", "Arrow moves at 3 pixels per frame, multiplied by '(1+n)/2', where 'n' is this value.");
			break;
		}
		case itype_script1: case itype_script2: case itype_script3: case itype_script4:
		case itype_script5: case itype_script6: case itype_script7: case itype_script8:
		case itype_script9: case itype_script10:
		{
			_SET(misc[0], "Step Speed:", "The speed, in 100ths of pixel/frame");
			_SET(flag[0], "No Gfx Flip", "If checked, the sprite is not flipped based on direction.");
			break;
		}
		case itype_custom1: case itype_custom2: case itype_custom3: case itype_custom4:
		case itype_custom5: case itype_custom6: case itype_custom7: case itype_custom8:
		case itype_custom9: case itype_custom10:
		{
			break;
		}
		case itype_icerod:
		{
			_SET(misc[0], "Step Speed:", "The speed, in 100ths of pixel/frame");
			_SET(flag[0], "No Gfx Flip", "If checked, the sprite is not flipped based on direction.");
			break;
		}
		case itype_flippers:
		{
			_SET(flag[0], "No Diving", "If checked, these flippers cannot dive.");
			_SET(flag[2], "Can Swim in Lava", "These flippers will be able to swim in liquid marked as 'Lava'.");
			if(!FLAG(1))
			{
				_SET(flag[1], "Cancellable Diving", "Pressing B will immediately cancel a dive");
				_SET(misc[0], "Dive Length:", "Length, in frames, of the dive.");
				_SET(misc[1], "Dive Cooldown:", "Cooldown, in frames, between coming up from a dive and being able to dive again.");
			}
			break;
		}
		case itype_raft:
		{
			_SET(misc[0], "Speed Modifier:", "Valid values -8 to 5."
				"\nIf positive, move 2^n pixels per frame."
				"\nIf negative, move 1 pixel per 2^n frames."
				"\nA value of 0 will stop the raft dead. This is useless unless combined with scripts.");
			break;
		}
		case itype_atkring:
		{
			_SET(misc[0], "Bonus Damage", "This amount of damage is *added* to the damage, *after* the multiplier.");
			_SET(misc[1], "Damage Multiplier", "The amount of damage dealt is multiplied by this value.");
			break;
		}
		case itype_lantern:
		{
			_SET(misc[0], "Shape", "What shape to use for the light area emitted.\n"
				"0 = circular, 1 = cone in front");
			_SET(misc[1], "Range", "The range, in pixels, of the light.");
			break;
		}
		case itype_bottle:
		{
			_SET(misc[0], "Slot:", "Which slot this bottle item is attached to. Valid vals 0-255.");
			break;
		}
		case itype_bottlefill:
		{
			_SET(misc[0], "Contents:", "What contents to place in an empty bottle when picked up");
			break;
		}
		case itype_bugnet:
		{
			_SET(flag[0], "Can't catch fairies", "If checked, no longer catches fairies"
				" it collides with");
			_SET(flag[1], "Right-handed", "Swaps swing direction of the weapon");
			break;
		}
		case itype_mirror:
		{
			_SET(misc[0], "Warp Effect", "What warp effect to use during the warp to another dmap.\n"
				"0=None, 1=Zap, 2=Wave, 3=Blackscr, 4=OpenWipe");
			_SET(misc[1], "Cont. Warp Effect", "What warp effect to use during the warp to the continue point.\n"
				"0=None, 1=Zap, 2=Wave, 3=Blackscr, 4=OpenWipe");
			_SET(flag[0], "Place Return Portal", "If checked, places a return portal when"
				" mirroring to a new dmap");
			_SET(flag[1], "Continue acts as F6->Continue",
				"When used on a dmap with 'Mirror Continues instead of Warping' checked, "
				"activates F6->Continue instead of Farore's Wind effect if enabled.");
			_SET(wpn[0], "Portal Sprite", "Sprite of the Return Portal");
			_SET(actionsnd[0], "Warp Sound", "Sound played for the warp to a new dmap");
			_SET(actionsnd[1], "Continue Sound", "Sound played for a continue warp");
			break;
		}
	}
	#undef _SET
	#undef FLAG
}

ItemEditorDialog::ItemEditorDialog(itemdata const& ref, char const* str, int32_t index):
	local_itemref(ref), itemname(str), index(index),
	list_items(GUI::ListData::itemclass(true)),
	list_counters(GUI::ListData::counters()),
	list_sprites(GUI::ListData::miscsprites()),
	list_itemdatscript(GUI::ListData::itemdata_script()),
	list_itemsprscript(GUI::ListData::itemsprite_script()),
	list_weaponscript(GUI::ListData::lweapon_script()),
	list_weaptype(GUI::ListData::lweaptypes()),
	list_deftypes(GUI::ListData::deftypes())
{}

ItemEditorDialog::ItemEditorDialog(int32_t index):
	ItemEditorDialog(itemsbuf[index], item_string[index], index)
{}

//{ Macros

#define DISABLE_WEAP_DATA true
#define ATTR_WID 6_em
#define ATTR_LAB_WID 12_em
#define SPR_LAB_WID sized(14_em,10_em)
#define ACTION_LAB_WID 8_em
#define ACTION_FIELD_WID 6_em
#define FLAGS_WID 20_em

#define NUM_FIELD(member,_min,_max,wid) \
TextField( \
	type = GUI::TextField::type::INT_DECIMAL, width = wid, \
	low = _min, high = _max, val = local_itemref.member, \
	onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val) \
	{ \
		local_itemref.member = val; \
	})

#define ATTRIB_FIELD(member, index) \
l_attribs[index] = Label(textAlign = 2, width = ATTR_LAB_WID), \
ib_attribs[index] = Button(forceFitH = true, text = "?", \
	disabled = true, \
	onPressFunc = [&]() \
	{ \
		InfoDialog("Attribute Info",h_attribs[index]).show(); \
	}), \
TextField(maxLength = 11, \
	type = GUI::TextField::type::INT_DECIMAL, width = ATTR_WID, \
	val = local_itemref.member, \
	onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val) \
	{ \
		local_itemref.member = val; \
	})

#define FLAG_CHECK(index, bit) \
Row(padding = 0_px, \
	ib_flags[index] = Button(forceFitH = true, text = "?", \
		disabled = true, \
		onPressFunc = [&]() \
		{ \
			InfoDialog("Flags Info",h_flags[index]).show(); \
		}), \
	l_flags[index] = Checkbox( \
		width = FLAGS_WID, \
		checked = (local_itemref.flags & bit), \
		onToggleFunc = [&](bool state) \
		{ \
			SETFLAG(local_itemref.flags,bit,state); \
			loadItemClass(); \
		} \
	) \
)
#define FLAG_CHECK_NOINFO(index, bit) \
l_flags[index] = Checkbox( \
	width = FLAGS_WID, \
	checked = (local_itemref.flags & bit), \
	onToggleFunc = [&](bool state) \
	{ \
		SETFLAG(local_itemref.flags,bit,state); \
	} \
) \

#define SPRITE_DROP(ind, mem) \
Row(vPadding = 0_px, \
	l_spr[ind] = Label(textAlign = 2, width = SPR_LAB_WID, topMargin = 1_px), \
	ib_spr[ind] = Button(forceFitH = true, text = "?", \
		disabled = true, \
		onPressFunc = [&]() \
		{ \
			InfoDialog("Sprite Info",h_spr[ind]).show(); \
		}), \
	DropDownList( \
		maxwidth = sized(18_em, 14_em), \
		data = list_sprites, \
		selectedValue = local_itemref.mem, \
		onSelectFunc = [&](int32_t val) \
		{ \
			local_itemref.mem = val; \
		} \
	) \
)

//}

static size_t itmtabs[4] = {0};
std::shared_ptr<GUI::Widget> ItemEditorDialog::view()
{
	using namespace GUI::Builder;
	using namespace GUI::Props;
	
	char titlebuf[256];
	sprintf(titlebuf, "Item Editor (%d): %s", index, itemname.c_str());
	if(is_large)
	{
		window = Window(
			use_vsync = true,
			title = titlebuf,
			onEnter = message::OK,
			onClose = message::CANCEL,
			Column(
				Row(
					Rows<2>(padding = 0_px,
						Label(text = "Name:"),
						TextField(
							fitParent = true,
							maxLength = 63,
							text = itemname,
							onValChangedFunc = [&](GUI::TextField::type,std::string_view str,int32_t)
							{
								itemname = str;
								char buf[256];
								sprintf(buf, "Item Editor (%d): %s", index, itemname.c_str());
								window->setTitle(buf);
							}
						),
						Label(text = "Type:"),
						Row(
							height = sized(16_px, 21_px),
							DropDownList(data = list_items,
								fitParent = true, padding = 0_px,
								selectedValue = local_itemref.family,
								onSelectionChanged = message::ITEMCLASS
							),
							Button(width = 1.5_em, padding = 0_px, text = "?", hAlign = 1.0, onPressFunc = [&]()
							{
								InfoDialog(moduledata.item_editor_type_names[local_itemref.family],
									moduledata.itemclass_help_strings[local_itemref.family]
								).show();
							})
						)
					),
					Column(vAlign = 0.0, hAlign = 0.0, padding = 0_px,
						Checkbox(
							hAlign = 0.0,
							checked = (local_itemref.flags & ITEM_GAMEDATA),
							text = "Equipment Item",
							onToggleFunc = [&](bool state)
							{
								SETFLAG(local_itemref.flags,ITEM_GAMEDATA,state);
							}
						)
					)
				),
				TabPanel(
					ptr = &itmtabs[0],
					TabRef(name = "Data", TabPanel(
						ptr = &itmtabs[1],
						TabRef(name = "Attrib", Column(
							Rows<6>(framed = true,
								Label(width=ATTR_LAB_WID,textAlign=2,text="Level:"),
								Button(forceFitH = true, text = "?",
									onPressFunc = [&]()
									{
										InfoDialog("Level Info","Most passive items only take effect if they are the highest level you own.\n"
											"By default, subscreens use the highest level item of an itemclass for displaying.").show();
									}),
								NUM_FIELD(fam_type, 1, 255, ATTR_WID),
								l_power = Label(width=ATTR_LAB_WID,textAlign=2),
								ib_power = Button(forceFitH = true, text = "?",
									disabled = true,
									onPressFunc = [&]()
									{
										InfoDialog("Power Info",h_power).show();
									}),
								NUM_FIELD(power, 0, 255, ATTR_WID)
							),
							Rows<6>(framed = true,
								ATTRIB_FIELD(misc1,0),
								ATTRIB_FIELD(misc6,5),
								
								ATTRIB_FIELD(misc2,1),
								ATTRIB_FIELD(misc7,6),
								
								ATTRIB_FIELD(misc3,2),
								ATTRIB_FIELD(misc8,7),
								
								ATTRIB_FIELD(misc4,3),
								ATTRIB_FIELD(misc9,8),
								
								ATTRIB_FIELD(misc5,4),
								ATTRIB_FIELD(misc10,9)
							)
						)),
						TabRef(name = "Flags", Column(padding = 0_px,
							Rows<2>(
								framed = true,
								frameText = "General Flags",
								topPadding = DEFAULT_PADDING+0.4_em,
								bottomPadding = DEFAULT_PADDING+1_px,
								bottomMargin = 1_em,
								Checkbox(
									width = FLAGS_WID,
									checked = (local_itemref.flags & ITEM_EDIBLE),
									text = "Can Be Eaten By Enemies",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_EDIBLE,state);
									}
								),
								FLAG_CHECK_NOINFO(15,ITEM_PASSIVESCRIPT),
								Checkbox(
									width = FLAGS_WID,
									checked = (local_itemref.flags & ITEM_SIDESWIM_DISABLED),
									text = "Disabled In Sideview Water",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_SIDESWIM_DISABLED,state);
									}
								),
								Checkbox(
									width = FLAGS_WID,
									checked = (local_itemref.flags & ITEM_BUNNY_ENABLED),
									text = "Usable as a Bunny",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_BUNNY_ENABLED,state);
									}
								)
							),
							Columns<8>(
								framed = true,
								frameText = "Variable Flags",
								topPadding = DEFAULT_PADDING+0.4_em,
								bottomPadding = DEFAULT_PADDING+1_px,
								FLAG_CHECK(0,ITEM_FLAG1),
								FLAG_CHECK(1,ITEM_FLAG2),
								FLAG_CHECK(2,ITEM_FLAG3),
								FLAG_CHECK(3,ITEM_FLAG4),
								FLAG_CHECK(4,ITEM_FLAG5),
								FLAG_CHECK(5,ITEM_FLAG6),
								FLAG_CHECK(6,ITEM_FLAG7),
								FLAG_CHECK(7,ITEM_FLAG8),
								FLAG_CHECK(8,ITEM_FLAG9),
								FLAG_CHECK(9,ITEM_FLAG10),
								FLAG_CHECK(10,ITEM_FLAG11),
								FLAG_CHECK(11,ITEM_FLAG12),
								FLAG_CHECK(12,ITEM_FLAG13),
								FLAG_CHECK(13,ITEM_FLAG14),
								FLAG_CHECK(14,ITEM_FLAG15)
							)
						)),
						TabRef(name = "Action", Column(
							Rows<2>(framed = true, frameText = "Use Cost",
								padding = DEFAULT_PADDING*2,
								margins = DEFAULT_PADDING,
								TextField(
									val = local_itemref.magic,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.magic = val;
									}
								),
								DropDownList(
									data = list_counters,
									selectedValue = local_itemref.cost_counter,
									onSelectFunc = [&](int32_t val)
									{
										local_itemref.cost_counter = val;
									}
								),
								Label(text = "Timer:", textAlign = 2, forceFitW = true),
								TextField(
									val = local_itemref.magiccosttimer,
									type = GUI::TextField::type::INT_DECIMAL,
									minwidth = ACTION_FIELD_WID, fitParent = true, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.magiccosttimer = val;
									}
								)
							),
							Rows<3>(framed = true, frameText = "SFX",
								padding = DEFAULT_PADDING*2,
								margins = DEFAULT_PADDING,
								l_sfx[0] = Label(textAlign = 2, width = ACTION_LAB_WID),
								ib_sfx[0] = Button(forceFitH = true, text = "?",
									disabled = true,
									onPressFunc = [&]()
									{
										InfoDialog("SFX Info",h_sfx[0]).show();
									}),
								TextField(
									val = local_itemref.usesound,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.usesound = val;
									}
								),
								l_sfx[1] = Label(textAlign = 2, width = ACTION_LAB_WID),
								ib_sfx[1] = Button(forceFitH = true, text = "?",
									disabled = true,
									onPressFunc = [&]()
									{
										InfoDialog("SFX Info",h_sfx[1]).show();
									}),
								TextField(
									val = local_itemref.usesound2,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.usesound2 = val;
									}
								)
							),
							Rows<2>(
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_DOWNGRADE),
									text = "Remove Item When Used",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_DOWNGRADE,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_VALIDATEONLY),
									text = "Only Validate Cost",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_VALIDATEONLY,state);
									}
								)
							)
						)),
						TabRef(name = "Pickup", Column(
							Rows<4>(
								//
								Label(text = "Counter:", hAlign = 1.0),
								DropDownList(
									fitParent = true,
									data = list_counters,
									selectedValue = local_itemref.count,
									onSelectFunc = [&](int32_t val)
									{
										local_itemref.count = val;
									}
								),_d,_d,
								//
								Label(text = "Increase By:", hAlign = 1.0),
								TextField(
									val = ((local_itemref.amount & 0x4000) ? -1 : 1)*(local_itemref.amount & 0x3FFF),
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 65535,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.amount &= 0x8000;
										local_itemref.amount |= ((val&0x3FFF)|(val<0?0x4000:0));
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.amount & 0x8000),
									text = "Gradual",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.amount,0x8000,state);
									}
								),_d,
								//
								Label(text = "Increase Max:", hAlign = 1.0),
								TextField(
									val = local_itemref.setmax,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, low = -32768, high = 32767,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.setmax = val;
									}
								),
								Label(text = "But Not Above:", hAlign = 1.0),
								TextField(
									val = local_itemref.max,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 65535,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.max = val;
									}
								),
								//
								Label(text = "Sound:", hAlign = 1.0),
								TextField(
									val = local_itemref.playsound,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.playsound = val;
									}
								),_d,_d,
								//
								Label(text = "Hearts Required:", hAlign = 1.0),
								TextField(
									val = local_itemref.pickup_hearts,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.pickup_hearts = val;
									}
								),_d,_d
							),
							Column(
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_KEEPOLD),
									text = "Keep Lower Level Items",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_KEEPOLD,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_GAINOLD),
									text = "Gain All Lower Level Items",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_GAINOLD,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_COMBINE),
									text = "Upgrade When Collected Twice",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_COMBINE,state);
									}
								)
							),
							Row(
								Label(text = "String:"),
								TextField(
									val = local_itemref.pstring,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 65535,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.pstring = val;
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup_string_flags & itemdataPSTRING_ALWAYS),
									text = "Always",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup_string_flags,itemdataPSTRING_ALWAYS,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup_string_flags & itemdataPSTRING_IP_HOLDUP),
									text = "Only Held",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup_string_flags,itemdataPSTRING_IP_HOLDUP,state);
									}
								)
							)
						)),
						TabRef(name = "P. Flags", Column(
							Row(
								Label(text = "Flag Behavior:"),
								DropDownList(
									maxwidth = 10_em,
									data = PFlagTypeList,
									selectedValue = local_itemref.pickupflag,
									onSelectFunc = [&](int32_t val)
									{
										local_itemref.pickupflag = val;
									}
								)
							),
							Columns<8>(
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<0)),
									text = "Large Collision Rectangle (INTERNAL)",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<0),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<1)),
									text = "Hold Up Item",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<1),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<2)),
									text = "Sets Screen State ST_ITEM",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<2),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<3)),
									text = "Dummy Item",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<3),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<4)),
									text = "Shop Item (INTERNAL)",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<4),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<5)),
									text = "Pay for Info (INTERNAL)",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<5),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<6)),
									text = "Item Fades",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<6),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<7)),
									text = "Enemy Carries Item",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<7),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<8)),
									text = "Item Disappears",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<8),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<9)),
									text = "Big Triforce (INTERNAL)",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<9),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<10)),
									text = "Invisible",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<10),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<11)),
									text = "Triggers Screen State ST_SP_ITEM",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<11),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<12)),
									text = "Triggers Screen Secrets",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<12),state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<13)),
									text = "Always Grabbable",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup,(1<<13),state);
									}
								),
								Checkbox(
									disabled = true,
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<14)),
									text = "--",
									onToggleFunc = [&](bool state)
									{
										//SETFLAG(local_itemref.pickup,(1<<14),state);
									}
								),
								Checkbox(
									disabled = true,
									hAlign = 0.0,
									checked = (local_itemref.pickup & (1<<15)),
									text = "--",
									onToggleFunc = [&](bool state)
									{
										//SETFLAG(local_itemref.pickup,(1<<15),state);
									}
								)
							)
						)),
						TabRef(name = "Weapon Data", Row(
							Row(
								vAlign = 0.0,
								Rows<4>(
									Label(hAlign = 1.0, text = "Weapon Type:"),
									DropDownList(
										fitParent = true,
										data = list_weaptype,
										selectedValue = local_itemref.useweapon,
										onSelectFunc = [&](int32_t val)
										{
											local_itemref.useweapon = val;
										}
									), _d, _d,
									Label(hAlign = 1.0, text = "Default Defense:"),
									DropDownList(
										fitParent = true,
										data = list_deftypes,
										selectedValue = local_itemref.usedefence,
										onSelectFunc = [&](int32_t val)
										{
											local_itemref.usedefence = val;
										}
									), _d, _d,
									Label(hAlign = 1.0, text = "Movement Pattern:"),
									DropDownList(
										disabled = DISABLE_WEAP_DATA,
										fitParent = true,
										data = WeapMoveTypeList,
										selectedValue = local_itemref.weap_pattern[0],
										onSelectFunc = [&](int32_t val)
										{
											local_itemref.weap_pattern[0] = val;
										}
									), _d, _d,
									Label(hAlign = 1.0, text = "Movement Arg 1:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[1],
										type = GUI::TextField::type::INT_DECIMAL,
										hAlign = 0.0, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[1] = val;
										}
									),
									Label(hAlign = 1.0, text = "Weapon Range:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weaprange,
										type = GUI::TextField::type::INT_DECIMAL,
										fitParent = true, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weaprange = val;
										}
									),
									Label(hAlign = 1.0, text = "Movement Arg 2:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[2],
										type = GUI::TextField::type::INT_DECIMAL,
										hAlign = 0.0, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[2] = val;
										}
									),
									Label(hAlign = 1.0, text = "Weapon Duration:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weapduration,
										type = GUI::TextField::type::INT_DECIMAL,
										fitParent = true, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weapduration = val;
										}
									),
									Label(hAlign = 1.0, text = "Movement Arg 3:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[3],
										type = GUI::TextField::type::INT_DECIMAL,
										hAlign = 0.0, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[3] = val;
										}
									),
									Label(hAlign = 1.0, text = "Other 1:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[5],
										type = GUI::TextField::type::INT_DECIMAL,
										fitParent = true, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[5] = val;
										}
									),
									Label(hAlign = 1.0, text = "Movement Arg 4:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[4],
										type = GUI::TextField::type::INT_DECIMAL,
										hAlign = 0.0, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[4] = val;
										}
									),
									Label(hAlign = 1.0, text = "Other 2:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[6],
										type = GUI::TextField::type::INT_DECIMAL,
										fitParent = true, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[6] = val;
										}
									)
								)
							)
						))
					)),
					TabRef(name = "Graphics", TabPanel(
						ptr = &itmtabs[2],
						TabRef(name = "GFX", Row(
							Rows<3>(
								Label(text = "Flash CSet:", hAlign = 1.0),
								TextField(
									val = (local_itemref.csets>>4), rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 16,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.csets &= 0x0F;
										local_itemref.csets |= val<<4;
									}
								),
								DummyWidget(),
								Label(text = "Animation Frames:", hAlign = 1.0),
								TextField(
									val = local_itemref.frames, rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.frames = val;
										animFrame->setFrames(val);
									}
								),
								DummyWidget(),
								Label(text = "Animation Speed:", hAlign = 1.0),
								TextField(
									val = local_itemref.speed, rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.speed = val;
										animFrame->setSpeed(val);
									}
								),
								DummyWidget(),
								Label(text = "Initial Delay:", hAlign = 1.0),
								TextField(
									val = local_itemref.delay, rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.delay = val;
										animFrame->setDelay(val);
									}
								),
								DummyWidget(),
								Label(text = "Player Tile Modifier:", hAlign = 1.0),
								TextField(
									val = local_itemref.ltm, rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = (0-(NEWMAXTILES-1)), high = (NEWMAXTILES-1),
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.ltm = val;
									}
								),
								Button(
									width = 1.5_em, leftPadding = 0_px, forceFitH = true,
									text = "?", hAlign = 1.0, onPressFunc = [&]()
									{
										InfoDialog("Tile Modifiers",
											"The tile modifiers of the highest level item owned from each itemclass"
											" are added up, and the total is added to the Player's tile.\n"
											"If the player is a Bunny, then only items which have the 'Usable as a Bunny'"
											" flag checked will have their tile modifiers count, but the 'Bunny Tile Mod'"
											" found in Init Data is also added."
										).show();
									}
								)
							),
							Column(
								SelTileSwatch(
									tile = local_itemref.tile,
									cset = (local_itemref.csets & 0x0F),
									onSelectFunc = [&](int32_t t, int32_t c)
									{
										local_itemref.tile = t;
										local_itemref.csets &= 0xF0;
										local_itemref.csets |= c&0x0F;
										animFrame->setTile(t);
										animFrame->setCSet(c);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.misc & 1),
									text = "Flash",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.misc,1,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.misc & 2),
									text = "2-Hand",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.misc,2,state);
									}
								),
								animFrame = TileFrame(
									hAlign = 0.0,
									tile = local_itemref.tile,
									cset = (local_itemref.csets & 0xF),
									frames = local_itemref.frames,
									speed = local_itemref.speed,
									delay = local_itemref.delay
								)
							)
						)),
						TabRef(name = "Sprites", Columns<5>(
							SPRITE_DROP(0,wpn),
							SPRITE_DROP(1,wpn2),
							SPRITE_DROP(2,wpn3),
							SPRITE_DROP(3,wpn4),
							SPRITE_DROP(4,wpn5),
							SPRITE_DROP(5,wpn6),
							SPRITE_DROP(6,wpn7),
							SPRITE_DROP(7,wpn8),
							SPRITE_DROP(8,wpn9),
							SPRITE_DROP(9,wpn10)
						)),
						TabRef(name = "Size", Row(
							Columns<5>(
								vAlign = 0.0,
								Row(
									Label(textAlign = 2, width = 6_em, text = "TileWidth:"),
									TextField(
										val = local_itemref.tilew,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 32,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.tilew = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_TILEWIDTH),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_TILEWIDTH,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "TileHeight:"),
									TextField(
										val = local_itemref.tileh,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 32,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.tileh = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_TILEHEIGHT),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_TILEHEIGHT,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitXOffset:"),
									TextField(
										val = local_itemref.hxofs,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.hxofs = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_X_OFFSET),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_X_OFFSET,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitYOffset:"),
									TextField(
										val = local_itemref.hyofs,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.hyofs = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_Y_OFFSET),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_Y_OFFSET,state);
										}
									)
								),
								_d,
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitWidth:"),
									TextField(
										val = local_itemref.hxsz,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.hxsz = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_WIDTH),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_WIDTH,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitHeight:"),
									TextField(
										val = local_itemref.hysz,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.hysz = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_HEIGHT),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_HEIGHT,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitZHeight:"),
									TextField(
										val = local_itemref.hzsz,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.hzsz = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_Z_HEIGHT),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_Z_HEIGHT,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "DrawXOffset:"),
									TextField(
										val = local_itemref.xofs,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.xofs = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_DRAW_X_OFFSET),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_DRAW_X_OFFSET,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "DrawYOffset:"),
									TextField(
										val = local_itemref.yofs,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.yofs = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_DRAW_Y_OFFSET),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_DRAW_Y_OFFSET,state);
										}
									)
								)
							)
						)),
						TabRef(name = "Weapon Size", Row(
							Columns<5>(
								vAlign = 0.0,
								Row(
									Label(textAlign = 2, width = 6_em, text = "TileWidth:"),
									TextField(
										val = local_itemref.weap_tilew,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 32,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_tilew = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_TILEWIDTH),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_TILEWIDTH,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "TileHeight:"),
									TextField(
										val = local_itemref.weap_tileh,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 32,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_tileh = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_TILEHEIGHT),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_TILEHEIGHT,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitXOffset:"),
									TextField(
										val = local_itemref.weap_hxofs,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_hxofs = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_X_OFFSET),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_X_OFFSET,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitYOffset:"),
									TextField(
										val = local_itemref.weap_hyofs,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_hyofs = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_Y_OFFSET),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_Y_OFFSET,state);
										}
									)
								),
								_d,
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitWidth:"),
									TextField(
										val = local_itemref.weap_hxsz,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_hxsz = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_WIDTH),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_WIDTH,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitHeight:"),
									TextField(
										val = local_itemref.weap_hysz,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_hysz = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_HEIGHT),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_HEIGHT,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "HitZHeight:"),
									TextField(
										val = local_itemref.weap_hzsz,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_hzsz = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_Z_HEIGHT),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_Z_HEIGHT,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "DrawXOffset:"),
									TextField(
										val = local_itemref.weap_xofs,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_xofs = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_DRAW_X_OFFSET),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_DRAW_X_OFFSET,state);
										}
									)
								),
								Row(
									Label(textAlign = 2, width = 6_em, text = "DrawYOffset:"),
									TextField(
										val = local_itemref.weap_yofs,
										type = GUI::TextField::type::INT_DECIMAL,
										width = ACTION_FIELD_WID, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_yofs = val;
										}
									),
									Checkbox(
										hAlign = 0.0,
										checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_DRAW_Y_OFFSET),
										text = "Enabled",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_DRAW_Y_OFFSET,state);
										}
									)
								)
							)
						))
					)),
					TabRef(name = "Scripts", TabPanel(
						ptr = &itmtabs[3],
						TabRef(name = "Item", Row(
							Column(
								INITD_ROW(0, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(1, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(2, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(3, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(4, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(5, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(6, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(7, local_itemref.initiald, local_itemref.initD_label)
							),
							Column(
								padding = 0_px, fitParent = true,
								Rows<2>(vAlign = 0.0,
									SCRIPT_LIST("Action Script:", list_itemdatscript, local_itemref.script),
									SCRIPT_LIST("Pickup Script:", list_itemdatscript, local_itemref.collect_script),
									SCRIPT_LIST("Sprite Script:", list_itemsprscript, local_itemref.sprite_script)
								),
								Rows<2>(hAlign = 1.0,
									Label(text = "A1:"),
									TextField(
										val = local_itemref.initiala[0],
										type = GUI::TextField::type::INT_DECIMAL,
										high = 32,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.initiala[0] = val;
										}
									),
									Label(text = "A2:"),
									TextField(
										val = local_itemref.initiala[1],
										type = GUI::TextField::type::INT_DECIMAL,
										high = 32,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.initiala[1] = val;
										}
									)
								)
							)
						)),
						TabRef(name = "Weapon", Row(
							Column(
								INITD_ROW(0, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(1, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(2, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(3, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(4, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(5, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(6, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(7, local_itemref.weap_initiald, local_itemref.weapon_initD_label)
							),
							Rows<2>(vAlign = 0.0,
								SCRIPT_LIST("Weapon Script:", list_weaponscript, local_itemref.weaponscript)
							)
						))
					))
				),
				Row(
					vAlign = 1.0,
					spacing = 2_em,
					Button(
						focused = true,
						text = "OK",
						minwidth = 90_lpx,
						onClick = message::OK),
					Button(
						text = "Cancel",
						minwidth = 90_lpx,
						onClick = message::CANCEL),
					Button(
						text = "Default",
						minwidth = 90_lpx,
						onClick = message::DEFAULT)
				)
			)
		);
	}
	else
	{
		window = Window(
			use_vsync = true,
			title = titlebuf,
			onEnter = message::OK,
			onClose = message::CANCEL,
			Column(
				TabPanel(
					ptr = &itmtabs[0],
					TabRef(name = "Basic", Column(
						Rows<2>(padding = 0_px,
							Label(text = "Name:"),
							TextField(
								fitParent = true,
								maxLength = 63,
								text = itemname,
								onValChangedFunc = [&](GUI::TextField::type,std::string_view str,int32_t)
								{
									itemname = str;
									char buf[256];
									sprintf(buf, "Item Editor (%d): %s", index, itemname.c_str());
									window->setTitle(buf);
								}
							),
							Label(text = "Type:"),
							Row(
								height = sized(16_px, 21_px),
								DropDownList(data = list_items,
									fitParent = true, padding = 0_px,
									selectedValue = local_itemref.family,
									onSelectionChanged = message::ITEMCLASS
								),
								Button(width = 1.5_em, padding = 0_px, text = "?", hAlign = 1.0, onPressFunc = [&]()
								{
									InfoDialog(moduledata.item_editor_type_names[local_itemref.family],
										moduledata.itemclass_help_strings[local_itemref.family]
									).show();
								})
							)
						),
						Column(vAlign = 0.0, hAlign = 0.0, padding = 0_px,
							Checkbox(
								hAlign = 0.0,
								checked = (local_itemref.flags & ITEM_GAMEDATA),
								text = "Equipment Item",
								onToggleFunc = [&](bool state)
								{
									SETFLAG(local_itemref.flags,ITEM_GAMEDATA,state);
								}
							)
						)
					)),
					TabRef(name = "Data", TabPanel(
						ptr = &itmtabs[1],
						TabRef(name = "Attrib", ScrollingPane(
							Rows<3>(
								Label(width=ATTR_LAB_WID,textAlign=2,text="Level:"),
								Button(forceFitH = true, text = "?",
									onPressFunc = [&]()
									{
										InfoDialog("Level Info","Most passive items only take effect if they are the highest level you own.\n"
											"By default, subscreens use the highest level item of an itemclass for displaying.").show();
									}),
								NUM_FIELD(fam_type, 1, 255, ATTR_WID),
								
								l_power = Label(width=ATTR_LAB_WID,textAlign=2),
								ib_power = Button(forceFitH = true, text = "?",
									disabled = true,
									onPressFunc = [&]()
									{
										InfoDialog("Attribute Info",h_power).show();
									}),
								NUM_FIELD(power, 0, 255, ATTR_WID),
								
								ATTRIB_FIELD(misc1,0),
								ATTRIB_FIELD(misc2,1),
								ATTRIB_FIELD(misc3,2),
								ATTRIB_FIELD(misc4,3),
								ATTRIB_FIELD(misc5,4),
								ATTRIB_FIELD(misc6,5),
								ATTRIB_FIELD(misc7,6),
								ATTRIB_FIELD(misc8,7),
								ATTRIB_FIELD(misc9,8),
								ATTRIB_FIELD(misc10,9)
							)
						)),
						TabRef(name = "Flags", ScrollingPane(
							Column(
								topMargin = 6_px,
								Column(
									framed = true,
									frameText = "General Flags",
									topPadding = DEFAULT_PADDING+0.4_em,
									bottomPadding = DEFAULT_PADDING+1_px,
									bottomMargin = 1_em,
									Checkbox(
										width = FLAGS_WID,
										checked = (local_itemref.flags & ITEM_EDIBLE),
										text = "Can Be Eaten By Enemies",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.flags,ITEM_EDIBLE,state);
										}
									),
									FLAG_CHECK_NOINFO(15,ITEM_PASSIVESCRIPT),
									Checkbox(
										width = FLAGS_WID,
										checked = (local_itemref.flags & ITEM_SIDESWIM_DISABLED),
										text = "Disabled In Sideview Water",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.flags,ITEM_SIDESWIM_DISABLED,state);
										}
									),
									Checkbox(
										width = FLAGS_WID,
										checked = (local_itemref.flags & ITEM_BUNNY_ENABLED),
										text = "Usable as a Bunny",
										onToggleFunc = [&](bool state)
										{
											SETFLAG(local_itemref.flags,ITEM_BUNNY_ENABLED,state);
										}
									)
								),
								Column(
									framed = true,
									frameText = "Variable Flags",
									topPadding = DEFAULT_PADDING+0.4_em,
									bottomPadding = DEFAULT_PADDING+1_px,
									FLAG_CHECK(0,ITEM_FLAG1),
									FLAG_CHECK(1,ITEM_FLAG2),
									FLAG_CHECK(2,ITEM_FLAG3),
									FLAG_CHECK(3,ITEM_FLAG4),
									FLAG_CHECK(4,ITEM_FLAG5),
									FLAG_CHECK(5,ITEM_FLAG6),
									FLAG_CHECK(6,ITEM_FLAG7),
									FLAG_CHECK(7,ITEM_FLAG8),
									FLAG_CHECK(8,ITEM_FLAG9),
									FLAG_CHECK(9,ITEM_FLAG10),
									FLAG_CHECK(10,ITEM_FLAG11),
									FLAG_CHECK(11,ITEM_FLAG12),
									FLAG_CHECK(12,ITEM_FLAG13),
									FLAG_CHECK(13,ITEM_FLAG14),
									FLAG_CHECK(14,ITEM_FLAG15)
								)
							)
						)),
						TabRef(name = "Action", Column(
							Rows<2>(framed = true, frameText = "Use Cost",
								padding = DEFAULT_PADDING*2,
								margins = DEFAULT_PADDING,
								TextField(
									val = local_itemref.magic,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.magic = val;
									}
								),
								DropDownList(
									data = list_counters,
									selectedValue = local_itemref.cost_counter,
									onSelectFunc = [&](int32_t val)
									{
										local_itemref.cost_counter = val;
									}
								),
								Label(text = "Timer:", textAlign = 2, forceFitW = true),
								TextField(
									val = local_itemref.magiccosttimer,
									type = GUI::TextField::type::INT_DECIMAL,
									minwidth = ACTION_FIELD_WID, fitParent = true, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.magiccosttimer = val;
									}
								)
							),
							Rows<3>(framed = true, frameText = "SFX",
								padding = DEFAULT_PADDING*2,
								margins = DEFAULT_PADDING,
								l_sfx[0] = Label(textAlign = 2, width = ACTION_LAB_WID),
								ib_sfx[0] = Button(forceFitH = true, text = "?",
									disabled = true,
									onPressFunc = [&]()
									{
										InfoDialog("SFX Info",h_sfx[0]).show();
									}),
								TextField(
									val = local_itemref.usesound,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.usesound = val;
									}
								),
								l_sfx[1] = Label(textAlign = 2, width = ACTION_LAB_WID),
								ib_sfx[1] = Button(forceFitH = true, text = "?",
									disabled = true,
									onPressFunc = [&]()
									{
										InfoDialog("SFX Info",h_sfx[1]).show();
									}),
								TextField(
									val = local_itemref.usesound2,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.usesound2 = val;
									}
								)
							),
							Rows<2>(
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_DOWNGRADE),
									text = "Remove Item When Used",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_DOWNGRADE,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_VALIDATEONLY),
									text = "Only Validate Cost",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_VALIDATEONLY,state);
									}
								)
							)
						)),
						TabRef(name = "Pickup", Column(
							Rows<4>(
								//
								Label(text = "Counter:", hAlign = 1.0),
								DropDownList(
									fitParent = true,
									data = list_counters,
									selectedValue = local_itemref.count,
									onSelectFunc = [&](int32_t val)
									{
										local_itemref.count = val;
									}
								),_d,_d,
								//
								Label(text = "Increase By:", hAlign = 1.0),
								TextField(
									val = ((local_itemref.amount & 0x4000) ? -1 : 1)*(local_itemref.amount & 0x3FFF),
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 65535,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.amount &= 0x8000;
										local_itemref.amount |= ((val&0x3FFF)|(val<0?0x4000:0));
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.amount & 0x8000),
									text = "Gradual",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.amount,0x8000,state);
									}
								),_d,
								//
								Label(text = "Increase Max:", hAlign = 1.0),
								TextField(
									val = local_itemref.setmax,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, low = -32768, high = 32767,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.setmax = val;
									}
								),
								Label(text = "But Not Above:", hAlign = 1.0),
								TextField(
									val = local_itemref.max,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 65535,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.max = val;
									}
								),
								//
								Label(text = "Sound:", hAlign = 1.0),
								TextField(
									val = local_itemref.playsound,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.playsound = val;
									}
								),_d,_d,
								//
								Label(text = "Hearts Required:", hAlign = 1.0),
								TextField(
									val = local_itemref.pickup_hearts,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.pickup_hearts = val;
									}
								),_d,_d
							),
							Column(
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_KEEPOLD),
									text = "Keep Lower Level Items",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_KEEPOLD,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_GAINOLD),
									text = "Gain All Lower Level Items",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_GAINOLD,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.flags & ITEM_COMBINE),
									text = "Upgrade When Collected Twice",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.flags,ITEM_COMBINE,state);
									}
								)
							),
							Row(
								Label(text = "String:"),
								TextField(
									val = local_itemref.pstring,
									type = GUI::TextField::type::INT_DECIMAL,
									fitParent = true, high = 65535,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.pstring = val;
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup_string_flags & itemdataPSTRING_ALWAYS),
									text = "Always",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup_string_flags,itemdataPSTRING_ALWAYS,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.pickup_string_flags & itemdataPSTRING_IP_HOLDUP),
									text = "Only Held",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.pickup_string_flags,itemdataPSTRING_IP_HOLDUP,state);
									}
								)
							)
						)),
						TabRef(name = "P. Flags", Column(
							Row(
								Label(text = "Flag Behavior:"),
								DropDownList(
									maxwidth = 10_em,
									data = PFlagTypeList,
									selectedValue = local_itemref.pickupflag,
									onSelectFunc = [&](int32_t val)
									{
										local_itemref.pickupflag = val;
									}
								)
							),
							TabPanel(padding = 0_px,
								TabRef(name = "1",
									Column(
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<0)),
											text = "Large Collision Rectangle (INTERNAL)",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<0),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<1)),
											text = "Hold Up Item",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<1),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<2)),
											text = "Sets Screen State ST_ITEM",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<2),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<3)),
											text = "Dummy Item",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<3),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<4)),
											text = "Shop Item (INTERNAL)",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<4),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<5)),
											text = "Pay for Info (INTERNAL)",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<5),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<6)),
											text = "Item Fades",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<6),state);
											}
										)
									)
								),
								TabRef(name = "2",
									Column(
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<7)),
											text = "Enemy Carries Item",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<7),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<8)),
											text = "Item Disappears",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<8),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<9)),
											text = "Big Triforce (INTERNAL)",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<9),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<10)),
											text = "Invisible",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<10),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<11)),
											text = "Triggers Screen State ST_SP_ITEM",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<11),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<12)),
											text = "Triggers Screen Secrets",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<12),state);
											}
										),
										Checkbox(
											hAlign = 0.0,
											checked = (local_itemref.pickup & (1<<13)),
											text = "Always Grabbable",
											onToggleFunc = [&](bool state)
											{
												SETFLAG(local_itemref.pickup,(1<<13),state);
											}
										)
									)
								)
							)
						)),
						TabRef(name = "Weapon Data", Row(
							Column(
								vAlign = 0.0,
								Rows<2>(
									Label(hAlign = 1.0, text = "Weapon Type:"),
									DropDownList(
										fitParent = true,
										data = list_weaptype,
										selectedValue = local_itemref.useweapon,
										onSelectFunc = [&](int32_t val)
										{
											local_itemref.useweapon = val;
										}
									),
									Label(hAlign = 1.0, text = "Default Defense:"),
									DropDownList(
										fitParent = true,
										data = list_deftypes,
										selectedValue = local_itemref.usedefence,
										onSelectFunc = [&](int32_t val)
										{
											local_itemref.usedefence = val;
										}
									),
									Label(hAlign = 1.0, text = "Movement Pattern:"),
									DropDownList(
										disabled = DISABLE_WEAP_DATA,
										fitParent = true,
										data = WeapMoveTypeList,
										selectedValue = local_itemref.weap_pattern[0],
										onSelectFunc = [&](int32_t val)
										{
											local_itemref.weap_pattern[0] = val;
										}
									)
								),
								Rows<4>(
									Label(hAlign = 1.0, text = "Movement Arg 1:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[1],
										type = GUI::TextField::type::INT_DECIMAL,
										hAlign = 0.0, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[1] = val;
										}
									),
									Label(hAlign = 1.0, text = "Weapon Range:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weaprange,
										type = GUI::TextField::type::INT_DECIMAL,
										fitParent = true, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weaprange = val;
										}
									),
									Label(hAlign = 1.0, text = "Movement Arg 2:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[2],
										type = GUI::TextField::type::INT_DECIMAL,
										hAlign = 0.0, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[2] = val;
										}
									),
									Label(hAlign = 1.0, text = "Weapon Duration:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weapduration,
										type = GUI::TextField::type::INT_DECIMAL,
										fitParent = true, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weapduration = val;
										}
									),
									Label(hAlign = 1.0, text = "Movement Arg 3:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[3],
										type = GUI::TextField::type::INT_DECIMAL,
										hAlign = 0.0, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[3] = val;
										}
									),
									Label(hAlign = 1.0, text = "Other 1:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[5],
										type = GUI::TextField::type::INT_DECIMAL,
										fitParent = true, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[5] = val;
										}
									),
									Label(hAlign = 1.0, text = "Movement Arg 4:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[4],
										type = GUI::TextField::type::INT_DECIMAL,
										hAlign = 0.0, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[4] = val;
										}
									),
									Label(hAlign = 1.0, text = "Other 2:"),
									TextField(disabled = DISABLE_WEAP_DATA,
										val = local_itemref.weap_pattern[6],
										type = GUI::TextField::type::INT_DECIMAL,
										fitParent = true, low = -214748, high = 214748,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.weap_pattern[6] = val;
										}
									)
								)
							)
						))
					)),
					TabRef(name = "Graphics", TabPanel(
						ptr = &itmtabs[2],
						TabRef(name = "GFX", Row(
							Rows<3>(
								Label(text = "Flash CSet:", hAlign = 1.0),
								TextField(
									val = (local_itemref.csets>>4), rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 16,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.csets &= 0x0F;
										local_itemref.csets |= val<<4;
									}
								),
								DummyWidget(),
								Label(text = "Animation Frames:", hAlign = 1.0),
								TextField(
									val = local_itemref.frames, rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.frames = val;
										animFrame->setFrames(val);
									}
								),
								DummyWidget(),
								Label(text = "Animation Speed:", hAlign = 1.0),
								TextField(
									val = local_itemref.speed, rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.speed = val;
										animFrame->setSpeed(val);
									}
								),
								DummyWidget(),
								Label(text = "Initial Delay:", hAlign = 1.0),
								TextField(
									val = local_itemref.delay, rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 255,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.delay = val;
										animFrame->setDelay(val);
									}
								),
								DummyWidget(),
								Label(text = "Player Tile Modifier:", hAlign = 1.0),
								TextField(
									val = local_itemref.ltm, rightPadding = 0_px,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = (0-(NEWMAXTILES-1)), high = (NEWMAXTILES-1),
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.ltm = val;
									}
								),
								Button(
									width = 1.5_em, leftPadding = 0_px, forceFitH = true,
									text = "?", hAlign = 1.0, onPressFunc = [&]()
									{
										InfoDialog("Tile Modifiers",
											"The tile modifiers of the highest level item owned from each itemclass"
											" are added up, and the total is added to the Player's tile.\n"
											"If the player is a Bunny, then only items which have the 'Usable as a Bunny'"
											" flag checked will have their tile modifiers count, but the 'Bunny Tile Mod'"
											" found in Init Data is also added."
										).show();
									}
								)
							),
							Column(
								SelTileSwatch(
									tile = local_itemref.tile,
									cset = (local_itemref.csets & 0x0F),
									onSelectFunc = [&](int32_t t, int32_t c)
									{
										local_itemref.tile = t;
										local_itemref.csets &= 0xF0;
										local_itemref.csets |= c&0x0F;
										animFrame->setTile(t);
										animFrame->setCSet(c);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.misc & 1),
									text = "Flash",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.misc,1,state);
									}
								),
								Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.misc & 2),
									text = "2-Hand",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.misc,2,state);
									}
								),
								animFrame = TileFrame(
									hAlign = 0.0,
									tile = local_itemref.tile,
									cset = (local_itemref.csets & 0xF),
									frames = local_itemref.frames,
									speed = local_itemref.speed,
									delay = local_itemref.delay
								)
							)
						)),
						TabRef(name = "Sprites", 
							TabPanel(
								TabRef(name = "1", Column(
									SPRITE_DROP(0,wpn),
									SPRITE_DROP(1,wpn2),
									SPRITE_DROP(2,wpn3),
									SPRITE_DROP(3,wpn4),
									SPRITE_DROP(4,wpn5)
								)),
								TabRef(name = "2", Column(
									SPRITE_DROP(5,wpn6),
									SPRITE_DROP(6,wpn7),
									SPRITE_DROP(7,wpn8),
									SPRITE_DROP(8,wpn9),
									SPRITE_DROP(9,wpn10)
								))
							)
						),
						TabRef(name = "Size", Row(
							Rows<2>(
								vAlign = 0.0,
								Label(textAlign = 2, text = "TileWidth:"),
								TextField(
									val = local_itemref.tilew,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 32,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.tilew = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_TILEWIDTH),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_TILEWIDTH,state);
									}
								),
								Label(textAlign = 2, text = "TileHeight:"),
								TextField(
									val = local_itemref.tileh,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 32,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.tileh = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_TILEHEIGHT),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_TILEHEIGHT,state);
									}
								),
								Label(textAlign = 2, text = "HitXOffset:"),
								TextField(
									val = local_itemref.hxofs,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = -214748, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.hxofs = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_X_OFFSET),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_X_OFFSET,state);
									}
								),
								Label(textAlign = 2, text = "HitYOffset:"),
								TextField(
									val = local_itemref.hyofs,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = -214748, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.hyofs = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_Y_OFFSET),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_Y_OFFSET,state);
									}
								)
							),
							Rows<2>(
								vAlign = 0.0,
								Label(textAlign = 2, text = "HitWidth:"),
								TextField(
									val = local_itemref.hxsz,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.hxsz = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_WIDTH),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_WIDTH,state);
									}
								),
								Label(textAlign = 2, text = "HitHeight:"),
								TextField(
									val = local_itemref.hysz,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.hysz = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_HEIGHT),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_HEIGHT,state);
									}
								),
								Label(textAlign = 2, text = "HitZHeight:"),
								TextField(
									val = local_itemref.hzsz,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.hzsz = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_HIT_Z_HEIGHT),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_HIT_Z_HEIGHT,state);
									}
								),
								Label(textAlign = 2, text = "DrawXOffset:"),
								TextField(
									val = local_itemref.xofs,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = -214748, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.xofs = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_DRAW_X_OFFSET),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_DRAW_X_OFFSET,state);
									}
								),
								Label(textAlign = 2, text = "DrawYOffset:"),
								TextField(
									val = local_itemref.yofs,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = -214748, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.yofs = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.overrideFLAGS & itemdataOVERRIDE_DRAW_Y_OFFSET),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.overrideFLAGS,itemdataOVERRIDE_DRAW_Y_OFFSET,state);
									}
								)
							)
						)),
						TabRef(name = "Weapon Size", Row(
							Rows<2>(
								vAlign = 0.0,
								Label(textAlign = 2, text = "TileWidth:"),
								TextField(
									val = local_itemref.weap_tilew,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 32,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.weap_tilew = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_TILEWIDTH),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_TILEWIDTH,state);
									}
								),
								Label(textAlign = 2, text = "TileHeight:"),
								TextField(
									val = local_itemref.weap_tileh,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 32,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.weap_tileh = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_TILEHEIGHT),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_TILEHEIGHT,state);
									}
								),
								Label(textAlign = 2, text = "HitXOffset:"),
								TextField(
									val = local_itemref.weap_hxofs,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = -214748, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.weap_hxofs = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_X_OFFSET),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_X_OFFSET,state);
									}
								),
								Label(textAlign = 2, text = "HitYOffset:"),
								TextField(
									val = local_itemref.weap_hyofs,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = -214748, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.weap_hyofs = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_Y_OFFSET),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_Y_OFFSET,state);
									}
								)
							),
							Rows<2>(
								vAlign = 0.0,
								Label(textAlign = 2, text = "HitWidth:"),
								TextField(
									val = local_itemref.weap_hxsz,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.weap_hxsz = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_WIDTH),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_WIDTH,state);
									}
								),
								Label(textAlign = 2, text = "HitHeight:"),
								TextField(
									val = local_itemref.weap_hysz,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.weap_hysz = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_HEIGHT),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_HEIGHT,state);
									}
								),
								Label(textAlign = 2, text = "HitZHeight:"),
								TextField(
									val = local_itemref.weap_hzsz,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.weap_hzsz = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_HIT_Z_HEIGHT),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_HIT_Z_HEIGHT,state);
									}
								),
								Label(textAlign = 2, text = "DrawXOffset:"),
								TextField(
									val = local_itemref.weap_xofs,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = -214748, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.weap_xofs = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_DRAW_X_OFFSET),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_DRAW_X_OFFSET,state);
									}
								),
								Label(textAlign = 2, text = "DrawYOffset:"),
								TextField(
									val = local_itemref.weap_yofs,
									type = GUI::TextField::type::INT_DECIMAL,
									width = ACTION_FIELD_WID, low = -214748, high = 214748,
									onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
									{
										local_itemref.weap_yofs = val;
									}
								),
								_d,Checkbox(
									hAlign = 0.0,
									checked = (local_itemref.weapoverrideFLAGS & itemdataOVERRIDE_DRAW_Y_OFFSET),
									text = "Enabled",
									onToggleFunc = [&](bool state)
									{
										SETFLAG(local_itemref.weapoverrideFLAGS,itemdataOVERRIDE_DRAW_Y_OFFSET,state);
									}
								)
							)
						))
					)),
					TabRef(name = "Scripts", TabPanel(
						ptr = &itmtabs[3],
						TabRef(name = "Item", Row(
							Column(
								INITD_ROW(0, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(1, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(2, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(3, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(4, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(5, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(6, local_itemref.initiald, local_itemref.initD_label),
								INITD_ROW(7, local_itemref.initiald, local_itemref.initD_label)
							),
							Column(
								padding = 0_px, fitParent = true,
								Rows<2>(vAlign = 0.0,
									SCRIPT_LIST("Action Script:", list_itemdatscript, local_itemref.script),
									SCRIPT_LIST("Pickup Script:", list_itemdatscript, local_itemref.collect_script),
									SCRIPT_LIST("Sprite Script:", list_itemsprscript, local_itemref.sprite_script)
								),
								Rows<2>(hAlign = 1.0,
									Label(text = "A1:"),
									TextField(
										val = local_itemref.initiala[0],
										type = GUI::TextField::type::INT_DECIMAL,
										high = 32,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.initiala[0] = val;
										}
									),
									Label(text = "A2:"),
									TextField(
										val = local_itemref.initiala[1],
										type = GUI::TextField::type::INT_DECIMAL,
										high = 32,
										onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val)
										{
											local_itemref.initiala[1] = val;
										}
									)
								)
							)
						)),
						TabRef(name = "Weapon", Row(
							Column(
								INITD_ROW(0, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(1, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(2, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(3, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(4, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(5, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(6, local_itemref.weap_initiald, local_itemref.weapon_initD_label),
								INITD_ROW(7, local_itemref.weap_initiald, local_itemref.weapon_initD_label)
							),
							Rows<2>(vAlign = 0.0,
								SCRIPT_LIST("Weapon Script:", list_weaponscript, local_itemref.weaponscript)
							)
						))
					))
				),
				Row(
					vAlign = 1.0,
					spacing = 2_em,
					Button(
						focused = true,
						text = "OK",
						minwidth = 90_lpx,
						onClick = message::OK),
					Button(
						text = "Cancel",
						minwidth = 90_lpx,
						onClick = message::CANCEL),
					Button(
						text = "Default",
						minwidth = 90_lpx,
						onClick = message::DEFAULT)
				)
			)
		);
	}
	loadItemClass();
	return window;
}

void ItemEditorDialog::loadItemClass()
{
	ItemNameInfo inf;
	loadinfo(&inf, local_itemref);
	
	#define __SET(obj, mem) \
	l_##obj->setText(inf.mem.size() ? inf.mem : defInfo.mem); \
	h_##obj = (inf.h_##mem.size() ? inf.h_##mem : ""); \
	if(ib_##obj) \
		ib_##obj->setDisabled(h_##obj.empty());
	
	__SET(power, power);
	
	__SET(attribs[0], misc[0]);
	__SET(attribs[1], misc[1]);
	__SET(attribs[2], misc[2]);
	__SET(attribs[3], misc[3]);
	__SET(attribs[4], misc[4]);
	__SET(attribs[5], misc[5]);
	__SET(attribs[6], misc[6]);
	__SET(attribs[7], misc[7]);
	__SET(attribs[8], misc[8]);
	__SET(attribs[9], misc[9]);
	
	__SET(flags[0], flag[0]);
	__SET(flags[1], flag[1]);
	__SET(flags[2], flag[2]);
	__SET(flags[3], flag[3]);
	__SET(flags[4], flag[4]);
	__SET(flags[5], flag[5]);
	__SET(flags[6], flag[6]);
	__SET(flags[7], flag[7]);
	__SET(flags[8], flag[8]);
	__SET(flags[9], flag[9]);
	__SET(flags[10], flag[10]);
	__SET(flags[11], flag[11]);
	__SET(flags[12], flag[12]);
	__SET(flags[13], flag[13]);
	__SET(flags[14], flag[14]);
	__SET(flags[15], flag[15]);
	
	__SET(sfx[0], actionsnd[0]);
	__SET(sfx[1], actionsnd[1]);
	
	__SET(spr[0], wpn[0]);
	__SET(spr[1], wpn[1]);
	__SET(spr[2], wpn[2]);
	__SET(spr[3], wpn[3]);
	__SET(spr[4], wpn[4]);
	__SET(spr[5], wpn[5]);
	__SET(spr[6], wpn[6]);
	__SET(spr[7], wpn[7]);
	__SET(spr[8], wpn[8]);
	__SET(spr[9], wpn[9]);
	#undef __SET
}

bool ItemEditorDialog::handleMessage(const GUI::DialogMessage<message>& msg)
{
	switch(msg.message)
	{
		case message::ITEMCLASS:
		{
			local_itemref.family = int32_t(msg.argument);
			loadItemClass();
			return false;
		}

		case message::DEFAULT:
		{
			bool cancel = false;
			AlertDialog(
				"Reset itemdata?",
				"Reset this item to default?",
				[&](bool ret)
				{
					cancel = !ret;
				}).show();
			if(cancel) return false;
			_reset_default = true;
			reset_ref = local_itemref;
			reset_name = itemname;
			return true;
		}

		case message::OK:
			saved = false;
			itemsbuf[index] = local_itemref;
			strcpy(item_string[index], itemname.c_str());
			return true;

		case message::CANCEL:
		default:
			return true;
	}
}

