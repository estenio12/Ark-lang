/*
 * Copyright 2026 Ark-lang Authors
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "Definitions.hpp"
#include "TokenManager.hpp"

namespace Ark::Ast
{
    class AstNode;
    class AstBranch;
    class ConstantDeclaration;
    class VariableDeclaration;
    class StructDeclaration;
    class ModuleDeclaration;
    class ImportDeclaration;
    class ExternDeclaration;
    class FunctionDeclaration;
    class AnonimousFunction;
    class TypeFunction;
    class TypeIdentifier;
    class ParameterList;
    class Parameter;
    class StructDeclaration;
    class StructInherit;
    class StructBlockScope;
    class StructInstance;
    class StructValueAssignList;
    class StructValueAssignment;
    class GenericArgs;
    class GenericParams;
    class DataDeclaration;
    class AccessModifier;
    class Expression;
    class BlockScope;
    class Statement;
    class ReturnStmt;
    class LoopDeclartion;
    class WhileStmt;
    class ForStmt;
    class EnumDeclaration;
    class EnumBlockScope;
    class EnumItemList;
    class EnumItem;
    class Interator;
    class ConditionStmt;
    class ArgumentList;
    class CallFunction;
    class TypedList;
    class TypedArray;
    class Identifier;
    class TypeNative;
    class LiteralValue;
    class LiteralString;
    class LiteralChar;
    class LiteralBool;
    class LiteralFloat;
    class LiteralInt;
    class BinOp;
    class UnaryOp;
    class AssignmentStmt;
    class LoopBlockScope;

    enum class TYPE : uint8_t
    {
        LITERAL_INT, LITERAL_FLOAT, LITERAL_BOOL, LITERAL_CHAR, LITERAL_STRING, LITERAL_VALUE,
        TYPE_NATIVE, IDENTIFIER, TYPE_ARRAY, TYPE_LIST, CALL_FUNCTION, ARGUMENT_LIST, CONDITION_STMT, INTERATOR,
        ENUM_ITEM, ENUM_ITEM_LIST, ENUM_BLOCK_SCOPE, ENUM_DECLARATION, FOR_STMT, WHILE_STMT,
        LOOP_DECLARATION, RETURN_STMT, STATEMENT, BLOCK_SCOPE, EXPRESSION, VARIABLE_DECLARATION,
        CONSTANT_DECLARATION, ACCESS_MODIFIER, DATA_DECLARATION, GENERIC_PARAM, GENERIC_ARGS, STRUCT_VALUE_ASSIGNMENT,
        STRUCT_VALUE_ASSIGN_LIST, STRUCT_INSTANCE, STRUCT_BLOCK_SCOPE, STRUCT_INHERIT, STRUCT_DECLARATION, PARAMETER,
        PARAMETER_LIST, TYPE_IDENTIFER, TYPE_FUNCTION, FUNCTION_DECLARATION, ANONIMOUS_FUNCTION,
        EXTERN_DECLARATION, MODULE_DECLARATION, IMPORT_DECLARATION, BIN_OP, UNARY_OP, UNKNOW,
        ASSIGNMENT_STMT, OP_ASSIGNMENT, LOOP_BLOCK_SCOPE
    };

    class AstNode
    {
        public:
            std::string source_file;
            TYPE type = TYPE::UNKNOW;
            uint64_t line = 0;
            uint64_t col = 0;

            virtual std::string Dump(int ident) = 0;

        private:
            int ident_factor = 4;
    };

    class AstBranch
    {
        public:
            std::vector<std::unique_ptr<AstNode>> branches;

        public:
            AstBranch() = default;
    };

    class LiteralInt : public AstNode
    {
        public:
            int value = 0;
            LiteralInt() { type = TYPE::LITERAL_INT; }
    };

    class LiteralFloat : public AstNode
    {
        public:
            float value = 0.f;
            LiteralFloat() { type = TYPE::LITERAL_FLOAT; }
    };

    class LiteralBool : public AstNode
    {
        public:
            bool value = false;
            LiteralBool() { type = TYPE::LITERAL_BOOL; }
    };

    class LiteralChar : public AstNode
    {
        public:
            char value = '\0';
            LiteralChar() { type = TYPE::LITERAL_CHAR; }
    };

    class LiteralString : public AstNode
    {
        public:
            std::string value = "";
            LiteralString() { type = TYPE::LITERAL_STRING; }
    };

    class TypeIdentifier : public AstNode
    {
        TypeIdentifier() { type = TYPE::TYPE_IDENTIFER; }

        std::unique_ptr<AstNode> base_type;
        std::unique_ptr<GenericArgs> generic_args;

        bool is_function_type = false;
        bool is_array_type = false;
    };

    class TypeArray : public AstNode
    {
        public:
            TypeArray() { type = TYPE::TYPE_ARRAY; }

            std::unique_ptr<TypeIdentifier> element_type;
            uint32_t array_size = 0;
    };

    class Identifier : public AstNode
    {
        public:
            std::string value = "";
            Identifier() { type = TYPE::IDENTIFIER; }
    };

    class VariableDeclaration : public AstNode
    {
        public:
            VariableDeclaration() { type = TYPE::VARIABLE_DECLARATION; }

            std::string identifier;
            std::unique_ptr<TypeIdentifier> type_def;
            std::unique_ptr<Expression> value;
    };

    class ConstantDeclaration : public AstNode
    {
        public:
            ConstantDeclaration() { type = TYPE::CONSTANT_DECLARATION; }

            std::string identifier;
            std::unique_ptr<TypeIdentifier> type_def;
            std::unique_ptr<Expression> value;
    };

    class FunctionDeclaration : public AstNode 
    {
        public:
            FunctionDeclaration() { type = TYPE::FUNCTION_DECLARATION; }

            std::string identifier;
            std::unique_ptr<ParameterList> parameters;
            std::unique_ptr<TypeIdentifier> type_def;
            std::unique_ptr<BlockScope> body;
            bool is_internal = false;
    };

    class TypeList : public AstNode
    {
        public:
            TypeList() { type = TYPE::TYPE_LIST; }

            std::vector<std::unique_ptr<TypeIdentifier>> type_list;
    };

    class CallFunction : public AstNode
    {
        public:
            CallFunction() { type = TYPE::CALL_FUNCTION; }

            std::unique_ptr<Identifier> identifier;
            std::unique_ptr<GenericArgs> generic_args;
            std::unique_ptr<ArgumentList> args;
    };

    class BinOp : public AstNode
    {
        public:
            BinOp() { type = TYPE::BIN_OP; }

            Ark::Token oper;
            uint8_t priority = 0;

            std::unique_ptr<AstNode> left;
            std::unique_ptr<AstNode> right;
    };

    class GenericArgs : public AstNode
    {
        public: 
            GenericArgs() { type = TYPE::GENERIC_ARGS; }

            std::vector<std::unique_ptr<TypeIdentifier>> arg_list;
    };

    class GenericParam : public AstNode
    {
        public: 
            GenericParam() { type = TYPE::GENERIC_PARAM; }

            std::vector<std::unique_ptr<Identifier>> param_list;
    };

    class UnaryOp : public AstNode
    {
        public: 
            UnaryOp() { type = TYPE::UNARY_OP; }

            std::string value;
    };

    class OpAssignment : public AstNode
    {
        public: 
            OpAssignment() { type = TYPE::OP_ASSIGNMENT; }

            std::string value;
    };

    class AssignmentStmt : public AstNode
    {
        public: 
            AssignmentStmt() { type = TYPE::ASSIGNMENT_STMT; }

            std::unique_ptr<Identifier> identifier;
            std::unique_ptr<OpAssignment> oper;
            std::unique_ptr<Expression> expression;
    };

    class ArgumentList : public AstNode
    {
        public: 
            ArgumentList() { type = TYPE::ARGUMENT_LIST; }

            std::vector<std::unique_ptr<Expression>> expressions;
    };

    struct IfBranch 
    {
        std::unique_ptr<Expression> condition;
        std::unique_ptr<BlockScope> body;
    };
    class ConditionStmt : public AstNode
    {
        public: 
            ConditionStmt() { type = TYPE::CONDITION_STMT; }

            std::unique_ptr<IfBranch> main_branch;
            std::vector<std::unique_ptr<IfBranch>> elif_branch;
            std::unique_ptr<BlockScope> else_body;
    };

    class Interator : public AstNode
    {
        public: 
            Interator() { type = TYPE::INTERATOR; }

            std::string value;
    };

    class EnumItem : public AstNode
    {
        public: 
            EnumItem() { type = TYPE::ENUM_ITEM; }

            std::vector<std::pair<std::string, std::unique_ptr<LiteralInt>>> items;
    };

    class EnumItemList : public AstNode
    {
        public: 
            EnumItemList() { type = TYPE::ENUM_ITEM_LIST; }

            std::vector<std::unique_ptr<EnumItem>> items_list;
    };

    class EnumBlockScope : public AstNode
    {
        public: 
            EnumBlockScope() { type = TYPE::ENUM_BLOCK_SCOPE; }

            std::unique_ptr<EnumItemList> body;
    };

    class EnumDeclaration : public AstNode
    {
        public: 
            EnumDeclaration() { type = TYPE::ENUM_DECLARATION; }

            std::unique_ptr<EnumItemList> body;
            bool is_internal = false;
    };

    class ForStmt : public AstNode
    {
        public: 
            ForStmt() { type = TYPE::FOR_STMT; }

            std::unique_ptr<ConstantDeclaration> value;
            std::unique_ptr<Interator> iterator;
            std::unique_ptr<TypeIdentifier> target;
    };

    class WhileStmt : public AstNode
    {
        public: 
            WhileStmt() { type = TYPE::WHILE_STMT; }

            std::unique_ptr<Expression> expression;
    };
    
    class LoopBlockScope: public AstNode
    {
        public:
            LoopBlockScope() { type = TYPE::LOOP_BLOCK_SCOPE; }

            std::unique_ptr<AstNode> statement;
    };
    
    class LoopDeclaration: public AstNode
    {
        public:
            LoopDeclaration() { type = TYPE::LOOP_DECLARATION; }

            std::unique_ptr<AstNode> loop_decl_stmt;
            std::unique_ptr<LoopBlockScope> body; 
    };
}

