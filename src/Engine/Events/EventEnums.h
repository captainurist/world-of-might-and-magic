#pragma once

enum class EventType : uint8_t {
    EVENT_Invalid = 0,
    EVENT_Exit = 1,
    EVENT_SpeakInHouse = 2,
    EVENT_PlaySound = 3,
    EVENT_MouseOver = 4,
    EVENT_LocationName = 5,
    EVENT_MoveToMap = 6,
    EVENT_OpenChest = 7,
    EVENT_ShowFace = 8,
    EVENT_ReceiveDamage = 9,
    EVENT_SetSnow = 10,
    EVENT_SetTexture = 11,
    EVENT_ShowMovie = 12,
    EVENT_SetSprite = 13,
    EVENT_Compare = 14,
    EVENT_ChangeDoorState = 15,
    EVENT_Add = 16,
    EVENT_Substract = 17,
    EVENT_Set = 18,
    EVENT_SummonMonsters = 19,

    EVENT_CastSpell = 21,
    EVENT_SpeakNPC = 22,
    EVENT_SetFacesBit = 23,
    EVENT_ToggleActorFlag = 24,
    EVENT_RandomGoTo = 25,
    EVENT_InputString = 26,

    EVENT_StatusText = 29,
    EVENT_ShowMessage = 30,
    EVENT_OnTimer = 31,
    EVENT_ToggleIndoorLight = 32,
    EVENT_PressAnyKey = 33,
    EVENT_SummonItem = 34,
    EVENT_ForPartyMember = 35,
    EVENT_Jmp = 36,
    EVENT_OnMapReload = 37,
    EVENT_OnLongTimer = 38,
    EVENT_SetNPCTopic = 39,
    EVENT_MoveNPC = 40,
    EVENT_GiveItem = 41,
    EVENT_ChangeEvent = 42,
    EVENT_CheckSkill = 43,
    EVENT_OnCanShowDialogItemCmp = 44,
    EVENT_EndCanShowDialogItem = 45,
    EVENT_SetCanShowDialogItem = 46,
    EVENT_SetNPCGroupNews = 47,
    EVENT_SetActorGroup = 48,
    EVENT_NPCSetItem = 49,
    EVENT_SetNPCGreeting = 50,
    EVENT_IsActorAlive = 51,
    EVENT_IsActorAssasinated = 52,
    EVENT_OnMapLeave = 53,
    EVENT_ChangeGroup = 54,
    EVENT_ChangeGroupAlly = 55,
    EVENT_CheckSeason = 56,
    EVENT_ToggleActorGroupFlag = 57,
    EVENT_ToggleChestFlag = 58,
    EVENT_CharacterAnimation = 59,
    EVENT_SetActorItem = 60,
    EVENT_OnDateTimer = 61,
    EVENT_EnableDateTimer = 62,
    EVENT_StopAnimation = 63,
    EVENT_CheckItemsCount = 64,
    EVENT_RemoveItems = 65,
    EVENT_SpecialJump = 66,
    EVENT_IsTotalBountyHuntingAwardInRange = 67,
    EVENT_IsNPCInParty = 68,
};
using enum EventType;

