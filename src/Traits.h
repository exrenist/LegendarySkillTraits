#pragma once

#include "pch.h"
#include "RE/S/SendHUDMessage.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string_view>

namespace LegendaryTraits
{
    inline constexpr std::size_t kSkillCount = 18;

    inline constexpr std::array<const char*, kSkillCount> kSkillNames = {
        "One-Handed", "Two-Handed", "Archery", "Block", "Smithing",
        "Heavy Armor", "Light Armor", "Pickpocket", "Lockpicking", "Sneak",
        "Alchemy", "Speech", "Alteration", "Conjuration", "Destruction",
        "Illusion", "Restoration", "Enchanting"
    };

    inline constexpr std::array<RE::FormID, kSkillCount> kTraitSpellIDs = {
        0x804, 0x805, 0x806, 0x807, 0x80F, 0x808, 0x809, 0x814,
        0x813, 0x812, 0x810, 0x815, 0x80C, 0x80E, 0x80A, 0x80D,
        0x80B, 0x811
    };

    inline std::array<RE::SpellItem*, kSkillCount> g_traitSpells{};

    struct TraitSettings
    {
        float oneHandedDamagePerLevel = 8.0f;
        float oneHandedCritPerLevel = 4.0f;
        float twoHandedDamagePerLevel = 10.0f;
        float twoHandedCritPerLevel = 5.0f;
        float archeryDamagePerLevel = 8.0f;
        float archeryCritPerLevel = 4.0f;
        float archeryStaggerPerLevel = 5.0f;
        float blockElementalReductionPerLevel = 5.0f;
        float sneakEffectivenessPerLevel = 5.0f;
        float sneakAttackMultPerLevel = 10.0f;
        float lockpickingWidthPerLevel = 5.0f;
        std::int32_t lockpickingGoldPerLevel = 30;
        float pickpocketChancePerLevel = 5.0f;
        std::int32_t pickpocketGoldPerLevel = 15;
        float destructionPowerPerLevel = 10.0f;
        float restorationPowerPerLevel = 10.0f;
        float alterationDurationPerLevel = 15.0f;
        float illusionPowerPerLevel = 10.0f;
        float vendorPriceBonusPerLevel = 5.0f;
        float smithingArmorPenPerLevel = 5.0f;
        float alchemyPotencyPerLevel = 3.0f;
        float enchantingPowerPerLevel = 5.0f;
        float enchantingChargeReductionPerLevel = 1.5f;
        float healthPercentPerLevel = 0.05f;
        float staggerResistPerLevel = 0.08f;
        float staminaRatePerLevel = 10.0f;
        float lightArmorStaminaBonusPerLevel = 5.0f;
        float carryWeightPerLevel = 25.0f;
        float poisonResistPerLevel = 10.0f;
        float spellAbsorptionPerLevel = 3.0f;

        bool enableOneHandedDamage = true;
        bool enableOneHandedCrit = true;
        bool enableTwoHandedDamage = true;
        bool enableTwoHandedCrit = true;
        bool enableArcheryDamage = true;
        bool enableArcheryCrit = true;
        bool enableArcheryStagger = true;
        bool enableBlockResistMaster = true;
        bool enableBlockResistFire = true;
        bool enableBlockResistFrost = true;
        bool enableBlockResistShock = true;
        bool enableBlockResistPoison = true;
        bool enableBlockResistMagic = true;
        bool enableHeavyArmorHealth = true;
        bool enableHeavyArmorMass = true;
        bool enableLightArmorStaminaRate = true;
        bool enableLightArmorStaminaPool = true;
        bool enableDestructionPower = true;
        bool enableRestorationPower = true;
        bool enableRestorationPoisonResist = true;
        bool enableAlterationDuration = true;
        bool enableAlterationAbsorption = true;
        bool enableIllusionPower = true;
        bool enableSmithingCarryWeight = true;
        bool enableSmithingArmorPen = true;
        bool enableAlchemyPotency = true;
        bool enableEnchantingPower = true;
        bool enableEnchantingChargeRefund = true;
        bool enableSneakEffectiveness = true;
        bool enableSneakAttackMult = true;
        bool enableLockpickingWidth = true;
        bool enableLockpickingGold = true;
        bool enablePickpocketChance = true;
        bool enablePickpocketGold = true;
        bool enableSpeechBarter = true;

