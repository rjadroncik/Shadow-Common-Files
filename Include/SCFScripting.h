#pragma once

//Reserved class keys: 0x0301 - 0x0350

#include <SCFStandard.h>

#define SCF_SCRIPTING_1

/*
#include "../Source/Scripting/Runtime/Assembler.h"
#include "../Source/Scripting/Runtime/Executable.h"

#include "../Source/Scripting/Runtime/Language.h"
#include "../Source/Scripting/Runtime/Compiler.h"
*/

#include "../Source/Scripting/Description/Argument.h"
#include "../Source/Scripting/Description/MethodSignature.h"
#include "../Source/Scripting/Description/Method.h"
#include "../Source/Scripting/Description/Property.h"

#include "../Source/Scripting/Description/Enum.h"
#include "../Source/Scripting/Description/Interface.h"
#include "../Source/Scripting/Description/Class.h"

#include "../Source/Scripting/Description/Package.h"


#include "../Source/Scripting/Compiler/Scanner/TokenKeyword.h"
#include "../Source/Scripting/Compiler/Scanner/TokenIdentifier.h"
#include "../Source/Scripting/Compiler/Scanner/TokenOperator.h"

#include "../Source/Scripting/Compiler/Scanner/TokenChar.h"
#include "../Source/Scripting/Compiler/Scanner/TokenString.h"
#include "../Source/Scripting/Compiler/Scanner/TokenNumber.h"
#include "../Source/Scripting/Compiler/Scanner/TokenType.h"

#include "../Source/Scripting/Compiler/Scanner.h"

#include "../Source/Scripting/Compiler/Parser.h"
#include "../Source/Scripting/Compiler/Compiler.h"
