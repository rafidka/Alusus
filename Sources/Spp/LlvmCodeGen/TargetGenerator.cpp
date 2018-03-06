/**
 * @file Spp/LlvmCodeGen/TargetGenerator.cpp
 * Contains the implementation of class Spp::LlvmCodeGen::TargetGenerator.
 *
 * @copyright Copyright (C) 2018 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#include "spp.h"

namespace Spp { namespace LlvmCodeGen
{

//==============================================================================
// Initialization Functions

void TargetGenerator::initBindings()
{
  auto targetGeneration = ti_cast<CodeGen::TargetGeneration>(this);

  // Property Getters
  targetGeneration->getExecutionContext = &TargetGenerator::getExecutionContext;

  // Type Generation Functions
  targetGeneration->generateVoidType = &TargetGenerator::generateVoidType;
  targetGeneration->generateIntType = &TargetGenerator::generateIntType;
  targetGeneration->generateFloatType = &TargetGenerator::generateFloatType;
  targetGeneration->generatePointerType = &TargetGenerator::generatePointerType;
  targetGeneration->generateArrayType = &TargetGenerator::generateArrayType;
  targetGeneration->generateStructTypeDecl = &TargetGenerator::generateStructTypeDecl;
  targetGeneration->generateStructTypeBody = &TargetGenerator::generateStructTypeBody;
  targetGeneration->getTypeAllocationSize = &TargetGenerator::getTypeAllocationSize;

  // Function Generation Functions
  targetGeneration->generateFunctionDecl = &TargetGenerator::generateFunctionDecl;
  targetGeneration->prepareFunctionBody = &TargetGenerator::prepareFunctionBody;
  targetGeneration->finishFunctionBody = &TargetGenerator::finishFunctionBody;

  // Variable Definition Generation Functions
  targetGeneration->generateGlobalVariable = &TargetGenerator::generateGlobalVariable;
  targetGeneration->generateLocalVariable = &TargetGenerator::generateLocalVariable;

  // Statements Generation Functions
  targetGeneration->prepareIfStatement = &TargetGenerator::prepareIfStatement;
  targetGeneration->finishIfStatement = &TargetGenerator::finishIfStatement;
  targetGeneration->prepareWhileStatement = &TargetGenerator::prepareWhileStatement;
  targetGeneration->finishWhileStatement = &TargetGenerator::finishWhileStatement;
  targetGeneration->prepareForStatement = &TargetGenerator::prepareForStatement;
  targetGeneration->finishForStatement = &TargetGenerator::finishForStatement;

  // Casting Generation Functions
  targetGeneration->generateCastIntToInt = &TargetGenerator::generateCastIntToInt;
  targetGeneration->generateCastIntToFloat = &TargetGenerator::generateCastIntToFloat;
  targetGeneration->generateCastFloatToInt = &TargetGenerator::generateCastFloatToInt;
  targetGeneration->generateCastFloatToFloat = &TargetGenerator::generateCastFloatToFloat;
  targetGeneration->generateCastIntToPointer = &TargetGenerator::generateCastIntToPointer;
  targetGeneration->generateCastPointerToInt = &TargetGenerator::generateCastPointerToInt;
  targetGeneration->generateCastPointerToPointer = &TargetGenerator::generateCastPointerToPointer;

  // Operation Generation Functions
  targetGeneration->generateVarReference = &TargetGenerator::generateVarReference;
  targetGeneration->generateMemberVarReference = &TargetGenerator::generateMemberVarReference;
  targetGeneration->generateArrayElementReference = &TargetGenerator::generateArrayElementReference;
  targetGeneration->generateDereference = &TargetGenerator::generateDereference;
  targetGeneration->generateAssign = &TargetGenerator::generateAssign;
  targetGeneration->generateFunctionCall = &TargetGenerator::generateFunctionCall;
  targetGeneration->generateReturn = &TargetGenerator::generateReturn;

  // Math Ops Generation Functions
  targetGeneration->generateAdd = &TargetGenerator::generateAdd;
  targetGeneration->generateSub = &TargetGenerator::generateSub;
  targetGeneration->generateMul = &TargetGenerator::generateMul;
  targetGeneration->generateDiv = &TargetGenerator::generateDiv;
  targetGeneration->generateRem = &TargetGenerator::generateRem;
  targetGeneration->generateShr = &TargetGenerator::generateShr;
  targetGeneration->generateShl = &TargetGenerator::generateShl;
  targetGeneration->generateAnd = &TargetGenerator::generateAnd;
  targetGeneration->generateOr = &TargetGenerator::generateOr;
  targetGeneration->generateXor = &TargetGenerator::generateXor;
  targetGeneration->generateNot = &TargetGenerator::generateNot;
  targetGeneration->generateNeg = &TargetGenerator::generateNeg;
  targetGeneration->generateEarlyInc = &TargetGenerator::generateEarlyInc;
  targetGeneration->generateEarlyDec = &TargetGenerator::generateEarlyDec;
  targetGeneration->generateLateInc = &TargetGenerator::generateLateInc;
  targetGeneration->generateLateDec = &TargetGenerator::generateLateDec;
  targetGeneration->generateAddAssign = &TargetGenerator::generateAddAssign;
  targetGeneration->generateSubAssign = &TargetGenerator::generateSubAssign;
  targetGeneration->generateMulAssign = &TargetGenerator::generateMulAssign;
  targetGeneration->generateDivAssign = &TargetGenerator::generateDivAssign;
  targetGeneration->generateRemAssign = &TargetGenerator::generateRemAssign;
  targetGeneration->generateShrAssign = &TargetGenerator::generateShrAssign;
  targetGeneration->generateShlAssign = &TargetGenerator::generateShlAssign;
  targetGeneration->generateAndAssign = &TargetGenerator::generateAndAssign;
  targetGeneration->generateOrAssign = &TargetGenerator::generateOrAssign;
  targetGeneration->generateXorAssign = &TargetGenerator::generateXorAssign;

  // Comparison Ops Generation Functions
  targetGeneration->generateEqual = &TargetGenerator::generateEqual;
  targetGeneration->generateNotEqual = &TargetGenerator::generateNotEqual;
  targetGeneration->generateGreaterThan = &TargetGenerator::generateGreaterThan;
  targetGeneration->generateGreaterThanOrEqual = &TargetGenerator::generateGreaterThanOrEqual;
  targetGeneration->generateLessThan = &TargetGenerator::generateLessThan;
  targetGeneration->generateLessThanOrEqual = &TargetGenerator::generateLessThanOrEqual;

  // Literal Generation Functions
  targetGeneration->generateIntLiteral = &TargetGenerator::generateIntLiteral;
  targetGeneration->generateFloatLiteral = &TargetGenerator::generateFloatLiteral;
  targetGeneration->generateStringLiteral = &TargetGenerator::generateStringLiteral;
  targetGeneration->generateNullPtrLiteral = &TargetGenerator::generateNullPtrLiteral;
}


//==============================================================================
// Main Operation Functions

void TargetGenerator::prepareBuild(Core::Processing::NoticeStore *noticeStore)
{
  llvm::InitializeNativeTarget();

  this->llvmDataLayout = std::make_shared<llvm::DataLayout>("");
  this->llvmModule = std::make_shared<llvm::Module>("AlususProgram", llvm::getGlobalContext());
  this->llvmModule->setDataLayout(this->llvmDataLayout->getStringRepresentation());
  this->executionContext = std::make_shared<ExecutionContext>(llvmDataLayout->getPointerSizeInBits());
  this->noticeStore = noticeStore;
}


void TargetGenerator::dumpIr(Core::Basic::OutStream &out)
{
  if (this->llvmModule == 0) {
    throw EXCEPTION(GenericException, STR("LLVM module is not generated yet."));
  }

  llvm::raw_os_ostream ostream(out);
  llvm::PassManager passManager;
  passManager.add(llvm::createPrintModulePass(&ostream));
  passManager.run(*(this->llvmModule.get()));
}


void TargetGenerator::execute(Char const *entry)
{
  if (this->llvmModule == 0) {
    throw EXCEPTION(GenericException, STR("LLVM module is not generated yet."));
  }

  auto ee = llvm::ExecutionEngine::createJIT(this->llvmModule.get());
  auto func = ee->FindFunctionNamed(entry);
  std::vector<llvm::GenericValue> args(0);
  ee->runFunction(func, args);
}


//==============================================================================
// Type Generation Functions

Bool TargetGenerator::generateVoidType(TioSharedPtr &type)
{
  auto llvmType = llvm::Type::getVoidTy(llvm::getGlobalContext());
  type = std::make_shared<Type>(Type::Category::VOID, llvmType);
  return true;
}


Bool TargetGenerator::generateIntType(Word bitCount, TioSharedPtr &type)
{
  // TODO: Support 128 bits?
  if (bitCount != 1 && bitCount != 8 && bitCount != 16 && bitCount != 32 && bitCount != 64) {
    this->noticeStore->add(std::make_shared<CodeGen::InvalidIntegerBitCountNotice>());
    return false;
  }
  auto llvmType = llvm::Type::getIntNTy(llvm::getGlobalContext(), bitCount);
  type = std::make_shared<Type>(Type::Category::INT, llvmType, bitCount);
  return true;
}


Bool TargetGenerator::generateFloatType(Word bitCount, TioSharedPtr &type)
{
  llvm::Type *llvmType;
  switch (bitCount) {
    case 32:
      llvmType = llvm::Type::getFloatTy(llvm::getGlobalContext());
      break;
    case 64:
      llvmType = llvm::Type::getDoubleTy(llvm::getGlobalContext());
      break;
    // TODO: Support 128 bits?
    // case 128:
    //   llvmType = llvm::Type::getFP128Ty(llvm::getGlobalContext());
    //   break;
    default:
      this->noticeStore->add(std::make_shared<CodeGen::InvalidFloatBitCountNotice>());
      return false;
  }
  type = std::make_shared<Type>(Type::Category::FLOAT, llvmType, bitCount);
  return true;
}


Bool TargetGenerator::generatePointerType(TiObject *contentType, TioSharedPtr &type)
{
  PREPARE_ARG(contentType, contentTypeWrapper, Type);
  auto llvmType = contentTypeWrapper->getLlvmType()->getPointerTo();
  type = std::make_shared<Type>(Type::Category::POINTER, llvmType, contentTypeWrapper);
  return true;
}


Bool TargetGenerator::generateArrayType(TiObject *contentType, Word size, TioSharedPtr &type)
{
  PREPARE_ARG(contentType, contentTypeWrapper, Type);
  auto llvmType = llvm::ArrayType::get(contentTypeWrapper->getLlvmType(), size);
  type = std::make_shared<Type>(Type::Category::ARRAY, llvmType, contentTypeWrapper, size);
  return true;
}


Bool TargetGenerator::generateStructTypeDecl(
  Char const *name, TioSharedPtr &type
) {
  auto llvmType = llvm::StructType::create(llvm::getGlobalContext(), name);
  type = std::make_shared<Type>(Type::Category::STRUCT, llvmType, (Type*)0, name);
  return true;
}


Bool TargetGenerator::generateStructTypeBody(
  TiObject *type, Core::Basic::MapContainer<TiObject> *membersTypes,
  Core::Basic::SharedList<TiObject, TiObject> *members
) {
  VALIDATE_NOT_NULL(membersTypes, members);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() != Type::Category::STRUCT) {
    throw EXCEPTION(InvalidArgumentException, STR("type"), STR("Invalid target type category."));
  }

  std::vector<llvm::Type*> structMembers;
  structMembers.reserve(membersTypes->getElementCount());
  for (Int i = 0; i < membersTypes->getElementCount(); ++i) {
    auto contentTypeWrapper = ti_cast<Type>(membersTypes->getElement(i));
    if (contentTypeWrapper == 0) {
      throw EXCEPTION(
        InvalidArgumentException, STR("membersTypes"), STR("Not all elements are instances of LlvmCodeGen::Type")
      );
    }
    structMembers.push_back(contentTypeWrapper->getLlvmType());

    SharedPtr<Variable> var = std::make_shared<Variable>();
    var->setLlvmStructIndex(i);
    members->add(var);
  }
  static_cast<llvm::StructType*>(tgType->getLlvmType())->setBody(structMembers);
  return true;
}


Word TargetGenerator::getTypeAllocationSize(TiObject *type)
{
  PREPARE_ARG(type, tgType, Type);
  return this->llvmDataLayout->getTypeAllocSize(tgType->getLlvmType());
}


//==============================================================================
// Function Generation Functions

Bool TargetGenerator::generateFunctionDecl(
  Char const *name, Core::Basic::MapContainer<TiObject>* argTypes, TiObject *retType, Bool variadic,
  TioSharedPtr &function
) {
  VALIDATE_NOT_NULL(name, argTypes, retType);

  // Prepare args
  std::vector<llvm::Type*> llvmArgTypes;
  llvmArgTypes.reserve(argTypes->getElementCount());
  for (Int i = 0; i < argTypes->getElementCount(); ++i) {
    auto contentTypeWrapper = ti_cast<Type>(argTypes->getElement(i));
    if (contentTypeWrapper == 0) {
      throw EXCEPTION(
        InvalidArgumentException, STR("argTypes"), STR("Not all elements are instances of LlvmCodeGen::Type")
      );
    }
    llvmArgTypes.push_back(contentTypeWrapper->getLlvmType());
  }

  // Prepare ret type.
  auto retTypeWrapper = ti_cast<Type>(retType);
  if (retTypeWrapper == 0) {
    throw EXCEPTION(
      InvalidArgumentException, STR("retType"), STR("Not an instance of LlvmCodeGen::Type")
    );
  }

  // Create the function.
  auto llvmFuncType = llvm::FunctionType::get(retTypeWrapper->getLlvmType(), llvmArgTypes, variadic);
  auto llvmFunc = llvm::Function::Create(
    llvmFuncType, llvm::Function::ExternalLinkage, name, this->llvmModule.get()
  );
  function = Core::Basic::Box<llvm::Function*>::create(llvmFunc);
  return true;
}


Bool TargetGenerator::prepareFunctionBody(
  TiObject *function, Core::Basic::MapContainer<TiObject>* argTypes, TiObject *retType,
  Bool variadic, Core::Basic::SharedList<TiObject, TiObject> *args, TioSharedPtr &context
) {
  VALIDATE_NOT_NULL(function, argTypes, retType, args);

  // Create the block
  PREPARE_ARG(function, funcWrapper, Core::Basic::Box<llvm::Function*>);
  auto llvmFunc = funcWrapper->get();
  auto block = std::make_shared<Block>();
  block->setLlvmBlock(llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), llvmFunc));
  block->setIrBuilder(new llvm::IRBuilder<>(block->getLlvmBlock()));
  block->setLlvmFunction(llvmFunc);

  // Prepare the function arguments.
  auto i = 0;
  for (auto iter = llvmFunc->arg_begin(); i != argTypes->getElementCount(); ++iter, ++i) {
    auto argType = ti_cast<Type>(argTypes->getElement(i));
    if (argType == 0) {
      throw EXCEPTION(
        InvalidArgumentException, STR("argTypes"), STR("Some elements are null or are not of type Type.")
      );
    }

    iter->setName(argTypes->getElementKey(i).c_str());
    auto llvmAlloca = block->getIrBuilder()->CreateAlloca(
      argType->getLlvmType(), 0, argTypes->getElementKey(i).c_str()
    );
    block->getIrBuilder()->CreateStore(iter, llvmAlloca);

    SharedPtr<Variable> var = std::make_shared<Variable>();
    var->setLlvmAllocaInst(llvmAlloca);
    args->add(var);
  }

  context = block;
  return true;
}


Bool TargetGenerator::finishFunctionBody(
  TiObject *function, Core::Basic::MapContainer<TiObject> *argTypes, TiObject *retType,
  Bool variadic, Core::Basic::ListContainer<TiObject> *args, TiObject *context
) {
  PREPARE_ARG(retType, type, Type);
  if (type->getCategory() == Type::Category::VOID) {
    return this->generateReturn(context, 0, 0);
  } else {
    return true;
  }
}


//==============================================================================
// Variable Definition Generation Functions

Bool TargetGenerator::generateGlobalVariable(
  TiObject *type, Char const* name, TiObject *defaultValue, TioSharedPtr &result
) {
  PREPARE_ARG(type, typeWrapper, Type);
  PREPARE_ARG(defaultValue, valWrapper, Value);

  SharedPtr<Variable> var = std::make_shared<Variable>();
  var->setLlvmGlobalVariable(new llvm::GlobalVariable(
    *(this->llvmModule.get()), typeWrapper->getLlvmType(), false, llvm::GlobalVariable::ExternalLinkage,
    valWrapper->getLlvmConstant(), name
  ));

  result = var;
  return true;
}


Bool TargetGenerator::generateLocalVariable(
  TiObject *context, TiObject *type, Char const* name, TiObject *defaultValue, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(type, typeWrapper, Type);
  auto valWrapper = ti_cast<Value>(defaultValue);

  SharedPtr<Variable> var = std::make_shared<Variable>();
  var->setLlvmAllocaInst(block->getIrBuilder()->CreateAlloca(typeWrapper->getLlvmType(), 0, name));
  if (valWrapper != 0) {
    block->getIrBuilder()->CreateStore(valWrapper->getLlvmValue(), var->getLlvmAllocaInst());
  }

  result = var;
  return true;
}


//==============================================================================
// Statements Generation Functions

Bool TargetGenerator::prepareIfStatement(TiObject *context, Bool withElse, SharedPtr<CodeGen::IfTgContext> &ifTgContext)
{
  PREPARE_ARG(context, block, Block);

  SharedPtr<IfContext> ifContext = std::make_shared<IfContext>();

  // Prepare condition context.
  ifContext->setConditionBlock(Core::Basic::getSharedPtr(block));

  // Prepare body context.
  auto bodyBlock = std::make_shared<Block>();
  bodyBlock->setLlvmBlock(
    llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction())
  );
  bodyBlock->setIrBuilder(new llvm::IRBuilder<>(bodyBlock->getLlvmBlock()));
  bodyBlock->setLlvmFunction(block->getLlvmFunction());
  ifContext->setBodyBlock(bodyBlock);

  // Prepare else context.
  if (withElse) {
    auto elseBlock = std::make_shared<Block>();
    elseBlock->setLlvmBlock(
      llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction())
    );
    elseBlock->setIrBuilder(new llvm::IRBuilder<>(elseBlock->getLlvmBlock()));
    elseBlock->setLlvmFunction(block->getLlvmFunction());
    ifContext->setElseBlock(elseBlock);
  }

  ifTgContext = ifContext;
  return true;
}


Bool TargetGenerator::finishIfStatement(TiObject *context, CodeGen::IfTgContext *ifTgContext, TiObject *conditionVal)
 {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(ifTgContext, ifContext, IfContext);
  PREPARE_ARG(conditionVal, valWrapper, Value);

  // Create a merge block and jump to it from if and else bodies.
  auto mergeLlvmBlock = llvm::BasicBlock::Create(
    llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction()
  );
  ifContext->getBodyBlock()->getIrBuilder()->CreateBr(mergeLlvmBlock);
  if (ifContext->getElseBlock() != 0) {
    ifContext->getElseBlock()->getIrBuilder()->CreateBr(mergeLlvmBlock);
  }

  // Create the if statement.
  block->getIrBuilder()->CreateCondBr(
    valWrapper->getLlvmValue(),
    ifContext->getBodyBlock()->getLlvmBlock(),
    ifContext->getElseBlock() != 0 ? ifContext->getElseBlock()->getLlvmBlock() : mergeLlvmBlock
  );

  // Set insert point to the merge body.
  block->getIrBuilder()->SetInsertPoint(mergeLlvmBlock);
  block->setLlvmBlock(mergeLlvmBlock);

  return true;
}


Bool TargetGenerator::prepareWhileStatement(TiObject *context, SharedPtr<CodeGen::LoopTgContext> &loopTgContext)
{
  PREPARE_ARG(context, block, Block);

  SharedPtr<LoopContext> loopContext = std::make_shared<LoopContext>();

  // Prepare condition context.
  auto condBlock = std::make_shared<Block>();
  condBlock->setLlvmBlock(
    llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction())
  );
  condBlock->setIrBuilder(new llvm::IRBuilder<>(condBlock->getLlvmBlock()));
  condBlock->setLlvmFunction(block->getLlvmFunction());
  loopContext->setConditionBlock(condBlock);

  // Prepare body context.
  auto bodyBlock = std::make_shared<Block>();
  bodyBlock->setLlvmBlock(
    llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction())
  );
  bodyBlock->setIrBuilder(new llvm::IRBuilder<>(bodyBlock->getLlvmBlock()));
  bodyBlock->setLlvmFunction(block->getLlvmFunction());
  loopContext->setBodyBlock(bodyBlock);

  // Prepare exit block.
  auto exitBlock = std::make_shared<Block>();
  exitBlock->setLlvmBlock(
    llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction())
  );
  exitBlock->setLlvmFunction(block->getLlvmFunction());
  loopContext->setExitBlock(exitBlock);

  loopTgContext = loopContext;
  return true;
}


Bool TargetGenerator::finishWhileStatement(
  TiObject *context, CodeGen::LoopTgContext *loopTgContext, TiObject *conditionVal
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(loopTgContext, loopContext, LoopContext);
  PREPARE_ARG(conditionVal, valWrapper, Value);

  // Jump to condition block.
  block->getIrBuilder()->CreateBr(loopContext->getConditionBlock()->getLlvmBlock());

  // Jump from body to condition block.
  loopContext->getBodyBlock()->getIrBuilder()->CreateBr(loopContext->getConditionBlock()->getLlvmBlock());

  // Create condition branch.
  loopContext->getConditionBlock()->getIrBuilder()->CreateCondBr(
    valWrapper->getLlvmValue(), loopContext->getBodyBlock()->getLlvmBlock(), loopContext->getExitBlock()->getLlvmBlock()
  );

  // Set insert point.
  block->getIrBuilder()->SetInsertPoint(loopContext->getExitBlock()->getLlvmBlock());
  block->setLlvmBlock(loopContext->getExitBlock()->getLlvmBlock());

  return true;
}


Bool TargetGenerator::prepareForStatement(TiObject *context, SharedPtr<CodeGen::LoopTgContext> &loopTgContext)
{
  PREPARE_ARG(context, block, Block);

  SharedPtr<LoopContext> loopContext = std::make_shared<LoopContext>();

  // Prepare condition context.
  auto condBlock = std::make_shared<Block>();
  condBlock->setLlvmBlock(
    llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction())
  );
  condBlock->setIrBuilder(new llvm::IRBuilder<>(condBlock->getLlvmBlock()));
  condBlock->setLlvmFunction(block->getLlvmFunction());
  loopContext->setConditionBlock(condBlock);

  // Prepare increment context.
  auto updaterBlock = std::make_shared<Block>();
  updaterBlock->setLlvmBlock(
    llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction())
  );
  updaterBlock->setIrBuilder(new llvm::IRBuilder<>(updaterBlock->getLlvmBlock()));
  updaterBlock->setLlvmFunction(block->getLlvmFunction());
  loopContext->setUpdaterBlock(updaterBlock);

  // Prepare body context.
  auto bodyBlock = std::make_shared<Block>();
  bodyBlock->setLlvmBlock(
    llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction())
  );
  bodyBlock->setIrBuilder(new llvm::IRBuilder<>(bodyBlock->getLlvmBlock()));
  bodyBlock->setLlvmFunction(block->getLlvmFunction());
  loopContext->setBodyBlock(bodyBlock);

  // Prepare exit block.
  auto exitBlock = std::make_shared<Block>();
  exitBlock->setLlvmBlock(
    llvm::BasicBlock::Create(llvm::getGlobalContext(), this->getNewBlockName(), block->getLlvmFunction())
  );
  exitBlock->setLlvmFunction(block->getLlvmFunction());
  loopContext->setExitBlock(exitBlock);

  loopTgContext = loopContext;
  return true;
}


Bool TargetGenerator::finishForStatement(
  TiObject *context, CodeGen::LoopTgContext *loopTgContext, TiObject *conditionVal
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(loopTgContext, loopContext, LoopContext);
  PREPARE_ARG(conditionVal, valWrapper, Value);

  // Jump to condition block.
  block->getIrBuilder()->CreateBr(loopContext->getConditionBlock()->getLlvmBlock());

  // Jump from body to update block.
  loopContext->getBodyBlock()->getIrBuilder()->CreateBr(loopContext->getUpdaterBlock()->getLlvmBlock());

  // Jump from update to condition block.
  loopContext->getUpdaterBlock()->getIrBuilder()->CreateBr(loopContext->getConditionBlock()->getLlvmBlock());

  // Create condition branch.
  loopContext->getConditionBlock()->getIrBuilder()->CreateCondBr(
    valWrapper->getLlvmValue(), loopContext->getBodyBlock()->getLlvmBlock(), loopContext->getExitBlock()->getLlvmBlock()
  );

  // Set insert point.
  block->getIrBuilder()->SetInsertPoint(loopContext->getExitBlock()->getLlvmBlock());
  block->setLlvmBlock(loopContext->getExitBlock()->getLlvmBlock());

  return true;
}


//==============================================================================
// Casting Generation Functions

Bool TargetGenerator::generateCastIntToInt(
  TiObject *context, TiObject *srcType, TiObject *destType, TiObject *srcVal, TioSharedPtr &destVal
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, cgSrcVal, Value);
  PREPARE_ARG(destType, typeWrapper, Type);
  auto llvmCastedValue = block->getIrBuilder()->CreateIntCast(
    cgSrcVal->getLlvmValue(), typeWrapper->getLlvmType(), true
  );
  destVal = std::make_shared<Value>(llvmCastedValue, false);
  return true;
}


Bool TargetGenerator::generateCastIntToFloat(
  TiObject *context, TiObject *srcType, TiObject *destType, TiObject *srcVal, TioSharedPtr &destVal
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, cgSrcVal, Value);
  PREPARE_ARG(destType, typeWrapper, Type);
  auto llvmCastedValue = block->getIrBuilder()->CreateSIToFP(cgSrcVal->getLlvmValue(), typeWrapper->getLlvmType());
  destVal = std::make_shared<Value>(llvmCastedValue, false);
  return true;
}


Bool TargetGenerator::generateCastFloatToInt(
  TiObject *context, TiObject *srcType, TiObject *destType, TiObject *srcVal, TioSharedPtr &destVal
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, cgSrcVal, Value);
  PREPARE_ARG(destType, typeWrapper, Type);
  auto llvmCastedValue = block->getIrBuilder()->CreateFPToSI(cgSrcVal->getLlvmValue(), typeWrapper->getLlvmType());
  destVal = std::make_shared<Value>(llvmCastedValue, false);
  return true;
}


Bool TargetGenerator::generateCastFloatToFloat(
  TiObject *context, TiObject *srcType, TiObject *destType, TiObject *srcVal, TioSharedPtr &destVal
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, cgSrcVal, Value);
  PREPARE_ARG(srcType, srcTypeWrapper, Type);
  PREPARE_ARG(destType, destTypeWrapper, Type);

  llvm::Value *llvmCastedValue;
  if (srcTypeWrapper->getSize() > destTypeWrapper->getSize()) {
    llvmCastedValue = block->getIrBuilder()->CreateFPTrunc(cgSrcVal->getLlvmValue(), destTypeWrapper->getLlvmType());
  } else {
    llvmCastedValue = block->getIrBuilder()->CreateFPExt(cgSrcVal->getLlvmValue(), destTypeWrapper->getLlvmType());
  }
  destVal = std::make_shared<Value>(llvmCastedValue, false);
  return true;
}


Bool TargetGenerator::generateCastIntToPointer(
  TiObject *context, TiObject *srcType, TiObject *destType, TiObject *srcVal, TioSharedPtr &destVal
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, cgSrcVal, Value);
  PREPARE_ARG(destType, typeWrapper, Type);
  auto llvmCastedValue = block->getIrBuilder()->CreateIntToPtr(cgSrcVal->getLlvmValue(), typeWrapper->getLlvmType());
  destVal = std::make_shared<Value>(llvmCastedValue, false);
  return true;
}


Bool TargetGenerator::generateCastPointerToInt(
  TiObject *context, TiObject *srcType, TiObject *destType, TiObject *srcVal, TioSharedPtr &destVal
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, cgSrcVal, Value);
  PREPARE_ARG(destType, typeWrapper, Type);
  auto llvmCastedValue = block->getIrBuilder()->CreatePtrToInt(cgSrcVal->getLlvmValue(), typeWrapper->getLlvmType());
  destVal = std::make_shared<Value>(llvmCastedValue, false);
  return true;
}


Bool TargetGenerator::generateCastPointerToPointer(
  TiObject *context, TiObject *srcType, TiObject *destType, TiObject *srcVal, TioSharedPtr &destVal
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, cgSrcVal, Value);
  PREPARE_ARG(destType, typeWrapper, Type);
  auto llvmCastedValue = block->getIrBuilder()->CreateBitCast(cgSrcVal->getLlvmValue(), typeWrapper->getLlvmType());
  destVal = std::make_shared<Value>(llvmCastedValue, false);
  return true;
}


//==============================================================================
// Operation Generation Functions

Bool TargetGenerator::generateVarReference(
  TiObject *context, TiObject *varType, TiObject *varDefinition, TioSharedPtr &result
) {
  PREPARE_ARG(varDefinition, var, Variable);
  if (var->getLlvmAllocaInst() != 0) {
    result = std::make_shared<Value>(var->getLlvmAllocaInst(), false);
  } else if (var->getLlvmGlobalVariable() != 0) {
    result = std::make_shared<Value>(var->getLlvmGlobalVariable(), false);
  } else {
    throw EXCEPTION(GenericException, STR("Variable definition was not generated correctly."));
  }
  return true;
}


Bool TargetGenerator::generateMemberVarReference(
  TiObject *context, TiObject *structType, TiObject *memberType,
  TiObject *memberVarDef, TiObject *structRef, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(structRef, tgStructRef, Value);
  PREPARE_ARG(memberVarDef, tgMemberVarDef, Variable);
  PREPARE_ARG(structType, tgStructType, Type);

  llvm::Value *llvmPtr;
  if (tgStructType->getCategory() == Type::Category::POINTER) {
    llvmPtr = tgStructRef->getLlvmValue();
  } else {
    llvmPtr = block->getIrBuilder()->CreateAlloca(tgStructType->getLlvmType(), 0);
    block->getIrBuilder()->CreateStore(tgStructRef->getLlvmValue(), llvmPtr);
  }

  auto zero = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0, true));
  auto index = llvm::ConstantInt::get(
    llvm::getGlobalContext(), llvm::APInt(32, tgMemberVarDef->getLlvmStructIndex(), true)
  );
  auto llvmResult = block->getIrBuilder()->CreateGEP(
    llvmPtr, llvm::makeArrayRef(std::vector<llvm::Value*>({ zero, index })), ""
  );
  result = std::make_shared<Value>(llvmResult, false);
  return true;
}


Bool TargetGenerator::generateArrayElementReference(
  TiObject *context, TiObject *arrayType, TiObject *elementType, TiObject *index, TiObject *arrayRef,
  TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(arrayRef, tgArrayRef, Value);
  PREPARE_ARG(arrayType, tgArrayType, Type);
  PREPARE_ARG(index, tgIndex, Value);

  llvm::Value *llvmPtr;
  if (tgArrayType->getCategory() == Type::Category::POINTER) {
    llvmPtr = tgArrayRef->getLlvmValue();
  } else {
    llvmPtr = block->getIrBuilder()->CreateAlloca(tgArrayType->getLlvmType(), 0);
    block->getIrBuilder()->CreateStore(tgArrayRef->getLlvmValue(), llvmPtr);
  }

  auto zero = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0, true));
  auto llvmResult = block->getIrBuilder()->CreateGEP(
    llvmPtr, llvm::makeArrayRef(std::vector<llvm::Value*>({ zero, tgIndex->getLlvmValue() })), ""
  );
  result = std::make_shared<Value>(llvmResult, false);
  return true;
}


Bool TargetGenerator::generateDereference(
  TiObject *context, TiObject *contentType, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, cgSrcVal, Value);
  auto llvmResult = block->getIrBuilder()->CreateLoad(cgSrcVal->getLlvmValue());
  result = std::make_shared<Value>(llvmResult, false);
  return true;
}


Bool TargetGenerator::generateAssign(
  TiObject *context, TiObject *contentType, TiObject *srcVal, TiObject *destRef, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, cgSrcVal, Value);
  PREPARE_ARG(destRef, cgDestRef, Value);
  auto llvmResult = block->getIrBuilder()->CreateStore(cgSrcVal->getLlvmValue(), cgDestRef->getLlvmValue());
  result = std::make_shared<Value>(llvmResult, false);
  return true;
}


Bool TargetGenerator::generateFunctionCall(
  TiObject *context, TiObject *function,
  Core::Basic::Container<Core::Basic::TiObject>* arguments, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(function, llvmFuncBox, Core::Basic::Box<llvm::Function*>);

  // Create function call.
  std::vector<llvm::Value*> args;
  for (Int i = 0; i < arguments->getElementCount(); ++i) {
    auto llvmValBox = ti_cast<Value>(arguments->getElement(i));
    if (llvmValBox == 0) {
      throw EXCEPTION(InvalidArgumentException, STR("arguments"), STR("Some elements are null or of invalid type."));
    }
    args.push_back(llvmValBox->getLlvmValue());
  }
  auto llvmCall = block->getIrBuilder()->CreateCall(llvmFuncBox->get(), args);
  result = std::make_shared<Value>(llvmCall, false);
  return true;
}


Bool TargetGenerator::generateReturn(
  TiObject *context, TiObject *retType, TiObject *retVal
) {
  PREPARE_ARG(context, block, Block);
  if (retVal != 0) {
    PREPARE_ARG(retVal, retValBox, Value);
    block->getIrBuilder()->CreateRet(retValBox->getLlvmValue());
  } else {
    block->getIrBuilder()->CreateRetVoid();
  }
  return true;
}


//==============================================================================
// Math Ops Generation Functions

Bool TargetGenerator::generateAdd(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateAdd(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFAdd(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateSub(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateSub(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFSub(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateMul(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateMul(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFMul(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateDiv(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateSDiv(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFDiv(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateRem(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateSRem(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateShr(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateAShr(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateShl(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateShl(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateAnd(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateAnd(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateOr(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateOr(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateXor(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcVal1Box, Value);
  PREPARE_ARG(srcVal2, srcVal2Box, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateXor(srcVal1Box->getLlvmValue(), srcVal2Box->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateNot(
  TiObject *context, TiObject *type, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateNot(srcValBox->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateNeg(
  TiObject *context, TiObject *type, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateNeg(srcValBox->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFNeg(srcValBox->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateEarlyInc(
  TiObject *context, TiObject *type, TiObject *destVar, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto size = tgType->getSize();
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateAdd(
      llvmVal, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(size, 1, true))
    );
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    if (size == 32) {
      llvmResult = block->getIrBuilder()->CreateFAdd(
        llvmVal, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat((Float)1))
      );
    } else {
      llvmResult = block->getIrBuilder()->CreateFAdd(
        llvmVal, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat((Double)1))
      );
    }
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = std::make_shared<Value>(llvmResult, false);
  return true;
}


Bool TargetGenerator::generateEarlyDec(
  TiObject *context, TiObject *type, TiObject *destVar, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto size = tgType->getSize();
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateSub(
      llvmVal, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(size, 1, true))
    );
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    if (size == 32) {
      llvmResult = block->getIrBuilder()->CreateFSub(
        llvmVal, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat((Float)1))
      );
    } else {
      llvmResult = block->getIrBuilder()->CreateFSub(
        llvmVal, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat((Double)1))
      );
    }
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = std::make_shared<Value>(llvmResult, false);
  return true;
}


Bool TargetGenerator::generateLateInc(
  TiObject *context, TiObject *type, TiObject *destVar, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto size = tgType->getSize();
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateAdd(
      llvmVal, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(size, 1, true))
    );
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    if (size == 32) {
      llvmResult = block->getIrBuilder()->CreateFAdd(
        llvmVal, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat((Float)1))
      );
    } else {
      llvmResult = block->getIrBuilder()->CreateFAdd(
        llvmVal, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat((Double)1))
      );
    }
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = std::make_shared<Value>(llvmVal, false);
  return true;
}


Bool TargetGenerator::generateLateDec(
  TiObject *context, TiObject *type, TiObject *destVar, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto size = tgType->getSize();
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateSub(
      llvmVal, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(size, 1, true))
    );
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    if (size == 32) {
      llvmResult = block->getIrBuilder()->CreateFSub(
        llvmVal, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat((Float)1))
      );
    } else {
      llvmResult = block->getIrBuilder()->CreateFSub(
        llvmVal, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat((Double)1))
      );
    }
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = std::make_shared<Value>(llvmVal, false);
  return true;
}


Bool TargetGenerator::generateAddAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateAdd(llvmVal, srcValBox->getLlvmValue());
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    llvmResult = block->getIrBuilder()->CreateFAdd(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


Bool TargetGenerator::generateSubAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateSub(llvmVal, srcValBox->getLlvmValue());
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    llvmResult = block->getIrBuilder()->CreateFSub(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


Bool TargetGenerator::generateMulAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateMul(llvmVal, srcValBox->getLlvmValue());
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    llvmResult = block->getIrBuilder()->CreateFMul(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


Bool TargetGenerator::generateDivAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateSDiv(llvmVal, srcValBox->getLlvmValue());
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    llvmResult = block->getIrBuilder()->CreateFDiv(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


Bool TargetGenerator::generateRemAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateSRem(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


Bool TargetGenerator::generateShrAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateAShr(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


Bool TargetGenerator::generateShlAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateShl(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


Bool TargetGenerator::generateAndAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateAnd(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


Bool TargetGenerator::generateOrAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateOr(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


Bool TargetGenerator::generateXorAssign(
  TiObject *context, TiObject *type, TiObject *destVar, TiObject *srcVal, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(destVar, destVarBox, Value);
  PREPARE_ARG(srcVal, srcValBox, Value);
  PREPARE_ARG(type, tgType, Type);
  auto llvmVal = block->getIrBuilder()->CreateLoad(destVarBox->getLlvmValue());
  llvm::Value *llvmResult;
  if (tgType->getCategory() == Type::Category::INT) {
    llvmResult = block->getIrBuilder()->CreateXor(llvmVal, srcValBox->getLlvmValue());
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
  block->getIrBuilder()->CreateStore(llvmResult, destVarBox->getLlvmValue());
  result = getSharedPtr(destVar);
  return true;
}


//==============================================================================
// Comparison Ops Generation Functions

Bool TargetGenerator::generateEqual(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcValBox1, Value);
  PREPARE_ARG(srcVal2, srcValBox2, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateICmpEQ(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFCmpOEQ(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateNotEqual(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcValBox1, Value);
  PREPARE_ARG(srcVal2, srcValBox2, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateICmpNE(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFCmpONE(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateGreaterThan(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcValBox1, Value);
  PREPARE_ARG(srcVal2, srcValBox2, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateICmpSGT(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFCmpOGT(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateGreaterThanOrEqual(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcValBox1, Value);
  PREPARE_ARG(srcVal2, srcValBox2, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateICmpSGE(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFCmpOGE(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateLessThan(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcValBox1, Value);
  PREPARE_ARG(srcVal2, srcValBox2, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateICmpSLT(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFCmpOLT(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


Bool TargetGenerator::generateLessThanOrEqual(
  TiObject *context, TiObject *type, TiObject *srcVal1, TiObject *srcVal2, TioSharedPtr &result
) {
  PREPARE_ARG(context, block, Block);
  PREPARE_ARG(srcVal1, srcValBox1, Value);
  PREPARE_ARG(srcVal2, srcValBox2, Value);
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() == Type::Category::INT) {
    auto llvmResult = block->getIrBuilder()->CreateICmpSLE(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else if (tgType->getCategory() == Type::Category::FLOAT) {
    auto llvmResult = block->getIrBuilder()->CreateFCmpOLE(srcValBox1->getLlvmValue(), srcValBox2->getLlvmValue());
    result = std::make_shared<Value>(llvmResult, false);
    return true;
  } else {
    throw EXCEPTION(GenericException, STR("Invalid operation."));
  }
}


//==============================================================================
// Literal Generation Functions

Bool TargetGenerator::generateIntLiteral(
  TiObject *context, Word bitCount, Long value, TioSharedPtr &destVal
) {
  auto llvmResult = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitCount, value, true));
  destVal = std::make_shared<Value>(llvmResult, true);
  return true;
}


Bool TargetGenerator::generateFloatLiteral(
  TiObject *context, Word bitCount, Double value, TioSharedPtr &destVal
) {
  llvm::Value *llvmResult;
  if (bitCount == 32) {
    llvmResult = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat((Float)value));
  } else {
    llvmResult = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(value));
  }
  destVal = std::make_shared<Value>(llvmResult, true);
  return true;
}


Bool TargetGenerator::generateStringLiteral(
  TiObject *context, Char const* value, TiObject *charType, TiObject *strType, TioSharedPtr &destVal
) {
  PREPARE_ARG(charType, cgCharType, Type);
  PREPARE_ARG(strType, cgStrType, Type);

  Word len = Core::Basic::getStrLen(value);

  // Prepare the llvm constant array.
  std::vector<llvm::Constant*> llvmCharArray;
  llvmCharArray.reserve(len + 1);
  for (Word i = 0; i < len; i++) {
    llvmCharArray.push_back(llvm::ConstantInt::get(cgCharType->getLlvmType(), value[i]));
  }
  llvmCharArray.push_back(llvm::ConstantInt::get(cgCharType->getLlvmType(), 0));

  // Create an anonymous global variable.
  auto llvmStrConst = llvm::ConstantArray::get(static_cast<llvm::ArrayType*>(cgStrType->getLlvmType()), llvmCharArray);
  auto llvmVar = new llvm::GlobalVariable(
    *(this->llvmModule.get()), cgStrType->getLlvmType(), true,
    llvm::GlobalValue::PrivateLinkage, llvmStrConst, this->getAnonymouseVarName().c_str()
  );
  llvmVar->setAlignment(1);

  // Get a pointer to the global variable and return it as the value of the string.
  std::vector<llvm::Constant*> indices;
  llvm::ConstantInt *zero = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, 0));
  indices.push_back(zero);
  indices.push_back(zero);
  auto llvmResult = llvm::ConstantExpr::getGetElementPtr(llvmVar, indices);

  destVal = std::make_shared<Value>(llvmResult, true);
  return true;
}


Bool TargetGenerator::generateNullPtrLiteral(
  TiObject *context, TiObject *type, TioSharedPtr &destVal
) {
  PREPARE_ARG(type, tgType, Type);
  if (tgType->getCategory() != Type::Category::POINTER) {
    throw EXCEPTION(InvalidArgumentException, STR("type"), STR("Must have a category of POINTER."));
  }
  auto llvmResult = llvm::ConstantPointerNull::get(static_cast<llvm::PointerType*>(tgType->getLlvmType()));
  destVal = std::make_shared<Value>(llvmResult, true);
  return true;
}


//==============================================================================
// Helper Functions

Str TargetGenerator::getNewBlockName()
{
  return Str("__block") + std::to_string(this->blockIndex++);
}


Str TargetGenerator::getAnonymouseVarName()
{
  return Str("__anonymous") + std::to_string(this->anonymousVarIndex++);
}

} } // namespace
