#include "CardManager.h"

std::unordered_map<int, int> CardManager::cardIndexer;
std::unordered_map<int, Card> CardManager::cardLookup;
std::unordered_map<int, Card> CardManager::cardLookupByIndex;
std::map<std::string, int> CardManager::cardLookupCharToIndex;

void CardManager::initialize()
{
	Card copper, silver, gold, estate, duchy, province, curse,
	thief, militia, spy, cellar, chapel, chancellor, workshop, feast, remodel, throneRoom, library, mine, moat,
	councilRoom, moneylender, adventurer, bureaucrat, witch,													
	market, laboratory, festival, village, smithy, woodcutter,													
	gardens,
	nullcard;	

	nullcard.cost = -1; nullcard.name = "NoCard"; nullcard.id = -1; nullcard.charId = "z"; nullcard.cardType = -1;

	copper.cost = 0; copper.name = "Copper"; copper.id = COPPER; copper.charId = "0"; copper.cardType = TREASURECARD;

	silver.cost = 3; silver.name = "Silver"; silver.id = SILVER; silver.charId = "1"; silver.cardType = TREASURECARD;

	gold.cost = 6; gold.name = "Gold"; gold.id = GOLD; gold.charId = "2"; gold.cardType = TREASURECARD;

	estate.cost = 2; estate.name = "Estate"; estate.id = ESTATE; estate.charId = "3"; estate.cardType = VICTORYCARD;

	duchy.cost = 5; duchy.name = "Duchy"; duchy.id = DUCHY; duchy.charId = "4"; duchy.cardType = VICTORYCARD;

	province.cost = 8; province.name = "Province"; province.id = PROVINCE; province.charId = "5"; province.cardType = VICTORYCARD;

	curse.cost = 0; curse.name = "Curse"; curse.id = CURSE; curse.charId = "6"; curse.cardType = CURSECARD;

	thief.cost = 4;	thief.name = "Thief"; thief.id = THIEF; thief.charId = "7"; thief.cardType = ACTIONCARD;

	militia.cost = 4; militia.name = "Militia"; militia.id = MILITIA; militia.charId = "8"; militia.cardType = ACTIONCARD;

	spy.cost = 4; spy.name = "Spy"; spy.id = SPY; spy.charId = "9"; spy.cardType = ACTIONCARD;

	cellar.cost = 2; cellar.name = "Cellar"; cellar.id = CELLAR; cellar.charId = "a"; cellar.cardType = ACTIONCARD;

	chapel.cost = 2; chapel.name = "Chapel"; chapel.id = CHAPEL; chapel.charId = "b"; chapel.cardType = ACTIONCARD;
	 
	chancellor.cost = 3; chancellor.name = "Chancellor"; chancellor.id = CHANCELLOR;  chancellor.charId = "c"; chancellor.cardType = ACTIONCARD;

	workshop.cost = 3; workshop.name = "Workshop"; workshop.id = WORKSHOP; workshop.charId = "d"; workshop.cardType = ACTIONCARD;

	feast.cost = 4; feast.name = "Feast"; feast.id = FEAST; feast.charId = "e"; feast.cardType = ACTIONCARD;

	remodel.cost = 4; remodel.name = "Remodel"; remodel.id = REMODEL; remodel.charId = "f"; remodel.cardType = ACTIONCARD;

	throneRoom.cost = 4; throneRoom.name = "Throne Room"; throneRoom.id = THRONEROOM; throneRoom.charId = "g"; throneRoom.cardType = ACTIONCARD;

	library.cost = 5; library.name = "Library"; library.id = LIBRARY; library.charId = "h"; library.cardType = ACTIONCARD;

	mine.cost = 5; mine.name = "Mine"; mine.id = MINE; mine.charId = "i"; mine.cardType = ACTIONCARD;

	moat.cost = 2; moat.name = "Moat"; moat.id = MOAT; moat.charId = "j"; moat.cardType = ACTIONCARD;

	councilRoom.cost = 5; councilRoom.name = "Council Room"; councilRoom.id = COUNCILROOM; councilRoom.charId = "k"; councilRoom.cardType = ACTIONCARD;

	moneylender.cost = 4; moneylender.name = "Moneylender"; moneylender.id = MONEYLENDER; moneylender.charId = "j"; moneylender.cardType = ACTIONCARD;

	adventurer.cost = 6; adventurer.name = "Adventurer"; adventurer.id = ADVENTURER; adventurer.charId = "l"; adventurer.cardType = ACTIONCARD;

	bureaucrat.cost = 4; bureaucrat.name = "Bureaucrat"; bureaucrat.id = BUREAUCRAT; bureaucrat.charId = "m"; bureaucrat.cardType = ACTIONCARD;

	witch.cost = 5; witch.name = "Witch"; witch.id = WITCH; witch.charId = "n"; witch.cardType = ACTIONCARD;

	market.cost = 5; market.name = "Market"; market.id = MARKET; market.charId = "o"; market.cardType = ACTIONCARD;

	laboratory.cost = 5; laboratory.name = "Laboratory"; laboratory.id = LABORATORY; laboratory.charId = "p"; laboratory.cardType = ACTIONCARD;

	festival.cost = 5; festival.name = "Festival"; festival.id = FESTIVAL; festival.charId = "q"; festival.cardType = ACTIONCARD;
	
	village.cost = 3; village.name = "Village"; village.id = VILLAGE; village.charId = "r"; village.cardType = ACTIONCARD;

	smithy.cost = 4; smithy.name = "Smithy"; smithy.id = SMITHY; smithy.charId = "s"; smithy.cardType = ACTIONCARD;

	woodcutter.cost = 3; woodcutter.name = "Woodcutter"; woodcutter.id = WOODCUTTER; woodcutter.charId = "t"; woodcutter.cardType = ACTIONCARD;

	gardens.cost = 4; gardens.name = "Gardens"; gardens.id = GARDENS; gardens.charId = "u"; gardens.cardType = VICTORYCARD;

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
	cardIndexer[REMODEL] = 16;
	cardIndexer[BUREAUCRAT] = 17;
	cardIndexer[THIEF] = 18;

	cardLookup[-1] = nullcard;
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
	cardLookup[REMODEL] = remodel;
	cardLookup[THIEF] = thief;

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
	cardLookupByIndex[cardIndexer[REMODEL]] = remodel;
	cardLookupByIndex[cardIndexer[THIEF]] = thief;

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
	cardLookupCharToIndex[cardLookup[REMODEL].charId] = cardIndexer[REMODEL];
	cardLookupCharToIndex[cardLookup[THIEF].charId] = cardIndexer[THIEF];
}