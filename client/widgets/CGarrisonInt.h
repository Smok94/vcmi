/*
 * CGarrisonInt.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#pragma once

#include "../windows/CWindowObject.h"

class CGarrisonInt;
class CButton;
class CArmedInstance;
class CAnimImage;
class CCreatureSet;
class CGarrisonSlot;
class CStackInstance;
class CLabel;

/// A single garrison slot which holds one creature of a specific amount
class CGarrisonSlot : public CIntObject
{
	SlotID ID; //for identification
	CGarrisonInt *owner;
	const CStackInstance *myStack; //nullptr if slot is empty
	const CCreature *creature;

	/// Type of Garrison for slot (up or down)
	enum EGarrisonType
	{
		UP=0,  ///< 0 - up garrison (Garrisoned)
		DOWN,  ///< 1 - down garrison (Visiting)
	} upg; ///< Flag indicating if it is the up or down garrison

	CAnimImage * creatureImage;
	CAnimImage * selectionImage; // image for selection, not always visible
	CLabel * stackCount;

	bool viewInfo();
	bool highlightOrDropArtifact();
	bool split();
	bool mustForceReselection() const;

	void setHighlight(bool on);
public:
	virtual void hover (bool on) override; //call-in
	const CArmedInstance * getObj() const;
	bool our() const;
	bool ally() const;
	void clickRight(tribool down, bool previousState) override;
	void clickLeft(tribool down, bool previousState) override;
	void update();
	CGarrisonSlot(CGarrisonInt *Owner, int x, int y, SlotID IID, EGarrisonType Upg=EGarrisonType::UP, const CStackInstance * Creature=nullptr);

	friend class CGarrisonInt;
};

/// Class which manages slots of upper and lower garrison, splitting of units
class CGarrisonInt :public CIntObject
{
	/// Chosen slot. Should be changed only via selectSlot.
	CGarrisonSlot *highlighted;
	bool inSplittingMode;

public:
	void selectSlot(CGarrisonSlot * slot); ///< @param slot null = deselect
	const CGarrisonSlot * getSelection();

	void setSplittingMode(bool on);
	bool getSplittingMode();

	int interx;  ///< Space between slots
	Point garOffset;  ///< Offset between garrisons (not used if only one hero)
	std::vector<CButton *> splitButtons;  ///< May be empty if no buttons

	SlotID p2; ///< TODO: comment me
	bool pb,
		 smallIcons,      ///< true - 32x32 imgs, false - 58x64
		 removableUnits,  ///< player Can remove units from up
		 twoRows,         ///< slots Will be placed in 2 rows
		 owned[2];        ///< player Owns up or down army ([0] upper, [1] lower)

	std::vector<CGarrisonSlot*> availableSlots;  ///< Slots of upper and lower garrison

	const CArmedInstance *armedObjs[2];  ///< [0] is upper, [1] is down

	void setArmy(const CArmedInstance *army, bool bottomGarrison);
	void addSplitBtn(CButton * button);

	void createSlots();
	void recreateSlots();

	void splitClick();  ///< handles click on split button
	void splitStacks(int amountLeft, int amountRight);  ///< TODO: comment me

	/// Constructor
	/// @param x, y Position
	/// @param inx Distance between slots;
	/// @param garsOffset
	/// @param pomsur, SurOffset UNUSED
	/// @param s1, s2 Top and bottom armies
	/// @param _removableUnits You can take units from top
	/// @param smallImgs Units images size 64x58 or 32x32
	/// @param _twoRows Display slots in 2 row (1st row = 4 slots, 2nd = 3 slots)
	CGarrisonInt(int x, int y,
	             int inx,
	             const Point &garsOffset,
	             SDL_Surface *pomsur, const Point &SurOffset,
	             const CArmedInstance *s1, const CArmedInstance *s2=nullptr,
	             bool _removableUnits = true,
	             bool smallImgs = false,
	             bool _twoRows=false);
};

class CGarrisonHolder
{
public:
	CGarrisonHolder();
	virtual void updateGarrisons()=0;
};

class CWindowWithGarrison : public virtual CGarrisonHolder
{
public:
	CGarrisonInt *garr;
	virtual void updateGarrisons() override;
};

/// Garrison window where you can take creatures out of the hero to place it on the garrison
class CGarrisonWindow : public CWindowObject, public CWindowWithGarrison
{
public:
	CButton * quit;

	CGarrisonWindow(const CArmedInstance *up, const CGHeroInstance *down, bool removableUnits); //c-tor
};
