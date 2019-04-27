# Nintendo 64 C Notepad++ Syntax Highlighter
Provides Syntax Highlighting and Auto-Completion functions for Nintendo 64 C. For use with Notepad++.<br/><br/>

### Installation
* Download this repository
* Go to your notepad++ install folder (for exmaple C:\Program Files\Notepad++\)
* Place the APIs folder inside the Notepad++\plugins folder
* If you are using a version of Notepad++ LOWER than 7.6, place the .dll and Config folder in here as well. 
* If you are using Notepad++ 7.6 or higher go to C:\Programdata\Notepad++\plugins and create a new folder called N64C and place the .dll in there. Then go to C:\Users\YOURUSERNAME\AppData\Roaming\Notepad++\plugins\ and place the config folder in here, overwriting if necessary.
* Open Notepad++
* Open a Nintendo 64 C source file
* Click language -> C (N64)
* Click on Settings -> Preferences -> Auto-Completion, enable both Auto-Completion and Function parameters hints<br/><br/>

### Configuration
Highlighting - Settings -> Style Configurator -> C (N64), or edit the XML file inside the config folder.

Autocompletion - Edit the XML file inside the notepad++/plugins/APIs folder<br/><br/>

### How to compile the source code
1. Download and install a copy of [Visual Studio 2008](https://www.microsoft.com/en-us/download/details.aspx?id=7873).
2. Download a copy of [Notepad++ 5.8's source code](https://notepad-plus-plus.org/download/v5.8.html).
3. Place the NP++ Source code in a folder. 
4. Place this syntax highlighter's source code in the same folder so it is next to the PowerEditor and Scintilla folders.
5. Rename the sourcecode folder to N64C
6. Open the project in Visual Studio 8.
7. Ensure that you are compiling as "Unicode Release".
8. Compile Scintilla, then compile PowerEditor, then compile this syntax highlighter.<br/><br/>

### Bugs
Multiline Preprocessor commands (through the use of the \ character) do not color properly.

CommentDoc lines have not been implemented.

Please notify me about missing functions so that I can add them to the XML file.<br/><br/>

### FAQ
**Q:** Function 'X' is missing or has something wrong.<br/>
**A:** You can correct it by editing the XML file, but please contact me too so I can correct it for everyone else!<br/><br/>

**Q:** Can you make this for "Editor Name Here"?<br/>
**A:** I don't use "Editor Name Here" so I can't help much, however given how easy it is to open the XML file there's nothing stopping you from grabbing all the function names and adding them yourself<br/><br/>

### Special Thanks
Kyle Fleming (aka Garthex) for the original plugin.
