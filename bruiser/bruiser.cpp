
/***************************************************Project Mutator****************************************************/
//-*-c++-*-
/*first line intentionally left blank.*/
/*the source code for the mutator code breaker.*/
/*Copyright (C) 2017 Farzad Sadeghi

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/
/**********************************************************************************************************************/
/*included modules*/
/*project headers*/
#include "bruiser.h"
#include "../mutator_aux.h"
/*standard headers*/
#include <string>
#include <iostream>
#include <regex>
/*LLVM headers*/
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Function.h"
/*other*/
#include "curses.h"
/**********************************************************************************************************************/
/*used namespaces*/
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
/**********************************************************************************************************************/
#define __DBG_1
#if 0
#undef __DBG_1
#endif
/**********************************************************************************************************************/
/*global vars*/
static llvm::cl::OptionCategory BruiserCategory("Empty");

bruiser::M0_ERR m0_err;
bruiser::BruiserReport BruiseRep;
/**********************************************************************************************************************/
cl::opt<bool> Intrusive("intrusive", cl::desc("If set true. bruiser will mutate the source."), cl::init(true), cl::cat(BruiserCategory), cl::ZeroOrMore);
cl::opt<std::string> M0XMLPath("xmlpath", cl::desc("tells bruiser where to find the XML file containing the Mutator-LVL0 report."), cl::init(bruiser::M0REP), cl::cat(BruiserCategory), cl::ZeroOrMore);
/**********************************************************************************************************************/
/*the implementation of the bruiser logger.*/
bruiser::BruiserReport::BruiserReport () 
{
  BruiserLog.open("bruiser.log");
}

bruiser::BruiserReport::~BruiserReport() 
{
  BruiserLog.close();
}

template <typename T>
/**
 * @brief Will print the argument in the log file. Expects to receive valid types usable for a stream.
 *
 * @param __arg
 *
 * @return Returns true if the write was successful, false otherwise.
 */
bool bruiser::BruiserReport::PrintToLog(T __arg)
{
  BruiserLog << __arg << "\n";
  return !BruiserLog.bad();
}
/**********************************************************************************************************************/
bruiser::TypeInfo::TypeInfo(const clang::ast_type_traits::DynTypedNode* __dtn) : DTN(__dtn) {};

bruiser::TypeInfo::~TypeInfo() {};

const clang::Type* bruiser::TypeInfo::getTypeInfo(clang::ASTContext* __astc)
{
  const clang::Expr* EXP = DTN->get<clang::Expr>();

  const clang::Type* TP = EXP->getType().getTypePtr();

  return __astc->getCanonicalType(TP);
}
/**********************************************************************************************************************/
class AbstractMatcherHandler : public virtual MatchFinder::MatchCallback
{
  public:
    AbstractMatcherHandler (Rewriter &Rewrite) : R(Rewrite) {}

  public:
    virtual void run(const MatchFinder::MatchResult &MR)
    {

    }

  private:
    Rewriter &R;
};
/**********************************************************************************************************************/
class MatcherHandlerLVL0 : public AbstractMatcherHandler
{
  public:
    explicit MatcherHandlerLVL0 (Rewriter &Rewrite) : AbstractMatcherHandler(Rewrite) {}

    virtual ~MatcherHandlerLVL0() {}

    virtual void run(const MatchFinder::MatchResult &MR) override
    {

    }

  private:
};
/**********************************************************************************************************************/
class NameFinder
{
  public:
    NameFinder () {}

    class runDeclRefExprMatcher
    {
      public:
        runDeclRefExprMatcher (Rewriter &__rwrt) : LVL0Proto(__rwrt), __rwrt(__rwrt) {}

        virtual void runMatcher(const StringRef __sr, ASTContext &__ctx)
        {
          Matcher.addMatcher(declRefExpr(to(namedDecl(hasName(__sr.str())))).bind("declrefexpbyname"), &LVL0Proto);
          Matcher.matchAST(__ctx);
        }

      private:
        MatchFinder Matcher;
        MatcherHandlerLVL0 LVL0Proto;
        Rewriter __rwrt;
        StringRef __sr;
    };

    class runNamedDeclMatcher
    {
      public:
        runNamedDeclMatcher (Rewriter &__rwrt) : LVL0Proto(__rwrt), __rwrt(__rwrt) {}

