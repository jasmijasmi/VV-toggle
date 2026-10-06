# Vibrant Toggle

Client-only C++ mod for LeviLamina 26.51.6 (26.51 client series).

Press **Y** while playing to toggle Minecraft's Vibrant Visuals. Turning it off restores the Simple/Fancy mode last used by the mod during this session; it defaults to Fancy if you started with Vibrant Visuals enabled. The binding uses `ll::input::KeyRegistry` and can be remapped in keyboard settings. Input on chat, inventory, pause, and other screens is ignored.

The toggle uses the active vanilla HUD screen model and its named `MinecraftScreenModel::setGraphicsMode(int)` API. It avoids virtual calls through the incompletely described advanced-graphics interface. Minecraft controls availability and the mod verifies the resulting graphics selection. Turn off ray tracing separately. Disabling leaves the graphics selection in place; re-enabling requires restarting because LL invalidates disabled key handles.

## Build and install

Install xmake, Visual Studio C++ Build Tools with a Windows SDK, and LLVM/clang-cl. From this directory run:

```powershell
xmake repo -u
xmake f -y -p windows -a x64 -m release --target_type=client
xmake -y
```

The LL mod packer places the mod in `bin/`. Copy the generated `vibrant-toggle` directory, including its DLL and generated `manifest.json`, into your LeviLamina client's `mods/` directory. Restart Minecraft. Do not copy the source manifest with `${...}` placeholders.

The GitHub workflows build only the client variant. The LIP metadata targets `JasminMunteanu/test`; release assets use the repository name `test`.

## Validation

If Y does nothing, fully restart Minecraft after replacing the DLL. Check the
LeviLamina log for `Vibrant Toggle loaded`, `Press Y in game`, and
`Y toggle received`. The new action ID `toggle_vibrant_visuals_y` avoids retaining
the original O binding. A received callback logs the screen and gameplay-input
state; subsequent messages explain whether Minecraft accepted the transition.

The Windows x64 release DLL compiled, linked, and packaged successfully against LeviLamina Client 26.51.6 using xmake 3.1.1, Visual Studio Build Tools 2022 (17.14.41), Windows SDK 10.0.26100, and LLVM 23.1.2. The user confirmed that the screen-model implementation works in-game. The local package is `bin/vibrant-toggle-client-windows-x64.zip`. The following broader checks remain useful:

1. Y enables Vibrant Visuals from Fancy, and a second press restores Fancy; repeat with Simple.
2. Holding Y produces one transition per press.
3. Typing Y in chat or using inventory/pause menus does not change graphics.
4. Remapping the key works.
5. Unsupported devices/worlds/packs and ray-traced mode log a reason without changing graphics; disabling the mod stops the key from acting.

## API references

- [LL KeyRegistry 26.51.6](https://github.com/LiteLDev/LeviLamina/blob/v26.51.6/src-client/ll/api/input/KeyRegistry.h)
- [Native graphics controller](https://github.com/LiteLDev/LeviLamina/blob/v26.51.6/src/mc/options/IAdvancedGraphicsOptions.h)
- [Graphics modes](https://github.com/LiteLDev/LeviLamina/blob/v26.51.6/src/mc/options/GraphicsMode.h)

## License

CC0-1.0; see LICENSE.
