# Nintendo 64 C Notepad++ Syntax Highlighter
Provides Syntax Highlighting and Auto-Completion functions for Nintendo 64 C. For use with Notepad++.<br/><br/>

### Installation
* Download this repository
* Go to your notepad++ install folder (for exmaple C:\ProgramFiles\Notepad++\)
* Put the dll file, APIs folder and config folder inside the Notepad++\plugins folder, replacing if necissary
* Open Notepad++
* Open a Nintendo 64 C source file
* Click language -> C (N64)
* Click on Settings -> Preferences -> Auto-Completion, enable both Auto-Completion and Function parameters hints<br/><br/>

### Configuration
Highlighting - Settings -> Style Configurator -> C (N64), or edit the XML file inside the notepad++/plugins/config folder.
Autocompletion - Edit the XML file inside the notepad++/plugins/APIs folder<br/><br/>

### Bugs
* -- is treated like a comment instead of a subtraction operation
* #'s are not properly colored<br/>

Please notify me about missing functions so that I can add them to the XML file
<br/><br/>
### FAQ
**Q:** Function 'X' is missing or has something wrong.<br/>
**A:** You can correct it by editing the XML file, but please contact me too so I can correct it for everyone else!<br/><br/>

**Q:** Can you make this for "Editor Name Here"?<br/>
**A:** I don't use "Editor Name Here" so I can't help much, however given how easy it is to open the XML file there's nothing stopping you from grabbing all the function names and adding them yourself<br/><br/>
