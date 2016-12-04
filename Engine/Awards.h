#pragma once
#include <array>

/*  353 */
#pragma pack(push, 1)
struct Award
{
  const char   *pText;
  unsigned int  uPriority;
};
#pragma pack(pop)
extern std::array<Award, 105> pAwards;



enum AwardType: unsigned __int32
{
    Award_Invalid = 0
  , Award_Fine = 1
  , Award_EmeraldIsle_CompletedMainQuest = 2
  , Award_EmeraldIsle_FoundMissingCompetitors = 3
  , Award_Harmondale_CastleCleared = 4
  , Award_BarrowDowns_RescuedMiners = 5
  , Award_Erathia_RescuedLorenSteel = 6
  , Award_Avlee_BroughtFakeLorenSteelToErathia = 7
  , Award_Avlee_BroughtFortRiverstridePlans = 8
  , Award_Erathia_DeliveredFakePlansToElves = 9
  , Award_Promotion_Rogue = 10
  , Award_Promotion_Rogue_Honorary = 11
  , Award_Promotion_Spy = 12
  , Award_Promotion_Spy_Honorary = 13
  , Award_Promotion_Assassin = 14
  , Award_Promotion_Assassin_Honorary = 15
  , Award_Promotion_Chevalier = 16
  , Award_Promotion_Chevalier_Honorary = 17
  , Award_Promotion_Champion = 18
  , Award_Promotion_Champion_Honorary = 19
  , Award_Promotion_BlackKnight = 20
  , Award_Promotion_BlackKnight_Honorary = 21
  , Award_Promotion_Initiate = 22
  , Award_Promotion_Initiate_Honorary = 23
  , Award_Promotion_Master = 24
  , Award_Promotion_Master_Honorary = 25
  , Award_Promotion_Ninja = 26
  , Award_Promotion_Ninja_Honorary = 27
  , Award_Promotion_Hunter = 28
  , Award_Promotion_Hunter_Honorary = 29
  , Award_Promotion_RangerLord = 30
  , Award_Promotion_RangerLord_Honorary = 31
  , Award_Promotion_BountyHunter = 32
  , Award_Promotion_BountyHunter_Honorary = 33
  , Award_Promotion_WarriorMage = 34
  , Award_Promotion_WarriorMage_Honorary = 35
  , Award_Promotion_MasterArcher = 36
  , Award_Promotion_MasterArcher_Honorary = 37
  , Award_Promotion_Sniper = 38
  , Award_Promotion_Sniper_Honorary = 39
  , Award_Promotion_Crusader = 40
  , Award_Promotion_Crusader_Honorary = 41
  , Award_Promotion_Hero = 42
  , Award_Promotion_Hero_Honorary = 43
  , Award_Promotion_Villian = 44
  , Award_Promotion_Villian_Honorary = 45
  , Award_Celestia_SurvivedWallOfMist = 46
  , Award_47 = 47
  , Award_KilledXenofex = 48
  , Award_Tatalia_SolvedWineCellarMystery = 49
  , Award_Membership_ElementalGuilds = 50
  , Award_Membership_SelfGuilds = 51
  , Award_Membership_AirGuild = 52
  , Award_Membership_EarthGuild = 53
  , Award_Membership_FireGuild = 54
  , Award_Membership_WaterGuild = 55
  , Award_Membership_BodyGuild = 56
  , Award_Membership_MindGuild = 57
  , Award_Membership_SpiritGuild = 58
  , Award_Membership_LightGuild = 59
  , Award_Membership_DarkGuild = 60
  , Award_Avlee_RetrievedForestHeart = 61
  , Award_Promotion_Priest = 62
  , Award_Promotion_Priest_Honorary = 63
  , Award_Promotion_PriestOfLight = 64
  , Award_Promotion_PriestOfLight_Honorary = 65
  , Award_Promotion_PriestOfDark = 66
  , Award_Promotion_PriestOfDark_Honorary = 67
  , Award_Promotion_GreatDruid = 68
  , Award_Promotion_GreatDruid_Honorary = 69
  , Award_Promotion_ArchDruid = 70
  , Award_Promotion_ArchDruid_Honorary = 71
  , Award_Promotion_Warlock = 72
  , Award_Promotion_Warlock_Honorary = 73
  , Award_Promotion_Wizard = 74
  , Award_Promotion_Wizard_Honorary = 75
  , Award_Promotion_Archmage = 76
  , Award_Promotion_Archmage_Honorary = 77
  , Award_Promotion_Lich = 78
  , Award_Promotion_Lich_Honorary = 79
  , Award_ThePit_RetrievedSoulJars = 80
  , Award_Celestia_KilledTolberti = 81
  , Award_ThePit_ClearedKlankersLab = 82
  , Award_ThePit_KilledRobertTheWise = 83
  , Award_ThePit_SurvivedBreedingZone = 84
  , Award_Deaths = 85
  , Award_BountiesCollected = 86
  , Award_PrisonTerms = 87
  , Award_Arena_PageWins = 88
  , Award_Arena_SquireWins = 89
  , Award_Arena_KnightWins = 90
  , Award_Arena_LordWins = 91
  , Award_Harmondale_ReturnedLanterOfLight = 92
  , Award_Nighon_RetrievedHaldarsRemains = 93
  , Award_Harmondale_ReturnedDavriksSignetRing = 94
  , Award_Erathia_ReturnedParsonsQuill = 95
  , Award_RetrievedFaeriePipes = 96
  , Award_BarrowDowns_KilledTroglodytesInLowerCity = 97
  , Award_Deyja_KilledAllGriffins = 98
  , Award_Bracada_RetrievedSeasonsStole = 99
  , Award_Avlee_PlacedThreeStatuettes = 100
  , Award_101 = 101
  , Award_ArcomageChampion = 102
  , Award_ArcomageWins = 103
  , Award_ArcomageLoses = 104

};
extern std::array<AwardType, 1000> achieved_awards;
extern int       num_achieved_awards;
extern int       full_num_items_in_book;