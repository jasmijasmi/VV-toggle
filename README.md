# Vibrant Toggle

Client-only C++ mod for LeviLamina .

Press **Y** while playing to toggle Minecraft's Vibrant Visuals. Turning it off restores the Simple/Fancy mode last used by the mod during this session; it defaults to Fancy if you started with Vibrant Visuals enabled. 
## Build and install

Install xmake, Visual Studio C++ Build Tools with a Windows SDK, and LLVM/clang-cl. From this directory run:

```powershell
xmake repo -u
xmake f -y -p windows -a x64 -m release --target_type=client
xmake -y
```

## Notes
Unsupported devices/worlds/packs and ray-traced mode log a reason without changing graphics; disabling the mod stops the key from acting.


## License

CC0-1.0; see LICENSE.
