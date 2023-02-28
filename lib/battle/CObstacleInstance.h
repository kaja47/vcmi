/*
 * CObstacleInstance.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#pragma once
#include "BattleHex.h"

VCMI_LIB_NAMESPACE_BEGIN

class ObstacleInfo;
class ObstacleChanges;
class JsonSerializeFormat;

struct DLL_LINKAGE CObstacleInstance
{
	enum EObstacleType : ui8
	{
		//ABSOLUTE needs an underscore because it's a Win
		USUAL, ABSOLUTE_OBSTACLE, SPELL_CREATED, MOAT
	};

	BattleHex pos; //position on battlefield, typically left bottom corner
	EObstacleType obstacleType = USUAL;
	si32 uniqueID = -1;
	si32 ID = -1; //ID of obstacle (defines type of it)

	virtual ~CObstacleInstance() = default;

	const ObstacleInfo &getInfo() const; //allowed only when not generated by spell (usual or absolute)

	std::vector<BattleHex> getBlockedTiles() const;
	std::vector<BattleHex> getStoppingTile() const; //hexes that will stop stack move

	//The two functions below describe how the obstacle affects affected tiles
	//additional effects (like hurting stack or disappearing) are hardcoded for appropriate obstacleTypes
	virtual bool blocksTiles() const;
	virtual bool stopsMovement() const; //if unit stepping onto obstacle, can't continue movement (in general, doesn't checks for the side)
	virtual bool triggersEffects() const;

	virtual std::vector<BattleHex> getAffectedTiles() const;
	virtual bool visibleForSide(ui8 side, bool hasNativeStack) const; //0 attacker

	virtual void battleTurnPassed(){};

	virtual int getAnimationYOffset(int imageHeight) const;

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & ID;
		h & pos;
		h & obstacleType;
		h & uniqueID;
	}
};

struct DLL_LINKAGE MoatObstacle : CObstacleInstance
{
	std::vector<BattleHex> getAffectedTiles() const override; //for special effects (not blocking)
};

struct DLL_LINKAGE SpellCreatedObstacle : CObstacleInstance
{
	int32_t turnsRemaining;
	int32_t casterSpellPower;
	int32_t spellLevel;
	si8 casterSide; //0 - obstacle created by attacker; 1 - by defender

	bool hidden;
	bool passable;
	bool trigger;
	bool trap;
	bool removeOnTrigger;

	bool revealed;

	std::string appearSound;
	std::string appearAnimation;
	std::string triggerSound;
	std::string triggerAnimation;
	std::string animation;

	int animationYOffset;

	std::vector<BattleHex> customSize;

	SpellCreatedObstacle();

	std::vector<BattleHex> getAffectedTiles() const override;
	bool visibleForSide(ui8 side, bool hasNativeStack) const override;

	bool blocksTiles() const override;
	bool stopsMovement() const override;
	bool triggersEffects() const override;

	void battleTurnPassed() override;

	int getAnimationYOffset(int imageHeight) const override;

	void toInfo(ObstacleChanges & info);
	void fromInfo(const ObstacleChanges & info);

	void serializeJson(JsonSerializeFormat & handler);

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & static_cast<CObstacleInstance&>(*this);
		h & turnsRemaining;
		h & casterSpellPower;
		h & spellLevel;
		h & casterSide;

		h & hidden;
		h & passable;
		h & trigger;
		h & trap;

		h & customSize;
	}
};

VCMI_LIB_NAMESPACE_END