        bool showInActiveEffects = true;
        bool verboseLogging = false;
        
        // Modules
        bool recalibrateAllTraits = false;
        bool prepareForUninstall = false;
    };

    inline TraitSettings g_settings{};

    struct AppliedModifiers
    {
        float carryWeight = 0.0f;
        float health = 0.0f;
        float staminaRate = 0.0f;
        float stamina = 0.0f;
        float mass = 0.0f;
        float absorbChance = 0.0f;
        float resistFire = 0.0f;
        float resistFrost = 0.0f;
        float resistShock = 0.0f;
        float resistMagic = 0.0f;
        float poisonResist = 0.0f;
        float sneakingPower = 0.0f;
        float sneakingMod = 0.0f;
        float destructionPower = 0.0f;
        float restorationPower = 0.0f;
        float illusionPower = 0.0f;
        float alterationPower = 0.0f;
        float pickpocket = 0.0f;
        float lockpicking = 0.0f;
        float speechcraft = 0.0f;
        float alchemy = 0.0f;
        float enchanting = 0.0f;
        float oneHandedPower = 0.0f;
        float twoHandedPower = 0.0f;
        float marksmanPower = 0.0f;
        float criticalChance = 0.0f;
    };

    inline AppliedModifiers g_appliedMods{};
    inline std::array<std::uint32_t, kSkillCount> g_legendaryCounts{};
    inline std::atomic<bool> g_isApplyQueued{ false };

    inline bool g_lastRecalibrateState = false;
    inline bool g_recalibrateInitialized = false;

    namespace Serialization
    {
        inline constexpr std::uint32_t kSerializationVersion = 2;
        inline constexpr std::uint32_t kDataKey = 'MODS';

        inline void SaveCallback(SKSE::SerializationInterface* a_intfc)
        {
            if (!a_intfc->OpenRecord(kDataKey, kSerializationVersion)) return;
            a_intfc->WriteRecordData(&g_appliedMods, sizeof(g_appliedMods));
        }

        inline void LoadCallback(SKSE::SerializationInterface* a_intfc)
        {
            std::uint32_t type, version, length;
            while (a_intfc->GetNextRecordInfo(type, version, length)) {
                if (type == kDataKey && version == kSerializationVersion && length == sizeof(g_appliedMods)) {
                    a_intfc->ReadRecordData(&g_appliedMods, sizeof(g_appliedMods));
                }
            }
        }

        inline void RevertCallback(SKSE::SerializationInterface*)
        {
            g_appliedMods = {};
        }
    }

    inline bool AreSkillsLoaded()
    {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->Is3DLoaded()) {
            return false;
        }

        const auto& infoData = player->GetInfoRuntimeData();
        const auto playerSkills = infoData.skills;
        if (!playerSkills || !playerSkills->data) {
            return false;
        }

