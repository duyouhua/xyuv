/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 1 "parser.ypp"

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Stian Valentin Svedenborg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "minicalc.h"
#include "ast.h"
#include "operations.h"
#include <iostream>
#include <cassert>
#include <unordered_map>
#include <functional>

static const std::unordered_map<std::string, std::function<int(int, int)>> binary_functions{
        {"pow", minicalc_pow},
        {"next_multiple", minicalc_next_multiple},
        {"gcd", minicalc_gcd},
        {"lcm", minicalc_lcm}
};

static const std::unordered_map<std::string, std::function<int(int)>> unary_functions{
        {"abs", minicalc_abs},
};
#line 52 "parser.c"
#include "parser.h"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
#if INTERFACE
#define TOK_LOGIC_AND                       1
#define TOK_LOGIC_OR                        2
#define TOK_LOGIC_EQ                        3
#define TOK_LOGIC_NE                        4
#define TOK_LOGIC_LT                        5
#define TOK_LOGIC_GT                        6
#define TOK_LOGIC_LE                        7
#define TOK_LOGIC_GE                        8
#define TOK_PLUS                            9
#define TOK_MINUS                          10
#define TOK_DIV                            11
#define TOK_MUL                            12
#define TOK_MOD                            13
#define TOK_UNARY_MINUS                    14
#define TOK_LOGIC_NEG                      15
#define TOK_POW                            16
#define TOK_LPARENS                        17
#define TOK_RPARENS                        18
#define TOK_INT                            19
#define TOK_IDENTIFIER                     20
#define TOK_LPAREN                         21
#define TOK_COMMA                          22
#define TOK_RPAREN                         23
#define TOK_TRUE                           24
#define TOK_FALSE                          25
#define TOK_IF                             26
#define TOK_BOOL                           27
#endif
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    ParseTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 34
#define YYACTIONTYPE unsigned char
#if INTERFACE
#define ParseTOKENTYPE MiniCalc::Token*
#endif
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  node* yy35;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#if INTERFACE
#define ParseARG_SDECL  MiniCalc *owner ;
#define ParseARG_PDECL , MiniCalc *owner 
#define ParseARG_FETCH  MiniCalc *owner  = yypParser->owner 
#define ParseARG_STORE yypParser->owner  = owner 
#endif
#define YYNSTATE 68
#define YYNRULE 29
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
#define YY_ACTTAB_COUNT (147)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    15,   14,   13,   12,   11,   10,   25,   22,   24,   23,
 /*    10 */    21,   97,   97,   20,   25,   22,   24,   23,   21,   68,
 /*    20 */    65,   20,   27,   43,   15,   14,   13,   12,   11,   10,
 /*    30 */    25,   22,   24,   23,   21,   19,   40,   20,   98,   56,
 /*    40 */     2,   27,   44,    6,   55,   50,    4,   26,   45,   64,
 /*    50 */    63,   49,   48,   20,   25,   22,   24,   23,   21,   27,
 /*    60 */    46,   20,   25,   22,   24,   23,   21,   17,   58,   20,
 /*    70 */    18,   25,   22,   24,   23,   21,   66,    5,   20,   25,
 /*    80 */    22,   24,   23,   21,    9,   65,   20,   41,   25,   22,
 /*    90 */    24,   23,   21,   60,   54,   20,   25,   22,   24,   23,
 /*   100 */    21,    7,   19,   20,   24,   23,   21,    3,    1,   20,
 /*   110 */    59,   55,   50,   16,    3,    1,    3,    1,   49,   27,
 /*   120 */    61,   99,   99,   27,   47,   53,   42,   52,   67,   62,
 /*   130 */    51,   28,   33,   32,   39,    8,   38,   37,   57,   36,
 /*   140 */    35,   34,   99,   31,   99,   30,   29,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*    10 */    13,    1,    2,   16,    9,   10,   11,   12,   13,    0,
 /*    20 */    23,   16,   29,   30,    3,    4,    5,    6,    7,    8,
 /*    30 */     9,   10,   11,   12,   13,   10,   29,   16,   31,   32,
 /*    40 */    15,   29,   30,   21,   19,   20,   21,   29,   30,   24,
 /*    50 */    25,   26,   27,   16,    9,   10,   11,   12,   13,   29,
 /*    60 */    30,   16,    9,   10,   11,   12,   13,   22,   23,   16,
 /*    70 */    21,    9,   10,   11,   12,   13,   23,   21,   16,    9,
 /*    80 */    10,   11,   12,   13,   21,   23,   16,   29,    9,   10,
 /*    90 */    11,   12,   13,   23,   29,   16,    9,   10,   11,   12,
 /*   100 */    13,   22,   10,   16,   11,   12,   13,    1,    2,   16,
 /*   110 */    23,   19,   20,   21,    1,    2,    1,    2,   26,   29,
 /*   120 */    30,   33,   33,   29,   30,   29,   29,   29,   29,   23,
 /*   130 */    29,   29,   29,   29,   29,   22,   29,   29,   23,   29,
 /*   140 */    29,   29,   33,   29,   33,   29,   29,
};
#define YY_SHIFT_USE_DFLT (-4)
#define YY_SHIFT_COUNT (56)
#define YY_SHIFT_MIN   (-3)
#define YY_SHIFT_MAX   (115)
static const signed char yy_shift_ofst[] = {
 /*     0 */    92,   25,   25,   25,   25,   25,   25,   92,   92,   92,
 /*    10 */    92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
 /*    20 */    92,   92,   92,   92,   92,   92,   -3,   21,   45,   87,
 /*    30 */    79,   70,   62,   53,    5,    5,    5,    5,    5,    5,
 /*    40 */     5,   93,   93,  115,  113,  106,   10,   10,   63,   56,
 /*    50 */    49,   37,   37,   37,   37,   22,   19,
};
#define YY_REDUCE_USE_DFLT (-8)
#define YY_REDUCE_COUNT (25)
#define YY_REDUCE_MIN   (-7)
#define YY_REDUCE_MAX   (117)
static const signed char yy_reduce_ofst[] = {
 /*     0 */     7,   94,   90,   30,   18,   12,   -7,  117,  116,  114,
 /*    10 */   112,  111,  110,  108,  107,  105,  104,  103,  102,  101,
 /*    20 */    99,   98,   97,   96,   65,   58,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    97,   97,   97,   97,   97,   97,   97,   97,   97,   97,
 /*    10 */    97,   97,   97,   97,   97,   97,   97,   97,   97,   97,
 /*    20 */    97,   97,   97,   97,   97,   97,   97,   97,   97,   97,
 /*    30 */    97,   97,   97,   97,   90,   89,   88,   87,   86,   85,
 /*    40 */    69,   71,   74,   97,   97,   97,   92,   93,   97,   97,
 /*    50 */    78,   77,   75,   73,   72,   70,   97,   95,   80,   94,
 /*    60 */    96,   91,   84,   83,   82,   81,   79,   76,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "LOGIC_AND",     "LOGIC_OR",      "LOGIC_EQ",    
  "LOGIC_NE",      "LOGIC_LT",      "LOGIC_GT",      "LOGIC_LE",    
  "LOGIC_GE",      "PLUS",          "MINUS",         "DIV",         
  "MUL",           "MOD",           "UNARY_MINUS",   "LOGIC_NEG",   
  "POW",           "LPARENS",       "RPARENS",       "INT",         
  "IDENTIFIER",    "LPAREN",        "COMMA",         "RPAREN",      
  "TRUE",          "FALSE",         "IF",            "BOOL",        
  "error",         "expr",          "bool_expr",     "start",       
  "result",      
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "start ::= result",
 /*   1 */ "result ::= expr",
 /*   2 */ "expr ::= INT",
 /*   3 */ "expr ::= expr PLUS expr",
 /*   4 */ "expr ::= expr DIV expr",
 /*   5 */ "expr ::= expr MUL expr",
 /*   6 */ "expr ::= expr MINUS expr",
 /*   7 */ "expr ::= expr MOD expr",
 /*   8 */ "expr ::= expr POW expr",
 /*   9 */ "expr ::= MINUS expr",
 /*  10 */ "expr ::= IDENTIFIER",
 /*  11 */ "expr ::= IDENTIFIER LPAREN expr COMMA expr RPAREN",
 /*  12 */ "expr ::= IDENTIFIER LPAREN expr RPAREN",
 /*  13 */ "expr ::= LPAREN expr RPAREN",
 /*  14 */ "bool_expr ::= TRUE",
 /*  15 */ "bool_expr ::= FALSE",
 /*  16 */ "bool_expr ::= LPAREN bool_expr RPAREN",
 /*  17 */ "bool_expr ::= expr LOGIC_EQ expr",
 /*  18 */ "bool_expr ::= expr LOGIC_NE expr",
 /*  19 */ "bool_expr ::= expr LOGIC_LT expr",
 /*  20 */ "bool_expr ::= expr LOGIC_GT expr",
 /*  21 */ "bool_expr ::= expr LOGIC_LE expr",
 /*  22 */ "bool_expr ::= expr LOGIC_GE expr",
 /*  23 */ "bool_expr ::= LOGIC_NEG bool_expr",
 /*  24 */ "bool_expr ::= bool_expr LOGIC_AND bool_expr",
 /*  25 */ "bool_expr ::= bool_expr LOGIC_OR bool_expr",
 /*  26 */ "expr ::= IF LPAREN bool_expr COMMA expr COMMA expr RPAREN",
 /*  27 */ "expr ::= INT LPAREN bool_expr RPAREN",
 /*  28 */ "bool_expr ::= BOOL LPAREN expr RPAREN",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
      /* TERMINAL Destructor */
    case 1: /* LOGIC_AND */
    case 2: /* LOGIC_OR */
    case 3: /* LOGIC_EQ */
    case 4: /* LOGIC_NE */
    case 5: /* LOGIC_LT */
    case 6: /* LOGIC_GT */
    case 7: /* LOGIC_LE */
    case 8: /* LOGIC_GE */
    case 9: /* PLUS */
    case 10: /* MINUS */
    case 11: /* DIV */
    case 12: /* MUL */
    case 13: /* MOD */
    case 14: /* UNARY_MINUS */
    case 15: /* LOGIC_NEG */
    case 16: /* POW */
    case 17: /* LPARENS */
    case 18: /* RPARENS */
    case 19: /* INT */
    case 20: /* IDENTIFIER */
    case 21: /* LPAREN */
    case 22: /* COMMA */
    case 23: /* RPAREN */
    case 24: /* TRUE */
    case 25: /* FALSE */
    case 26: /* IF */
    case 27: /* BOOL */
{
#line 64 "parser.ypp"

    (void) owner;
    delete (yypminor->yy0);

#line 535 "parser.c"
}
      break;
    case 29: /* expr */
    case 30: /* bool_expr */
{
#line 73 "parser.ypp"

    delete (yypminor->yy35);

#line 545 "parser.c"
}
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from ParseAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_COUNT
   || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   ParseARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 31, 1 },
  { 32, 1 },
  { 29, 1 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 2 },
  { 29, 1 },
  { 29, 6 },
  { 29, 4 },
  { 29, 3 },
  { 30, 1 },
  { 30, 1 },
  { 30, 3 },
  { 30, 3 },
  { 30, 3 },
  { 30, 3 },
  { 30, 3 },
  { 30, 3 },
  { 30, 3 },
  { 30, 2 },
  { 30, 3 },
  { 30, 3 },
  { 29, 8 },
  { 29, 4 },
  { 30, 4 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;


  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 1: /* result ::= expr */
#line 83 "parser.ypp"
{
    owner->set_root(yymsp[0].minor.yy35);
}
#line 871 "parser.c"
        break;
      case 2: /* expr ::= INT */
#line 87 "parser.ypp"
{
    yygotominor.yy35 = create_node(yymsp[0].minor.yy0->value);
}
#line 878 "parser.c"
        break;
      case 3: /* expr ::= expr PLUS expr */
#line 91 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_add);   yy_destructor(yypParser,9,&yymsp[-1].minor);
}
#line 884 "parser.c"
        break;
      case 4: /* expr ::= expr DIV expr */
