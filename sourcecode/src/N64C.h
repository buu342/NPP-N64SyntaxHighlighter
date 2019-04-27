#ifndef N64C_H
#define N64C_H

// Notepad++ includes
#include "precompiledHeaders.h"
#include "PluginInterface.h"

// Scintilla includes
#include "ILexer.h"
#include "PropSetSimple.h"
#include "WordList.h"
#include "LexerBase.h"

#include "LexAccessor.h"
#include "Accessor.h"
#include "LexerModule.h"
#include "StyleContext.h"
#include "CharacterSet.h"

#ifdef UNICODE
	#define generic_strncpy_s wcsncpy_s
#else
	#define generic_strncpy_s strncpy_s
#endif

namespace N64C
{
	static const generic_string PLUGIN_NAME = TEXT("&N64 C Lexer");
	static const std::string LEXER_NAME = "C (N64)";
	static const generic_string LEXER_STATUS_TEXT = TEXT("Nintendo64 C source file");

	static const int numPluginMenuItems = 1;
	FuncItem pluginMenuItems[numPluginMenuItems];
	static const generic_string aboutMenuItem = TEXT("&About");
	void aboutDlg();

	NppData nppData;

	const int DEFAULT = 0;

	const int PREPROCESSOR = 1;
	const int COMMENTBLOCK = 2;
	const int COMMENTLINE = 3;
	const int COMMENTDOC = 4; // TODO: Implement this ( /** )

	const int NUMBER = 5;
	const int OPERATOR = 6;
	const int IDENTIFIER = 9;

	const int STRING = 7;
	const int CHARACTER = 8;
	const int STRINGEOL = 10;

	const int WORD0 = 20;
	const int WORD1 = 21;
	const int WORD2 = 22;
	const int WORD3 = 23;
	const int WORD4 = 24;
	const int WORD5 = 25;
	const int WORD6 = 26;
	const int WORD7 = 27;
	const int WORD8 = 28;

	class LexerN64C : public LexerBase {
	public:
		LexerN64C() {}

		void SCI_METHOD Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess);
		void SCI_METHOD Fold(unsigned int startPos, int length, int initStyle, IDocument *pAccess);

		static ILexer *LexerFactory() {
			try {
				return new LexerN64C();
			} catch (...) {
				// Should not throw into caller as may be compiled with different compiler or options
				return 0;
			}
		}
	};
	
	void Colourise_Doc(unsigned int startPos, int length, int initStyle, WordList *keywordlists[], Accessor &styler);
	void Fold_Doc(unsigned int startPos, int length, int initStyle, Accessor &styler);
};

#endif
