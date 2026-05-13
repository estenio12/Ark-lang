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
    class Cast;

    class AstNode
    {
        public:
            std::string source_file;
            Ark::Ast::AstType type = Ark::Ast::AstType::UNKNOW;
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
            LiteralInt() { type = Ark::Ast::AstType::LITERAL_INT; }
    };

    class LiteralFloat : public AstNode
    {
        public:
            float value = 0.f;
            LiteralFloat() { type = Ark::Ast::AstType::LITERAL_FLOAT; }
    };

    class LiteralBool : public AstNode
    {
        public:
            bool value = false;
            LiteralBool() { type = Ark::Ast::AstType::LITERAL_BOOL; }
    };

    class LiteralChar : public AstNode
    {
        public:
            char value = '\0';
            LiteralChar() { type = Ark::Ast::AstType::LITERAL_CHAR; }
    };

    class LiteralString : public AstNode
    {
        public:
            std::string value = "";
            LiteralString() { type = Ark::Ast::AstType::LITERAL_STRING; }
    };

    class TypeIdentifier : public AstNode
    {
        TypeIdentifier() { type = Ark::Ast::AstType::TYPE_IDENTIFER; }

        std::unique_ptr<AstNode> base_type;
        std::unique_ptr<GenericArgs> generic_args;

        bool is_function_type = false;
        bool is_array_type = false;
    };

    class TypeArray : public AstNode
    {
        public:
            TypeArray() { type = Ark::Ast::AstType::TYPE_ARRAY; }

            std::unique_ptr<TypeIdentifier> element_type;
            uint32_t array_size = 0;
    };

    class Identifier : public AstNode
    {
        public:
            std::string value = "";
            Identifier() { type = Ark::Ast::AstType::IDENTIFIER; }
    };

    class VariableDeclaration : public AstNode
    {
        public:
            VariableDeclaration() { type = Ark::Ast::AstType::VARIABLE_DECLARATION; }

            std::string identifier;
            std::unique_ptr<TypeIdentifier> type_def;
            std::unique_ptr<Expression> value;
    };

    class ConstantDeclaration : public AstNode
    {
        public:
            ConstantDeclaration() { type = Ark::Ast::AstType::CONSTANT_DECLARATION; }

            std::string identifier;
            std::unique_ptr<TypeIdentifier> type_def;
            std::unique_ptr<Expression> value;
    };

    class FunctionDeclaration : public AstNode 
    {
        public:
            FunctionDeclaration() { type = Ark::Ast::AstType::FUNCTION_DECLARATION; }

            std::string identifier;
            std::unique_ptr<ParameterList> parameters;
            std::unique_ptr<TypeIdentifier> type_def;
            std::unique_ptr<BlockScope> body;
            bool is_internal = false;
    };

    class TypeList : public AstNode
    {
        public:
            TypeList() { type = Ark::Ast::AstType::TYPE_LIST; }

            std::vector<std::unique_ptr<TypeIdentifier>> type_list;
    };

    class CallFunction : public AstNode
    {
        public:
            CallFunction() { type = Ark::Ast::AstType::CALL_FUNCTION; }

            std::unique_ptr<Identifier> identifier;
            std::unique_ptr<GenericArgs> generic_args;
            std::unique_ptr<ArgumentList> args;
    };

    class BinOp : public AstNode
    {
        public:
            BinOp() { type = Ark::Ast::AstType::BIN_OP; }

            Ark::Token oper;
            uint8_t priority = 0;

            std::unique_ptr<AstNode> left;
            std::unique_ptr<AstNode> right;
    };

    class GenericArgs : public AstNode
    {
        public: 
            GenericArgs() { type = Ark::Ast::AstType::GENERIC_ARGS; }

            std::vector<std::unique_ptr<TypeIdentifier>> arg_list;
    };

    class GenericParam : public AstNode
    {
        public: 
            GenericParam() { type = Ark::Ast::AstType::GENERIC_PARAM; }

            std::vector<std::unique_ptr<Identifier>> param_list;
    };

    class UnaryOp : public AstNode
    {
        public: 
            UnaryOp() { type = Ark::Ast::AstType::UNARY_OP; }

            std::string value;
    };

    class OpAssignment : public AstNode
    {
        public: 
            OpAssignment() { type = Ark::Ast::AstType::OP_ASSIGNMENT; }

            std::string value;
    };

    class AssignmentStmt : public AstNode
    {
        public: 
            AssignmentStmt() { type = Ark::Ast::AstType::ASSIGNMENT_STMT; }

            std::unique_ptr<Identifier> identifier;
            std::unique_ptr<OpAssignment> oper;
            std::unique_ptr<Expression> expression;
    };

    class ArgumentList : public AstNode
    {
        public: 
            ArgumentList() { type = Ark::Ast::AstType::ARGUMENT_LIST; }

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
            ConditionStmt() { type = Ark::Ast::AstType::CONDITION_STMT; }

            std::unique_ptr<IfBranch> main_branch;
            std::vector<std::unique_ptr<IfBranch>> elif_branch;
            std::unique_ptr<BlockScope> else_body;
    };

    class Interator : public AstNode
    {
        public: 
            Interator() { type = Ark::Ast::AstType::INTERATOR; }

            std::string value;
    };

    class EnumItem : public AstNode
    {
        public: 
            EnumItem() { type = Ark::Ast::AstType::ENUM_ITEM; }

            std::vector<std::pair<std::string, std::unique_ptr<LiteralInt>>> items;
    };

    class EnumItemList : public AstNode
    {
        public: 
            EnumItemList() { type = Ark::Ast::AstType::ENUM_ITEM_LIST; }

            std::vector<std::unique_ptr<EnumItem>> items_list;
    };

    class EnumBlockScope : public AstNode
    {
        public: 
            EnumBlockScope() { type = Ark::Ast::AstType::ENUM_BLOCK_SCOPE; }

            std::unique_ptr<EnumItemList> body;
    };

    class EnumDeclaration : public AstNode
    {
        public: 
            EnumDeclaration() { type = Ark::Ast::AstType::ENUM_DECLARATION; }

            std::unique_ptr<EnumItemList> body;
            bool is_internal = false;
    };

    class ForStmt : public AstNode
    {
        public: 
            ForStmt() { type = Ark::Ast::AstType::FOR_STMT; }

            std::unique_ptr<ConstantDeclaration> value;
            std::unique_ptr<Interator> iterator;
            std::unique_ptr<TypeIdentifier> target;
    };

    class WhileStmt : public AstNode
    {
        public: 
            WhileStmt() { type = Ark::Ast::AstType::WHILE_STMT; }

            std::unique_ptr<Expression> expression;
    };
    
    class LoopBlockScope: public AstNode
    {
        public:
            LoopBlockScope() { type = Ark::Ast::AstType::LOOP_BLOCK_SCOPE; }

            std::unique_ptr<AstNode> statement;
    };
    
    class LoopDeclaration: public AstNode
    {
        public:
            LoopDeclaration() { type = Ark::Ast::AstType::LOOP_DECLARATION; }

            std::unique_ptr<AstNode> loop_decl_stmt;
            std::unique_ptr<LoopBlockScope> body; 
    };
    
    class ReturnStmt: public AstNode
    {
        public:
            ReturnStmt() { type = Ark::Ast::AstType::RETURN_STMT; }

            std::unique_ptr<Expression> loop_decl_stmt;
    };
    
    class Statement: public AstNode
    {
        public:
            Statement() { type = Ark::Ast::AstType::STATEMENT; }

            std::unique_ptr<AstNode> body;
    };
    
    class BlockScope: public AstNode
    {
        public:
            BlockScope() { type = Ark::Ast::AstType::BLOCK_SCOPE; }

            std::unique_ptr<Statement> body;
    };
    
    class Expression: public AstNode
    {
        public:
            Expression() { type = Ark::Ast::AstType::EXPRESSION; }

            std::unique_ptr<BinOp> value;
    };
    
    class DataDeclaration: public AstNode
    {
        public:
            DataDeclaration() { type = Ark::Ast::AstType::DATA_DECLARATION; }

            std::unique_ptr<AstNode> var_decl;
            std::unique_ptr<Expression> init_value;
            bool is_internal = false;
    };
    
    class StructValueAssignment: public AstNode
    {
        public:
            StructValueAssignment() { type = Ark::Ast::AstType::STRUCT_VALUE_ASSIGNMENT; }

            std::unique_ptr<Identifier> identifier;
            std::unique_ptr<LiteralValue> value;
            bool is_internal = false;
    };
    
    class StructValueAssignList: public AstNode
    {
        public:
            StructValueAssignList() { type = Ark::Ast::AstType::STRUCT_VALUE_ASSIGN_LIST; }

            std::vector<std::unique_ptr<StructValueAssignment>> assignments;
    };
    
    class StructInstace: public AstNode
    {
        public:
            StructInstace() { type = Ark::Ast::AstType::STRUCT_INSTANCE; }

            std::unique_ptr<Identifier> identificador;
            std::unique_ptr<StructValueAssignList> assignments;
    };
    
    class StructBlockScope: public AstNode
    {
        public:
            StructBlockScope() { type = Ark::Ast::AstType::STRUCT_BLOCK_SCOPE; }

            std::unique_ptr<DataDeclaration> value;
    };
    
    class StructInherit: public AstNode
    {
        public:
            StructInherit() { type = Ark::Ast::AstType::STRUCT_BLOCK_SCOPE; }

            std::unique_ptr<Identifier> identifier;
            bool has_priority = false;
    };
    
    class StructDeclaration: public AstNode
    {
        public:
            StructDeclaration() { type = Ark::Ast::AstType::STRUCT_DECLARATION; }

            std::unique_ptr<Identifier> identifier;
            std::unique_ptr<GenericParam> generics;
            std::vector<std::unique_ptr<StructInherit>> inheritances;
            std::unique_ptr<StructBlockScope> body;
            bool is_internal = false;
    };
    
    class Parameter: public AstNode
    {
        public:
            Parameter() { type = Ark::Ast::AstType::PARAMETER; }

            std::unique_ptr<AstNode> var_decl;
            std::unique_ptr<LiteralValue> body;
    };
    
    class ParameterList: public AstNode
    {
        public:
            ParameterList() { type = Ark::Ast::AstType::PARAMETER_LIST; }

            std::vector<std::unique_ptr<LiteralValue>> value;
    };
    
    class TypeFunction: public AstNode
    {
        public:
            TypeFunction() { type = Ark::Ast::AstType::TYPE_FUNCTION; }

            std::unique_ptr<TypeList> type_list;
            std::unique_ptr<TypeIdentifier> type_def;
    };
    
    class AnonimousFunction: public AstNode
    {
        public:
            AnonimousFunction() { type = Ark::Ast::AstType::ANONIMOUS_FUNCTION; }

            std::unique_ptr<ParameterList> param_list;
            std::unique_ptr<TypeIdentifier> type_def;
            std::unique_ptr<BlockScope> body;
    };
    
    class ExternDeclaration: public AstNode
    {
        public:
            ExternDeclaration() { type = Ark::Ast::AstType::EXTERN_DECLARATION; }

            std::unique_ptr<TypeList> type_list;
            std::unique_ptr<TypeIdentifier> type_def;
    };
    
    class ModuleDeclaration: public AstNode
    {
        public:
            ModuleDeclaration() { type = Ark::Ast::AstType::MODULE_DECLARATION; }

            std::vector<std::unique_ptr<Identifier>> value;
    };
    
    class ImportDeclaration: public AstNode
    {
        public:
            ImportDeclaration() { type = Ark::Ast::AstType::IMPORT_DECLARATION; }

            std::vector<std::unique_ptr<Identifier>> value;
    };
    
    class Cast: public AstNode
    {
        public:
            Cast() { type = Ark::Ast::AstType::CAST; }

            std::unique_ptr<Expression> value;
            std::unique_ptr<TypeIdentifier> type_def;
    };
    
    class LiteralValue: public AstNode
    {
        public:
            LiteralValue() { type = Ark::Ast::AstType::LITERAL_VALUE; }

            std::unique_ptr<AstNode> value;
    };
}