#line 92 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_div);   yy_destructor(yypParser,11,&yymsp[-1].minor);
}
#line 890 "parser.c"
        break;
      case 5: /* expr ::= expr MUL expr */
#line 93 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_mul);   yy_destructor(yypParser,12,&yymsp[-1].minor);
}
#line 896 "parser.c"
        break;
      case 6: /* expr ::= expr MINUS expr */
#line 94 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_sub);   yy_destructor(yypParser,10,&yymsp[-1].minor);
}
#line 902 "parser.c"
        break;
      case 7: /* expr ::= expr MOD expr */
#line 95 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_mod);   yy_destructor(yypParser,13,&yymsp[-1].minor);
}
#line 908 "parser.c"
        break;
      case 8: /* expr ::= expr POW expr */
#line 96 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_pow);   yy_destructor(yypParser,16,&yymsp[-1].minor);
}
#line 914 "parser.c"
        break;
      case 9: /* expr ::= MINUS expr */
#line 98 "parser.ypp"
{
    yygotominor.yy35 = create_node(yymsp[0].minor.yy35, minicalc_negate);
  yy_destructor(yypParser,10,&yymsp[-1].minor);
}
#line 922 "parser.c"
        break;
      case 10: /* expr ::= IDENTIFIER */
#line 101 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[0].minor.yy0->identifier, *owner); }
#line 927 "parser.c"
        break;
      case 11: /* expr ::= IDENTIFIER LPAREN expr COMMA expr RPAREN */