        return playerSkills->data->skills[0].level > 0.0f;
    }

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

    inline void SanitizeSpellEffects(RE::SpellItem* a_spell)
    {
        if (!a_spell) return;
        for (auto* effect : a_spell->effects) {
            if (effect) {
                effect->effectItem.duration = 0;
                effect->effectItem.area = 0;
                effect->effectItem.magnitude = 0.0f;
            }
        }
    }

    inline void InitializeSpellCache()
    {
        const auto dataHandler = RE::TESDataHandler::GetSingleton();
        if (!dataHandler) return;

        constexpr auto modName = "LegendarySkillTraits.esl";
        for (std::size_t i = 0; i < kSkillCount; ++i) {
            g_traitSpells[i] = dataHandler->LookupForm<RE::SpellItem>(kTraitSpellIDs[i], modName);
            if (g_traitSpells[i]) {
                SanitizeSpellEffects(g_traitSpells[i]);
            }
        }
    }

    inline bool IsSkillTraitEnabled(std::size_t a_skillIndex)
    {
        switch (a_skillIndex) {
        case 0: return g_settings.enableOneHandedDamage || g_settings.enableOneHandedCrit;
        case 1: return g_settings.enableTwoHandedDamage || g_settings.enableTwoHandedCrit;
        case 2: return g_settings.enableArcheryDamage || g_settings.enableArcheryCrit || g_settings.enableArcheryStagger;
        case 3: return g_settings.enableBlockResistMaster && (g_settings.enableBlockResistFire || g_settings.enableBlockResistFrost || g_settings.enableBlockResistShock || g_settings.enableBlockResistPoison || g_settings.enableBlockResistMagic);
        case 4: return g_settings.enableSmithingCarryWeight || g_settings.enableSmithingArmorPen;
        case 5: return g_settings.enableHeavyArmorHealth || g_settings.enableHeavyArmorMass;
        case 6: return g_settings.enableLightArmorStaminaRate || g_settings.enableLightArmorStaminaPool;
        case 7: return g_settings.enablePickpocketChance || g_settings.enablePickpocketGold;
        case 8: return g_settings.enableLockpickingWidth || g_settings.enableLockpickingGold;
        case 9: return g_settings.enableSneakEffectiveness || g_settings.enableSneakAttackMult;
        case 10: return g_settings.enableAlchemyPotency;
        case 11: return g_settings.enableSpeechBarter;
        case 12: return g_settings.enableAlterationDuration || g_settings.enableAlterationAbsorption;
        case 13: return true;
        case 14: return g_settings.enableDestructionPower;
        case 15: return g_settings.enableIllusionPower;
        case 16: return g_settings.enableRestorationPower || g_settings.enableRestorationPoisonResist;
        case 17: return g_settings.enableEnchantingPower || g_settings.enableEnchantingChargeRefund;
        default: return true;
        }
    }

    inline void UpdateActiveEffectSpells(RE::PlayerCharacter* a_player, bool a_forceRemoveAll = false)
    {
        if (!a_player) return;

        for (std::size_t i = 0; i < kSkillCount; ++i) {
            const auto spell = g_traitSpells[i];
            if (!spell) continue;

            const bool hasSpell = a_player->HasSpell(spell);

            if (a_forceRemoveAll) {
                if (hasSpell) a_player->RemoveSpell(spell);
                continue;
            }

            const bool isEnabled = IsSkillTraitEnabled(i);
            const bool shouldHaveSpell = g_settings.showInActiveEffects && isEnabled && (g_legendaryCounts[i] > 0);

            if (shouldHaveSpell && !hasSpell) {
                SanitizeSpellEffects(spell);
                a_player->AddSpell(spell);
            } else if (!shouldHaveSpell && hasSpell) {
                a_player->RemoveSpell(spell);
            }
        }
    }

    inline void ApplyTraits()
    {
        if (!AreSkillsLoaded()) {
            return;
        }

        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) return;
        const auto avOwner = player->AsActorValueOwner();
        if (!avOwner) return;

        // ==========================================
        // MODULE: Safe Uninstall (Wipes everything permanently)
        // ==========================================
        if (g_settings.prepareForUninstall) {
            bool wipedAny = false;
            auto wipeStat = [&](RE::ActorValue av, float& trackedVal) {
                if (std::abs(trackedVal) > 0.01f) {
                    avOwner->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, -trackedVal);
                    trackedVal = 0.0f;
                    wipedAny = true;
                }
            };

            wipeStat(RE::ActorValue::kCarryWeight, g_appliedMods.carryWeight);
            wipeStat(RE::ActorValue::kHealth, g_appliedMods.health);
            wipeStat(RE::ActorValue::kStaminaRate, g_appliedMods.staminaRate);
            wipeStat(RE::ActorValue::kStamina, g_appliedMods.stamina);
            wipeStat(RE::ActorValue::kMass, g_appliedMods.mass);
            wipeStat(RE::ActorValue::kAbsorbChance, g_appliedMods.absorbChance);
            wipeStat(RE::ActorValue::kResistFire, g_appliedMods.resistFire);
            wipeStat(RE::ActorValue::kResistFrost, g_appliedMods.resistFrost);
            wipeStat(RE::ActorValue::kResistShock, g_appliedMods.resistShock);
            wipeStat(RE::ActorValue::kResistMagic, g_appliedMods.resistMagic);
            wipeStat(RE::ActorValue::kPoisonResist, g_appliedMods.poisonResist);
            wipeStat(RE::ActorValue::kSneakingPowerModifier, g_appliedMods.sneakingPower);
            wipeStat(RE::ActorValue::kSneakingModifier, g_appliedMods.sneakingMod);
            wipeStat(RE::ActorValue::kDestructionPowerModifier, g_appliedMods.destructionPower);
            wipeStat(RE::ActorValue::kRestorationPowerModifier, g_appliedMods.restorationPower);
            wipeStat(RE::ActorValue::kIllusionPowerModifier, g_appliedMods.illusionPower);
            wipeStat(RE::ActorValue::kAlterationPowerModifier, g_appliedMods.alterationPower);
            wipeStat(RE::ActorValue::kPickpocketModifier, g_appliedMods.pickpocket);
            wipeStat(RE::ActorValue::kLockpickingModifier, g_appliedMods.lockpicking);
            wipeStat(RE::ActorValue::kSpeechcraftModifier, g_appliedMods.speechcraft);
            wipeStat(RE::ActorValue::kAlchemyPowerModifier, g_appliedMods.alchemy);
            wipeStat(RE::ActorValue::kEnchantingModifier, g_appliedMods.enchanting);
            wipeStat(RE::ActorValue::kOneHandedPowerModifier, g_appliedMods.oneHandedPower);
            wipeStat(RE::ActorValue::kTwoHandedPowerModifier, g_appliedMods.twoHandedPower);
            wipeStat(RE::ActorValue::kMarksmanPowerModifier, g_appliedMods.marksmanPower);
            wipeStat(RE::ActorValue::kCriticalChance, g_appliedMods.criticalChance);

            UpdateActiveEffectSpells(player, true);
            g_appliedMods = {}; // Hard reset tracking memory

            if (wipedAny) {
                RE::SendHUDMessage::ShowHUDMessage("Legendary Traits: Safe to uninstall. Please save your game.");
            }
            return; // Halt any further trait application
        }

        UpdateSkillCounts();

        // Step 1: Calculate Intended Target Values
        AppliedModifiers intended = {};
        
        const auto smLvl = g_legendaryCounts[4];
        if (smLvl > 0) {
            if (g_settings.enableSmithingCarryWeight) intended.carryWeight = static_cast<float>(smLvl) * g_settings.carryWeightPerLevel;
            if (g_settings.enableSmithingArmorPen) {
                float pen = static_cast<float>(smLvl) * g_settings.smithingArmorPenPerLevel;
                intended.oneHandedPower += pen;
                intended.twoHandedPower += pen;
                intended.marksmanPower += pen;
            }
        }

        const auto hvLvl = g_legendaryCounts[5];
        if (hvLvl > 0) {
            if (g_settings.enableHeavyArmorHealth) {
                const float baseHealth = avOwner->GetBaseActorValue(RE::ActorValue::kHealth);
                intended.health = baseHealth * (g_settings.healthPercentPerLevel * static_cast<float>(hvLvl));
            }
            if (g_settings.enableHeavyArmorMass) intended.mass = static_cast<float>(hvLvl) * g_settings.staggerResistPerLevel;
        }

        const auto ltLvl = g_legendaryCounts[6];
        if (ltLvl > 0) {
            if (g_settings.enableLightArmorStaminaRate) intended.staminaRate = static_cast<float>(ltLvl) * g_settings.staminaRatePerLevel;
            if (g_settings.enableLightArmorStaminaPool) intended.stamina = static_cast<float>(ltLvl) * g_settings.lightArmorStaminaBonusPerLevel;
        }

        const auto restLvl = g_legendaryCounts[16];
        if (restLvl > 0) {
            if (g_settings.enableRestorationPower) intended.restorationPower = static_cast<float>(restLvl) * g_settings.restorationPowerPerLevel;
            if (g_settings.enableRestorationPoisonResist) intended.poisonResist += static_cast<float>(restLvl) * g_settings.poisonResistPerLevel;
        }

        const auto altLvl = g_legendaryCounts[12];
        if (altLvl > 0) {
            if (g_settings.enableAlterationDuration) intended.alterationPower = static_cast<float>(altLvl) * g_settings.alterationDurationPerLevel;
            if (g_settings.enableAlterationAbsorption) intended.absorbChance = static_cast<float>(altLvl) * g_settings.spellAbsorptionPerLevel;
        }

        const auto oneLvl = g_legendaryCounts[0];
        if (oneLvl > 0) {
            if (g_settings.enableOneHandedDamage) intended.oneHandedPower += static_cast<float>(oneLvl) * g_settings.oneHandedDamagePerLevel;
            if (g_settings.enableOneHandedCrit) intended.criticalChance += static_cast<float>(oneLvl) * g_settings.oneHandedCritPerLevel;
        }

        const auto twoLvl = g_legendaryCounts[1];
        if (twoLvl > 0) {
            if (g_settings.enableTwoHandedDamage) intended.twoHandedPower += static_cast<float>(twoLvl) * g_settings.twoHandedDamagePerLevel;
            if (g_settings.enableTwoHandedCrit) intended.criticalChance += static_cast<float>(twoLvl) * g_settings.twoHandedCritPerLevel;
        }

        const auto archLvl = g_legendaryCounts[2];
        if (archLvl > 0) {
            if (g_settings.enableArcheryDamage) intended.marksmanPower += static_cast<float>(archLvl) * g_settings.archeryDamagePerLevel;
            if (g_settings.enableArcheryCrit) intended.criticalChance += static_cast<float>(archLvl) * g_settings.archeryCritPerLevel;
        }

        const auto blkLvl = g_legendaryCounts[3];
        if (blkLvl > 0 && g_settings.enableBlockResistMaster) {
            const float baseRes = static_cast<float>(blkLvl) * g_settings.blockElementalReductionPerLevel;
            if (g_settings.enableBlockResistFire) intended.resistFire = baseRes;
            if (g_settings.enableBlockResistFrost) intended.resistFrost = baseRes;
            if (g_settings.enableBlockResistShock) intended.resistShock = baseRes;
            if (g_settings.enableBlockResistPoison) intended.poisonResist += baseRes;
            if (g_settings.enableBlockResistMagic) intended.resistMagic = baseRes;
        }

        const auto snkLvl = g_legendaryCounts[9];
        if (snkLvl > 0) {
            if (g_settings.enableSneakAttackMult) intended.sneakingPower = static_cast<float>(snkLvl) * g_settings.sneakAttackMultPerLevel;
            if (g_settings.enableSneakEffectiveness) intended.sneakingMod = static_cast<float>(snkLvl) * g_settings.sneakEffectivenessPerLevel;
        }

        const auto destLvl = g_legendaryCounts[14];
        if (destLvl > 0 && g_settings.enableDestructionPower) intended.destructionPower = static_cast<float>(destLvl) * g_settings.destructionPowerPerLevel;

        const auto illuLvl = g_legendaryCounts[15];
        if (illuLvl > 0 && g_settings.enableIllusionPower) intended.illusionPower = static_cast<float>(illuLvl) * g_settings.illusionPowerPerLevel;

        const auto pickLvl = g_legendaryCounts[7];
        if (pickLvl > 0 && g_settings.enablePickpocketChance) intended.pickpocket = static_cast<float>(pickLvl) * g_settings.pickpocketChancePerLevel;

        const auto lockLvl = g_legendaryCounts[8];
        if (lockLvl > 0 && g_settings.enableLockpickingWidth) intended.lockpicking = static_cast<float>(lockLvl) * g_settings.lockpickingWidthPerLevel;

        const auto spchLvl = g_legendaryCounts[11];
        if (spchLvl > 0 && g_settings.enableSpeechBarter) intended.speechcraft = static_cast<float>(spchLvl) * g_settings.vendorPriceBonusPerLevel;

        const auto alchLvl = g_legendaryCounts[10];
        if (alchLvl > 0 && g_settings.enableAlchemyPotency) intended.alchemy = static_cast<float>(alchLvl) * g_settings.alchemyPotencyPerLevel;

        const auto enchLvl = g_legendaryCounts[17];
        if (enchLvl > 0 && g_settings.enableEnchantingPower) intended.enchanting = static_cast<float>(enchLvl) * g_settings.enchantingPowerPerLevel;

        // Step 2: MODULE - Safe Update (Recalibrate Bloat Purge) Check
        bool isRecalibrateRequested = false;
        if (!g_recalibrateInitialized) {
            g_lastRecalibrateState = g_settings.recalibrateAllTraits;
            g_recalibrateInitialized = true;
        } else if (g_settings.recalibrateAllTraits != g_lastRecalibrateState) {
            g_lastRecalibrateState = g_settings.recalibrateAllTraits;
            isRecalibrateRequested = true;
        }

        // Step 3: Unbreakable Lambda for Permanent ActorValue Modifiers
        auto applyStat = [&](RE::ActorValue av, float& trackedVal, float intendedVal) {
            
            if (isRecalibrateRequested) {
                // If bloat exists from legacy v2.0.0 on kTemporary, wipe it down to 0
                float legacyTempBloat = player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, av);
                if (std::abs(legacyTempBloat) > 0.01f) {
                    avOwner->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kTemporary, av, -legacyTempBloat);
                }
            }

            // Sync kPermanent purely by delta (Calculates mathematically perfect without touching gear)
            float delta = intendedVal - trackedVal;
            if (std::abs(delta) > 0.01f) {
                avOwner->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, av, delta);
                trackedVal = intendedVal;
            }
        };

        applyStat(RE::ActorValue::kCarryWeight, g_appliedMods.carryWeight, intended.carryWeight);
        applyStat(RE::ActorValue::kHealth, g_appliedMods.health, intended.health);
        applyStat(RE::ActorValue::kStaminaRate, g_appliedMods.staminaRate, intended.staminaRate);
        applyStat(RE::ActorValue::kStamina, g_appliedMods.stamina, intended.stamina);
        applyStat(RE::ActorValue::kMass, g_appliedMods.mass, intended.mass);
        applyStat(RE::ActorValue::kAbsorbChance, g_appliedMods.absorbChance, intended.absorbChance);
        applyStat(RE::ActorValue::kResistFire, g_appliedMods.resistFire, intended.resistFire);
        applyStat(RE::ActorValue::kResistFrost, g_appliedMods.resistFrost, intended.resistFrost);
        applyStat(RE::ActorValue::kResistShock, g_appliedMods.resistShock, intended.resistShock);
        applyStat(RE::ActorValue::kResistMagic, g_appliedMods.resistMagic, intended.resistMagic);
        applyStat(RE::ActorValue::kPoisonResist, g_appliedMods.poisonResist, intended.poisonResist);
        applyStat(RE::ActorValue::kSneakingPowerModifier, g_appliedMods.sneakingPower, intended.sneakingPower);
        applyStat(RE::ActorValue::kSneakingModifier, g_appliedMods.sneakingMod, intended.sneakingMod);
        applyStat(RE::ActorValue::kDestructionPowerModifier, g_appliedMods.destructionPower, intended.destructionPower);
        applyStat(RE::ActorValue::kRestorationPowerModifier, g_appliedMods.restorationPower, intended.restorationPower);
        applyStat(RE::ActorValue::kIllusionPowerModifier, g_appliedMods.illusionPower, intended.illusionPower);
        applyStat(RE::ActorValue::kAlterationPowerModifier, g_appliedMods.alterationPower, intended.alterationPower);
        applyStat(RE::ActorValue::kPickpocketModifier, g_appliedMods.pickpocket, intended.pickpocket);
        applyStat(RE::ActorValue::kLockpickingModifier, g_appliedMods.lockpicking, intended.lockpicking);
        applyStat(RE::ActorValue::kSpeechcraftModifier, g_appliedMods.speechcraft, intended.speechcraft);
        applyStat(RE::ActorValue::kAlchemyPowerModifier, g_appliedMods.alchemy, intended.alchemy);
        applyStat(RE::ActorValue::kEnchantingModifier, g_appliedMods.enchanting, intended.enchanting);
        applyStat(RE::ActorValue::kOneHandedPowerModifier, g_appliedMods.oneHandedPower, intended.oneHandedPower);
        applyStat(RE::ActorValue::kTwoHandedPowerModifier, g_appliedMods.twoHandedPower, intended.twoHandedPower);
        applyStat(RE::ActorValue::kMarksmanPowerModifier, g_appliedMods.marksmanPower, intended.marksmanPower);
        applyStat(RE::ActorValue::kCriticalChance, g_appliedMods.criticalChance, intended.criticalChance);

        if (isRecalibrateRequested) {
            RE::SendHUDMessage::ShowHUDMessage("Legendary Traits: All attributes safely updated and recalibrated.");
        }

        UpdateActiveEffectSpells(player, false);
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
            if (!a_event || g_settings.prepareForUninstall) return RE::BSEventNotifyControl::kContinue;

            if (a_event->stat == "Locks Picked" && g_settings.enableLockpickingGold) {
                const auto lockLevel = g_legendaryCounts[8];
                if (lockLevel > 0) {
                    AwardGoldSilently(static_cast<std::int32_t>(lockLevel) * g_settings.lockpickingGoldPerLevel);
                }
            } else if (a_event->stat == "Pockets Picked" && g_settings.enablePickpocketGold) {
                const auto pickLevel = g_legendaryCounts[7];
                if (pickLevel > 0) {
                    AwardGoldSilently(static_cast<std::int32_t>(pickLevel) * g_settings.pickpocketGoldPerLevel);
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
            if (!a_event || g_settings.prepareForUninstall) return RE::BSEventNotifyControl::kContinue;

            const auto player = RE::PlayerCharacter::GetSingleton();
            if (!player) return RE::BSEventNotifyControl::kContinue;

            if (a_event->cause.get() == player) {
                const auto targetRef = a_event->target.get();
                if (!targetRef) return RE::BSEventNotifyControl::kContinue;

                const auto targetActor = targetRef->As<RE::Actor>();
                if (!targetActor) return RE::BSEventNotifyControl::kContinue;

                const auto sourceForm = a_event->source ? RE::TESForm::LookupByID(a_event->source) : nullptr;
                if (!sourceForm) return RE::BSEventNotifyControl::kContinue;

                if (g_settings.enableArcheryStagger) {
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
                }

                if (g_settings.enableEnchantingChargeRefund) {
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