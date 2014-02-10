#include "CardManager.h"


void CardManager::initialize()
{
	copper.cost = 0; copper.name = "Copper"; cardLookup[COPPER] = copper;

	silver.cost = 3; silver.name = "Silver"; cardLookup[SILVER] = silver;

	gold.cost = 6; gold.name = "Gold"; cardLookup[GOLD] = gold;

	estate.cost = 2; estate.name = "Estate"; cardLookup[ESTATE] = estate;

	duchy.cost = 5; duchy.name = "Duchy"; cardLookup[DUCHY] = duchy;

	province.cost = 8; province.name = "Province"; cardLookup[PROVINCE] = province;

	curse.cost = 0; curse.name = "Curse"; cardLookup[CURSE] = curse;

	thief.cost = 4;	thief.name = "Thief"; cardLookup[THIEF] = thief;

	militia.cost = 4; militia.name = "Militia"; cardLookup[MILITIA] = militia;

	spy.cost = 4; spy.name = "Spy"; cardLookup[SPY] = spy;

	cellar.cost = 2; cellar.name = "Cellar"; cardLookup[CELLAR] = cellar;

	chapel.cost = 2; chapel.name = "Chapel"; cardLookup[CHAPEL] = chapel;

	chancellor.cost = 3; chancellor.name = "Chancellor"; cardLookup[CHANCELLOR] = chancellor;

	workshop.cost = 3; workshop.name = "Workshop"; cardLookup[WORKSHOP] = workshop;

	feast.cost = 4; feast.name = "Feast"; cardLookup[FEAST] = feast;

	remodel.cost = 4; remodel.name = "Remodel"; cardLookup[REMODEL] = remodel;

	throneRoom.cost = 4; throneRoom.name = "Throne Room"; cardLookup[THRONEROOM] = throneRoom;

	library.cost = 5; library.name = "Library"; cardLookup[LIBRARY] = library;

	mine.cost = 5; mine.name = "Mine"; cardLookup[MINE] = mine;

	moat.cost = 2; moat.name = "Moat"; cardLookup[MOAT] = moat;

	councilRoom.cost = 5; councilRoom.name = "Council Room"; cardLookup[COUNCILROOM] = councilRoom;

	moneylender.cost = 4; moneylender.name = "Moneylender"; cardLookup[MONEYLENDER] = moneylender;

	adventurer.cost = 6; adventurer.name = "Adventurer"; cardLookup[ADVENTURER] = adventurer;

	bureaucrat.cost = 4; bureaucrat.name = "Bureaucrat"; cardLookup[BUREAUCRAT] = bureaucrat;

	witch.cost = 5; witch.name = "Witch"; cardLookup[WITCH] = witch;

	market.cost = 5; market.name = "Market"; cardLookup[MARKET] = market;

	laboratory.cost = 5; laboratory.name = "Laboratory"; cardLookup[LABORATORY] = laboratory;

	festival.cost = 5; festival.name = "Festival"; cardLookup[FESTIVAL] = festival;
	
	village.cost = 3; village.name = "Village"; cardLookup[VILLAGE] = village;

	smithy.cost = 4; smithy.name = "Smithy"; cardLookup[SMITHY] = smithy;

	woodcutter.cost = 3; woodcutter.name = "Woodcutter"; cardLookup[WOODCUTTER] = woodcutter;

	gardens.cost = 4; gardens.name = "Gardens"; cardLookup[GARDENS] = gardens;

	cardIndexer[COPPER] = 0;
	cardIndexer[SILVER] = 1;
	cardIndexer[GOLD] = 2;
	cardIndexer[ESTATE] = 3;
	cardIndexer[DUCHY] = 4;
	cardIndexer[PROVINCE] = 5;
	cardIndexer[CURSE] = 6;

	cardLookupByIndex[cardIndexer[COPPER]] = copper;
	cardLookupByIndex[cardIndexer[SILVER]] = silver;
	cardLookupByIndex[cardIndexer[GOLD]] = gold;
	cardLookupByIndex[cardIndexer[ESTATE]] = estate;
	cardLookupByIndex[cardIndexer[DUCHY]] = duchy;
	cardLookupByIndex[cardIndexer[PROVINCE]] = province;
	cardLookupByIndex[cardIndexer[CURSE]] = curse;
}