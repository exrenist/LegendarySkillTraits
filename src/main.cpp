#include "pch.h"
#include "Traits.h"
#include <fstream>
#include <filesystem>
#include <string>

namespace SettingsManager
{
    inline const std::filesystem::path kUserConfigPath = "Data/MCM/Settings/LegendarySkillTraits.ini";
    inline const std::filesystem::path kDefaultConfigPath = "Data/MCM/Config/LegendarySkillTraits/settings.ini";

    inline std::string Trim(const std::string& str)
    {
        const auto first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        const auto last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    inline bool ParseBoolValue(const std::string& str, float numVal)
    {
        if (str == "1" || str == "true" || str == "TRUE" || str == "True") return true;
        if (str == "0" || str == "false" || str == "FALSE" || str == "False") return false;
        return numVal != 0.0f;
    }

    inline void LoadSettings()
    {
        // Check if user has generated customized settings from MCM, otherwise fall back to default template
        const std::filesystem::path activePath = std::filesystem::exists(kUserConfigPath) ? kUserConfigPath : kDefaultConfigPath;

        if (!std::filesystem::exists(activePath)) {
            SKSE::log::info("No configuration file found. Using hardcoded trait defaults.");
            return;
        }

        std::ifstream file(activePath);
        if (!file.is_open()) {
            SKSE::log::warn("Failed to open configuration file: {}", activePath.string());
            return;
        }

        SKSE::log::info("Loading configuration from '{}'...", activePath.string());

        std::string line;
        while (std::getline(file, line)) {
            line = Trim(line);
            if (line.empty() || line.starts_with(';') || line.starts_with('#') || line.starts_with('[')) {
                continue;
            }

            const auto delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) {
                continue;
            }

            const auto key = Trim(line.substr(0, delimiterPos));
            const auto valStr = Trim(line.substr(delimiterPos + 1));

            try {
                const float val = std::stof(valStr);
                const bool boolVal = ParseBoolValue(valStr, val);

                // Combat & Weapon Math
                if (key == "fOneHandedDamagePerLevel") LegendaryTraits::g_settings.oneHandedDamagePerLevel = val;
                else if (key == "fOneHandedCritPerLevel") LegendaryTraits::g_settings.oneHandedCritPerLevel = val;
                else if (key == "fTwoHandedDamagePerLevel") LegendaryTraits::g_settings.twoHandedDamagePerLevel = val;
                else if (key == "fTwoHandedCritPerLevel") LegendaryTraits::g_settings.twoHandedCritPerLevel = val;
                else if (key == "fArcheryDamagePerLevel") LegendaryTraits::g_settings.archeryDamagePerLevel = val;
                else if (key == "fArcheryCritPerLevel") LegendaryTraits::g_settings.archeryCritPerLevel = val;
                else if (key == "fArcheryStaggerPerLevel") LegendaryTraits::g_settings.archeryStaggerPerLevel = val;
                else if (key == "fBlockElementalReductionPerLevel") LegendaryTraits::g_settings.blockElementalReductionPerLevel = val;
                else if (key == "fSneakEffectivenessPerLevel") LegendaryTraits::g_settings.sneakEffectivenessPerLevel = val;
                else if (key == "fSneakAttackMultPerLevel") LegendaryTraits::g_settings.sneakAttackMultPerLevel = val;
                else if (key == "fLockpickingWidthPerLevel") LegendaryTraits::g_settings.lockpickingWidthPerLevel = val;
                else if (key == "fLockpickingGoldPerLevel") LegendaryTraits::g_settings.lockpickingGoldPerLevel = static_cast<std::int32_t>(val);
                else if (key == "fPickpocketChancePerLevel") LegendaryTraits::g_settings.pickpocketChancePerLevel = val;
                else if (key == "fPickpocketGoldPerLevel") LegendaryTraits::g_settings.pickpocketGoldPerLevel = static_cast<std::int32_t>(val);

                // Magic Magnitudes & Durations
                else if (key == "fDestructionPowerPerLevel") LegendaryTraits::g_settings.destructionPowerPerLevel = val;
                else if (key == "fRestorationPowerPerLevel") LegendaryTraits::g_settings.restorationPowerPerLevel = val;
                else if (key == "fAlterationDurationPerLevel") LegendaryTraits::g_settings.alterationDurationPerLevel = val;
                else if (key == "fIllusionPowerPerLevel") LegendaryTraits::g_settings.illusionPowerPerLevel = val;

                // Crafting & Barter
                else if (key == "fVendorPriceBonusPerLevel") LegendaryTraits::g_settings.vendorPriceBonusPerLevel = val;
                else if (key == "fSmithingArmorPenPerLevel") LegendaryTraits::g_settings.smithingArmorPenPerLevel = val;
                else if (key == "fAlchemyPotencyPerLevel") LegendaryTraits::g_settings.alchemyPotencyPerLevel = val;
                else if (key == "fEnchantingPowerPerLevel") LegendaryTraits::g_settings.enchantingPowerPerLevel = val;
                else if (key == "fEnchantingChargeReductionPerLevel") LegendaryTraits::g_settings.enchantingChargeReductionPerLevel = (val / 100.0f) * 10.0f;

                // Core Character Attributes
                else if (key == "fHealthPercentPerLevel") LegendaryTraits::g_settings.healthPercentPerLevel = val / 100.0f;
                else if (key == "fStaggerResistPerLevel") LegendaryTraits::g_settings.staggerResistPerLevel = val / 100.0f;
                else if (key == "fStaminaRatePerLevel") LegendaryTraits::g_settings.staminaRatePerLevel = val;
                else if (key == "fLightArmorStaminaBonusPerLevel") LegendaryTraits::g_settings.lightArmorStaminaBonusPerLevel = val;
                else if (key == "fCarryWeightPerLevel") LegendaryTraits::g_settings.carryWeightPerLevel = val;
                else if (key == "fPoisonResistPerLevel") LegendaryTraits::g_settings.poisonResistPerLevel = val;
                else if (key == "fSpellAbsorptionPerLevel") LegendaryTraits::g_settings.spellAbsorptionPerLevel = val;

                // Feature Toggles (Combat)
                else if (key == "bEnableOneHandedDamage") LegendaryTraits::g_settings.enableOneHandedDamage = boolVal;
                else if (key == "bEnableOneHandedCrit") LegendaryTraits::g_settings.enableOneHandedCrit = boolVal;
                else if (key == "bEnableTwoHandedDamage") LegendaryTraits::g_settings.enableTwoHandedDamage = boolVal;
                else if (key == "bEnableTwoHandedCrit") LegendaryTraits::g_settings.enableTwoHandedCrit = boolVal;
                else if (key == "bEnableArcheryDamage") LegendaryTraits::g_settings.enableArcheryDamage = boolVal;
                else if (key == "bEnableArcheryCrit") LegendaryTraits::g_settings.enableArcheryCrit = boolVal;
                else if (key == "bEnableArcheryStagger") LegendaryTraits::g_settings.enableArcheryStagger = boolVal;

                // Feature Toggles (Block & Resistances)
                else if (key == "bEnableBlockResistMaster") LegendaryTraits::g_settings.enableBlockResistMaster = boolVal;
                else if (key == "bEnableBlockResistFire") LegendaryTraits::g_settings.enableBlockResistFire = boolVal;
                else if (key == "bEnableBlockResistFrost") LegendaryTraits::g_settings.enableBlockResistFrost = boolVal;
                else if (key == "bEnableBlockResistShock") LegendaryTraits::g_settings.enableBlockResistShock = boolVal;
                else if (key == "bEnableBlockResistPoison") LegendaryTraits::g_settings.enableBlockResistPoison = boolVal;
                else if (key == "bEnableBlockResistMagic") LegendaryTraits::g_settings.enableBlockResistMagic = boolVal;

                // Feature Toggles (Armor & Attributes)
                else if (key == "bEnableHeavyArmorHealth") LegendaryTraits::g_settings.enableHeavyArmorHealth = boolVal;
                else if (key == "bEnableHeavyArmorMass") LegendaryTraits::g_settings.enableHeavyArmorMass = boolVal;
                else if (key == "bEnableLightArmorStaminaRate") LegendaryTraits::g_settings.enableLightArmorStaminaRate = boolVal;
                else if (key == "bEnableLightArmorStaminaPool") LegendaryTraits::g_settings.enableLightArmorStaminaPool = boolVal;

                // Feature Toggles (Magic)
                else if (key == "bEnableDestructionPower") LegendaryTraits::g_settings.enableDestructionPower = boolVal;
                else if (key == "bEnableRestorationPower") LegendaryTraits::g_settings.enableRestorationPower = boolVal;
                else if (key == "bEnableRestorationPoisonResist") LegendaryTraits::g_settings.enableRestorationPoisonResist = boolVal;
                else if (key == "bEnableAlterationDuration") LegendaryTraits::g_settings.enableAlterationDuration = boolVal;
                else if (key == "bEnableAlterationAbsorption") LegendaryTraits::g_settings.enableAlterationAbsorption = boolVal;
                else if (key == "bEnableIllusionPower") LegendaryTraits::g_settings.enableIllusionPower = boolVal;

                // Feature Toggles (Crafting & Stealth)
                else if (key == "bEnableSmithingCarryWeight") LegendaryTraits::g_settings.enableSmithingCarryWeight = boolVal;
                else if (key == "bEnableSmithingArmorPen") LegendaryTraits::g_settings.enableSmithingArmorPen = boolVal;
                else if (key == "bEnableAlchemyPotency") LegendaryTraits::g_settings.enableAlchemyPotency = boolVal;
                else if (key == "bEnableEnchantingPower") LegendaryTraits::g_settings.enableEnchantingPower = boolVal;
                else if (key == "bEnableEnchantingChargeRefund") LegendaryTraits::g_settings.enableEnchantingChargeRefund = boolVal;
                else if (key == "bEnableSneakEffectiveness") LegendaryTraits::g_settings.enableSneakEffectiveness = boolVal;
                else if (key == "bEnableSneakAttackMult") LegendaryTraits::g_settings.enableSneakAttackMult = boolVal;
                else if (key == "bEnableLockpickingWidth") LegendaryTraits::g_settings.enableLockpickingWidth = boolVal;
                else if (key == "bEnableLockpickingGold") LegendaryTraits::g_settings.enableLockpickingGold = boolVal;
                else if (key == "bEnablePickpocketChance") LegendaryTraits::g_settings.enablePickpocketChance = boolVal;
                else if (key == "bEnablePickpocketGold") LegendaryTraits::g_settings.enablePickpocketGold = boolVal;
                else if (key == "bEnableSpeechBarter") LegendaryTraits::g_settings.enableSpeechBarter = boolVal;

                // Diagnostics
                else if (key == "bVerboseLogging") {
                    LegendaryTraits::g_settings.verboseLogging = boolVal;
                }
            } catch (...) {
            }
        }

        SKSE::log::info("Configuration applied successfully.");
    }
} // namespace SettingsManager

