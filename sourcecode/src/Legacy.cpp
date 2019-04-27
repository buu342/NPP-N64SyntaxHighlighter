#include "Legacy.h"

void LexOrFold(bool foldOrLex, unsigned int startPos, unsigned int length, int initStyle,
                  char *words[], WindowID window, char *props)
{
	// Create and initialize the WindowAccessor (including contained PropSet)
	PropSetSimple ps;
	ps.SetMultiple(props);
	WindowAccessor wa(window, ps);

	//  Create and initialize WordList(s).
	//  If you have an extremely large word file, or lots of styling rules you may want to speed
	//  up processing by storing the wordlists instead of reprocessing them on each call.
	int nWL = 0;
	for (; words[nWL]; nWL++) ;	// count # of WordList PTRs needed
	WordList** wl = new WordList* [nWL + 1];// alloc WordList PTRs
	int i = 0;
	for (; i < nWL; i++) {
		wl[i] = new WordList();	// (works or THROWS bad_alloc EXCEPTION)
		wl[i]->Set(words[i]);
	}
	wl[i] = 0;

	//  Call the internal folding and styling functions.
	// foldOrLex is false for lex and true for fold
	if (foldOrLex) {

		// This is a nice helpful routine to back up a line to fix broken folds.
		int lineCurrent = wa.GetLine(startPos);
		if (lineCurrent > 0) {
			lineCurrent--;
			int newStartPos = wa.LineStart(lineCurrent);
			length += startPos - newStartPos;
			startPos = newStartPos;
			initStyle = 0;
			if (startPos > 0) {
				initStyle = wa.StyleAt(startPos - 1);
			}
		}

		Fold_Doc(startPos, length, initStyle, wl, wa);

	}
	else {

		//  You may want to put a routine here to backtrack past leaking styles, typically
		//  multiline styles, or just put such logic in the Colour_Doc function itself.  Just
		//  be sure to do it prior to creating your Style Context.

		Colourise_Doc(startPos, length, initStyle, wl, wa);

	}

	//  The flush function is what actually finalizes settings the styles you just coloured.
	wa.Flush();

	// Clean up the wordlists before leaving.
	for (i = nWL - 1; i >= 0; i--)
		delete wl[i];
	delete [] wl;
}

void __stdcall NppLex(unsigned int lexer, unsigned int startPos, int length, int initStyle,
              char *words[], WindowID window, char *props)
{
	try
	{
		LexOrFold(0, startPos, length, initStyle, words, window, props);
	} catch (...) {
	}
}
void __stdcall NppFold(unsigned int lexer, unsigned int startPos, int length, int initStyle,
              char *words[], WindowID window, char *props)
{
	try
	{
		LexOrFold(1, startPos, length, initStyle, words, window, props);
	} catch (...) {
	}
}

// Extended to accept accented characters
static inline bool IsAWordChar(int ch) {
	return ch >= 0x80 ||
	       (isalnum(ch) || ch == '_');
}

static inline bool IsLuaOperator(int ch) {
	if (ch >= 0x80 || isalnum(ch)) {
		return false;
	}

	if (ch == '*' || ch == '/' || ch == '-' || ch == '+' ||
		ch == '(' || ch == ')' || ch == '=' ||
		ch == '{' || ch == '}' || ch == '~' ||
		/*ch == '[' ||*/ ch == ']' || ch == ';' ||
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
void Colourise_Doc(unsigned int startPos, unsigned int length, int initStyle, WordList *keywordlists[], Accessor &styler)
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
	if (initStyle == LITERALSTRING || initStyle == LUA_COMMENT) {
		sepCount = styler.GetLineState(currentLine - 1) & 0xFF;
	}

	// Do not leak onto next line
	if (initStyle == STRINGEOL || initStyle == LUA_COMMENTLINE || initStyle == CPP_COMMENTLINE) {
		initStyle = DEFAULT;
	}

	StyleContext sc(startPos, length, initStyle, styler);
	for (; sc.More(); sc.Forward()) {
		if (sc.atLineEnd) {
			// Update the line state, so it can be seen by next line
			currentLine = styler.GetLine(sc.currentPos);
			switch (sc.state) {
			case LITERALSTRING:
			case LUA_COMMENT:
				// Inside a literal string or block comment, we set the line state
				styler.SetLineState(currentLine, sepCount);
				break;
			default:
				// Reset the line state
				styler.SetLineState(currentLine, 0);
				break;
			}
		}

		// Handle string line continuation
		if ((sc.state == STRING || sc.state == CHARACTER) &&
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
				} else if (keywords5.InList(s)) {
					sc.ChangeState(WORD5);
				} else if (keywords8.InList(s)) {
					sc.ChangeState(WORD8);
				}
				sc.SetState(DEFAULT);
			}
		} else if (sc.state == LUA_COMMENTLINE || sc.state == CPP_COMMENTLINE) {
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
		} else if (sc.state == LITERALSTRING || sc.state == LUA_COMMENT) {
			if (sc.ch == ']') {
				int sep = LongDelimCheck(sc);
				if (sep == sepCount) {
					sc.Forward(sep);
					sc.ForwardSetState(DEFAULT);
				}
			}
		} else if (sc.state == CPP_COMMENT) {
			if (sc.Match('*', '/')) {
				sc.Forward();
				sc.ForwardSetState(DEFAULT);
			}
		} else {
			sc.SetState(DEFAULT);
		}

		// Determine if a new state should be entered.
		if (sc.state == DEFAULT) {
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
							if (keywords3.InList(s)) {
								sc.ChangeState(WORD3);
							} else if (keywords4.InList(s)) {
								sc.ChangeState(WORD4);
							} else if (keywords6.InList(s)) {
								sc.ChangeState(WORD6);
							}
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
			} else if (sc.ch == '[') {
				sepCount = LongDelimCheck(sc);
				if (sepCount == 0) {
					sc.SetState(OPERATOR);
				} else {
					sc.SetState(LITERALSTRING);
					sc.Forward(sepCount);
				}
			} else if (sc.Match('-', '-')) {
				sc.SetState(LUA_COMMENTLINE);
				if (sc.Match("--[")) {
					sc.Forward(2);
					sepCount = LongDelimCheck(sc);
					if (sepCount > 0) {
						sc.ChangeState(LUA_COMMENT);
						sc.Forward(sepCount);
					}
				} else {
					sc.Forward();
				}
			} else if (sc.Match('/', '/')) {
				sc.SetState(CPP_COMMENTLINE);
				sc.Forward();
			} else if (sc.Match('/', '*')) {
				sc.SetState(CPP_COMMENT);
				sc.Forward();
			} else if (IsLuaOperator(sc.ch)) {
				sc.SetState(OPERATOR);
			}
		}
	}
	sc.Complete();
}

