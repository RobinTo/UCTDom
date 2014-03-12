#include "CardManager.h"


void CardManager::initialize()
{
	Card copper, silver, gold, estate, duchy, province, curse,
	thief, militia, spy, cellar, chapel, chancellor, workshop, feast, remodel, throneRoom, library, mine, moat,
	councilRoom, moneylender, adventurer, bureaucrat, witch,													
	market, laboratory, festival, village, smithy, woodcutter,													
	gardens;	

	copper.cost = 0; copper.name = "Copper"; cardLookup[COPPER] = copper; copper.id = COPPER;

	silver.cost = 3; silver.name = "Silver"; cardLookup[SILVER] = silver; silver.id = SILVER;

	gold.cost = 6; gold.name = "Gold"; cardLookup[GOLD] = gold; gold.id = GOLD;

	estate.cost = 2; estate.name = "Estate"; cardLookup[ESTATE] = estate; estate.id = ESTATE;

	duchy.cost = 5; duchy.name = "Duchy"; cardLookup[DUCHY] = duchy; duchy.id = DUCHY;

	province.cost = 8; province.name = "Province"; cardLookup[PROVINCE] = province; province.id = PROVINCE;

	curse.cost = 0; curse.name = "Curse"; cardLookup[CURSE] = curse; curse.id = CURSE;

	thief.cost = 4;	thief.name = "Thief"; cardLookup[THIEF] = thief; thief.id = THIEF;

	militia.cost = 4; militia.name = "Militia"; cardLookup[MILITIA] = militia; militia.id = MILITIA;

	spy.cost = 4; spy.name = "Spy"; cardLookup[SPY] = spy; spy.id = SPY;

	cellar.cost = 2; cellar.name = "Cellar"; cardLookup[CELLAR] = cellar; cellar.id = CELLAR;

	chapel.cost = 2; chapel.name = "Chapel"; cardLookup[CHAPEL] = chapel; chapel.id = CHAPEL;
	 
	chancellor.cost = 3; chancellor.name = "Chancellor"; cardLookup[CHANCELLOR] = chancellor; chancellor.id = CHANCELLOR;

	workshop.cost = 3; workshop.name = "Workshop"; cardLookup[WORKSHOP] = workshop; workshop.id = WORKSHOP;

	feast.cost = 4; feast.name = "Feast"; cardLookup[FEAST] = feast; feast.id = FEAST;

	remodel.cost = 4; remodel.name = "Remodel"; cardLookup[REMODEL] = remodel; remodel.id = REMODEL;

	throneRoom.cost = 4; throneRoom.name = "Throne Room"; cardLookup[THRONEROOM] = throneRoom; throneRoom.id = THRONEROOM;

	library.cost = 5; library.name = "Library"; cardLookup[LIBRARY] = library; library.id = LIBRARY;

	mine.cost = 5; mine.name = "Mine"; cardLookup[MINE] = mine; mine.id = MINE;

	moat.cost = 2; moat.name = "Moat"; cardLookup[MOAT] = moat; moat.id = MOAT;

	councilRoom.cost = 5; councilRoom.name = "Council Room"; cardLookup[COUNCILROOM] = councilRoom; councilRoom.id = COUNCILROOM;

	moneylender.cost = 4; moneylender.name = "Moneylender"; cardLookup[MONEYLENDER] = moneylender; moneylender.id = MONEYLENDER;

	adventurer.cost = 6; adventurer.name = "Adventurer"; cardLookup[ADVENTURER] = adventurer; adventurer.id = ADVENTURER;

	bureaucrat.cost = 4; bureaucrat.name = "Bureaucrat"; cardLookup[BUREAUCRAT] = bureaucrat; bureaucrat.id = BUREAUCRAT;

	witch.cost = 5; witch.name = "Witch"; cardLookup[WITCH] = witch; witch.id = WITCH;

	market.cost = 5; market.name = "Market"; cardLookup[MARKET] = market; market.id = MARKET;

	laboratory.cost = 5; laboratory.name = "Laboratory"; cardLookup[LABORATORY] = laboratory; laboratory.id = LABORATORY;

	festival.cost = 5; festival.name = "Festival"; cardLookup[FESTIVAL] = festival; festival.id = FESTIVAL;
	
	village.cost = 3; village.name = "Village"; cardLookup[VILLAGE] = village; village.id = VILLAGE;

	smithy.cost = 4; smithy.name = "Smithy"; cardLookup[SMITHY] = smithy; smithy.id = SMITHY;

	woodcutter.cost = 3; woodcutter.name = "Woodcutter"; cardLookup[WOODCUTTER] = woodcutter; woodcutter.id = WOODCUTTER;

	gardens.cost = 4; gardens.name = "Gardens"; cardLookup[GARDENS] = gardens; gardens.id = GARDENS;

	cardIndexer[COPPER] = 0;
	cardIndexer[SILVER] = 1;
	cardIndexer[GOLD] = 2;
	cardIndexer[ESTATE] = 3;
	cardIndexer[DUCHY] = 4;
	cardIndexer[PROVINCE] = 5;
	cardIndexer[CURSE] = 6;
	cardIndexer[WOODCUTTER] = 7;
	cardIndexer[GARDENS] = 8;
	cardIndexer[FESTIVAL] = 9;

	cardLookupByIndex[cardIndexer[COPPER]] = copper;
	cardLookupByIndex[cardIndexer[SILVER]] = silver;
	cardLookupByIndex[cardIndexer[GOLD]] = gold;
	cardLookupByIndex[cardIndexer[ESTATE]] = estate;
	cardLookupByIndex[cardIndexer[DUCHY]] = duchy;
	cardLookupByIndex[cardIndexer[PROVINCE]] = province;
	cardLookupByIndex[cardIndexer[CURSE]] = curse;
	cardLookupByIndex[cardIndexer[WOODCUTTER]] = woodcutter;
	cardLookupByIndex[cardIndexer[GARDENS]] = gardens;
	cardLookupByIndex[cardIndexer[FESTIVAL]] = festival;
}