enum class VariableType {
    VAR_Sex = 0x1,
    VAR_Class = 0x2,
    VAR_CurrentHP = 0x3,
    VAR_MaxHP = 0x4,
    VAR_CurrentSP = 0x5,
    VAR_MaxSP = 0x6,
    VAR_ActualAC = 0x7,
    VAR_ACModifier = 0x8,
    VAR_BaseLevel = 0x9,
    VAR_LevelModifier = 0xA,
    VAR_Age = 0xB,
    VAR_Award = 0xC,
    VAR_Experience = 0xD,
    VAR_Race = 0xE,
    VAR_QBits_QuestsDone = 0x10,
    VAR_PlayerItemInHands = 0x11,
    VAR_Hour = 0x12,
    VAR_DayOfYear = 0x13,
    VAR_DayOfWeek = 0x14,
    VAR_FixedGold = 0x15,
    VAR_RandomGold = 0x16,
    VAR_FixedFood = 0x17,
    VAR_RandomFood = 0x18,
    VAR_MightBonus = 0x19,
    VAR_IntellectBonus = 0x1A,
    VAR_PersonalityBonus = 0x1B,
    VAR_EnduranceBonus = 0x1C,
    VAR_SpeedBonus = 0x1D,
    VAR_AccuracyBonus = 0x1E,
    VAR_LuckBonus = 0x1F,
    VAR_BaseMight = 0x20,
    VAR_BaseIntellect = 0x21,
    VAR_BasePersonality = 0x22,
    VAR_BaseEndurance = 0x23,
    VAR_BaseSpeed = 0x24,
    VAR_BaseAccuracy = 0x25,
    VAR_BaseLuck = 0x26,
    VAR_ActualMight = 0x27,
    VAR_ActualIntellect = 0x28,
    VAR_ActualPersonality = 0x29,
    VAR_ActualEndurance = 0x2A,
    VAR_ActualSpeed = 0x2B,
    VAR_ActualAccuracy = 0x2C,
    VAR_ActualLuck = 0x2D,
    VAR_FireResistance = 0x2E,
    VAR_AirResistance = 0x2F,
    VAR_WaterResistance = 0x30,
    VAR_EarthResistance = 0x31,
    VAR_SpiritResistance = 0x32,
    VAR_MindResistance = 0x33,
    VAR_BodyResistance = 0x34,
    VAR_LightResistance = 0x35,
    VAR_DarkResistance = 0x36,
    VAR_PhysicalResistance = 0x37,
    VAR_MagicResistance = 0x38,
    VAR_FireResistanceBonus = 0x39,
    VAR_AirResistanceBonus = 0x3A,
    VAR_WaterResistanceBonus = 0x3B,
    VAR_EarthResistanceBonus = 0x3C,
    VAR_SpiritResistanceBonus = 0x3D,
    VAR_MindResistanceBonus = 0x3E,
    VAR_BodyResistanceBonus = 0x3F,
    VAR_LightResistanceBonus = 0x40,
    VAR_DarkResistanceBonus = 0x41,
    VAR_PhysicalResistanceBonus = 0x42,
    VAR_MagicResistanceBonus = 0x43,
    VAR_StaffSkill = 0x44,
    VAR_SwordSkill = 0x45,
    VAR_DaggerSkill = 0x46,
    VAR_AxeSkill = 0x47,
    VAR_SpearSkill = 0x48,
    VAR_BowSkill = 0x49,
    VAR_MaceSkill = 0x4A,
    VAR_BlasterSkill = 0x4B,
    VAR_ShieldSkill = 0x4C,
    VAR_LeatherSkill = 0x4D,
    VAR_SkillChain = 0x4E,
    VAR_PlateSkill = 0x4F,
    VAR_FireSkill = 0x50,
    VAR_AirSkill = 0x51,
    VAR_WaterSkill = 0x52,
    VAR_EarthSkill = 0x53,
    VAR_SpiritSkill = 0x54,
    VAR_MindSkill = 0x55,
    VAR_BodySkill = 0x56,
    VAR_LightSkill = 0x57,
    VAR_DarkSkill = 0x58,
    VAR_IdentifyItemSkill = 0x59,
    VAR_MerchantSkill = 0x5A,
    VAR_RepairSkill = 0x5B,
    VAR_BodybuildingSkill = 0x5C,
    VAR_MeditationSkill = 0x5D,
    VAR_PerceptionSkill = 0x5E,
    VAR_DiplomacySkill = 0x5F,
    VAR_ThieverySkill = 0x60,
    VAR_DisarmTrapSkill = 0x61,
    VAR_DodgeSkill = 0x62,
    VAR_UnarmedSkill = 0x63,
    VAR_IdentifyMonsterSkill = 0x64,
    VAR_ArmsmasterSkill = 0x65,
    VAR_StealingSkill = 0x66,
    VAR_AlchemySkill = 0x67,
    VAR_LearningSkill = 0x68,
    VAR_Cursed = 0x69,
    VAR_Weak = 0x6A,
    VAR_Asleep = 0x6B,
    VAR_Afraid = 0x6C,
    VAR_Drunk = 0x6D,
    VAR_Insane = 0x6E,
    VAR_PoisonedGreen = 0x6F,
    VAR_DiseasedGreen = 0x70,
    VAR_PoisonedYellow = 0x71,
    VAR_DiseasedYellow = 0x72,
    VAR_PoisonedRed = 0x73,
    VAR_DiseasedRed = 0x74,
    VAR_Paralyzed = 0x75,
    VAR_Unconsious = 0x76,
    VAR_Dead = 0x77,
    VAR_Stoned = 0x78,
    VAR_Eradicated = 0x79,
    VAR_MajorCondition = 0x7A,
    VAR_MapPersistentVariable_0 = 0x7B,
    VAR_MapPersistentVariable_74 = 0xC5,
    VAR_MapPersistentDecorVariable_0 = 0xC6,
    VAR_MapPersistentDecorVariable_24 = 0xDE,
    VAR_AutoNotes = 0xDF,
    VAR_IsMightMoreThanBase = 0xE0,
    VAR_IsIntellectMoreThanBase = 0xE1,
    VAR_IsPersonalityMoreThanBase = 0xE2,
    VAR_IsEnduranceMoreThanBase = 0xE3,
    VAR_IsSpeedMoreThanBase = 0xE4,
    VAR_IsAccuracyMoreThanBase = 0xE5,
    VAR_IsLuckMoreThanBase = 0xE6,
    VAR_PlayerBits = 0xE7,
    VAR_NPCs2 = 0xE8,
    VAR_IsFlying = 0xF0,
    VAR_HiredNPCHasSpeciality = 0xF1,
    VAR_CircusPrises = 0xF2,
    VAR_NumSkillPoints = 0xF3,
    VAR_MonthIs = 0xF4,
    VAR_Counter1 = 0xF5,
    VAR_Counter2 = 0xF6,
    VAR_Counter3 = 0xF7,
    VAR_Counter4 = 0xF8,
    VAR_Counter5 = 0xF9,
    VAR_Counter6 = 0xFA,
    VAR_Counter7 = 0xFB,
    VAR_Counter8 = 0xFC,
    VAR_Counter9 = 0xFD,
    VAR_Counter10 = 0xFE,
    VAR_UnknownTimeEvent0 = 0xFF,
    VAR_UnknownTimeEvent19 = 0x112,
    VAR_ReputationInCurrentLocation = 0x113,
    VAR_History_0 = 0x114,
    VAR_History_28 = 0x130,
    VAR_Unknown1 = 0x131,
    VAR_GoldInBank = 0x132,
    VAR_NumDeaths = 0x133,
    VAR_NumBounties = 0x134,
    VAR_PrisonTerms = 0x135,
    VAR_ArenaWinsPage = 0x136,
    VAR_ArenaWinsSquire = 0x137,
    VAR_ArenaWinsKnight = 0x138,
    VAR_ArenaWinsLord = 0x139,
    VAR_Invisible = 0x13A,
    VAR_ItemEquipped = 0x13B,
};
using enum VariableType;

enum class SEASON {
    SPRING = 0,
    SUMMER = 1,
    AUTUMN = 2,
    WINTER = 3
};
using enum SEASON;

// TODO(Nik-RE-dev): currently exclusive for MM7, need to be independent from players number
enum class PLAYER_CHOOSE_POLICY {
    CHOOSE_PLAYER1 = 0,
    CHOOSE_PLAYER2 = 1,
    CHOOSE_PLAYER3 = 2,
    CHOOSE_PLAYER4 = 3,

    CHOOSE_ACTIVE = 4,
    CHOOSE_PARTY = 5,
    CHOOSE_RANDOM = 6
};
using enum PLAYER_CHOOSE_POLICY;