#line 103 "parser.ypp"
{
    auto it = binary_functions.find(yymsp[-5].minor.yy0->identifier);
    if (it != binary_functions.end()) {
        yygotominor.yy35 = create_node(yymsp[-3].minor.yy35, yymsp[-1].minor.yy35, it->second);
    }
    else {
        owner->parse_error("Undeclared function '" + yymsp[-5].minor.yy0->identifier + "'");
        yygotominor.yy35 = nullptr;
    }
  yy_destructor(yypParser,21,&yymsp[-4].minor);
  yy_destructor(yypParser,22,&yymsp[-2].minor);
  yy_destructor(yypParser,23,&yymsp[0].minor);
}
#line 944 "parser.c"
        break;
      case 12: /* expr ::= IDENTIFIER LPAREN expr RPAREN */
#line 115 "parser.ypp"
{
    auto it = unary_functions.find(yymsp[-3].minor.yy0->identifier);
    if (it != unary_functions.end()) {
        yygotominor.yy35 = create_node(yymsp[-1].minor.yy35, it->second);
    }
    else {
        owner->parse_error("Undeclared function '" + yymsp[-3].minor.yy0->identifier + "'");
        yygotominor.yy35 = nullptr;
    }
  yy_destructor(yypParser,21,&yymsp[-2].minor);
  yy_destructor(yypParser,23,&yymsp[0].minor);
}
#line 960 "parser.c"
        break;
      case 13: /* expr ::= LPAREN expr RPAREN */
      case 16: /* bool_expr ::= LPAREN bool_expr RPAREN */ yytestcase(yyruleno==16);
