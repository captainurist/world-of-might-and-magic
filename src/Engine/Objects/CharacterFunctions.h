#pragma once

#include <cassert>

#include "Engine/Spells/SpellEnums.h"

#include "Utility/Workaround/ToUnderlying.h"
#include "Utility/Segment.h"

#include "CharacterEnums.h"

inline Segment<CharacterSkillMastery> allSkillMasteries() {
    return Segment(CHARACTER_SKILL_MASTERY_FIRST, CHARACTER_SKILL_MASTERY_LAST);
}

inline Segment<CharacterSkillType> allSkills() {
    return Segment(CHARACTER_SKILL_FIRST, CHARACTER_SKILL_LAST);
}

/**
 * @return                              List of skills that are visible to the player and that are stored in a savegame.
 */
inline Segment<CharacterSkillType> allVisibleSkills() {
    return Segment(CHARACTER_SKILL_FIRST_VISIBLE, CHARACTER_SKILL_LAST_VISIBLE);
}

/**
 * @return                              List of skills that are drawn in the "Armor" section of the character
 *                                      screen's skills tab.
 */
inline std::initializer_list<CharacterSkillType> allArmorSkills() {
    static constexpr std::initializer_list<CharacterSkillType> result = {
        CHARACTER_SKILL_LEATHER, CHARACTER_SKILL_CHAIN, CHARACTER_SKILL_PLATE,
        CHARACTER_SKILL_SHIELD,  CHARACTER_SKILL_DODGE
    };

    return result;
}

/**
 * @return                              List of skills that are drawn in the "Weapons" section of the character
 *                                      screen's skills tab.
 */
inline std::initializer_list<CharacterSkillType> allWeaponSkills() {
    static constexpr std::initializer_list<CharacterSkillType> result = {
        CHARACTER_SKILL_AXE,   CHARACTER_SKILL_BOW,     CHARACTER_SKILL_DAGGER,
        CHARACTER_SKILL_MACE,  CHARACTER_SKILL_SPEAR,   CHARACTER_SKILL_STAFF,
        CHARACTER_SKILL_SWORD, CHARACTER_SKILL_UNARMED, CHARACTER_SKILL_BLASTER
        // CHARACTER_SKILL_CLUB is not displayed in skills.
    };

    return result;
}

/**
 * @return                              List of skills that are drawn in the "Misc" section of the character
 *                                      screen's skills tab.
 */
inline std::initializer_list<CharacterSkillType> allMiscSkills() {
    static constexpr std::initializer_list<CharacterSkillType> result = {
        CHARACTER_SKILL_ALCHEMY,      CHARACTER_SKILL_ARMSMASTER,
        CHARACTER_SKILL_BODYBUILDING, CHARACTER_SKILL_ITEM_ID,
        CHARACTER_SKILL_MONSTER_ID,   CHARACTER_SKILL_LEARNING,
        CHARACTER_SKILL_TRAP_DISARM,  CHARACTER_SKILL_MEDITATION,
        CHARACTER_SKILL_MERCHANT,     CHARACTER_SKILL_PERCEPTION,
        CHARACTER_SKILL_REPAIR,       CHARACTER_SKILL_STEALING
    };

    return result;
}

/**
 * @return                              List of skills that are drawn in the "Magic" section of the character
 *                                      screen's skills tab.
 */
inline std::initializer_list<CharacterSkillType> allMagicSkills() {
    static constexpr std::initializer_list<CharacterSkillType> result = {
        CHARACTER_SKILL_FIRE,  CHARACTER_SKILL_AIR,    CHARACTER_SKILL_WATER,
        CHARACTER_SKILL_EARTH, CHARACTER_SKILL_SPIRIT, CHARACTER_SKILL_MIND,
        CHARACTER_SKILL_BODY,  CHARACTER_SKILL_LIGHT,  CHARACTER_SKILL_DARK
    };

    return result;
}

inline CharacterSkillType skillForMagicSchool(MagicSchool school) {
    switch (school) {
    case MAGIC_SCHOOL_FIRE:     return CHARACTER_SKILL_FIRE;
    case MAGIC_SCHOOL_AIR:      return CHARACTER_SKILL_AIR;
    case MAGIC_SCHOOL_WATER:    return CHARACTER_SKILL_WATER;
    case MAGIC_SCHOOL_EARTH:    return CHARACTER_SKILL_EARTH;
    case MAGIC_SCHOOL_SPIRIT:   return CHARACTER_SKILL_SPIRIT;
    case MAGIC_SCHOOL_MIND:     return CHARACTER_SKILL_MIND;
    case MAGIC_SCHOOL_BODY:     return CHARACTER_SKILL_BODY;
    case MAGIC_SCHOOL_LIGHT:    return CHARACTER_SKILL_LIGHT;
    case MAGIC_SCHOOL_DARK:     return CHARACTER_SKILL_DARK;
    default:
        assert(false);
        return CHARACTER_SKILL_INVALID;
    }
}

inline CharacterSkillType skillForSpell(SpellId spell) {
    if (isRegularSpell(spell)) {
        return skillForMagicSchool(magicSchoolForSpell(spell));
    } else if (spell == SPELL_BOW_ARROW) {
        return CHARACTER_SKILL_BOW;
    } else if (spell == SPELL_LASER_PROJECTILE) {
        return CHARACTER_SKILL_BLASTER;
    } else {
        assert(false && "Unknown spell");
        return CHARACTER_SKILL_INVALID;
    }
}

inline CharacterClassType getTier1Class(CharacterClassType classType) {
    return static_cast<CharacterClassType>(std::to_underlying(classType) & ~3);
}

inline CharacterClassType getTier2Class(CharacterClassType classType) {
    return static_cast<CharacterClassType>((std::to_underlying(classType) & ~3) + 1);
}

inline CharacterClassType getTier3LightClass(CharacterClassType classType) {
    return static_cast<CharacterClassType>((std::to_underlying(classType) & ~3) + 2);
}

inline CharacterClassType getTier3DarkClass(CharacterClassType classType) {
    return static_cast<CharacterClassType>((std::to_underlying(classType) & ~3) + 3);
}

inline int getClassTier(CharacterClassType classType) {
    int index = (std::to_underlying(classType) & 3);
    return index == 3 ? 3 : index + 1;
}

/**
 * Get promotions of higher tier class relative to given one.
 *
 * Base class is of tier 1.
 * After initial promotion class becomes tier 2.
 * Tier 2 class is promoted through light or dark path to tier 3 class.
 *
 * @param classType     Character class.
 */
inline Segment<CharacterClassType> getClassPromotions(CharacterClassType classType) {
    int tier = getClassTier(classType);

    if (tier == 1) {
        return {getTier2Class(classType), getTier3DarkClass(classType)};
    } else if (tier == 2) {
        return {getTier3LightClass(classType), getTier3DarkClass(classType)};
    } else {
        return {}; // tier 3 max
    }
}

inline Segment<CharacterAttributeType> enchantableAttributes() {
    return Segment(CHARACTER_ATTRIBUTE_FIRST_ENCHANTABLE, CHARACTER_ATTRIBUTE_LAST_ENCHANTABLE);
}

inline Segment<CharacterAttributeType> statAttributes() {
    return Segment(CHARACTER_ATTRIBUTE_FIRST_STAT, CHARACTER_ATTRIBUTE_LAST_STAT);
}
