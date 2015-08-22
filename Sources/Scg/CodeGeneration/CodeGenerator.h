/**
 * @file Scg/CodeGeneration/CodeGenerator.h
 *
 * @copyright Copyright (C) 2014 Rafid Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#ifndef __CodeGenerator_h__
#define __CodeGenerator_h__

// Alusus Core forward declarations.
#include <Basic/SharedPtr.h>
#include <Basic/basic.h>
#include <core_fwd.h>

// Scg CodeGeneration type definitions.
#include <typedefs.h>
#include <Types/ValueTypeSpec.h>

namespace Scg
{
class BinaryOperator;
class Block;
class CallFunction;
class DeclareExtFunction;
class DefineFunction;
class DefineStruct;
class DefineVariable;
class Expression;
class ForStatement;
class IfStatement;
class List;
class Module;
class Return;
class ValueTypeSpec;
class WhileStatement;

class CodeGenerator
{
public:
  Core::Basic::Word getStatementListId()
  {
    return statementListId;
  }
  Core::Basic::Word getExpressionId()
  {
    return expressionId;
  }
  Core::Basic::Word getListExpId()
  {
    return listExpId;
  }
  Core::Basic::Word getLowLinkExpId()
  {
    return lowLinkExpId;
  }
  Core::Basic::Word getLowerLinkExpId()
  {
    return lowerLinkExpId;
  }
  Core::Basic::Word getLowestLinkExpId()
  {
    return lowestLinkExpId;
  }
  Core::Basic::Word getFunctionalExpId()
  {
    return functionalExpId;
  }
  Core::Basic::Word getSubjectId()
  {
    return subjectId;
  }
  Core::Basic::Word getSubSubjectId()
  {
    return subSubjectId;
  }
  Core::Basic::Word getParameterId()
  {
    return parameterId;
  }
  Core::Basic::Word getLiteralId()
  {
    return literalId;
  }
  Core::Basic::Word getComparisonExpId()
  {
    return comparisonExpId;
  }
  Core::Basic::Word getAssignmentExpId()
  {
    return assignmentExpId;
  }
  Core::Basic::Word getAddExpId()
  {
    return addExpId;
  }
  Core::Basic::Word getMulExpId()
  {
    return mulExpId;
  }
  Core::Basic::Word getParamPassId()
  {
    return paramPassId;
  }
  Core::Basic::Word getLinkExpId()
  {
    return linkExpId;
  }
  Core::Basic::Word getPostfixTildeExpId()
  {
    return postfixTildeExpId;
  }
  Core::Basic::Word getIdentifierTokenId()
  {
    return identifierTokenId;
  }

  Core::Basic::Word getDefId()
  {
    return defId;
  }
  Core::Basic::Word getReturnId()
  {
    return returnId;
  }
  Core::Basic::Word getIfId()
  {
    return ifId;
  }
  Core::Basic::Word getForId()
  {
    return forId;
  }
  Core::Basic::Word getWhileId()
  {
    return whileId;
  }
  Core::Basic::Word getFunctionId()
  {
    return functionId;
  }
  Core::Basic::Word getStructureId()
  {
    return structureId;
  }
  Core::Basic::Word getLinkId()
  {
    return linkId;
  }
  Core::Basic::Word getCastTildeId()
  {
    return castTildeId;
  }
  Core::Basic::Word getPointerTildeId()
  {
    return pointerTildeId;
  }
  Core::Basic::Word getContentTildeId()
  {
    return contentTildeId;
  }
  Core::Basic::Word getSizeTildeId()
  {
    return sizeTildeId;
  }
  Core::Basic::Word getUnaryExpId()
  {
    return unaryExpId;
  }

  void setBuildMsgStore(Core::Processing::BuildMsgStore *store)
  {
    this->buildMsgStore = store;
  }
  Core::Processing::BuildMsgStore* getBuildMsgStore()
  {
    return this->buildMsgStore;
  }

  void setAliasDictionary(Core::Data::SharedMap *dictionary)
  {
    this->aliasDictionary = dictionary;
  }

private:
  Core::Basic::Word statementListId;
  Core::Basic::Word expressionId;
  Core::Basic::Word listExpId;
  Core::Basic::Word lowLinkExpId;
  Core::Basic::Word lowerLinkExpId;
  Core::Basic::Word lowestLinkExpId;
  Core::Basic::Word functionalExpId;
  Core::Basic::Word subjectId;
  Core::Basic::Word subSubjectId;
  Core::Basic::Word parameterId;
  Core::Basic::Word literalId;
  Core::Basic::Word comparisonExpId;
  Core::Basic::Word assignmentExpId;
  Core::Basic::Word addExpId;
  Core::Basic::Word mulExpId;
  Core::Basic::Word paramPassId;
  Core::Basic::Word linkExpId;
  Core::Basic::Word postfixTildeExpId;
  Core::Basic::Word unaryExpId;
  Core::Basic::Word identifierTokenId;

  Core::Basic::Word defId;
  Core::Basic::Word returnId;
  Core::Basic::Word ifId;
  Core::Basic::Word forId;
  Core::Basic::Word whileId;
  Core::Basic::Word functionId;
  Core::Basic::Word structureId;
  Core::Basic::Word linkId;
  Core::Basic::Word castTildeId;
  Core::Basic::Word pointerTildeId;
  Core::Basic::Word contentTildeId;
  Core::Basic::Word sizeTildeId;

  Core::Processing::BuildMsgStore *buildMsgStore = nullptr;

  // TODO: Remove this when proper alias implementation is done.
  Core::Data::SharedMap *aliasDictionary = nullptr;

public:
  CodeGenerator()
  {
    initializeIds();
  }
  ~CodeGenerator() {}

public:
  Module *generateModule(const std::string &name, const Core::Basic::SharedPtr<Core::Data::Module> &srcModule);
  Block *generateSet(const Core::Basic::SharedPtr<Core::Data::ParsedList> &list);
  Block *generateInnerSet(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &item);
  ExpressionArray generateStatement(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &item);
  ExpressionArray generateDefine(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &item);
  ExpressionArray generateDefineVariable(Core::Basic::Char const *name,
                                         const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &item, bool isAssignment);
  DefineFunction *generateDefineFunction(Core::Basic::Char const *name,
                                         const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &expr);
  DefineStruct *generateDefineStructure(Core::Basic::Char const *name,
                                        const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &expr);

  Expression *generateExpression(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &item);
  Return *generateReturn(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &ptr);

private:
  void initializeIds();
  Expression *generateVariableRef(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &param);
  Expression *generateConst(const Core::Basic::SharedPtr<Core::Data::ParsedToken> &literal);
  List *generateList(const Core::Basic::SharedPtr<Core::Data::ParsedList> &listExpr);
  Expression *generateBinaryOperator(const Core::Basic::SharedPtr<Core::Data::ParsedList> &cmpExpr);
  Expression *generateUnaryOperator(const Core::Basic::SharedPtr<Core::Data::ParsedList> &cmpExpr);
  IfStatement *generateIfStatement(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &command);
  ForStatement *generateForStatement(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &command);
  WhileStatement *generateWhileStatement(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &command);

public:
  /**
   * Parses an AST block that represents a token, e.g. variable name,
   * number, etc.
   *
   * @param[in] item  A pointer to the root of the AST block.
   */
  Core::Basic::Char const* parseToken(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &item);

  /**
   * Parses an AST block that is generated for a variable type, e.g. int,
   * float, ptr[int], ptr[ptr[double]], etc.
   *
   * @param[in] item      A pointer to the root of the AST block.
   *
   * @return A pointer to ValueTypeSpec specifying the parsed type. The
   * content of the pointer is allocated in the heap and should be freed by
   * the caller.
   */
  ValueTypeSpec *parseVariableType(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &item);

  /**
   * Parses a colon-separated variable definition where the name of the
   * variable is specified before the colon and the type of the variable is
   * after the colon.
   *
   * @param[in] astBlockRoot  A pointer to the root of the AST block to be parsed.
   *
   * @return A VariableDefinition value specifying the name and type of the
   * variable.
   */
  VariableDefinition parseVariableDefinition(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject> &astBlockRoot);

  /**
   * Parses a comma-separated list of variable definitions which is used
   * to specify the arguments of a function, where each argument is a
   * variable definition.
   *
   * @param[in] astBlockRoot  A pointer to the root of the AST block to be parsed.
   *
   * @return A VariableDefinitionArray containing the variable definitions.
   */
  VariableDefinitionArray parseFunctionArguments(const Core::Basic::SharedPtr<Core::Basic::IdentifiableObject>
      &astBlockRoot);

  // TODO: Remove this when proper alias implementation is done.
  /**
   * @brief Translate the given name if it's found in the alias dictionary.
   * @return Returns the translated string. If no dictionary is found or the
   *         given name is not found the function will return the input as is.
   */
  Core::Basic::Char const* translateAliasedName(Core::Basic::Char const *name);

}; // CodeGenerator
} // Scg

#endif // __CodeGenerator_h__
