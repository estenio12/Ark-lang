#pragma once
#include <string>

namespace Ark
{
    namespace KEYWORDS
    {
        constexpr std::string_view TI8        = "i8";
        constexpr std::string_view TI16       = "i16";
        constexpr std::string_view TI32       = "i32";
        constexpr std::string_view TI64       = "i64";
        constexpr std::string_view TUI8       = "ui8";
        constexpr std::string_view TUI16      = "ui16";
        constexpr std::string_view TUI32      = "ui32";
        constexpr std::string_view TUI64      = "ui64";
        constexpr std::string_view TF32       = "f32";
        constexpr std::string_view TF64       = "f64";
        constexpr std::string_view TBOOL      = "bool";
        constexpr std::string_view TVOID      = "void";
        constexpr std::string_view TSTRING    = "string";
        constexpr std::string_view TRETURN    = "return";
        constexpr std::string_view TINTERNAL  = "internal";
        constexpr std::string_view TVAR       = "var";
        constexpr std::string_view Tconstexpr = "constexpr";
        constexpr std::string_view TEND       = "end";
        constexpr std::string_view TEXTENDS   = "extends";
        constexpr std::string_view TFUN       = "fun";
        constexpr std::string_view TMODULE    = "module";
        constexpr std::string_view TIMPORT    = "import";
        constexpr std::string_view TIF        = "if";
        constexpr std::string_view TELIF      = "elif";
        constexpr std::string_view TELSE      = "else";
        constexpr std::string_view TFOR       = "for";
        constexpr std::string_view TWHILE     = "while";
        constexpr std::string_view TENUM      = "enum";
        constexpr std::string_view TIN        = "in";
        constexpr std::string_view TOF        = "of";
        constexpr std::string_view TEXTERN    = "extern";
        constexpr std::string_view TTRUE      = "true";
        constexpr std::string_view TFALSE     = "false";
        constexpr std::string_view TSTRUCT    = "struct";
    }

    namespace OP_ARITHMETIC
    {
        constexpr std::string_view ADD    = "+";
        constexpr std::string_view SUB    = "-";
        constexpr std::string_view MULT   = "*";
        constexpr std::string_view DIV    = "/";
        constexpr std::string_view ADDEQ  = "+=";
        constexpr std::string_view SUBEQ  = "-=";
        constexpr std::string_view MULTEQ = "*=";
        constexpr std::string_view DIVEQ  = "/=";
    }

    namespace OP_LOGICAL
    {
        constexpr std::string_view OR  = "||";
        constexpr std::string_view AND = "&&";
        constexpr std::string_view NOT = "!";
    }

    namespace OP_COMPARISON
    {
        constexpr std::string_view EQ   = "==";
        constexpr std::string_view DIFF = "!=";
        constexpr std::string_view GT   = ">";
        constexpr std::string_view LT   = "<";
        constexpr std::string_view GTEQ = ">=";
        constexpr std::string_view LTEQ = "<=";
    }

    namespace OP_BITWISE
    {
        constexpr std::string_view OR     = "|";
        constexpr std::string_view AND    = "&";
        constexpr std::string_view XOR    = "^";
        constexpr std::string_view NOT    = "~";
        constexpr std::string_view LSHIFT = "<<";
        constexpr std::string_view RSHIFT = ">>";
    }

    namespace DELIMITER
    {
        constexpr std::string_view COMMA     = ",";
        constexpr std::string_view COLON     = ":";
        constexpr std::string_view LPARAN    = "(";
        constexpr std::string_view RPARAN    = ")";
        constexpr std::string_view LBRACE    = "{";
        constexpr std::string_view RBRACE    = "}";
        constexpr std::string_view LBRACKET  = "[";
        constexpr std::string_view RBRACKET  = "]";
        constexpr std::string_view SEMICOLON = ";";
        constexpr std::string_view ARROW     = "->";
        constexpr std::string_view SCOPEACCESS = "::";
    }

    namespace AST_DEF
    {
        enum TYPE
        {
            LITERAL_INT,
            LITERAL_FLOAT,
            LITERAL_BOOL,
            LITERAL_CHAR,
            LITERAL_STRING,
            LITERAL_VALUE,
            TYPE_NATIVE,
            IDENTIFIER,
            TYPE_ARRAY,
            TYPE_LIST,
            CALL_FUNCTION,
            OP_ARITHMETIC,
            OP_LOGICAL,
            OP_COMPARISON,
            OP_BITWISE,
            ARGUMENT_LIST,
            CONDITION_STMT,
            INTERATOR,
            ENUM_ITEM,
            ENUM_ITEM_LIST,
            ENUM_BLOCK_SCOPE,
            ENUM_DECLARATION,
            FOR_STMT,
            WHILE_STMT,
            LOOP_DECLARATION,
            RETURN_STMT,
            STATEMENT,
            BLOCK_SCOPE,
            EXPRESSION,
            VARIABLE_DECLARATION,
            constexprANT_DECLARATION,
            ACCESS_MODIFIER,
            DATA_DECLARATION,
            GENERIC_PARAM,
            GENERIC_ARGS,
            STRUCT_VALUE_ASSIGNMENT,
            STRUCT_VALUE_ASSIGN_LIST,
            STRUCT_INSTANCE,
            STRUCT_BLOCK_SCOPE,
            STRUCT_INHERIT,
            STRUCT_DECLARATION,
            PARAMETER,
            PARAMETER_LIST,
            TYPE_IDENTIFER,
            TYPE_FUNCTION,
            FUNCTION_DECLARATION,
            ANONIMOUS_FUNCTION,
            EXTERN_DECLARATION,
            MODULE_DECLARATION,
            IMPORT_DECLARATION
        };
    }
}