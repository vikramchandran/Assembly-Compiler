#include <stdio.h>
#include "parser-errors.h"

// Helper Functions
int CheckImproperStatements(AST* ast, int is_for, int* incorrect_returns);

/*
  Tracks if when we have run out of tokens. Used to prevent redundant error
  messages.
*/
int eof_reached = 0;

/*
  Used to track the name of the current file. This is show if an unexpect eof
  occurs we can determine in what file it occurred.
*/
char* curr_filename;

/*
  Function that takes in an AST and determines if any error has occurred.
  Returns 0 if no errors are in the AST and otherwise returns a nonzero value.
*/
int CheckErrors(AST* ast) {
  int incorrect_returns = 0;
  return CheckImproperStatements(ast, 0, &incorrect_returns);
}

/*
  Function that takes in an AST and determines if any errors
  occurred in its creation. In addition to the AST, the function also contains
  parameters 'is_for', which is true if the current node is a descendent of a
  For Loop node, and 'incorrect_returns' which is pointer to an integer. This
  value is only useful for descendent of a func decl node that contains a
  body (a function definition). It should point to 0 if the function always
  successfully returns and should be nonzero otherwise.

  This function shouldreturn 0 if there are no errors and otherwise a nonzero
  value if there are ANY ERRORS. (Note that 'incorrect_returns' must be used to
  determine the return value of the function and simply setting it to the
  correct value will not be sufficient).

  There are 4 types of errors:

  1. An Error Node is present in the AST. This means an error occurred in
  parsing. There is no need to print any output message as one as already
  occurred during parsing.

  2. A Break statement outside a for loop. If a Break statement is encountered
  while
  'is_for' is false then an error has occurred. In a perfect world, our grammar
  would've
  been strict enough to only allow break statements to occur within for loops,
  but this would've complicated the grammar.
  You should display an output message to STDERR indicating the problem. This is
  not
  quired for grading but will be essential for debugging any tests you write.


  3. A Continue statement outside a for loop. As with Break statements, we
  could've
  prevented this from occuring with a more complex grammar.
  If a Continue statement is encountered while is_for is false then
  an error has occurred. You should display an output message to STDERR
  indicating the problem. This is not required for grading but will be
  essential for debugging any tests you write.

  4. A function does not always result in a return. Our language requires that
  all functions always return some value. You do not need to worry about type
  checking but all possible execution paths must always result in a return.
  Note that we do not analyze under what circumstances a function is taken and
  so we must always return without knowing what any expression evaluates to.
  If any function does not always return then an error has occurred. You should
  display an output message to STDERR indicating the problem. This is not
  required for grading but will be essential for debugging any tests you write.

  Hint 1: Checking if a function returns is complicated by if statements. For a
  function to always return as a result of an if statement both an if and an
  else must always return.

  Hint 2: Is it possible to know if you will enter a for loop without doing
  any evaluation?

*/


