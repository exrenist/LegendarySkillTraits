#pragma once

#include "pch.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <string_view>

namespace LegendaryTraits
{
    inline constexpr std::size_t kSkillCount = 18;

    inline constexpr std::array<const char*, kSkillCount> kSkillNames = {
        "One-Handed",    // 0
        "Two-Handed",    // 1
        "Archery",       // 2
        "Block",         // 3
        "Smithing",      // 4
        "Heavy Armor",   // 5
        "Light Armor",   // 6
        "Pickpocket",    // 7
        "Lockpicking",   // 8
        "Sneak",         // 9
        "Alchemy",       // 10
        "Speech",        // 11
        "Alteration",    // 12
        "Conjuration",   // 13
        "Destruction",   // 14
        "Illusion",      // 15
        "Restoration",   // 16
        "Enchanting"     // 17
    };

    struct TraitSettings
    {
        // Combat & Weapon Math
        float oneHandedDamagePerLevel = 8.0f;
        float oneHandedCritPerLevel = 4.0f;
        float twoHandedDamagePerLevel = 10.0f;
        float twoHandedCritPerLevel = 5.0f;
        float archeryDamagePerLevel = 8.0f;
        float archeryCritPerLevel = 4.0f;
        float archeryStaggerPerLevel = 5.0f;
        float blockElementalReductionPerLevel = 5.0f;    // +5% Passive Elemental, Magic & Poison Resistance per legendary level
        float sneakEffectivenessPerLevel = 5.0f;
        float sneakAttackMultPerLevel = 10.0f;
        float lockpickingWidthPerLevel = 5.0f;
        std::int32_t lockpickingGoldPerLevel = 30;
        float pickpocketChancePerLevel = 5.0f;
        std::int32_t pickpocketGoldPerLevel = 15;

        // Magic Magnitudes & Durations
        float destructionPowerPerLevel = 10.0f;
        float restorationPowerPerLevel = 10.0f;
        float alterationDurationPerLevel = 15.0f;
        float illusionPowerPerLevel = 10.0f;

        // Crafting & Barter
        float vendorPriceBonusPerLevel = 5.0f;
        float smithingArmorPenPerLevel = 5.0f;
        float alchemyPotencyPerLevel = 3.0f;
        float enchantingPowerPerLevel = 5.0f;
        float enchantingChargeReductionPerLevel = 1.5f;

        // Core Character Attributes
        float healthPercentPerLevel = 0.05f;
        float staggerResistPerLevel = 0.08f;
        float staminaRatePerLevel = 10.0f;
        float lightArmorStaminaBonusPerLevel = 5.0f;
        float carryWeightPerLevel = 25.0f;
        float poisonResistPerLevel = 10.0f;
        float spellAbsorptionPerLevel = 3.0f;

        // Diagnostics
        bool verboseLogging = false;
    };

    inline TraitSettings g_settings{};

    struct AppliedModifiers
    {
        float carryWeight = 0.0f;
        float health = 0.0f;
        float staminaRate = 0.0f;
        float lightArmorStamina = 0.0f;
        float poisonResist = 0.0f;
        float spellAbsorption = 0.0f;
        float massModifier = 0.0f;

        float oneHandedDamage = 0.0f;
        float twoHandedDamage = 0.0f;
        float archeryDamage = 0.0f;
        float smithingPenetration = 0.0f;
        float criticalChance = 0.0f;
        float blockElementalResist = 0.0f;
        float sneakAttackMult = 0.0f;

        float destructionPower = 0.0f;
        float restorationPower = 0.0f;
        float illusionPower = 0.0f;
        float alterationDuration = 0.0f;
        float conjurationPower = 0.0f;

        float sneakModifier = 0.0f;
        float pickpocketModifier = 0.0f;
        float lockpickingModifier = 0.0f;
        float speechcraftModifier = 0.0f;

        float alchemyPotency = 0.0f;
        float enchantingModifier = 0.0f;
    };

