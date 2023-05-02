/*
 * Bonus.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#pragma once

#include "../JsonNode.h"

VCMI_LIB_NAMESPACE_BEGIN

struct Bonus;
class IBonusBearer;
class CBonusSystemNode;
class ILimiter;
class IPropagator;
class IUpdater;
class BonusList;

using TBonusSubtype = int32_t;
using TBonusListPtr = std::shared_ptr<BonusList>;
using TConstBonusListPtr = std::shared_ptr<const BonusList>;
using TLimiterPtr = std::shared_ptr<ILimiter>;
using TPropagatorPtr = std::shared_ptr<IPropagator>;
using TUpdaterPtr = std::shared_ptr<IUpdater>;

class DLL_LINKAGE CAddInfo : public std::vector<si32>
{
public:
	enum { NONE = -1 };

	CAddInfo();
	CAddInfo(si32 value);

	bool operator==(si32 value) const;
	bool operator!=(si32 value) const;

	si32 & operator[](size_type pos);
	si32 operator[](size_type pos) const;

	std::string toString() const;
	JsonNode toJsonNode() const;
};

#define BONUS_TREE_DESERIALIZATION_FIX if(!h.saving && h.smartPointerSerialization) deserializationFix();

#define BONUS_LIST										\
	BONUS_NAME(NONE) 									\
	BONUS_NAME(LEVEL_COUNTER) /* for commander artifacts*/ \
	BONUS_NAME(MOVEMENT) /*Subtype is 1 - land, 0 - sea*/ \
	BONUS_NAME(MORALE) \
	BONUS_NAME(LUCK) \
	BONUS_NAME(PRIMARY_SKILL) /*uses subtype to pick skill; additional info if set: 1 - only melee, 2 - only distance*/  \
	BONUS_NAME(SIGHT_RADIUS) \
	BONUS_NAME(MANA_REGENERATION) /*points per turn apart from normal (1 + mysticism)*/  \
	BONUS_NAME(FULL_MANA_REGENERATION) /*all mana points are replenished every day*/  \
	BONUS_NAME(NONEVIL_ALIGNMENT_MIX) /*good and neutral creatures can be mixed without morale penalty*/  \
	BONUS_NAME(SURRENDER_DISCOUNT) /*%*/  \
	BONUS_NAME(STACKS_SPEED)  /*additional info - percent of speed bonus applied after direct bonuses; >0 - added, <0 - subtracted to this part*/ \
	BONUS_NAME(FLYING_MOVEMENT) /*value - penalty percentage*/ \
	BONUS_NAME(SPELL_DURATION) \
	BONUS_NAME(AIR_SPELL_DMG_PREMY) \
	BONUS_NAME(EARTH_SPELL_DMG_PREMY) \
	BONUS_NAME(FIRE_SPELL_DMG_PREMY) \
	BONUS_NAME(WATER_SPELL_DMG_PREMY) \
	BONUS_NAME(WATER_WALKING) /*value - penalty percentage*/ \
	BONUS_NAME(NEGATE_ALL_NATURAL_IMMUNITIES) \
	BONUS_NAME(STACK_HEALTH) \
	BONUS_NAME(FIRE_SPELLS) \
	BONUS_NAME(AIR_SPELLS) \
	BONUS_NAME(WATER_SPELLS) \
	BONUS_NAME(EARTH_SPELLS) \
	BONUS_NAME(GENERATE_RESOURCE) /*daily value, uses subtype (resource type)*/  \
	BONUS_NAME(CREATURE_GROWTH) /*for legion artifacts: value - week growth bonus, subtype - monster level if aplicable*/  \
	BONUS_NAME(WHIRLPOOL_PROTECTION) /*hero won't lose army when teleporting through whirlpool*/  \
	BONUS_NAME(SPELL) /*hero knows spell, val - skill level (0 - 3), subtype - spell id*/  \
	BONUS_NAME(SPELLS_OF_LEVEL) /*hero knows all spells of given level, val - skill level; subtype - level*/  \
	BONUS_NAME(BATTLE_NO_FLEEING) /*for shackles of war*/ \
	BONUS_NAME(MAGIC_SCHOOL_SKILL) /* //eg. for magic plains terrain, subtype: school of magic (0 - all, 1 - fire, 2 - air, 4 - water, 8 - earth), value - level*/ \
	BONUS_NAME(FREE_SHOOTING) /*stacks can shoot even if otherwise blocked (sharpshooter's bow effect)*/ \
	BONUS_NAME(OPENING_BATTLE_SPELL) /*casts a spell at expert level at beginning of battle, val - spell power, subtype - spell id*/ \
	BONUS_NAME(IMPROVED_NECROMANCY) /* raise more powerful creatures: subtype - creature type raised, addInfo - [required necromancy level, required stack level], val - necromancy level for this purpose */ \
	BONUS_NAME(CREATURE_GROWTH_PERCENT) /*increases growth of all units in all towns, val - percentage*/ \
	BONUS_NAME(FREE_SHIP_BOARDING) /*movement points preserved with ship boarding and landing*/  \
	BONUS_NAME(FLYING)									\
	BONUS_NAME(SHOOTER)									\
	BONUS_NAME(CHARGE_IMMUNITY)							\
	BONUS_NAME(ADDITIONAL_ATTACK)						\
	BONUS_NAME(UNLIMITED_RETALIATIONS)					\
	BONUS_NAME(NO_MELEE_PENALTY)						\
	BONUS_NAME(JOUSTING) /*for champions*/				\
	BONUS_NAME(HATE) /*eg. angels hate devils, subtype - ID of hated creature, val - damage bonus percent */ \
	BONUS_NAME(KING) /* val - required slayer bonus val to affect */\
	BONUS_NAME(MAGIC_RESISTANCE) /*in % (value)*/		\
	BONUS_NAME(CHANGES_SPELL_COST_FOR_ALLY) /*in mana points (value) , eg. mage*/ \
	BONUS_NAME(CHANGES_SPELL_COST_FOR_ENEMY) /*in mana points (value) , eg. pegasus */ \
	BONUS_NAME(SPELL_AFTER_ATTACK) /* subtype - spell id, value - chance %, addInfo[0] - level, addInfo[1] -> [0 - all attacks, 1 - shot only, 2 - melee only] */ \
	BONUS_NAME(SPELL_BEFORE_ATTACK) /* subtype - spell id, value - chance %, addInfo[0] - level, addInfo[1] -> [0 - all attacks, 1 - shot only, 2 - melee only] */ \
	BONUS_NAME(SPELL_RESISTANCE_AURA) /*eg. unicorns, value - resistance bonus in % for adjacent creatures*/ \
	BONUS_NAME(LEVEL_SPELL_IMMUNITY) /*creature is immune to all spell with level below or equal to value of this bonus */ \
	BONUS_NAME(BLOCK_MAGIC_ABOVE) /*blocks casting spells of the level > value */ \
	BONUS_NAME(BLOCK_ALL_MAGIC) /*blocks casting spells*/ \
	BONUS_NAME(TWO_HEX_ATTACK_BREATH) /*eg. dragons*/	\
	BONUS_NAME(SPELL_DAMAGE_REDUCTION) /*eg. golems; value - reduction in %, subtype - spell school; -1 - all, 0 - air, 1 - fire, 2 - water, 3 - earth*/ \
	BONUS_NAME(NO_WALL_PENALTY)							\
	BONUS_NAME(NON_LIVING) /*eg. golems, cannot be rised or healed, only neutral morale */				\
	BONUS_NAME(RANDOM_SPELLCASTER) /*eg. master genie, val - level*/ \
	BONUS_NAME(BLOCKS_RETALIATION) /*eg. naga*/			\
	BONUS_NAME(SPELL_IMMUNITY) /*subid - spell id*/		\
	BONUS_NAME(MANA_CHANNELING) /*value in %, eg. familiar*/ \
	BONUS_NAME(SPELL_LIKE_ATTACK) /*subtype - spell, value - spell level; range is taken from spell, but damage from creature; eg. magog*/ \
	BONUS_NAME(THREE_HEADED_ATTACK) /*eg. cerberus*/	\
	BONUS_NAME(GENERAL_DAMAGE_PREMY)						\
	BONUS_NAME(FIRE_IMMUNITY)	/*subtype 0 - all, 1 - all except positive, 2 - only damage spells*/						\
	BONUS_NAME(WATER_IMMUNITY)							\
	BONUS_NAME(EARTH_IMMUNITY)							\
	BONUS_NAME(AIR_IMMUNITY)							\
	BONUS_NAME(MIND_IMMUNITY)							\
	BONUS_NAME(FIRE_SHIELD)								\
	BONUS_NAME(UNDEAD)									\
	BONUS_NAME(HP_REGENERATION) /*creature regenerates val HP every new round*/					\
	BONUS_NAME(MANA_DRAIN) /*value - spell points per turn*/ \
	BONUS_NAME(LIFE_DRAIN)								\
	BONUS_NAME(DOUBLE_DAMAGE_CHANCE) /*value in %, eg. dread knight*/ \
	BONUS_NAME(RETURN_AFTER_STRIKE)						\
	BONUS_NAME(SPELLCASTER) /*subtype - spell id, value - level of school, additional info - weighted chance. use SPECIFIC_SPELL_POWER, CREATURE_SPELL_POWER or CREATURE_ENCHANT_POWER for calculating the power*/ \
	BONUS_NAME(CATAPULT)								\
	BONUS_NAME(ENEMY_DEFENCE_REDUCTION) /*in % (value) eg. behemots*/ \
	BONUS_NAME(GENERAL_DAMAGE_REDUCTION) /* shield / air shield effect, also armorer skill/petrify effect for subtype -1*/ \
	BONUS_NAME(GENERAL_ATTACK_REDUCTION) /*eg. while stoned or blinded - in %,// subtype not used, use ONLY_MELEE_FIGHT / DISTANCE_FIGHT*/ \
	BONUS_NAME(DEFENSIVE_STANCE) /* val - bonus to defense while defending */ \
	BONUS_NAME(ATTACKS_ALL_ADJACENT) /*eg. hydra*/		\
	BONUS_NAME(MORE_DAMAGE_FROM_SPELL) /*value - damage increase in %, subtype - spell id*/ \
	BONUS_NAME(FEAR)									\
	BONUS_NAME(FEARLESS)								\
	BONUS_NAME(NO_DISTANCE_PENALTY)						\
	BONUS_NAME(ENCHANTER)/* for Enchanter spells, val - skill level, subtype - spell id, additionalInfo - cooldown */ \
	BONUS_NAME(HEALER)									\
	BONUS_NAME(SIEGE_WEAPON)							\
	BONUS_NAME(HYPNOTIZED)								\
	BONUS_NAME(NO_RETALIATION) /*temporary bonus for basilisk, unicorn and scorpicore paralyze*/\
	BONUS_NAME(ADDITIONAL_RETALIATION) /*value - number of additional retaliations*/ \
	BONUS_NAME(MAGIC_MIRROR) /* value - chance of redirecting in %*/ \
	BONUS_NAME(ALWAYS_MINIMUM_DAMAGE) /*unit does its minimum damage from range; subtype: -1 - any attack, 0 - melee, 1 - ranged, value: additional damage penalty (it'll subtracted from dmg), additional info - multiplicative anti-bonus for dmg in % [eg 20 means that creature will inflict 80% of normal minimal dmg]*/ \
	BONUS_NAME(ALWAYS_MAXIMUM_DAMAGE) /*eg. bless effect, subtype: -1 - any attack, 0 - melee, 1 - ranged, value: additional damage, additional info - multiplicative bonus for dmg in %*/ \
	BONUS_NAME(ATTACKS_NEAREST_CREATURE) /*while in berserk*/ \
	BONUS_NAME(IN_FRENZY) /*value - level*/				\
	BONUS_NAME(SLAYER) /*value - level*/				\
	BONUS_NAME(FORGETFULL) /*forgetfulness spell effect, value - level*/ \
	BONUS_NAME(NOT_ACTIVE) /* subtype - spell ID (paralyze, blind, stone gaze) for graphical effect*/ 								\
	BONUS_NAME(NO_LUCK) /*eg. when fighting on cursed ground*/	\
	BONUS_NAME(NO_MORALE) /*eg. when fighting on cursed ground*/ \
	BONUS_NAME(DARKNESS) /*val = radius */ \
	BONUS_NAME(SPECIAL_SPELL_LEV) /*subtype = id, val = value per level in percent*/\
	BONUS_NAME(SPELL_DAMAGE) /*val = value, now works for sorcery*/\
	BONUS_NAME(SPECIFIC_SPELL_DAMAGE) /*subtype = id of spell, val = value*/\
	BONUS_NAME(SPECIAL_PECULIAR_ENCHANT) /*blesses and curses with id = val dependent on unit's level, subtype = 0 or 1 for Coronius*/\
	BONUS_NAME(SPECIAL_UPGRADE) /*subtype = base, additionalInfo = target */\
	BONUS_NAME(DRAGON_NATURE) \
	BONUS_NAME(CREATURE_DAMAGE)/*subtype 0 = both, 1 = min, 2 = max*/\
	BONUS_NAME(EXP_MULTIPLIER)/* val - percent of additional exp gained by stack/commander (base value 100)*/\
	BONUS_NAME(SHOTS)\
	BONUS_NAME(DEATH_STARE) /*subtype 0 - gorgon, 1 - commander*/\
	BONUS_NAME(POISON) /*val - max health penalty from poison possible*/\
	BONUS_NAME(BIND_EFFECT) /*doesn't do anything particular, works as a marker)*/\
	BONUS_NAME(ACID_BREATH) /*additional val damage per creature after attack, additional info - chance in percent*/\
	BONUS_NAME(RECEPTIVE) /*accepts friendly spells even with immunity*/\
	BONUS_NAME(DIRECT_DAMAGE_IMMUNITY) /*direct damage spells, that is*/\
	BONUS_NAME(CASTS) /*how many times creature can cast activated spell*/ \
	BONUS_NAME(SPECIFIC_SPELL_POWER) /* value used for Thunderbolt and Resurrection cast by units, subtype - spell id */\
	BONUS_NAME(CREATURE_SPELL_POWER) /* value per unit, divided by 100 (so faerie Dragons have 800)*/ \
	BONUS_NAME(CREATURE_ENCHANT_POWER) /* total duration of spells cast by creature */ \
	BONUS_NAME(ENCHANTED) /* permanently enchanted with spell subID of level = val, if val > 3 then spell is mass and has level of val-3*/ \
	BONUS_NAME(REBIRTH) /* val - percent of life restored, subtype = 0 - regular, 1 - at least one unit (sacred Phoenix) */\
	BONUS_NAME(ADDITIONAL_UNITS) /*val of units with id = subtype will be added to hero's army at the beginning of battle */\
	BONUS_NAME(SPOILS_OF_WAR) /*val * 10^-6 * gained exp resources of subtype will be given to hero after battle*/\
	BONUS_NAME(BLOCK)\
	BONUS_NAME(DISGUISED) /* subtype - spell level */\
	BONUS_NAME(VISIONS) /* subtype - spell level */\
	BONUS_NAME(NO_TERRAIN_PENALTY) /* subtype - terrain type */\
	BONUS_NAME(SOUL_STEAL) /*val - number of units gained per enemy killed, subtype = 0 - gained units survive after battle, 1 - they do not*/ \
	BONUS_NAME(TRANSMUTATION) /*val - chance to trigger in %, subtype = 0 - resurrection based on HP, 1 - based on unit count, additional info - target creature ID (attacker default)*/\
	BONUS_NAME(SUMMON_GUARDIANS) /*val - amount in % of stack count, subtype = creature ID*/\
	BONUS_NAME(CATAPULT_EXTRA_SHOTS) /*val - power of catapult effect, requires CATAPULT bonus to work*/\
	BONUS_NAME(RANGED_RETALIATION) /*allows shooters to perform ranged retaliation*/\
	BONUS_NAME(BLOCKS_RANGED_RETALIATION) /*disallows ranged retaliation for shooter unit, BLOCKS_RETALIATION bonus is for melee retaliation only*/\
	BONUS_NAME(MANUAL_CONTROL) /* manually control warmachine with id = subtype, chance = val */  \
	BONUS_NAME(WIDE_BREATH) /* initial desigh: dragon breath affecting multiple nearby hexes */\
	BONUS_NAME(FIRST_STRIKE) /* first counterattack, then attack if possible */\
	BONUS_NAME(SYNERGY_TARGET) /* dummy skill for alternative upgrades mod */\
	BONUS_NAME(SHOOTS_ALL_ADJACENT) /* H4 Cyclops-like shoot (attacks all hexes neighboring with target) without spell-like mechanics */\
	BONUS_NAME(BLOCK_MAGIC_BELOW) /*blocks casting spells of the level < value */ \
	BONUS_NAME(DESTRUCTION) /*kills extra units after hit, subtype = 0 - kill percentage of units, 1 - kill amount, val = chance in percent to trigger, additional info - amount/percentage to kill*/ \
	BONUS_NAME(SPECIAL_CRYSTAL_GENERATION) /*crystal dragon crystal generation*/ \
	BONUS_NAME(NO_SPELLCAST_BY_DEFAULT) /*spellcast will not be default attack option for this creature*/ \
	BONUS_NAME(GARGOYLE) /* gargoyle is special than NON_LIVING, cannot be rised or healed */ \
	BONUS_NAME(SPECIAL_ADD_VALUE_ENCHANT) /*specialty spell like Aenin has, increased effect of spell, additionalInfo = value to add*/\
	BONUS_NAME(SPECIAL_FIXED_VALUE_ENCHANT) /*specialty spell like Melody has, constant spell effect (i.e. 3 luck), additionalInfo = value to fix.*/\
	BONUS_NAME(TOWN_MAGIC_WELL) /*one-time pseudo-bonus to implement Magic Well in the town*/\
	BONUS_NAME(LIMITED_SHOOTING_RANGE) /*limits range of shooting creatures, doesn't adjust any other mechanics (half vs full damage etc). val - range in hexes, additional info - optional new range for broken arrow mechanic */\
	BONUS_NAME(LEARN_BATTLE_SPELL_CHANCE) /*skill-agnostic eagle eye chance. subtype = 0 - from enemy, 1 - TODO: from entire battlefield*/\
	BONUS_NAME(LEARN_BATTLE_SPELL_LEVEL_LIMIT) /*skill-agnostic eagle eye limit, subtype - school (-1 for all), others TODO*/\
	BONUS_NAME(PERCENTAGE_DAMAGE_BOOST) /*skill-agnostic archery and offence, subtype is 0 for offence and 1 for archery*/\
	BONUS_NAME(LEARN_MEETING_SPELL_LIMIT) /*skill-agnostic scholar, subtype is -1 for all, TODO for others (> 0)*/\
	BONUS_NAME(ROUGH_TERRAIN_DISCOUNT) /*skill-agnostic pathfinding*/\
	BONUS_NAME(WANDERING_CREATURES_JOIN_BONUS) /*skill-agnostic diplomacy*/\
	BONUS_NAME(BEFORE_BATTLE_REPOSITION) /*skill-agnostic tactics, bonus for allowing tactics*/\
	BONUS_NAME(BEFORE_BATTLE_REPOSITION_BLOCK) /*skill-agnostic tactics, bonus for blocking opposite tactics. For now donble side tactics is TODO.*/\
	BONUS_NAME(HERO_EXPERIENCE_GAIN_PERCENT) /*skill-agnostic learning, and we can use it as a global effect also*/\
	BONUS_NAME(UNDEAD_RAISE_PERCENTAGE) /*Percentage of killed enemy creatures to be raised after battle as undead*/\
	BONUS_NAME(MANA_PER_KNOWLEDGE) /*Percentage rate of translating 10 hero knowledge to mana, used to intelligence and global bonus*/\
	BONUS_NAME(HERO_GRANTS_ATTACKS) /*If hero can grant additional attacks to creature, value is number of attacks, subtype is creatureID*/\
	BONUS_NAME(BONUS_DAMAGE_PERCENTAGE) /*If hero can grant conditional damage to creature, value is percentage, subtype is creatureID*/\
	BONUS_NAME(BONUS_DAMAGE_CHANCE) /*If hero can grant additional damage to creature, value is chance, subtype is creatureID*/\
	BONUS_NAME(MAX_LEARNABLE_SPELL_LEVEL) /*This can work as wisdom before. val = max learnable spell level*/\
	/* end of list */


