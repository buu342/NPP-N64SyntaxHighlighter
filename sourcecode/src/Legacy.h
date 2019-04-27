#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SCI_NAMESPACE

#include "OldPlatform.h"

#include "OldPropSet.h"
#include "OldAccessor.h"
#include "OldWindowAccessor.h"

#include "OldPropSetSimple.h"
#include "OldKeyWords.h"
#include "OldStyleContext.h"
#include "OldScintilla.h"

using namespace Scintilla;

const int DEFAULT = 0;

const int LUA_COMMENT = 1;
const int CPP_COMMENT = 2;
const int LUA_COMMENTLINE = 3;
const int CPP_COMMENTLINE = 4;

const int NUMBER = 5;
const int OPERATOR = 6;
const int IDENTIFIER = 7;

const int STRING = 8;
const int CHARACTER = 9;
const int STRINGEOL = 10;
const int LITERALSTRING = 11;

const int WORD0 = 20;
const int WORD1 = 21;
const int WORD2 = 22;
const int WORD3 = 23;
const int WORD4 = 24;
const int WORD5 = 25;
const int WORD6 = 26;
const int WORD7 = 27;
const int WORD8 = 28;

void Colourise_Doc(unsigned int startPos, unsigned int length, int initStyle, WordList *keywordlists[], Accessor &styler);
void Fold_Doc(unsigned int startPos, unsigned int length, int initStyle, WordList *[], Accessor &styler);
