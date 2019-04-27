#include "N64C.h"

#include "LexerBase.h"

using namespace N64C;


BOOL APIENTRY DllMain(HANDLE /*hModule*/, DWORD reasonForCall, LPVOID /*lpReserved*/)
{
	switch (reasonForCall)
	{
	case DLL_PROCESS_ATTACH:

		/* Set function pointers */
		pluginMenuItems[0]._pFunc = aboutDlg;
	    	
		/* Fill menu names */
		generic_strncpy_s(pluginMenuItems[0]._itemName, nbChar, aboutMenuItem.c_str(), _TRUNCATE);

		/* Set shortcuts */
		pluginMenuItems[0]._pShKey = NULL;

		pluginMenuItems[0]._cmdID = NULL;
		pluginMenuItems[0]._init2Check = false;

		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#define EXT_LEXER_DECL __declspec( dllexport ) __stdcall

extern "C" {

#ifdef UNICODE
__declspec(dllexport) BOOL isUnicode() {
	return TRUE;
}
#endif

__declspec(dllexport) const TCHAR * getName() { return PLUGIN_NAME.c_str(); }
__declspec(dllexport) void setInfo(NppData notpadPlusData){ nppData = notpadPlusData; }

__declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	*nbF = numPluginMenuItems;
	return pluginMenuItems;
}

__declspec(dllexport) void beNotified(SCNotification * /*notifyCode*/) {}
__declspec(dllexport) LRESULT messageProc(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/) { return TRUE; }


} // End extern "C"

int EXT_LEXER_DECL GetLexerCount() { return 1; }

void EXT_LEXER_DECL GetLexerName(unsigned int /*index*/, char *name, int buflength)
{
	*name = 0;
	if (buflength > 0) {
		strncpy_s(name, buflength, LEXER_NAME.c_str(), _TRUNCATE);
	}
}

void EXT_LEXER_DECL GetLexerStatusText(unsigned int /*Index*/, TCHAR *desc, int buflength)
{
	if (buflength > 0) {
		generic_strncpy_s(desc, buflength, LEXER_STATUS_TEXT.c_str(), _TRUNCATE);
	}
}

LexerFactoryFunction EXT_LEXER_DECL GetLexerFactory(unsigned int index) {
	if (index == 0)
		return LexerN64C::LexerFactory;
	else
		return 0;
}

void N64C::aboutDlg()
{
	::MessageBox(nppData._nppHandle,
		TEXT("Nintendo 64 C Syntax Highlighter 1.3\n")
		TEXT("https://github.com/buu342/N64-SyntaxHighlighter\n\n"),
		TEXT("About"),
		MB_OK);

}

void SCI_METHOD LexerN64C::Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess) {
	try {
		Accessor astyler(pAccess, &props);
		Colourise_Doc(startPos, length, initStyle, keyWordLists, astyler);
		astyler.Flush();
	} catch (...) {
		// Should not throw into caller as may be compiled with different compiler or options
		pAccess->SetErrorStatus(SC_STATUS_FAILURE);
	}
}
void SCI_METHOD LexerN64C::Fold(unsigned int startPos, int length, int initStyle, IDocument *pAccess) {
	try {
		Accessor astyler(pAccess, &props);
		Fold_Doc(startPos, length, initStyle, astyler);
		astyler.Flush();
	} catch (...) {
		// Should not throw into caller as may be compiled with different compiler or options
		pAccess->SetErrorStatus(SC_STATUS_FAILURE);
	}
}

// Extended to accept accented characters
static inline bool IsAWordChar(int ch) {
	return ch >= 0x80 ||
		   (isalnum(ch) || ch == '_');
}

static inline bool IsCOperator(int ch) {
	if (ch >= 0x80 || isalnum(ch)) {
		return false;
	}

	if (ch == '*' || ch == '/' || ch == '-' || ch == '+' ||
		ch == '(' || ch == ')' || ch == '=' ||
		ch == '{' || ch == '}' || ch == '~' ||
		ch == '[' || ch == ']' || ch == ';' ||
		ch == '<' || ch == '>' || ch == ',' ||
		/*ch == '.' ||*/ ch == '^' || ch == '%' || //ch == ':' ||
		ch == '#' || ch == '&' || ch == '|' ||
		ch == '!') {
		return true;
	}
	return false;
}