namespace LegendaryTraits
{
    inline void QueueApplyTraits()
    {
        if (g_isApplyQueued.exchange(true)) {
            return;
        }

        if (const auto task = SKSE::GetTaskInterface()) {
            task->AddTask([]() {
                g_isApplyQueued = false;
                SettingsManager::LoadSettings();
                ApplyTraits();
            });
        }
    }

    class MenuCloseHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
    {
    public:
        static MenuCloseHandler* GetSingleton()
        {
            static MenuCloseHandler singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
        {
            if (!a_event) {
                return RE::BSEventNotifyControl::kContinue;
            }

            if (!a_event->opening && (a_event->menuName == RE::StatsMenu::MENU_NAME || a_event->menuName == RE::JournalMenu::MENU_NAME)) {
                SKSE::log::info("Menu closed: {}. Scheduling safe trait evaluation...", a_event->menuName.data());
                QueueApplyTraits();
            }

            return RE::BSEventNotifyControl::kContinue;
        }

        static void Register()
        {
            const auto ui = RE::UI::GetSingleton();
            if (ui) {
                ui->AddEventSink(GetSingleton());
            }
        }
    };
} // namespace LegendaryTraits

void SetupLog()
{
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) {
        SKSE::stl::report_and_fail("SKSE log directory not found.");
    }

    auto plugin = SKSE::PluginDeclaration::GetSingleton();
    SKSE::log::info("{} v{} initializing on Skyrim 1.7.104...", plugin->GetName(), plugin->GetVersion().string());
}

void OnMessage(SKSE::MessagingInterface::Message* a_message)
{
    switch (a_message->type) {
    case SKSE::MessagingInterface::kDataLoaded:
        SKSE::log::info("Game data loaded. Registering event sinks...");
        SettingsManager::LoadSettings();
        LegendaryTraits::ThieveryEventHandler::Register();
        LegendaryTraits::CombatEventHandler::Register();
        LegendaryTraits::MenuCloseHandler::Register();
        SKSE::log::info("Event sinks registered successfully.");
        break;

    case SKSE::MessagingInterface::kPostLoadGame:
        SKSE::log::info("Save loaded. Clearing previous session tracking and scheduling trait application...");
        LegendaryTraits::g_appliedMods = {};
        LegendaryTraits::QueueApplyTraits();
        break;

    default:
        break;
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SetupLog();
    SKSE::Init(a_skse);

    const auto messaging = SKSE::GetMessagingInterface();
    if (!messaging || !messaging->RegisterListener(OnMessage)) {
        return false;
    }

    return true;
}