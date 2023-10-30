# Nintendo 64 C Notepad++ Syntax Highlighter
Provides Syntax Highlighting and Auto-Completion functions for Nintendo 64 C. For use with Notepad++.
![Screenshot](https://i.imgur.com/fZ1lWGY.png)<br/><br/>

### Installation
If you are unsure of your Notepad++ version, simply Launch the program and go to ? -> About Notepad++ (Shortcut F1)<br/>

<details><summary>Notepad++ 7.7 or higher</summary>
<p>

1. Download this repository
2. From the downloaded repository, open the APIs folder, copy the `C (N64).xml`, and paste it into the `autoCompletion` folder inside the Notepad++ install folder (for example `C:/Program Files/Notepad++/`).
3. Launch Notepad++
4. Click Language -> User Defined Language -> Define your language...
5. Click Import...
6. Go to the `UDL` folder inside the downloaded repository, and import the `.xml` file. It should say "import successful". You might need to restart Notepad++.
7. Open a Nintendo 64 C source file
8. Click Language -> C (N64). Since it is a UDL, it should be at the bottom.
9. Click on Settings -> Preferences -> Auto-Completion, enable both Auto-Completion and Function parameters hints
</p>
</details>

<details><summary>Notepad++ 7.6</summary>
<p>

1. Download this repository
2. Go to your Notepad++ install folder (for example `C:/Program Files/Notepad++/`)
3. Place the APIs folder inside the `Notepad++/plugins` folder
4. Go to `C:/ProgramData/Notepad++/plugins` and create a new folder called N64C and place the .dll in there. Then go to `C:/Users/YOURUSERNAME/AppData/Roaming/Notepad++/plugins/` and place the config folder in here, overwriting if necessary.
5. Launch Notepad++
6. Open a Nintendo 64 C source file
7. Click Language -> C (N64)
8. Click on Settings -> Preferences -> Auto-Completion, enable both Auto-Completion and Function parameters hints
</p>
</details>

<details><summary>Notepad++ 7.5 or lower</summary>
<p>

1. Download this repository
2. Go to your Notepad++ install folder (for example `C:/Program Files/Notepad++/`)
3. Place the APIs folder inside the `Notepad++/plugins` folder
4. Place the .dll and Config folder in here as well. 
5. Launch Notepad++
6. Open a Nintendo 64 C source file
7. Click language -> C (N64)
8. Click on Settings -> Preferences -> Auto-Completion, enable both Auto-Completion and Function parameters hints
</p>
</details>
</br>

### Configuration
<details><summary>Notepad++ 7.7 or higher</summary>
<p>

**Highlighting -** Language -> User Defined Language -> Define your language..., select `C (N64)` from the dropdown, and modify to your liking. Alternatively, go to `C:/Users/YOURUSERNAME/AppData/Roaming/Notepad++/` and modify `userDefineLang.xml`. Alternatively, manually edit the XML file and re-import it (make sure you remove the old one first or you'll have duplicate entries).

**Autocompletion -** Edit the XML file inside the `Notepad++/autoCompletion` folder
</p>
</details>

<details><summary>Notepad++ 7.6 or lower</summary>
<p>
    
**Highlighting -** Settings -> Style Configurator -> C (N64), or edit the XML file inside the config folder.

**Autocompletion -** Edit the XML file inside the `Notepad++/plugins/APIs` folder
</p>
</details>
</br>

### How to compile the source code
1. Download and install a copy of [Visual Studio 2008](https://www.microsoft.com/en-us/download/details.aspx?id=7873).
2. Download a copy of [Notepad++ 5.8's source code](https://github.com/notepad-plus-plus/notepad-plus-plus/releases/tag/v5.8).
3. Place the NP++ Source code in a folder. 
4. Place this syntax highlighter's source code in the same folder so it is next to the PowerEditor and Scintilla folders.
5. Rename the sourcecode folder to N64C
6. Open the project in Visual Studio 8.
7. Ensure that you are compiling as "Unicode Release".
8. Compile Scintilla, then compile PowerEditor, then compile this syntax highlighter.<br/><br/>

### Bugs
<details><summary>Notepad++ 7.7 or higher</summary>
<p>

* Comment Doc highlighting is not supported
</p>
</details>

<details><summary>Notepad++ 7.6 or lower</summary>
<p>

* #if and #end macros will not have code folding if there is a space between the hastag and first character (example, # if)
* The F in a floating point number is not highlighted (example, 10.0F). L is also not highlighted.
</p>
</details>

Please notify me about missing functions so that I can add them to the XML file.<br/><br/>

### FAQ
**Q:** Function 'X' is missing or has something wrong.<br/>
**A:** You can correct it by editing the XML file, but please contact me too so I can correct it for everyone else!<br/><br/>

**Q:** Can you make this for "Editor Name Here"?<br/>
**A:** I don't use "Editor Name Here" so I can't help much, however given how easy it is to open the XML file there's nothing stopping you from grabbing all the function names and adding them yourself<br/><br/>

### Special Thanks
Kyle Fleming (aka Garthex) for the original plugin.