        virtual void runMatcher(const StringRef __sr, ASTContext &__ctx)
        {
          Matcher.addMatcher(declRefExpr(to(namedDecl(hasName(__sr.str())))).bind("nameddeclbyname"), &LVL0Proto);
          Matcher.matchAST(__ctx);
        }

      private:
        MatchFinder Matcher;
        MatcherHandlerLVL0 LVL0Proto;
        Rewriter __rwrt;
        StringRef __sr;
    };

  private:
};
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
class IfBreaker : public MatchFinder::MatchCallback 
{
  public:
    IfBreaker (Rewriter &Rewrite) : Rewrite(Rewrite) {}

    virtual void run(const MatchFinder::MatchResult &MR)
    {
      if (MR.Nodes.getNodeAs<clang::UnaryOperator>("uno") != nullptr)
      {
        const clang::UnaryOperator* UO = MR.Nodes.getNodeAs<clang::UnaryOperator>("uno");

        SourceLocation SL = UO->getLocStart();
        CheckSLValidity(SL);
        SL = Devi::SourceLocationHasMacro(SL, Rewrite);

        const Expr* EXP = UO->getSubExpr();
        const ast_type_traits::DynTypedNode DynNode = ast_type_traits::DynTypedNode::create<clang::Expr>(*EXP);
        bruiser::TypeInfo TIProto(&DynNode);

        const clang::Type* CTP [[maybe_unused]] = TIProto.getTypeInfo(MR.Context);

        NameFinder::runDeclRefExprMatcher DRENameMatcher(Rewrite);

        DRENameMatcher.runMatcher(StringRef(), *MR.Context);

      }

      if (MR.Nodes.getNodeAs<clang::BinaryOperator>("dous") != nullptr)
      {
        const clang::BinaryOperator* BO = MR.Nodes.getNodeAs<clang::BinaryOperator>("dous");

        SourceLocation SL = BO->getLocStart();
        CheckSLValidity(SL);
        SL = Devi::SourceLocationHasMacro(SL, Rewrite);

        const Expr* LHS = BO->getLHS();
        const Expr* RHS = BO->getRHS();

        QualType LQT = LHS->getType();
        QualType RQT = RHS->getType();

        const clang::Type* LTP = LQT.getTypePtr();
        const clang::Type* RTP = RQT.getTypePtr();

        const clang::Type* CLTP [[maybe_unused]] = MR.Context->getCanonicalType(LTP);
        const clang::Type* CRTP [[maybe_unused]] = MR.Context->getCanonicalType(RTP);
      }
    }

  private:
    Rewriter &Rewrite;
    MatchFinder Matcher;
};
/**********************************************************************************************************************/
/**
 * @brief Hijacks the main main and replaces it with bruiser's main.
 */
class MainWrapper : public MatchFinder::MatchCallback
{
public:
  MainWrapper (Rewriter &Rewrite) : Rewrite(Rewrite) {}

  virtual void run(const MatchFinder::MatchResult &MR)
  {
    if (MR.Nodes.getNodeAs<clang::FunctionDecl>("mainwrapper") != nullptr)
    {
      const FunctionDecl* FD = MR.Nodes.getNodeAs<clang::FunctionDecl>("mainwrapper");

      SourceLocation SL = FD->getLocStart();
      CheckSLValidity(SL);
      SL = Devi::SourceLocationHasMacro(SL, Rewrite);

      SourceLocation SLE = FD->getLocEnd();
      CheckSLValidity(SLE);
      SLE = Devi::SourceLocationHasMacro(SLE, Rewrite);

      SourceRange SR(SL, SLE);

      std::string MainSig = Rewrite.getRewrittenText(SR); 

      size_t mainbegin = MainSig.find("main");

      StringRef __sr("sub_main");

      Rewrite.ReplaceText(SL.getLocWithOffset(mainbegin), 4U, __sr);

      /*@DEVI-obviously the best way to do this is to use the main signature already used, instead of going with a general predefined one. the current form is a temp.*/
      Rewrite.InsertTextAfter(SLE.getLocWithOffset(1U), StringRef("\n\nint main(int argc, const char **argv)\n{\n\treturn sub_main(argc, argv);\n}\n"));

      BruiseRep.PrintToLog("hijacked main main.");
    }
  }

