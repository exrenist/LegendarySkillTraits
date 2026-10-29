<p align="center">
<img style="height: 150px; background-color: transparent;" src="https://github.com/user-attachments/assets/e9b279f7-9c60-45f7-97c6-6840ee1b76fb" />
</p>

<div align="center">
<h1>
Legendary Skill Traits
</h1>
</div>

<div align="center">
A lightweight, high-performance SKSE plugin for Skyrim SE/AE (tested on 1.7.104+) that rewards players with powerful passive traits and attributes based on their legendary skill levels.
</div>

<p>&nbsp;</p>
<p>&nbsp;</p>

<div align="center">
<h2>
Disclaimer
</h2>
</div>

<div align="center">
<strong>I made this mod using AI because I'm not expert in coding and I don't want to spend days trying to figure out errors and engine hook fails, so please don't judge a book by it's cover.</strong>
</div>

<p>&nbsp;</p>
<p>&nbsp;</p>

## Requirements

* **SKSE64**
* **Address Library for SKSE**
* **MCM Helper**
* **SkyUI**

<p>&nbsp;</p>
<p>&nbsp;</p>

## Information
Mod hooks directly into Skyrim's actor value and skill-leveling engine routines to intercept legendary resets and dynamically calculate scaling passive stat bonuses in native memory. Also it calculates whenever you exit the Perk or MCM menu so there aren't any performance issues.

<p>&nbsp;</p>
<p>&nbsp;</p>

## Installation

1. Download the latest release archive (`.zip`) from GitHub or Nexus Mods.
2. Install it using your preferred mod manager (**MO2** or **Vortex**).
3. Ensure SKSE is running correctly.

<p>&nbsp;</p>
<p>&nbsp;</p>

<div align="center">
<h2>
Credits
</h2>
</div>

<div align="center">
Thanks to mcmex for the idea and ClibDT6 for making us modders job easier, without them I couldn't make this mod. Big thanks!
</div>

<br>

<div align="center">
<a href="https://www.nexusmods.com/skyrimspecialedition/mods/154240?tab=description" target="_blank" style="text-decoration:none;">
<img alt="Static Badge" src="https://img.shields.io/badge/ClibDT6-orange?style=flat&logo=data:image/png;base64,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&logoSize=auto" style="height: 25px; background-color: transparent;">
</a>
</div>
<br>
<div align="center">
<a href="https://www.nexusmods.com/profile/mcmex?gameId=1704" target="_blank" style="text-decoration:none;">
<img alt="Static Badge" src="https://img.shields.io/badge/mcmex-orange?style=flat&logo=data:image/png;base64,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&logoSize=auto" style="height: 25px; background-color: transparent;">
</a>
</div>
