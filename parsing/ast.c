#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"

/*
   Creates a new AST from a given type filename and linenum. You should not
   assume that filename will remain a legal pointer after this function
   terminates.
*/
AST* MakeAST(enum NodeType type, char* filename, int linenum) {
  /* YOUR CODE HERE. */
  AST* treenew = (AST *)malloc(sizeof(AST) * 1);
  treenew->size = 0;
  treenew->type = type;
  treenew->linenum = linenum;
  treenew->capacity = INITIAL_CAPACITY;
  int filelength = strlen(filename);
  treenew->filename = (char *)malloc(sizeof(char) * (filelength +1));
  for (int i = 0; i < filelength; i++) {
    treenew->filename[i] = filename[i];
  }
  treenew->filename[filelength] = '\0';
  treenew->children = (AST**)malloc(sizeof(AST*) * treenew->capacity);
  return treenew;
}

/*
   Takes in a given AST and mallocs a new AST with the exact same details as the
   original. Again you should assume the original may be freed at any point
   after this function terminates.
*/
AST* CopyAST(AST* original) {
  AST* ast = MakeAST(original->type, original->filename, original->linenum);
  ast->size = original->size;
  ast->capacity = original->capacity;
  ast->children = (AST**)realloc(ast->children, sizeof(AST*) * ast->capacity);
  if (ast->children == NULL) {
    allocation_failed();
  }
  for (int i = 0; i < ast->size; i++) {
    ast->children[i] = CopyAST(original->children[i]);
  }

  /* Start Unique to Copy */
  if (ast->type == NODETYPE_ID) {
    ast->data.identifier =
        (char*)malloc(sizeof(char) * (strlen(original->data.identifier) + 1));
    if (ast->data.identifier == NULL) {
      allocation_failed();
    }
    strcpy(ast->data.identifier, original->data.identifier);
  } else if (ast->type == NODETYPE_CONSTANT_STRING) {
    ast->data.string =
        (char*)malloc(sizeof(char) * (strlen(original->data.string) + 1));
    if (ast->data.string == NULL) {
      allocation_failed();
    }
    strcpy(ast->data.string, original->data.string);
  }
  /* End of Unique to Copy */
  return ast;
}

/*
   Takes in an two ASTs and concatenates the two by making node a child
   of tree.
*/
void AppendAST(AST* tree, AST* node) {
  /* YOUR CODE HERE */
  int newsize = tree->size + 1;
  if (tree->capacity <= newsize) {
    tree->children = (AST**)realloc(tree->children, (newsize + 1) * sizeof(AST*));
    tree->capacity = (newsize + 1);
  }
  tree->children[tree->size] = node;
  tree->size = newsize;
}

/*
   Frees the memory allocated by a single AST node.
*/
void FreeNode(AST* ast) {
  /* YOUR CODE HERE */
  // free(ast->size);
  // free(ast->capacity);
  free(ast->filename);
  free(ast->children);
  if (ast->type == NODETYPE_CONSTANT_STRING) {
    free(ast->data.string);
  }
  else if (ast->type == NODETYPE_ID) {
    free(ast->data.identifier);
  }
  free(ast); 
}

/*
   Frees all the memory allocated by an AST.
*/
void FreeAST(AST* ast) {
  /* YOUR CODE HERE */
  int i = 0;
  int astsize = ast->size;
  while(i < astsize) {
    FreeAST(ast->children[i]);
    i++;
  }
  FreeNode(ast);
}