// Test for [=[ ... ]=] delimiters, returns 0 if it's only a [ or ],
// return 1 for [[ or ]], returns >=2 for [=[ or ]=] and so on.
// The maximum number of '=' characters allowed is 254.
static int LongDelimCheck(StyleContext &sc) {
	int sep = 1;
	while (sc.GetRelative(sep) == '=' && sep < 0xFF)
		sep++;
	if (sc.GetRelative(sep) == sc.ch)
		return sep;
	return 0;
}


//  <---  Colourise --->
void N64C::Colourise_Doc(unsigned int startPos, int length, int initStyle, WordList *keywordlists[], Accessor &styler)
{
	WordList &keywords0 = *keywordlists[0];
	WordList &keywords1 = *keywordlists[1];
	WordList &keywords2 = *keywordlists[2];
	WordList &keywords3 = *keywordlists[3];
	WordList &keywords4 = *keywordlists[4];
	WordList &keywords5 = *keywordlists[5];
	WordList &keywords6 = *keywordlists[6];
	WordList &keywords7 = *keywordlists[7];
	WordList &keywords8 = *keywordlists[8];

	int currentLine = styler.GetLine(startPos);
	int sepCount = 0;
	if (initStyle == PREPROCESSOR) {
		sepCount = styler.GetLineState(currentLine - 1) & 0xFF;
	}

	// Do not leak onto next line
	if (initStyle == STRINGEOL || initStyle == COMMENTLINE || initStyle == PREPROCEOL ) {
		initStyle = DEFAULT;
	}

	StyleContext sc(startPos, length, initStyle, styler);
	for (; sc.More(); sc.Forward()) {
		if (sc.atLineEnd) {
			// Update the line state, so it can be seen by next line
			currentLine = styler.GetLine(sc.currentPos);
			styler.SetLineState(currentLine, 0);
		}

		// Handle string line continuation
		if ((sc.state == STRING || sc.state == CHARACTER || sc.state == PREPROCESSOR) &&
				sc.ch == '\\') {
			if (sc.chNext == '\n' || sc.chNext == '\r') {
				sc.Forward();
				if (sc.ch == '\r' && sc.chNext == '\n') {
					sc.Forward();
				}
				continue;
			}
		}

		// Determine if the current state should terminate.
		if (sc.state == OPERATOR || sc.state == NUMBER) {
			sc.SetState(DEFAULT);
		} else if (sc.state == IDENTIFIER) {
			if (!IsAWordChar(sc.ch) || (sc.currentPos+1 == startPos + length)) {
				if (IsAWordChar(sc.ch)) {
					sc.Forward(); // Checks words at the end of the document.
				}
				char s[100];
				sc.GetCurrent(s, sizeof(s));
				if (keywords0.InList(s)) {
					sc.ChangeState(WORD0);
				} else if (keywords1.InList(s)) {
					sc.ChangeState(WORD1);
				} else if (keywords2.InList(s)) {
					sc.ChangeState(WORD2);
				} else if (keywords3.InList(s)) {
					sc.ChangeState(WORD3);
				} else if (keywords4.InList(s)) {
					sc.ChangeState(WORD4);
				} else if (keywords5.InList(s)) {
					sc.ChangeState(WORD5);
				} else if (keywords6.InList(s)) {
					sc.ChangeState(WORD6);
				} else if (keywords7.InList(s)) {
					sc.ChangeState(WORD7);
				} else if (keywords8.InList(s)) {
					sc.ChangeState(WORD8);
				}
				sc.SetState(DEFAULT);
			}
		} else if (sc.state == PREPROCESSOR) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\n' || sc.chNext == '\r' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\n' || sc.ch == '\r') {
				sc.ForwardSetState(DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(PREPROCEOL);
				sc.ForwardSetState(DEFAULT);
			}
		} else if (sc.state == COMMENTLINE) {
			if (sc.atLineEnd) {
				sc.ForwardSetState(DEFAULT);
			}
		} else if (sc.state == STRING) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\"' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\"') {
				sc.ForwardSetState(DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(STRINGEOL);
				sc.ForwardSetState(DEFAULT);
			}
		} else if (sc.state == CHARACTER) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\'' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\'') {
				sc.ForwardSetState(DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(STRINGEOL);
				sc.ForwardSetState(DEFAULT);
			}
		} else if (sc.state == COMMENTBLOCK || sc.state == COMMENTDOC) {
			if (sc.Match('*', '/')) {
				sc.Forward();
				sc.ForwardSetState(DEFAULT);
			}
		} else {
			sc.SetState(DEFAULT);
		}

		// Determine if a new state should be entered.
		if (sc.state == DEFAULT || sc.state == PREPROCESSOR) {
			if (sc.state != PREPROCESSOR) {
				if (IsADigit(sc.ch) || (sc.ch == '.' && IsADigit(sc.chNext))) {
					sc.SetState(NUMBER);
					if (sc.ch == '0' && (sc.chNext == 'x' || sc.chNext == 'X')) {
						int j = 1;
						while (IsADigit(sc.GetRelative(j+1), 16)) {
							j++;
						}
						sc.Forward(j);
					} else {
						int j = 0;
						bool hitDot(false);
						bool hitE(false);
						int ch = sc.ch;
						int chNext = sc.chNext;
						while (IsADigit(ch) || (ch == '.' && !hitDot && !hitE) ||
								((ch == 'e' || ch == 'E') && !hitE && (IsADigit(chNext) ||
								((chNext == '-' || chNext == '+') && IsADigit(sc.GetRelative(j+2)))))) {
							if (ch == '.') {
								hitDot = true;
							} else if (ch == 'e' || ch == 'E') {
								hitE = true;
								j++;
								if (chNext == '-' || chNext == '+') {
									j++;
								}
								chNext = sc.GetRelative(j+1);
							}
							j++;
							ch = chNext;
							chNext = sc.GetRelative(j+1);
						}
						sc.Forward(j-1);
					}
				} else if (sc.ch == ':' || sc.ch == '.') {
					sc.SetState(OPERATOR);
					if (IsAWordChar(sc.chNext) || isspace(sc.chNext)) {
						int j = 1;
						int ch = sc.chNext;
						while((sc.currentPos + j < startPos + length) && isspace(ch)) {
							ch = sc.GetRelative(++j);
						}
						if (IsAWordChar(ch)) {
							int i = 0;
							char s[101];
							while (i < 100 && IsAWordChar(ch)) {
								s[i++] = ch;
								ch = sc.GetRelative(++j);
							}
							s[i++] = '\0';
							if (keywords7.InList(s)) {
								sc.Forward(j-i+1);
								sc.SetState(WORD7);
							}
							if (sc.state != OPERATOR) {
								sc.Forward(i-2);
							}
						}
					}
				} else if (IsAWordChar(sc.ch)) {
					sc.SetState(IDENTIFIER);
					int j = 0;
					int i = 0;
					char s[101];
					bool finished = false;
					int ch = sc.ch;

					while (i < 100 && (sc.currentPos + j < startPos + length) && !finished &&
							(IsAWordChar(ch) || isspace(ch) || (ch == '.' || ch == ':'))) {
						if (!IsAWordChar(ch)) {
							while((sc.currentPos + j < startPos + length) && isspace(ch)) {
								ch = sc.GetRelative(++j);
							}
							if (i < 100 && (ch == '.' || ch == ':')) {
								s[i++] = ch;
								ch = sc.GetRelative(++j);
								while((sc.currentPos + j < startPos + length) && isspace(ch)) {
									ch = sc.GetRelative(++j);
								}
								while(i < 100 && IsAWordChar(ch)) {
									s[i++] = ch;
									ch = sc.GetRelative(++j);
								}
								s[i++] = '\0';
								if (sc.state != IDENTIFIER) {
									sc.Forward(j-1);
								}
							}

							finished = true;
						} else {
							s[i++] = ch;
							ch = sc.GetRelative(++j);
						}
					}
				} else if (sc.ch == '\"') {
					sc.SetState(STRING);
				} else if (sc.ch == '\'') {
					sc.SetState(CHARACTER);
				} else if (sc.Match('/', '/')) {
					sc.SetState(COMMENTLINE);
					sc.Forward();
				} else if (sc.Match("/**")) {
					sc.SetState(COMMENTDOC);
					sc.Forward();
				} else if (sc.Match('/', '*')) {
					sc.SetState(COMMENTBLOCK);
					sc.Forward();
				} else if (sc.Match('#')) {
					sc.SetState(PREPROCESSOR);
					sc.Forward();
				} else if (IsCOperator(sc.ch)) {
					sc.SetState(OPERATOR);
				}
			} else if (sc.Match('/', '/')) {
				sc.SetState(COMMENTLINE);
				sc.Forward();
			} else if (sc.Match("/**")) {
				sc.SetState(COMMENTDOC);
				sc.Forward();
			} else if (sc.Match('/', '*')) {
				sc.SetState(COMMENTBLOCK);
				sc.Forward();
			}
		}
	}
	sc.Complete();
}