#define BONUS_SOURCE_LIST \
	BONUS_SOURCE(ARTIFACT)\
	BONUS_SOURCE(ARTIFACT_INSTANCE)\
	BONUS_SOURCE(OBJECT)\
	BONUS_SOURCE(CREATURE_ABILITY)\
	BONUS_SOURCE(TERRAIN_NATIVE)\
	BONUS_SOURCE(TERRAIN_OVERLAY)\
	BONUS_SOURCE(SPELL_EFFECT)\
	BONUS_SOURCE(TOWN_STRUCTURE)\
	BONUS_SOURCE(HERO_BASE_SKILL)\
	BONUS_SOURCE(SECONDARY_SKILL)\
	BONUS_SOURCE(HERO_SPECIAL)\
	BONUS_SOURCE(ARMY)\
	BONUS_SOURCE(CAMPAIGN_BONUS)\
	BONUS_SOURCE(SPECIAL_WEEK)\
	BONUS_SOURCE(STACK_EXPERIENCE)\
	BONUS_SOURCE(COMMANDER) /*TODO: consider using simply STACK_INSTANCE */\
	BONUS_SOURCE(GLOBAL) /*used for base bonuses which all heroes or all stacks should have*/\
	BONUS_SOURCE(OTHER) /*used for defensive stance and default value of spell level limit*/

