/*
 * CSpellWindow.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#pragma once

#include "CWindowObject.h"

struct SDL_Surface;
struct SDL_Rect;
class IImage;
class CAnimImage;
class CPicture;
class CLabel;
class CGHeroInstance;
class CGStatusBar;
class CPlayerInterface;
class CSpellWindow;
class CSpell;

/// The spell window
class CSpellWindow : public CWindowObject
{
	class SpellArea : public CIntObject
	{
		const CSpell * mySpell;
		int schoolLevel; //range: 0 none, 3 - expert
		int whichSchool; //0 - air magic, 1 - fire magic, 2 - water magic, 3 - earth magic,
		int spellCost;
		CSpellWindow * owner;
		CAnimImage * image;
		IImage * schoolBorder;
		CLabel * name, * level, * cost;
	public:
		SpellArea(SDL_Rect pos, CSpellWindow * owner);
		~SpellArea();
		void setSpell(const CSpell * spell);

		void clickLeft(tribool down, bool previousState) override;
		void clickRight(tribool down, bool previousState) override;
		void hover(bool on) override;
		void showAll(SDL_Surface * to) override;
	};

	class InteractiveArea : public CIntObject
	{
		std::function<void()> onLeft;
		CSpellWindow * owner;

		std::string hoverText;
		std::string helpText;
	public:
		void clickLeft(tribool down, bool previousState) override;
		void clickRight(tribool down, bool previousState) override;
		void hover(bool on) override;

		InteractiveArea(const SDL_Rect & myRect, std::function<void()> funcL, int helpTextId, CSpellWindow * _owner);//c-tor
	};

	CPicture * leftCorner, * rightCorner;

	std::shared_ptr<CAnimation> spells; //pictures of spells

	CAnimImage * spellTab; //school select
	CAnimImage * schools; //schools' pictures
	std::array< std::shared_ptr<CAnimation>, 4> schoolBorders; //schools' 'borders': [0]: air, [1]: fire, [2]: water, [3]: earth

	SpellArea * spellAreas[12];
	CLabel * mana;
	CGStatusBar * statusBar;

	int sitesPerTabAdv[5];
	int sitesPerTabBattle[5];

	bool battleSpellsOnly; //if true, only battle spells are displayed; if false, only adventure map spells are displayed
	Uint8 selectedTab; // 0 - air magic, 1 - fire magic, 2 - water magic, 3 - earth magic, 4 - all schools
	int currentPage; //changes when corners are clicked
	std::vector<const CSpell *> mySpells; //all spels in this spellbook

	const CGHeroInstance * myHero; //hero whose spells are presented
	CPlayerInterface * myInt;

	void computeSpellsPerArea(); //recalculates spellAreas::mySpell

	void setCurrentPage(int value);
	void turnPageLeft();
	void turnPageRight();

public:
	CSpellWindow(const CGHeroInstance * _myHero, CPlayerInterface * _myInt, bool openOnBattleSpells = true); //c-tor
	~CSpellWindow(); //d-tor

	void fexitb();
	void fadvSpellsb();
	void fbattleSpellsb();
	void fmanaPtsb();

	void fLcornerb();
	void fRcornerb();

	void selectSchool(int school); //schools: 0 - air magic, 1 - fire magic, 2 - water magic, 3 - earth magic, 4 - all schools
	int pagesWithinCurrentTab();

	void keyPressed(const SDL_KeyboardEvent & key) override;
	void show(SDL_Surface * to) override;
};
