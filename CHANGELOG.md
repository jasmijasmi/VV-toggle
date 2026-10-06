# Changelog

## [1.0.0] - 2026-10-06

- Register the Y binding during load, use a new action ID to avoid the old O mapping, and log received key callbacks.
- Use Minecraft's gameplay-input state instead of requiring an exact HUD screen name.
- Added the Y keybind using LL's client key registry.
- Toggle Vibrant Visuals through Minecraft's native graphics controller.
- Restore the previous vanilla mode and check graphics availability.
- Target LeviLamina Client 26.51.6 with client-only packaging and CI.
- Set the package version explicitly so builds from a source ZIP do not require Git metadata.