#line 126 "parser.ypp"
{ yygotominor.yy35 = yymsp[-1].minor.yy35;   yy_destructor(yypParser,21,&yymsp[-2].minor);
  yy_destructor(yypParser,23,&yymsp[0].minor);
}
#line 968 "parser.c"
        break;
      case 14: /* bool_expr ::= TRUE */
#line 128 "parser.ypp"
{ yygotominor.yy35 = create_node(1);   yy_destructor(yypParser,24,&yymsp[0].minor);
}
#line 974 "parser.c"
        break;
      case 15: /* bool_expr ::= FALSE */
#line 129 "parser.ypp"
{ yygotominor.yy35 = create_node(0);   yy_destructor(yypParser,25,&yymsp[0].minor);
}
#line 980 "parser.c"
        break;
      case 17: /* bool_expr ::= expr LOGIC_EQ expr */
#line 133 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_logic_eq);  yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 986 "parser.c"
        break;
      case 18: /* bool_expr ::= expr LOGIC_NE expr */
#line 134 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_logic_ne);  yy_destructor(yypParser,4,&yymsp[-1].minor);
}
#line 992 "parser.c"
        break;
      case 19: /* bool_expr ::= expr LOGIC_LT expr */
#line 135 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_logic_lt);  yy_destructor(yypParser,5,&yymsp[-1].minor);
}
#line 998 "parser.c"
        break;
      case 20: /* bool_expr ::= expr LOGIC_GT expr */
