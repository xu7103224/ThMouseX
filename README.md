ThMouseX
=======

Introduction
------------
ThMouseX is a fork from [ThMouse](https://github.com/hwei/ThMouse) made by hwei.

This is a tool that enables mouse control for Shoot 'em ups games, intended for Touhou Project series, allowing player character to move towards wherever the cursor points.

Demo clip: https://www.youtube.com/watch?v=uMkzmM13qpU

Download link
---
https://github.com/Meigyoku-Thmn/ThMouseX/releases <br>
(Required .NET Framework 4.8 from version 2.1.0)

Differences of the fork
-----------------------
* Support any game's resolutions
* Support DirectX8 and DirectX9 games
* Can be opened and closed at any time, it will automatically detect configured games
* You can use Lua script ([Lua](https://www.lua.org/), [LuaJIT](https://luajit.org/), or [NeoLua](https://github.com/neolithos/neolua)) to configure further.

Drawbacks
--------
* Only works with 32-bit games
* Doesn't work well with Steam Overlay, although this is minimal.
* Configuration is difficult.

Preconfigured games
-------------
This tool should be compatible with any Touhou games from 6 to the latest. Here is the list of preconfigured games in [Games.txt](https://github.com/Meigyoku-Thmn/ThMouseX/blob/master/ThMouseX/Games.txt):
* Touhou 6&emsp;&emsp;&ensp;東方紅魔郷 ～ Embodiment of Scarlet Devil (v1.02h)
* Touhou 7&emsp;&emsp;&ensp;東方妖々夢 ～ Perfect Cherry Blossom (v1.00b)
* Touhou 8&emsp;&emsp;&ensp;東方永夜抄 ～ Imperishable Night (v1.00d)
* Touhou 9&emsp;&emsp;&ensp;東方花映塚 ～ Phantasmagoria of Flower View (v1.50a)
* Touhou 9.5&emsp;&ensp; 東方文花帖 ～ Shoot the Bullet (v1.02a)
* Touhou 10&emsp;&emsp;東方風神録 ～ Mountain of Faith 
* Touhou 11&emsp;&emsp;東方地霊殿 ～ Subterranean Animism 
* Touhou 12&emsp;&emsp;東方星蓮船 ～ Undefined Fantastic Object (v1.00b)
* Touhou 12.5&emsp; Double Spoiler ～ 東方文花帖
* Touhou 12.8&emsp; 妖精大戦争 ～ 東方三月精 Fairy Wars 
* Touhou 13&emsp;&emsp;東方神霊廟 ～ Ten Desires (v1.00c)
* Touhou 14&emsp;&emsp;東方輝針城 ～ Double Dealing Character (v1.00b)
* Touhou 14.3&emsp; 弾幕アマノジャク ～ Impossible Spell Card
* Touhou 15&emsp;&emsp;東方紺珠伝 ～ Legacy of Lunatic Kingdom (v1.00b)
* Touhou 16&emsp;&emsp;東方天空璋 ～ Hidden Star in Four Seasons
* Touhou 16.5&emsp; 秘封ナイトメアダイアリー 〜 Violet Detector
* Touhou 17&emsp;&emsp;東方鬼形獣 ～ Wily Beast and Weakest Creature
* Touhou 18&emsp;&emsp;東方虹龍洞 ～ Unconnected Marketeerss
* Touhou 18.5&emsp; バレットフィリア達の闇市場 〜 100th Black Market
* Touhou 19&emsp;&emsp;東方獣王園 〜 Unfinished Dream of All Living Ghost (v1.00a) (will be included in ThMouseX 3.0.0)

It also have preconfiguration of some other games:
* [DANMAKAI: Red Forbidden Fruit](https://store.steampowered.com/app/1388230/DANMAKAI_Red_Forbidden_Fruit/)
* [東方幕華祭 〜 Fantastic Danmaku Festival](https://store.steampowered.com/app/882710/_TouHou_Makuka_Sai__Fantastic_Danmaku_Festival/)
* [東方幕華祭 春雪篇 〜 Fantastic Danmaku Festival Part II](https://store.steampowered.com/app/1031480/TouHou_Makuka_Sai__Fantastic_Danmaku_Festival_Part_II/)
* [東方眠世界 〜 Wonderful Waking World](https://oligarchomp.itch.io/wonderful-waking-world)
* [东方龙隐談 〜 Chaos of Black Loong](https://store.steampowered.com/app/915130/__Touhou_Chaos_of_Black_Loong/)

You can add more games to [Games2.txt](https://github.com/Meigyoku-Thmn/ThMouseX/blob/master/ThMouseX/Games2.txt) and copy it to ThMouseX's directory, side-by-side with Games.txt. (from version 3.0.0)

FAQ
---
### How to compile on your computer (not recommended for non-tech savvy)
This project can be compiled via Visual Studio (I use Visual Studio 2022), or just Visual Studio Build Tools:
* [Visual Studio](https://visualstudio.microsoft.com/), select "Desktop development with C++" during installation.
* [Visual Studio Build Tools](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools), select "Visual C++ build tools" during installation.

Also you need [.NET Framework 4.8 SDK](https://dotnet.microsoft.com/en-us/download/dotnet-framework).

You also need to setup [vcpkg](https://vcpkg.io/en/getting-started.html) and put vcpkg folder path into the PATH of environment variables.

Remember to **compile for 32-bit, toolset v143, .NET Framework 4.8**, and make sure these files and folder are in the same folder:
* ConfigScripts
* 0Harmony.dll
* Common.dll
* Cursor.png
* DX8Hook.dll
* Games.txt
* Neo.Lua.dll
* NeoLuaBootstrap.dll
* Sigil.dll
* ThMCore.dll
* THMouseX.exe
* ThMouseX.txt
* VirtualKeyCodes.txt

### Another way to compile without installing anything on your computer (recommended way)
- Fork the THMouseX repository
- Go to the "Actions" tab
- Select the "Deployment" workflow
- Run the workflow (refresh the page if nothing shows up)
- Wait for it, then download the produced zipped artifact (shown up in the page with url form `https://github.com/<username>/ThMouseX/actions/runs/<flowid>`).

### How to use ThMouseX?
1. Run ThMouseX.exe.
2. Run your game, or you can run your game first and then run ThMouseX.exe.
3. If the game is supported, a cursor will show upon entering the game.

### How to control?
* The character will move towards where the cursor points. Please note that it will NOT move immidiately with the cursor, because ThMouseX doesn't modifier any game's behavior.
* You still have to use left hand to focus and shoot.
* Left click to use bomb/spell.
* Right click to toggle mouse control.
* Press M to toggle Windows mouse cursor visibility.

### How to close it?
1. You can close your game first or ThMouseX first, it doesn't matter.
2. Double-click the ThMouseX icon on the taskbar and press the Quit button, or right click it and press Exit.

Additional Instructions
-----------------------
### Cursor sprite
The crosshair/cursor sprite may be changed:
1. Find your preferred crosshair in .png format and place it into the same folder as ThMouseX.exe.
2. Open up ThMouseX.txt.
3. Change the file name after "CursorTexture = " to the preferred crosshair file's name. (Don't forget the file extension name ".png".)

### Add your favorite games
You can extend ThMouseX to support more Shoot 'em ups games by modifying the "games.txt" file. You can also use Lua script for advanced cases.

Open the file with any text editor and try yourself.

(To document more)

### Sometime ThMouseX doesn't actually work, the game's character just keeps moving to a corner or is not bound exactly to the cursor
They are really rare bugs that I have yet found a way to fix.

Some workarounds:
* Restart ThMouseX
* Start ThMouseX FIRST, then start the game
* When starting ThMouseX, make sure your game's window is NOT minimized (this should be fixed from version 2.1.0).

### Steam Overlay stops functional, the game control is unusable too!
This is a limitation of how this tool hooks into the game's routine. Please avoid turning off ThMouseX while Steam Overlay is visible.

### Game crashes when exiting or lauching ThMouseX
Please avoid lauching and exiting ThMouseX many times while the game is running.

So far I only see this bug on .NET Framework games and it's a rare bug. It may has something with the library Lib.Harmony which ThMouseX uses.

### Antivirus programs' detection
This tool uses various code injection techniques, so it's normal that some antivirus programs don't like it. You can verify the source code and build it on your own using the instructions above, or set an exception case in the antivirus program for ThMouseX (including the DLLs).

### ThMouseX doesn't work with old versions of Fantastic Danmaku Festival I (or games that use .NET Framework 2/3)
Old (pre-Steam) versions of Fantastic Danmaku Festival use .NET Framework 2, so normally this tool will not work because it uses .NET Framework 4. But you can force the game to use .NET Framework 4 by putting this text file to the same place with THMHJ.exe:

```xml
<?xml version="1.0" encoding="utf-8" ?>
<configuration>
    <startup useLegacyV2RuntimeActivationPolicy="true"> 
        <supportedRuntime version="v4.0"/>
    </startup>
</configuration>
```

Ensure the name of this manifest file is `THMHJ.exe.config`, not `THMHJ.txt` or `THMHJ.exe.config.txt`.

This can also be applied to other applications that use .NET Framework 2 too.
