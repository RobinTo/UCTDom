#include "CardManager.h"


void CardManager::initialize()
{
	Card copper, silver, gold, estate, duchy, province, curse,
	thief, militia, spy, cellar, chapel, chancellor, workshop, feast, remodel, throneRoom, library, mine, moat,
	councilRoom, moneylender, adventurer, bureaucrat, witch,													
	market, laboratory, festival, village, smithy, woodcutter,													
	gardens;	

	copper.cost = 0; copper.name = "Copper"; copper.id = COPPER; copper.charId = "0";

	silver.cost = 3; silver.name = "Silver"; silver.id = SILVER; silver.charId = "1"; 

	gold.cost = 6; gold.name = "Gold"; gold.id = GOLD; gold.charId = "2";

	estate.cost = 2; estate.name = "Estate"; estate.id = ESTATE; estate.charId = "3";

	duchy.cost = 5; duchy.name = "Duchy"; duchy.id = DUCHY; duchy.charId = "4";

	province.cost = 8; province.name = "Province"; province.id = PROVINCE; province.charId = "5";

	curse.cost = 0; curse.name = "Curse"; curse.id = CURSE; curse.charId = "6";

	thief.cost = 4;	thief.name = "Thief"; thief.id = THIEF; thief.charId = "7";

	militia.cost = 4; militia.name = "Militia"; militia.id = MILITIA; militia.charId = "8";

	spy.cost = 4; spy.name = "Spy"; spy.id = SPY; spy.charId = "9";

	cellar.cost = 2; cellar.name = "Cellar"; cellar.id = CELLAR; cellar.charId = "a";

	chapel.cost = 2; chapel.name = "Chapel"; chapel.id = CHAPEL; chapel.charId = "b";
	 
	chancellor.cost = 3; chancellor.name = "Chancellor"; chancellor.id = CHANCELLOR;  chancellor.charId = "c";

	workshop.cost = 3; workshop.name = "Workshop"; workshop.id = WORKSHOP; workshop.charId = "d";

	feast.cost = 4; feast.name = "Feast"; feast.id = FEAST; feast.charId = "e";

	remodel.cost = 4; remodel.name = "Remodel"; remodel.id = REMODEL; remodel.charId = "f";

	throneRoom.cost = 4; throneRoom.name = "Throne Room"; throneRoom.id = THRONEROOM; throneRoom.charId = "g";

	library.cost = 5; library.name = "Library"; library.id = LIBRARY; library.charId = "h";

	mine.cost = 5; mine.name = "Mine"; mine.id = MINE; mine.charId = "i";

	moat.cost = 2; moat.name = "Moat"; moat.id = MOAT; moat.charId = "j";

	councilRoom.cost = 5; councilRoom.name = "Council Room"; councilRoom.id = COUNCILROOM; councilRoom.charId = "k";

	moneylender.cost = 4; moneylender.name = "Moneylender"; moneylender.id = MONEYLENDER; moneylender.charId = "j";

	adventurer.cost = 6; adventurer.name = "Adventurer"; adventurer.id = ADVENTURER; adventurer.charId = "l";

	bureaucrat.cost = 4; bureaucrat.name = "Bureaucrat"; bureaucrat.id = BUREAUCRAT; bureaucrat.charId = "m";

	witch.cost = 5; witch.name = "Witch"; witch.id = WITCH; witch.charId = "n";

	market.cost = 5; market.name = "Market"; market.id = MARKET; market.charId = "o";

	laboratory.cost = 5; laboratory.name = "Laboratory"; laboratory.id = LABORATORY; laboratory.charId = "p";

	festival.cost = 5; festival.name = "Festival"; festival.id = FESTIVAL; festival.charId = "q";
	
	village.cost = 3; village.name = "Village"; village.id = VILLAGE; village.charId = "r";

	smithy.cost = 4; smithy.name = "Smithy"; smithy.id = SMITHY; smithy.charId = "s";

	woodcutter.cost = 3; woodcutter.name = "Woodcutter"; woodcutter.id = WOODCUTTER; woodcutter.charId = "t";

	gardens.cost = 4; gardens.name = "Gardens"; gardens.id = GARDENS; gardens.charId = "u";

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
	cardIndexer[MONEYLENDER] = 10;
	cardIndexer[SMITHY] = 11;
	cardIndexer[VILLAGE] = 12;
	cardIndexer[MARKET] = 13;
	cardIndexer[LABORATORY] = 14;
	cardIndexer[WITCH] = 15;
	cardIndexer[BUREAUCRAT] = 16;

	cardLookup[COPPER] = copper;
	cardLookup[SILVER] = silver;
	cardLookup[GOLD] = gold;
	cardLookup[ESTATE] = estate;
	cardLookup[DUCHY] = duchy;
	cardLookup[PROVINCE] = province;
	cardLookup[CURSE] = curse;
	cardLookup[WOODCUTTER] = woodcutter;
	cardLookup[GARDENS] = gardens;
	cardLookup[FESTIVAL] = festival;
	cardLookup[MONEYLENDER] = moneylender;
	cardLookup[SMITHY] = smithy;
	cardLookup[VILLAGE] = village;
	cardLookup[MARKET] = market;
	cardLookup[LABORATORY] = laboratory;
	cardLookup[WITCH] = witch;
	cardLookup[BUREAUCRAT] = bureaucrat;

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
	cardLookupByIndex[cardIndexer[MONEYLENDER]] = moneylender;
	cardLookupByIndex[cardIndexer[SMITHY]] = smithy;
	cardLookupByIndex[cardIndexer[VILLAGE]] = village;
	cardLookupByIndex[cardIndexer[MARKET]] = market;
	cardLookupByIndex[cardIndexer[LABORATORY]] = laboratory;
	cardLookupByIndex[cardIndexer[WITCH]] = witch;
	cardLookupByIndex[cardIndexer[BUREAUCRAT]] = bureaucrat;

	cardLookupCharToIndex[cardLookup[COPPER].charId] = cardIndexer[COPPER];
	cardLookupCharToIndex[cardLookup[SILVER].charId] = cardIndexer[SILVER];
	cardLookupCharToIndex[cardLookup[GOLD].charId] = cardIndexer[GOLD];
	cardLookupCharToIndex[cardLookup[ESTATE].charId] = cardIndexer[ESTATE];
	cardLookupCharToIndex[cardLookup[DUCHY].charId] = cardIndexer[DUCHY];
	cardLookupCharToIndex[cardLookup[PROVINCE].charId] = cardIndexer[PROVINCE];
	cardLookupCharToIndex[cardLookup[CURSE].charId] = cardIndexer[CURSE];
	cardLookupCharToIndex[cardLookup[WOODCUTTER].charId] = cardIndexer[WOODCUTTER];
	cardLookupCharToIndex[cardLookup[GARDENS].charId] = cardIndexer[GARDENS];
	cardLookupCharToIndex[cardLookup[FESTIVAL].charId] = cardIndexer[FESTIVAL];
	cardLookupCharToIndex[cardLookup[MONEYLENDER].charId] = cardIndexer[MONEYLENDER];
	cardLookupCharToIndex[cardLookup[SMITHY].charId] = cardIndexer[SMITHY];
	cardLookupCharToIndex[cardLookup[VILLAGE].charId] = cardIndexer[VILLAGE];
	cardLookupCharToIndex[cardLookup[MARKET].charId] = cardIndexer[MARKET];
	cardLookupCharToIndex[cardLookup[LABORATORY].charId] = cardIndexer[LABORATORY];
	cardLookupCharToIndex[cardLookup[WITCH].charId] = cardIndexer[WITCH];
	cardLookupCharToIndex[cardLookup[BUREAUCRAT].charId] = cardIndexer[BUREAUCRAT];
}