#line 136 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_logic_gt);  yy_destructor(yypParser,6,&yymsp[-1].minor);
}
#line 1004 "parser.c"
        break;
      case 21: /* bool_expr ::= expr LOGIC_LE expr */
#line 137 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_logic_le);  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 1010 "parser.c"
        break;
      case 22: /* bool_expr ::= expr LOGIC_GE expr */
#line 138 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_logic_ge);  yy_destructor(yypParser,8,&yymsp[-1].minor);
}
#line 1016 "parser.c"
        break;
      case 23: /* bool_expr ::= LOGIC_NEG bool_expr */
#line 139 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[0].minor.yy35, minicalc_logic_neg);  yy_destructor(yypParser,15,&yymsp[-1].minor);
}
#line 1022 "parser.c"
        break;
      case 24: /* bool_expr ::= bool_expr LOGIC_AND bool_expr */
#line 141 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_logic_and);  yy_destructor(yypParser,1,&yymsp[-1].minor);
}
#line 1028 "parser.c"
        break;
      case 25: /* bool_expr ::= bool_expr LOGIC_OR bool_expr */
#line 142 "parser.ypp"
{ yygotominor.yy35 = create_node(yymsp[-2].minor.yy35, yymsp[0].minor.yy35, minicalc_logic_or);  yy_destructor(yypParser,2,&yymsp[-1].minor);
}
#line 1034 "parser.c"
        break;
      case 26: /* expr ::= IF LPAREN bool_expr COMMA expr COMMA expr RPAREN */
#line 145 "parser.ypp"
{
    yygotominor.yy35 = create_if_node(yymsp[-5].minor.yy35, yymsp[-3].minor.yy35, yymsp[-1].minor.yy35);
  yy_destructor(yypParser,26,&yymsp[-7].minor);
  yy_destructor(yypParser,21,&yymsp[-6].minor);
  yy_destructor(yypParser,22,&yymsp[-4].minor);
  yy_destructor(yypParser,22,&yymsp[-2].minor);
  yy_destructor(yypParser,23,&yymsp[0].minor);
}
#line 1046 "parser.c"
        break;
      case 27: /* expr ::= INT LPAREN bool_expr RPAREN */
#line 150 "parser.ypp"
{
    yygotominor.yy35 = yymsp[-1].minor.yy35;
  yy_destructor(yypParser,19,&yymsp[-3].minor);
  yy_destructor(yypParser,21,&yymsp[-2].minor);
  yy_destructor(yypParser,23,&yymsp[0].minor);
}
#line 1056 "parser.c"
        break;
      case 28: /* bool_expr ::= BOOL LPAREN expr RPAREN */
#line 155 "parser.ypp"
{
    yygotominor.yy35 = yymsp[-1].minor.yy35;
  yy_destructor(yypParser,27,&yymsp[-3].minor);
  yy_destructor(yypParser,21,&yymsp[-2].minor);
  yy_destructor(yypParser,23,&yymsp[0].minor);
}
#line 1066 "parser.c"
        break;
      default:
      /* (0) start ::= result */ yytestcase(yyruleno==0);
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YYNSTATE + YYNRULE + 1 );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 57 "parser.ypp"

   owner->parse_error("Syntax error!");

#line 1133 "parser.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact<YYNSTATE ){
      assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