int CheckImproperStatements(AST* ast, int is_for, int* incorrect_returns) {
  /* YOUR CODE HERE */
  if (ast->type == NODETYPE_CONTROL_FOR) {
    is_for = 1;
  }

  if (ast->type == NODETYPE_ERR) {
    *incorrect_returns = *incorrect_returns + 1;
  }
  
  if (!is_for) {
    if(ast->type == NODETYPE_BREAK) {
      fprintf(stderr, "Error: Break statement outside of for loop");
      *incorrect_returns = *incorrect_returns + 1;
    }
    if (ast->type == NODETYPE_CONTINUE) {
      fprintf(stderr, "Error: Continue statement outside of for loop");
      *incorrect_returns = *incorrect_returns + 1;
    }
  }


  int returnpresent = 1;
  int autokay = 1;
  if(ast->type == NODETYPE_FUNC_DECL) {
    autokay = 0;
    for(int i = 0;i < ast->size; i ++) {
      if (ast->children[i]->type == NODETYPE_BLOCK) {
        for(int j = 0;j < ast->children[i]->size; j ++) {
          if (ast->children[i]->children[j]->type == NODETYPE_RETURN) {
            autokay = 1;
          }
        }
      }
    }
  }

  if(ast->type == NODETYPE_FUNC_DECL) {
    returnpresent = 0;
    int ifreturnoccurred = 0;
    for(int i = 0;i < ast->size; i ++) {
      if (ast->children[i]->type == NODETYPE_BLOCK) {
        for(int j = 0;j < ast->children[i]->size; j ++) {
          if (ast->children[i]->children[j]->type == NODETYPE_CONTROL_IF_ELSE) {
            fprintf(stderr, "Triggering first if/else blocks\n");
            int numblocks = 0;
            for(int k = 0; k < ast->children[i]->children[j]->size; k ++) {
              if (ast->children[i]->children[j]->children[k]->type == NODETYPE_BLOCK) {
                fprintf(stderr, "Just reached a block\n");
                numblocks += 1;
                fprintf(stderr, "Here is the block number: %d \n", numblocks);
                
                if(numblocks == 1) {
                  fprintf(stderr, "Entering num block 1 if statement\n");
                  for(int l = 0; l < ast->children[i]->children[j]->children[k]->size; l ++) {
                    if(ast->children[i]->children[j]->children[k]->children[l]->type == NODETYPE_RETURN) {
                      fprintf(stderr, "Just saw return within if statement\n");
                      //returnpresent = 1;
                      ifreturnoccurred = 1;
                    }
                  }
                }
                else if(numblocks == 2) {
                  fprintf(stderr, "Entering num block 2 - else statement. Converting returnpresent to 0\n");
                  returnpresent = 0;
                  for(int l = 0; l < ast->children[i]->children[j]->children[k]->size; l ++) {
                    if(ast->children[i]->children[j]->children[k]->children[l]->type == NODETYPE_RETURN) {
                      if(ifreturnoccurred == 1) {
                        fprintf(stderr, "Just saw return within else statement\n");
                        returnpresent = 1;
                      }
                    }
                  }
                }

                
                // for(int l = 0; l < ast->children[i]->children[j]->children[k]->size; l ++) {
                //   if(numblocks == 1) {
                //     fprintf(stderr, "Triggering first if return statement\n");
                //     if(ast->children[i]->children[j]->children[k]->children[l]->type == NODETYPE_RETURN) {
                //       returnpresent = 1;
                //     }
                //   }
                //   else if(numblocks == 2) {
                //     fprintf(stderr, "Triggering else statement within if return statement\n");
                //     returnpresent = 0;
                //     if (ast->children[i]->children[j]->children[k]->children[l]->type == NODETYPE_RETURN) {
                //       returnpresent = 1;
                //       //fprintf(stderr, "Triggering else statement within if statement");
                //     }
                //   }
                // }  
              

              }
            }
          }
        }
      }
    }
  }
  if (autokay == 0 && returnpresent == 0) {
    fprintf(stderr, "Return statement never occurred in function");
    *incorrect_returns += 1;
  }



  // if (ast->type == NODETYPE_FUNC_DECL) {
  //   int nodepresent = 0;
  //   int breakagain = 0;
  //   for(int i = 0;i < ast->size; i ++) {
  //     if (ast->children[i]->type == NODETYPE_BLOCK) {
  //       for(int j = 0;j < ast->children[i]->size; j ++) {
  //         if (ast->children[i]->children[j]->type == NODETYPE_RETURN) {
  //           nodepresent = 1;
  //           breakagain = 1;
  //           break;
  //         }
  //         else if (ast->children[i]->children[j]->type == NODETYPE_CONTROL_IF_ELSE) {
  //           for(int k = 0; k < ast->children[i]->children[j]->size; k ++) {
  //             int checkifelse = 0;
  //             if (ast->children[i]->children[j]->children[k]->type == NODETYPE_BLOCK) {
  //               checkifelse += 1;
  //               for(int l = 0; l < ast->children[i]->children[j]->children[k]->size; l ++) {
  //                 if(checkifelse == 1) {
  //                   if(ast->children[i]->children[j]->children[k]->children[l]->type == NODETYPE_RETURN) {
  //                     nodepresent = 1;
  //                   }
  //                 }
  //                 else if(checkifelse == 2) {
  //                   nodepresent = 0;
  //                   if(ast->children[i]->children[j]->children[k]->children[l]->type == NODETYPE_RETURN) {
  //                     nodepresent = 1;
  //                     }
  //                   }
  //                 }
  //               }
  //             }
  //           }
  //         }
  //       }
  //       if(breakagain == 1) {
  //         break;
  //       }
  //     }
  //   if (nodepresent == 0) {
  //     fprintf(stderr, "Return statement never occurred in function");
  //     *incorrect_returns += 1;
  //   }     
  // }
  
  for (int i = 0; i < ast->size; i ++) {
    *incorrect_returns += CheckImproperStatements(ast->children[i], is_for, incorrect_returns);
    }
  
  return *incorrect_returns;
  }



/*
  Generates the error message for when there are not enough tokens to continue
  parsing rules. It switches eof_reached to 1 to prevent redundant print
  statements.
*/
void generate_eof_error() {
  if (!eof_reached) {
    eof_reached = 1;
    fprintf(stderr, "Error: Unexpected end of file reached in %s.\n",
            curr_filename);
  }
}

/*
  Function to assist the output of error messages when a token is needed to
  complete a rule in the grammar but is not found.
*/
void generate_error_message(char* missing, char* filename, int linenum) {
  fprintf(stderr, "Error: Expected %s on line %d of %s, but none found.\n",
          missing, linenum, filename);
}