    inline AppliedModifiers g_appliedMods{};
    inline std::array<std::uint32_t, kSkillCount> g_legendaryCounts{};
    inline std::atomic<bool> g_isApplyQueued{ false };

    inline void UpdateSkillCounts()
    {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) return;

        const auto& infoData = player->GetInfoRuntimeData();
        const auto playerSkills = infoData.skills;
        if (!playerSkills || !playerSkills->data) return;

        for (std::size_t i = 0; i < kSkillCount; ++i) {
            g_legendaryCounts[i] = playerSkills->data->legendaryLevels[i];
        }
    }

    inline void RevertOldModifiers(RE::PlayerCharacter* a_player)
    {
        if (!a_player) return;

        const auto avOwner = a_player->AsActorValueOwner();
        if (!avOwner) return;

        constexpr auto modifier = RE::ACTOR_VALUE_MODIFIER::kTemporary;

        if (g_appliedMods.carryWeight > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kCarryWeight, -g_appliedMods.carryWeight);
            g_appliedMods.carryWeight = 0.0f;
        }
        if (g_appliedMods.smithingPenetration > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kOneHandedPowerModifier, -g_appliedMods.smithingPenetration);
            avOwner->ModActorValue(modifier, RE::ActorValue::kTwoHandedPowerModifier, -g_appliedMods.smithingPenetration);
            avOwner->ModActorValue(modifier, RE::ActorValue::kMarksmanPowerModifier, -g_appliedMods.smithingPenetration);
            g_appliedMods.smithingPenetration = 0.0f;
        }
        if (g_appliedMods.health > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kHealth, -g_appliedMods.health);
            g_appliedMods.health = 0.0f;
        }
        if (g_appliedMods.staminaRate > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kStaminaRate, -g_appliedMods.staminaRate);
            g_appliedMods.staminaRate = 0.0f;
        }
        if (g_appliedMods.lightArmorStamina > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kStamina, -g_appliedMods.lightArmorStamina);
            g_appliedMods.lightArmorStamina = 0.0f;
        }
        if (g_appliedMods.poisonResist > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kPoisonResist, -g_appliedMods.poisonResist);
            g_appliedMods.poisonResist = 0.0f;
        }
        if (g_appliedMods.spellAbsorption > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kAbsorbChance, -g_appliedMods.spellAbsorption);
            g_appliedMods.spellAbsorption = 0.0f;
        }
        if (g_appliedMods.massModifier > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kMass, -g_appliedMods.massModifier);
            g_appliedMods.massModifier = 0.0f;
        }
        if (g_appliedMods.oneHandedDamage > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kOneHandedPowerModifier, -g_appliedMods.oneHandedDamage);
            g_appliedMods.oneHandedDamage = 0.0f;
        }
        if (g_appliedMods.twoHandedDamage > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kTwoHandedPowerModifier, -g_appliedMods.twoHandedDamage);
            g_appliedMods.twoHandedDamage = 0.0f;
        }
        if (g_appliedMods.archeryDamage > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kMarksmanPowerModifier, -g_appliedMods.archeryDamage);
            g_appliedMods.archeryDamage = 0.0f;
        }
        if (g_appliedMods.criticalChance > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kCriticalChance, -g_appliedMods.criticalChance);
            g_appliedMods.criticalChance = 0.0f;
        }
        if (g_appliedMods.blockElementalResist > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kResistFire, -g_appliedMods.blockElementalResist);
            avOwner->ModActorValue(modifier, RE::ActorValue::kResistFrost, -g_appliedMods.blockElementalResist);
            avOwner->ModActorValue(modifier, RE::ActorValue::kResistShock, -g_appliedMods.blockElementalResist);
            avOwner->ModActorValue(modifier, RE::ActorValue::kPoisonResist, -g_appliedMods.blockElementalResist);
            avOwner->ModActorValue(modifier, RE::ActorValue::kResistMagic, -g_appliedMods.blockElementalResist);
            g_appliedMods.blockElementalResist = 0.0f;
        }
        if (g_appliedMods.sneakAttackMult > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kSneakingPowerModifier, -g_appliedMods.sneakAttackMult);
            g_appliedMods.sneakAttackMult = 0.0f;
        }
        if (g_appliedMods.destructionPower > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kDestructionPowerModifier, -g_appliedMods.destructionPower);
            g_appliedMods.destructionPower = 0.0f;
        }
        if (g_appliedMods.restorationPower > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kRestorationPowerModifier, -g_appliedMods.restorationPower);
            g_appliedMods.restorationPower = 0.0f;
        }
        if (g_appliedMods.illusionPower > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kIllusionPowerModifier, -g_appliedMods.illusionPower);
            g_appliedMods.illusionPower = 0.0f;
        }
        if (g_appliedMods.alterationDuration > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kAlterationPowerModifier, -g_appliedMods.alterationDuration);
            g_appliedMods.alterationDuration = 0.0f;
        }
        if (g_appliedMods.conjurationPower > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kConjurationPowerModifier, -g_appliedMods.conjurationPower);
            g_appliedMods.conjurationPower = 0.0f;
        }
        if (g_appliedMods.sneakModifier > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kSneakingModifier, -g_appliedMods.sneakModifier);
            g_appliedMods.sneakModifier = 0.0f;
        }
        if (g_appliedMods.pickpocketModifier > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kPickpocketModifier, -g_appliedMods.pickpocketModifier);
            g_appliedMods.pickpocketModifier = 0.0f;
        }
        if (g_appliedMods.lockpickingModifier > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kLockpickingModifier, -g_appliedMods.lockpickingModifier);
            g_appliedMods.lockpickingModifier = 0.0f;
        }
        if (g_appliedMods.speechcraftModifier > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kSpeechcraftModifier, -g_appliedMods.speechcraftModifier);
            g_appliedMods.speechcraftModifier = 0.0f;
        }
        if (g_appliedMods.alchemyPotency > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kAlchemyPowerModifier, -g_appliedMods.alchemyPotency);
            g_appliedMods.alchemyPotency = 0.0f;
        }
        if (g_appliedMods.enchantingModifier > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kEnchantingModifier, -g_appliedMods.enchantingModifier);
            g_appliedMods.enchantingModifier = 0.0f;
        }
    }

    inline void ApplyTraits()
    {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) return;

        const auto avOwner = player->AsActorValueOwner();
        if (!avOwner) return;

        RevertOldModifiers(player);
        UpdateSkillCounts();

        SKSE::log::info("--- Applying Legendary Traits ---");
        bool anyTraitApplied = false;

        constexpr auto modifier = RE::ACTOR_VALUE_MODIFIER::kTemporary;

        // 1. Smithing
        const auto smithingLevel = g_legendaryCounts[4];
        if (smithingLevel > 0) {
            anyTraitApplied = true;
            const float bonusWeight = static_cast<float>(smithingLevel) * g_settings.carryWeightPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kCarryWeight, bonusWeight);
            g_appliedMods.carryWeight = bonusWeight;

            const float penBonus = static_cast<float>(smithingLevel) * g_settings.smithingArmorPenPerLevel;
            if (penBonus > 0.0f) {
                avOwner->ModActorValue(modifier, RE::ActorValue::kOneHandedPowerModifier, penBonus);
                avOwner->ModActorValue(modifier, RE::ActorValue::kTwoHandedPowerModifier, penBonus);
                avOwner->ModActorValue(modifier, RE::ActorValue::kMarksmanPowerModifier, penBonus);
                g_appliedMods.smithingPenetration = penBonus;
            }
            SKSE::log::info("Smithing (Legendary {}): +{:.1f} Carry Weight, +{:.1f}% Weapon Penetration", smithingLevel, bonusWeight, penBonus);
        }

        // 2. Heavy Armor
        const auto heavyArmorLevel = g_legendaryCounts[5];
        if (heavyArmorLevel > 0) {
            anyTraitApplied = true;
            const float baseHealth = avOwner->GetBaseActorValue(RE::ActorValue::kHealth);
            const float bonusHealth = baseHealth * (g_settings.healthPercentPerLevel * static_cast<float>(heavyArmorLevel));
            avOwner->ModActorValue(modifier, RE::ActorValue::kHealth, bonusHealth);
            g_appliedMods.health = bonusHealth;

            const float massBonus = static_cast<float>(heavyArmorLevel) * g_settings.staggerResistPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kMass, massBonus);
            g_appliedMods.massModifier = massBonus;
            SKSE::log::info("Heavy Armor (Legendary {}): +{:.1f} Health, +{:.2f} Mass", heavyArmorLevel, bonusHealth, massBonus);
        }

        // 3. Light Armor
        const auto lightArmorLevel = g_legendaryCounts[6];
        if (lightArmorLevel > 0) {
            anyTraitApplied = true;
            const float stamBonus = static_cast<float>(lightArmorLevel) * g_settings.staminaRatePerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kStaminaRate, stamBonus);
            g_appliedMods.staminaRate = stamBonus;

            const float staminaPoolBonus = static_cast<float>(lightArmorLevel) * g_settings.lightArmorStaminaBonusPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kStamina, staminaPoolBonus);
            g_appliedMods.lightArmorStamina = staminaPoolBonus;
            SKSE::log::info("Light Armor (Legendary {}): +{:.1f}% Stamina Regen, +{:.1f} Max Stamina", lightArmorLevel, stamBonus, staminaPoolBonus);
        }

        // 4. Restoration
        const auto restorationLevel = g_legendaryCounts[16];
        if (restorationLevel > 0) {
            anyTraitApplied = true;
            const float bonus = static_cast<float>(restorationLevel) * g_settings.poisonResistPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kPoisonResist, bonus);
            g_appliedMods.poisonResist = bonus;
            SKSE::log::info("Restoration (Legendary {}): +{:.1f}% Poison Resistance", restorationLevel, bonus);
        }

        // 5. Alteration
        const auto alterationLevel = g_legendaryCounts[12];
        if (alterationLevel > 0) {
            anyTraitApplied = true;
            const float bonus = static_cast<float>(alterationLevel) * g_settings.spellAbsorptionPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kAbsorbChance, bonus);
            g_appliedMods.spellAbsorption = bonus;
            SKSE::log::info("Alteration (Legendary {}): +{:.1f}% Spell Absorption", alterationLevel, bonus);
        }

        // 6. Weapons & Combat
        float totalCritChanceBonus = 0.0f;

        const auto oneHandedLevel = g_legendaryCounts[0];
        if (oneHandedLevel > 0) {
            anyTraitApplied = true;
            const float dmgBonus = static_cast<float>(oneHandedLevel) * g_settings.oneHandedDamagePerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kOneHandedPowerModifier, dmgBonus);
            g_appliedMods.oneHandedDamage = dmgBonus;
            totalCritChanceBonus += static_cast<float>(oneHandedLevel) * g_settings.oneHandedCritPerLevel;
            SKSE::log::info("One-Handed (Legendary {}): +{:.1f}% Damage", oneHandedLevel, dmgBonus);
        }

        const auto twoHandedLevel = g_legendaryCounts[1];
        if (twoHandedLevel > 0) {
            anyTraitApplied = true;
            const float dmgBonus = static_cast<float>(twoHandedLevel) * g_settings.twoHandedDamagePerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kTwoHandedPowerModifier, dmgBonus);
            g_appliedMods.twoHandedDamage = dmgBonus;
            totalCritChanceBonus += static_cast<float>(twoHandedLevel) * g_settings.twoHandedCritPerLevel;
            SKSE::log::info("Two-Handed (Legendary {}): +{:.1f}% Damage", twoHandedLevel, dmgBonus);
        }

        const auto archeryLevel = g_legendaryCounts[2];
        if (archeryLevel > 0) {
            anyTraitApplied = true;
            const float dmgBonus = static_cast<float>(archeryLevel) * g_settings.archeryDamagePerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kMarksmanPowerModifier, dmgBonus);
            g_appliedMods.archeryDamage = dmgBonus;
            totalCritChanceBonus += static_cast<float>(archeryLevel) * g_settings.archeryCritPerLevel;
            SKSE::log::info("Archery (Legendary {}): +{:.1f}% Damage", archeryLevel, dmgBonus);
        }

        if (totalCritChanceBonus > 0.0f) {
            avOwner->ModActorValue(modifier, RE::ActorValue::kCriticalChance, totalCritChanceBonus);
            g_appliedMods.criticalChance = totalCritChanceBonus;
            SKSE::log::info("Combat Critical Chance: Total +{:.1f}%", totalCritChanceBonus);
        }

        // Block: Passive Elemental, Magic & Poison Resistance
        const auto blockLevel = g_legendaryCounts[3];
        if (blockLevel > 0) {
            anyTraitApplied = true;
            const float resistAmount = static_cast<float>(blockLevel) * g_settings.blockElementalReductionPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kResistFire, resistAmount);
            avOwner->ModActorValue(modifier, RE::ActorValue::kResistFrost, resistAmount);
            avOwner->ModActorValue(modifier, RE::ActorValue::kResistShock, resistAmount);
            avOwner->ModActorValue(modifier, RE::ActorValue::kPoisonResist, resistAmount);
            avOwner->ModActorValue(modifier, RE::ActorValue::kResistMagic, resistAmount);
            g_appliedMods.blockElementalResist = resistAmount;

            SKSE::log::info("Block (Legendary {}): +{:.1f}% Passive Elemental, Magic & Poison Resistance", blockLevel, resistAmount);
        }

        const auto sneakLevel = g_legendaryCounts[9];
        if (sneakLevel > 0) {
            anyTraitApplied = true;
            const float sneakAtkBonus = static_cast<float>(sneakLevel) * g_settings.sneakAttackMultPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kSneakingPowerModifier, sneakAtkBonus);
            g_appliedMods.sneakAttackMult = sneakAtkBonus;
            
            const float stealthBonus = static_cast<float>(sneakLevel) * g_settings.sneakEffectivenessPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kSneakingModifier, stealthBonus);
            g_appliedMods.sneakModifier = stealthBonus;
            SKSE::log::info("Sneak (Legendary {}): +{:.1f}% Stealth, +{:.1f}% Sneak Attack Damage", sneakLevel, stealthBonus, sneakAtkBonus);
        }

        // 7. Magic Powers
        const auto destructionLevel = g_legendaryCounts[14];
        if (destructionLevel > 0) {
            anyTraitApplied = true;
            const float bonus = static_cast<float>(destructionLevel) * g_settings.destructionPowerPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kDestructionPowerModifier, bonus);
            g_appliedMods.destructionPower = bonus;
            SKSE::log::info("Destruction (Legendary {}): +{:.1f}% Spell Magnitude", destructionLevel, bonus);
        }

        if (restorationLevel > 0) {
            const float bonus = static_cast<float>(restorationLevel) * g_settings.restorationPowerPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kRestorationPowerModifier, bonus);
            g_appliedMods.restorationPower = bonus;
            SKSE::log::info("Restoration (Legendary {}): +{:.1f}% Healing Magnitude", restorationLevel, bonus);
        }

        const auto illusionLevel = g_legendaryCounts[15];
        if (illusionLevel > 0) {
            anyTraitApplied = true;
            const float bonus = static_cast<float>(illusionLevel) * g_settings.illusionPowerPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kIllusionPowerModifier, bonus);
            g_appliedMods.illusionPower = bonus;
            SKSE::log::info("Illusion (Legendary {}): +{:.1f}% Influence Magnitude", illusionLevel, bonus);
        }

        if (alterationLevel > 0) {
            const float bonus = static_cast<float>(alterationLevel) * g_settings.alterationDurationPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kAlterationPowerModifier, bonus);
            g_appliedMods.alterationDuration = bonus;
            SKSE::log::info("Alteration (Legendary {}): +{:.1f}% Duration", alterationLevel, bonus);
        }

        const auto conjurationLevel = g_legendaryCounts[13];
        if (conjurationLevel > 0) {
            anyTraitApplied = true;
            SKSE::log::info("Conjuration (Legendary {}): Active", conjurationLevel);
        }

        // 8. Thievery & Barter
        const auto pickpocketLevel = g_legendaryCounts[7];
        if (pickpocketLevel > 0) {
            anyTraitApplied = true;
            const float bonus = static_cast<float>(pickpocketLevel) * g_settings.pickpocketChancePerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kPickpocketModifier, bonus);
            g_appliedMods.pickpocketModifier = bonus;
            SKSE::log::info("Pickpocket (Legendary {}): +{:.1f}% Success Chance", pickpocketLevel, bonus);
        }

        const auto lockpickingLevel = g_legendaryCounts[8];
        if (lockpickingLevel > 0) {
            anyTraitApplied = true;
            const float bonus = static_cast<float>(lockpickingLevel) * g_settings.lockpickingWidthPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kLockpickingModifier, bonus);
            g_appliedMods.lockpickingModifier = bonus;
            SKSE::log::info("Lockpicking (Legendary {}): +{:.1f}% Sweet-spot Width", lockpickingLevel, bonus);
        }

        const auto speechLevel = g_legendaryCounts[11];
        if (speechLevel > 0) {
            anyTraitApplied = true;
            const float bonus = static_cast<float>(speechLevel) * g_settings.vendorPriceBonusPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kSpeechcraftModifier, bonus);
            g_appliedMods.speechcraftModifier = bonus;
            SKSE::log::info("Speech (Legendary {}): +{:.1f}% Barter Prices", speechLevel, bonus);
        }

        // 9. Crafting Potencies
        const auto alchemyLevel = g_legendaryCounts[10];
        if (alchemyLevel > 0) {
            anyTraitApplied = true;
            const float bonus = static_cast<float>(alchemyLevel) * g_settings.alchemyPotencyPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kAlchemyPowerModifier, bonus);
            g_appliedMods.alchemyPotency = bonus;
            SKSE::log::info("Alchemy (Legendary {}): +{:.1f}% Potency", alchemyLevel, bonus);
        }

        const auto enchantingLevel = g_legendaryCounts[17];
        if (enchantingLevel > 0) {
            anyTraitApplied = true;
            const float bonus = static_cast<float>(enchantingLevel) * g_settings.enchantingPowerPerLevel;
            avOwner->ModActorValue(modifier, RE::ActorValue::kEnchantingModifier, bonus);
            g_appliedMods.enchantingModifier = bonus;
            SKSE::log::info("Enchanting (Legendary {}): +{:.1f}% Enchantment Power (Arcane Enchanter)", enchantingLevel, bonus);
        }

        if (!anyTraitApplied) {
            SKSE::log::info("No legendary skills active currently.");
        } else {
            SKSE::log::info("All active traits applied successfully to Player.");
        }
    }

    class ThieveryEventHandler : public RE::BSTEventSink<RE::TESTrackedStatsEvent>
    {
    public:
        static ThieveryEventHandler* GetSingleton()
        {
            static ThieveryEventHandler singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::TESTrackedStatsEvent* a_event, RE::BSTEventSource<RE::TESTrackedStatsEvent>*) override
        {
            if (!a_event) return RE::BSEventNotifyControl::kContinue;

            if (a_event->stat == "Locks Picked") {
                const auto lockpickingLevel = g_legendaryCounts[8];
                if (lockpickingLevel > 0) {
                    AwardGoldSilently(static_cast<std::int32_t>(lockpickingLevel) * g_settings.lockpickingGoldPerLevel);
                }
            } else if (a_event->stat == "Pockets Picked") {
                const auto pickpocketLevel = g_legendaryCounts[7];
                if (pickpocketLevel > 0) {
                    AwardGoldSilently(static_cast<std::int32_t>(pickpocketLevel) * g_settings.pickpocketGoldPerLevel);
                }
            }

            return RE::BSEventNotifyControl::kContinue;
        }

        static void Register()
        {
            if (const auto eventHolder = RE::ScriptEventSourceHolder::GetSingleton()) {
                eventHolder->AddEventSink(GetSingleton());
            }
        }

    private:
        static void AwardGoldSilently(std::int32_t a_amount)
        {
            if (a_amount <= 0) return;
            const auto player = RE::PlayerCharacter::GetSingleton();
            if (!player) return;
            if (const auto goldItem = RE::TESForm::LookupByID<RE::TESBoundObject>(0x0000000F)) {
                player->AddObjectToContainer(goldItem, nullptr, a_amount, nullptr);
            }
        }
    };

    class CombatEventHandler : public RE::BSTEventSink<RE::TESHitEvent>
    {
    public:
        static CombatEventHandler* GetSingleton()
        {
            static CombatEventHandler singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*) override
        {
            if (!a_event) return RE::BSEventNotifyControl::kContinue;

            const auto player = RE::PlayerCharacter::GetSingleton();
            if (!player) return RE::BSEventNotifyControl::kContinue;

            // 1. ATTACKER IS PLAYER
            if (a_event->cause.get() == player) {
                const auto targetRef = a_event->target.get();
                if (!targetRef) return RE::BSEventNotifyControl::kContinue;

                const auto targetActor = targetRef->As<RE::Actor>();
                if (!targetActor) return RE::BSEventNotifyControl::kContinue;

                const auto sourceForm = a_event->source ? RE::TESForm::LookupByID(a_event->source) : nullptr;
                if (!sourceForm) return RE::BSEventNotifyControl::kContinue;

                // Archery stagger
                const auto archeryLevel = g_legendaryCounts[2];
                if (archeryLevel > 0) {
                    bool isRanged = false;
                    if (const auto weapon = sourceForm->As<RE::TESObjectWEAP>()) {
                        if (weapon->IsBow() || weapon->IsCrossbow()) isRanged = true;
                    } else if (sourceForm->As<RE::TESAmmo>()) {
                        isRanged = true;
                    }

                    if (isRanged) {
                        const float staggerChance = (std::min)(100.0f, static_cast<float>(archeryLevel) * g_settings.archeryStaggerPerLevel);
                        const float roll = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 100.0f;
                        if (roll <= staggerChance) {
                            targetActor->SetGraphVariableFloat("staggerMagnitude", 0.5f);
                            targetActor->NotifyAnimationGraph("staggerStart");
                        }
                    }
                }

                // Enchanting charge conservation
                const auto enchantingLevel = g_legendaryCounts[17];
                if (enchantingLevel > 0 && sourceForm->IsWeapon()) {
                    if (const auto weapon = sourceForm->As<RE::TESObjectWEAP>()) {
                        if (weapon->formEnchanting || weapon->amountofEnchantment > 0) {
                            if (const auto avOwner = player->AsActorValueOwner()) {
                                const float refundAmount = static_cast<float>(enchantingLevel) * g_settings.enchantingChargeReductionPerLevel;
                                avOwner->RestoreActorValue(RE::ActorValue::kRightItemCharge, refundAmount);
                            }
                        }
                    }
                }
            }

            return RE::BSEventNotifyControl::kContinue;
        }

        static void Register()
        {
            if (const auto eventHolder = RE::ScriptEventSourceHolder::GetSingleton()) {
                eventHolder->AddEventSink(GetSingleton());
            }
        }
    };
}