//  <--- Fold --->
void Fold_Doc(unsigned int startPos, unsigned int length, int initStyle, WordList *[], Accessor &styler)
{
	unsigned int lengthDoc = startPos + length;
	int visibleChars = 0;
	int lineCurrent = styler.GetLine(startPos);
	int levelPrev = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
	int levelCurrent = levelPrev;
	char chNext = styler[startPos];
	bool foldCompact = styler.GetPropertyInt("fold.compact", 1) != 0;
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
		if (style == WORD0 && stylePrev != WORD0) {
			if (ch == 'i' || ch == 'd' || ch == 'f' || ch == 'e' || ch == 'r' || ch == 'u') {
				unsigned int j;
				for (j = 0; j < 8 && iswordchar(styler.SafeGetCharAt(i + j)); j++) {
					s[j] = styler[i + j];
				}
				s[j] = '\0';

				if ((strcmp(s, "if") == 0) || (strcmp(s, "do") == 0) || (strcmp(s, "function") == 0) || (strcmp(s, "repeat") == 0)) {
					levelCurrent++;
				}
				if ((strcmp(s, "end") == 0) || (strcmp(s, "until") == 0)) {
					levelCurrent--;
				}
			}
		} else if (style == OPERATOR) {
			if (ch == '{'){
				levelCurrent++;
			} else if (ch == '}'){
				levelCurrent--;
			}
		} else if ((style == LITERALSTRING || style == LUA_COMMENT || style == CPP_COMMENT) &&
				!(stylePrev == LITERALSTRING || stylePrev == LUA_COMMENT || stylePrev == CPP_COMMENT) &&
				(ch == '[' || ch == '/' || ch == '-')) {
			levelCurrent++;
		} else if ((style == LITERALSTRING || style == LUA_COMMENT || style == CPP_COMMENT) &&
				!(styleNext == LITERALSTRING || styleNext == LUA_COMMENT || styleNext == CPP_COMMENT) &&
				(ch == ']' || ch == '/')) {
			levelCurrent--;
		} else if (style == CPP_COMMENTLINE || style == LUA_COMMENTLINE) {
			if ((ch == '/' && chNext == '/') || (ch == '-' && chNext == '-')) {
				char chNext2 = styler.SafeGetCharAt(i + 2);
				if (chNext2 == '{') {
					levelCurrent++;
				} else if (chNext2 == '}') {
					levelCurrent--;
				}
			}
		}

		if (!isspacechar(ch)) {
			visibleChars++;
		}
		if (atEOL || (i == lengthDoc-1)) {
			int lev = levelPrev;
			if (visibleChars == 0 && foldCompact) {
				lev |= SC_FOLDLEVELWHITEFLAG;
			}
			if (levelCurrent > levelPrev) {
				lev |= SC_FOLDLEVELHEADERFLAG;
			}
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelPrev = levelCurrent;
			visibleChars = 0;
		}
	}
	char lastChar = styler.SafeGetCharAt(lengthDoc-1);
	if ((unsigned)styler.Length() == lengthDoc && (lastChar == '\n' || lastChar == '\r')) {
		styler.SetLevel(lineCurrent, levelCurrent);
	}
}