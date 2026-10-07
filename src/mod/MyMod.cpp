// SPDX-License-Identifier: GPL-3.0-only

#include "mod/MyMod.h"

#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/input/KeyRegistry.h"
#include "mc/client/gui/screens/UIScene.h"
#include "mc/client/gui/screens/ScreenView.h"
#include "mc/client/gui/screens/controllers/HudScreenController.h"
#include "mc/client/gui/screens/interfaces/ISceneStack.h"
#include "mc/client/gui/screens/models/MinecraftScreenModel.h"
#include "mc/client/options/IOptionRegistry.h"
#include "mc/locale/I18n.h"


namespace my_mod {

MyMod& MyMod::getInstance() {
    static MyMod instance;
    return instance;
}

bool MyMod::load() {
    getSelf().getLogger().info("Vibrant Toggle loaded (LeviLamina Client 26.51.6).");
    // Register before Minecraft builds the default keyboard mappings/input handlers.
    auto& key = ll::input::KeyRegistry::getInstance().getOrCreateKey(
        "toggle_vibrant_visuals_y", {'Y'}, true, ll::mod::NativeMod::current()
    );
    // LL invalidates a mod's handles on disable and does not recreate them on enable.
    if (!key.isValid()) {
        getSelf().getLogger().error("Restart Minecraft to re-enable Vibrant Toggle.");
        return false;
    }
    key.registerButtonDownHandler([this](FocusImpact, IClientInstance& client) {
        if (!mEnabled) return;
        getSelf().getLogger().info("Y toggle received: screen={}, gameplayInput={}",
                                  client.getScreenName(), client.isInGameInputEnabled());
        if (client.getLocalPlayer() && client.isInGameInputEnabled()) {
            toggleVibrantVisuals(client);
        }
    });
    return true;
}

bool MyMod::enable() {
    if (mEnabled) return true;
    // Minecraft displays the action's translation key in keyboard settings.
    // Preserve the action ID so existing user bindings remain intact.
    getI18n().appendAdditionalTranslations(
        {{"key.vibrant-toggle.toggle_vibrant_visuals_y", "Vibrant Toggle"}}, ""
    );
    auto& key = ll::input::KeyRegistry::getInstance().getOrCreateKey(
        "toggle_vibrant_visuals_y", {'Y'}, true, ll::mod::NativeMod::current()
    );
    if (!key.isValid()) {
        getSelf().getLogger().error("Restart Minecraft to re-enable Vibrant Toggle.");
        return false;
    }
    mEnabled = true;
    getSelf().getLogger().info("Press Y in game to toggle Vibrant Visuals.");
    return true;
}

bool MyMod::disable() {
    mEnabled = false;
    // LL's KeyRegistry removes this mod's callbacks when it is disabled.
    return true;
}

void MyMod::toggleVibrantVisuals(IClientInstance& client) {
    auto& logger = getSelf().getLogger();
    auto& options = client.getOptions();
    auto current = options.getGraphicsMode();
    if (current == GraphicsMode::RayTraced) {
        logger.warn("Turn off ray tracing in Video settings before using Vibrant Toggle.");
        return;
    }

    // The named vanilla HUD is a UIScene backed by a HudScreenController.
    auto* scene = client.getCurrentSceneStack()->getTopScene();
    if (!scene || scene->getScreenName() != "hud_screen") {
        logger.warn("Graphics toggle requires the gameplay HUD.");
        return;
    }
    auto& hud = static_cast<UIScene&>(*scene);
    if (!hud.mScreenView || !hud.mScreenView->mController) {
        logger.warn("HUD controller is unavailable.");
        return;
    }
    auto& controller = static_cast<HudScreenController&>(*hud.mScreenView->mController);
    auto model = controller.mMinecraftScreenModel;
    if (!model) {
        logger.warn("HUD screen model is unavailable.");
        return;
    }

    auto target = current == GraphicsMode::Advanced ? mPreviousVanillaMode : GraphicsMode::Advanced;
    logger.info("Request via screen model: current={}, target={}",
                static_cast<int>(current), static_cast<int>(target));
    // This is a named MCAPI method, rather than a call through the incompletely
    // described IAdvancedGraphicsOptions vtable. It updates the settings model.
    model->setGraphicsMode(static_cast<int>(target));
    auto actual = options.getGraphicsMode();
    if (actual != target) {
        logger.warn("Screen-model transition not applied: requested={}, actual={}",
                    static_cast<int>(target), static_cast<int>(actual));
        return;
    }
    if (current == GraphicsMode::Simple || current == GraphicsMode::Fancy) {
        mPreviousVanillaMode = current;
    }
    options.saveIfNeeded();
    logger.info("Vibrant Visuals {}.", target == GraphicsMode::Advanced ? "enabled" : "disabled");
}

} // namespace my_mod

LL_REGISTER_MOD(my_mod::MyMod, my_mod::MyMod::getInstance());