#define BONUS_VALUE_LIST \
	BONUS_VALUE(ADDITIVE_VALUE)\
	BONUS_VALUE(BASE_NUMBER)\
	BONUS_VALUE(PERCENT_TO_ALL)\
	BONUS_VALUE(PERCENT_TO_BASE)\
	BONUS_VALUE(PERCENT_TO_SOURCE) /*Adds value only to bonuses with same source*/\
	BONUS_VALUE(PERCENT_TO_TARGET_TYPE) /*Adds value only to bonuses with SourceType target*/\
	BONUS_VALUE(INDEPENDENT_MAX) /*used for SPELL bonus */\
	BONUS_VALUE(INDEPENDENT_MIN) //used for SECONDARY_SKILL_PREMY bonus

/// Struct for handling bonuses of several types. Can be transferred to any hero
struct DLL_LINKAGE Bonus : public std::enable_shared_from_this<Bonus>
{
	enum BonusType
	{
#define BONUS_NAME(x) x,
		BONUS_LIST
#undef BONUS_NAME
	};
	enum BonusDuration //when bonus is automatically removed
	{
		PERMANENT = 1,
		ONE_BATTLE = 2, //at the end of battle
		ONE_DAY = 4,   //at the end of day
		ONE_WEEK = 8, //at the end of week (bonus lasts till the end of week, thats NOT 7 days
		N_TURNS = 16, //used during battles, after battle bonus is always removed
		N_DAYS = 32,
		UNTIL_BEING_ATTACKED = 64, /*removed after attack and counterattacks are performed*/
		UNTIL_ATTACK = 128, /*removed after attack and counterattacks are performed*/
		STACK_GETS_TURN = 256, /*removed when stack gets its turn - used for defensive stance*/
		COMMANDER_KILLED = 512
	};
	enum BonusSource
	{
#define BONUS_SOURCE(x) x,
		BONUS_SOURCE_LIST
#undef BONUS_SOURCE
		NUM_BONUS_SOURCE /*This is a dummy value, which will be always last*/
	};