  private:
  Rewriter &Rewrite;
};
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
class BruiserASTConsumer : public ASTConsumer {

public:
  BruiserASTConsumer(Rewriter &R) : HIfBreaker(R), HMainWrapper(R)
  {}

  void HandleTranslationUnit(ASTContext &Context) override 
  {
    Matcher.addMatcher(ifStmt(hasDescendant(expr(anyOf(unaryOperator().bind("uno"), binaryOperator().bind("dous"))))), &HIfBreaker);

    Matcher.addMatcher(functionDecl(hasName("main")).bind("mainwrapper"), &HMainWrapper);

    Matcher.matchAST(Context);
  }

private:
  IfBreaker HIfBreaker;
  MainWrapper HMainWrapper;
  MatchFinder Matcher;
  Rewriter R;
};
/**********************************************************************************************************************/
class BruiserFrontendAction : public ASTFrontendAction 
{
public:
  BruiserFrontendAction() {}
  void EndSourceFileAction() override 
  {
    TheRewriter.getEditBuffer(TheRewriter.getSourceMgr().getMainFileID()).write(llvm::outs());
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override 
  {
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return llvm::make_unique<BruiserASTConsumer>(TheRewriter);
  }

private:
  Rewriter TheRewriter;
};
/**********************************************************************************************************************/
/*Main*/
int main(int argc, const char **argv) 
{
  int RunResult;
  bruiser::ShellHistory shHistory;
  int InKey;

  {
    char command[130];
    while(true)
    {
      std::cout << ">>";
      InKey = getch();
      std::cin.getline(command, sizeof(command));
      shHistory.History.push_back(command);
#if defined(__DBG_1)
      std::cout << InKey << "\n";
      std::cout << shHistory.History.size() << "\n";
      std::cout << shHistory.History.capacity() << "\n";
#endif

      if (InKey == KEY_UP)
      {
        //std::cout << shHistory.History[];
        std::cout << "caught key_up";
      }
      else if(InKey == KEY_DOWN)
      {
        std::cout << "caught key_down";
      }

      if (std::strcmp(command, "exit") == 0 || std::strcmp(command, "quit") == 0)
      {
        return 0;
      }

      if (std::strcmp(command, "m0") == 0)
      {
        BruiseRep.PrintToLog("bruiser exited with:");
        BruiseRep.PrintToLog(RunResult);

        bruiser::ReadM0 M0Rep;
        tinyxml2::XMLError XMLErr;

        XMLErr = M0Rep.LoadXMLDoc();
        if (XMLErr != XML_SUCCESS)
        {
          std::cout << RED << "could not load m0 xml report.\n" << NORMAL;
          std::cout << RED << "tinyxml2 returned " << XMLErr << NORMAL;
          return XMLErr;
        }

        XMLErr = M0Rep.ReadFirstElement();
        if (XMLErr != XML_SUCCESS)
        {
          std::cerr << RED << "could not read first element of m0 xml report.\n" << NORMAL;
          return XMLErr;
        }

        bruiser::SearchM0(M0Rep.getRootPointer());
        continue;
      }

      if (std::strcmp(command, "hijack main") == 0)
      {
        CommonOptionsParser op(argc, argv, BruiserCategory);
        ClangTool Tool(op.getCompilations(), op.getSourcePathList());

        RunResult = Tool.run(newFrontendActionFactory<BruiserFrontendAction>().get());
        std::cout << CYAN <<"hijacking main returned " << RunResult << "\n";
        continue;
      }

      if (std::strcmp(command, "clear") == 0)
      {
        std::cout << CLEAR;
        continue;
      }

      if (std::strcmp(command, "shell") == 0)
      {
        system("bash -i");
        continue;
      }

      if (std::strcmp(command, "help") == 0)
      {
        std::cout << BROWN << "not implemented yet.\n" << NORMAL;
        continue;
      }

      if (std::strcmp(command, "history") == 0)
      {
        unsigned int _cnt = 0;
        for (auto &it : shHistory.History)
        {
          _cnt++;
          std::cout << _cnt << "." << it << "\n";
        }

        continue;
      }

      if (command[0] == '!')
      {
        std::cout << BROWN << "not implemented yet.\n" << NORMAL;
        continue;
      }

      std::cout << RED << "unknown command. run help.\n" << NORMAL;
    }
  }
}
/*last line intentionally left blank.*/