//  <--- Fold --->
void N64C::Fold_Doc(unsigned int startPos, int length, int initStyle, Accessor &styler)
{
	unsigned int lengthDoc = startPos + length;
	int lineCurrent = styler.GetLine(startPos);
	int levelCurrent = SC_FOLDLEVELBASE;
	if (lineCurrent > 1)
		levelCurrent = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
	else
		styler.SetLevel(lineCurrent, SC_FOLDLEVELBASE);
	int levelNext = levelCurrent;
	char chNext = styler[startPos];
	int styleNext = styler.StyleAt(startPos);
	int style = initStyle;
	char s[10];

	for (unsigned int i = startPos; i < lengthDoc; i++) {
		char ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);
		int stylePrev = style;
		style = styleNext;
		styleNext = styler.StyleAt(i + 1);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (style == PREPROCESSOR) {
			char chNext2 = styler.SafeGetCharAt(i + 2);
			char chNext3 = styler.SafeGetCharAt(i + 3);
			if (ch == '#' && chNext == 'i' && chNext2 == 'f') {
				levelNext++;
			} else if (ch == '#' && chNext == 'e' && chNext2 == 'n' && chNext3 == 'd') {
				levelNext--;
			}
		} else if (style == OPERATOR) {
			if (ch == '{'){
				levelNext++;
			} else if (ch == '}'){
				levelNext--;
			}
		} else if ((style == COMMENTBLOCK || style == COMMENTDOC) &&
				!(stylePrev == COMMENTBLOCK || stylePrev == COMMENTDOC) &&
				(ch == '/')) {
			levelNext++;
		} else if ((style == COMMENTBLOCK || style == COMMENTDOC) &&
				!(styleNext == COMMENTBLOCK || styleNext == COMMENTDOC) &&
				(ch == '/')) {
			levelNext--;
		} else if (style == COMMENTLINE) {
			if ((ch == '/' && chNext == '/')) {
				char chNext2 = styler.SafeGetCharAt(i + 2);
				if (chNext2 == '{') {
					levelNext++;
				} else if (chNext2 == '}') {
					levelNext--;
				}
			}
		}

		if (atEOL || (i == lengthDoc-1)) {
			int lev = levelNext;
			int levCur = levelCurrent;
			if (levelNext > levelCurrent) {
				levCur |= SC_FOLDLEVELHEADERFLAG;
			}
			styler.SetLevel(lineCurrent, levCur);
			styler.SetLevel(lineCurrent+1, lev);
			lineCurrent++;
			levelCurrent = levelNext;
		}
	}
}