	enum LimitEffect
	{
		NO_LIMIT = 0,
		ONLY_DISTANCE_FIGHT=1, ONLY_MELEE_FIGHT, //used to mark bonuses for attack/defense primary skills from spells like Precision (distance only)
	};

	enum ValueType
	{
#define BONUS_VALUE(x) x,
		BONUS_VALUE_LIST
#undef BONUS_VALUE
	};

	ui16 duration = PERMANENT; //uses BonusDuration values
	si16 turnsRemain = 0; //used if duration is N_TURNS, N_DAYS or ONE_WEEK

	BonusType type = NONE; //uses BonusType values - says to what is this bonus - 1 byte
	TBonusSubtype subtype = -1; //-1 if not applicable - 4 bytes

	BonusSource source = OTHER; //source type" uses BonusSource values - what gave that bonus
	BonusSource targetSourceType;//Bonuses of what origin this amplifies, uses BonusSource values. Needed for PERCENT_TO_TARGET_TYPE.
	si32 val = 0;
	ui32 sid = 0; //source id: id of object/artifact/spell
	ValueType valType = ADDITIVE_VALUE;
	std::string stacking; // bonuses with the same stacking value don't stack (e.g. Angel/Archangel morale bonus)

	CAddInfo additionalInfo;
	LimitEffect effectRange = NO_LIMIT; //if not NO_LIMIT, bonus will be omitted by default

	TLimiterPtr limiter;
	TPropagatorPtr propagator;
	TUpdaterPtr updater;
	TUpdaterPtr propagationUpdater;

	std::string description;

	Bonus(BonusDuration Duration, BonusType Type, BonusSource Src, si32 Val, ui32 ID, std::string Desc, si32 Subtype=-1);
	Bonus(BonusDuration Duration, BonusType Type, BonusSource Src, si32 Val, ui32 ID, si32 Subtype=-1, ValueType ValType = ADDITIVE_VALUE);
	Bonus() = default;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & duration;
		h & type;
		h & subtype;
		h & source;
		h & val;
		h & sid;
		h & description;
		h & additionalInfo;
		h & turnsRemain;
		h & valType;
		h & stacking;
		h & effectRange;
		h & limiter;
		h & propagator;
		h & updater;
		h & propagationUpdater;
		h & targetSourceType;
	}

	template <typename Ptr>
	static bool compareByAdditionalInfo(const Ptr& a, const Ptr& b)
	{
		return a->additionalInfo < b->additionalInfo;
	}
	static bool NDays(const Bonus *hb)
	{
		return hb->duration & Bonus::N_DAYS;
	}
	static bool NTurns(const Bonus *hb)
	{
		return hb->duration & Bonus::N_TURNS;
	}
	static bool OneDay(const Bonus *hb)
	{
		return hb->duration & Bonus::ONE_DAY;
	}
	static bool OneWeek(const Bonus *hb)
	{
		return hb->duration & Bonus::ONE_WEEK;
	}
	static bool OneBattle(const Bonus *hb)
	{
		return hb->duration & Bonus::ONE_BATTLE;
	}
	static bool Permanent(const Bonus *hb)
	{
		return hb->duration & Bonus::PERMANENT;
	}
	static bool UntilGetsTurn(const Bonus *hb)
	{
		return hb->duration & Bonus::STACK_GETS_TURN;
	}
	static bool UntilAttack(const Bonus *hb)
	{
		return hb->duration & Bonus::UNTIL_ATTACK;
	}
	static bool UntilBeingAttacked(const Bonus *hb)
	{
		return hb->duration & Bonus::UNTIL_BEING_ATTACKED;
	}
	static bool UntilCommanderKilled(const Bonus *hb)
	{
		return hb->duration & Bonus::COMMANDER_KILLED;
	}
	inline bool operator == (const BonusType & cf) const
	{
		return type == cf;
	}
	inline void operator += (const ui32 Val) //no return
	{
		val += Val;
	}
	STRONG_INLINE static ui32 getSid32(ui32 high, ui32 low)
	{
		return (high << 16) + low;
	}

	STRONG_INLINE static ui32 getHighFromSid32(ui32 sid)
	{
		return sid >> 16;
	}

	STRONG_INLINE static ui32 getLowFromSid32(ui32 sid)
	{
		return sid & 0x0000FFFF;
	}

	std::string Description(std::optional<si32> customValue = {}) const;
	JsonNode toJsonNode() const;
	std::string nameForBonus() const; // generate suitable name for bonus - e.g. for storing in json struct

	std::shared_ptr<Bonus> addLimiter(const TLimiterPtr & Limiter); //returns this for convenient chain-calls
	std::shared_ptr<Bonus> addPropagator(const TPropagatorPtr & Propagator); //returns this for convenient chain-calls
	std::shared_ptr<Bonus> addUpdater(const TUpdaterPtr & Updater); //returns this for convenient chain-calls
};

DLL_LINKAGE std::ostream & operator<<(std::ostream &out, const Bonus &bonus);

extern DLL_LINKAGE const std::map<std::string, Bonus::BonusType> bonusNameMap;
extern DLL_LINKAGE const std::map<std::string, Bonus::ValueType> bonusValueMap;
extern DLL_LINKAGE const std::map<std::string, Bonus::BonusSource> bonusSourceMap;
extern DLL_LINKAGE const std::map<std::string, ui16> bonusDurationMap;
extern DLL_LINKAGE const std::map<std::string, Bonus::LimitEffect> bonusLimitEffect;
extern DLL_LINKAGE const std::set<std::string> deprecatedBonusSet;

VCMI_LIB_NAMESPACE_END