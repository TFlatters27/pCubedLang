#ifndef AST_H
#define AST_H

#include <stdlib.h>

// Enum representing the different types of AST nodes.
enum ast_type {
  AST_INT,                // For integer literals
  AST_STRING,             // For string literals
  AST_BOOL,               // For boolean literals
  AST_CHAR,               // For character literals
  AST_REAL,               // For real literals
  AST_ASSIGNMENT,         // For variable assignments
  AST_INSTANTIATION,      // For object instantiation or function calls
  AST_COMPOUND,           // For compound statements (e.g., blocks of code)
  AST_NOOP,               // No operation
  AST_VARIABLE,           // For variable references
  AST_ARRAY,              // For array literals
  AST_BINARY_OP,          // For binary operations (e.g., +, -, *)
  AST_RELATIONAL_OP,      // For relational operations (e.g., <, >, <=, >=, =, !=)
  AST_UNARY_OP,           // For unary operations (e.g., -)
  AST_RECORD,             // For record creation
  AST_RECORD_ACCESS,      // For accessing a field in a record
  AST_SUBROUTINE,         // For subroutine definitions
  AST_RETURN,             // For RETURN statements in subroutines
  AST_OUTPUT,             // For OUTPUT statements
  AST_DEFINITE_LOOP,      // For count controlled loops (FOR loops)
  AST_INDEFINITE_LOOP,    // For condition controlled loops (REPEAT & WHILE loops)
};

// Structure representing an element (field) in a record.
typedef struct AST_RECORD_ELEMENT_STRUCT {
  char *element_name;         // Name of the element (e.g., "name", "age")
  struct AST_STRUCT *type;    // Type of the element (e.g., AST_STRING, AST_ARRAY)
} ast_record_element_;

// Structure representing an Abstract Syntax Tree (AST) node.
typedef struct AST_STRUCT {
  enum ast_type type;             // Type of AST node
  struct SCOPE_STRUCT *scope;     // Scope the AST node belongs to

  /* AST_LITERAL */
  int int_value;                  // Integer value (if this node is an integer literal)
  char *string_value;             // String value (if this node is a string literal)
  int bool_value;                 // Boolean value (if this node is a boolean literal)
  char char_value;                // Character value (if this node is a character literal)
  float real_value;               // Real value (if this node is a floating point literal)

  /* AST_VARIABLE */
  char *variable_name;            // Name of the variable (if this node is a variable reference)

  /* AST_ASSIGNMENT */
  struct AST_STRUCT *lhs;         // Left-hand side value to assign (variable or record access)
  struct AST_STRUCT *rhs;         // Right-hand side value to assign (expression being assigned)
  int constant;                   // Whether the assignment is a constant

  /* AST_INSTANTIATION / AST_FUNCTION_CALL */
  char *instantiated_type;        // Name of the type or function being instantiated/called
  struct AST_STRUCT **arguments;  // List of arguments (for function calls/instantiations)
  size_t arguments_size;          // Number of arguments

  /* AST_COMPOUND */
  struct AST_STRUCT **compound_value; // List of statements (if this node is a compound statement)
  size_t compound_size;               // Number of statements in the compound

  /* AST_ARRAY */
  struct AST_STRUCT **elements;   // List of elements (if this node is an array literal)
  size_t elements_size;           // Number of elements in the array

  /* AST_BINARY_OP */
  struct AST_STRUCT *left;        // Left operand (if this node is a binary operation)
  struct AST_STRUCT *right;       // Right operand (if this node is a binary operation)
  char *binary_op;                // Operator as a string (e.g., "+", "-", "*", "DIV", "MOD")

  /* AST_UNARY_OP */
  struct AST_STRUCT *operand;     // Operand (if this node is a unary operation)
  char *unary_op;                 // Operator as a string (e.g., "-")

  /* AST_RECORD */
  char *record_name;                  // Name of the record (e.g., "Student")
  ast_record_element_ **record_elements; // List of element declarations (e.g., fields like "name", "age")
  size_t record_elements_size;         // Number of element declarations

  /* AST_RECORD_ACCESS */
  struct AST_STRUCT *record;          // The record being accessed (e.g., student1)
  char *field_name;                   // The field being accessed (e.g., "passed")

  /* AST_SUBROUTINE */
  char *subroutine_name;            // Name of the subroutine
  struct AST_STRUCT **parameters;   // List of parameters (AST_VARIABLE nodes)
  size_t parameters_size;           // Number of parameters
  struct AST_STRUCT **body;         // Body of the subroutine (List of AST nodes)
  size_t body_size;                 // Number of statements in the body

  /* AST_RETURN */
  struct AST_STRUCT *return_value;  // Return value (if any) of the subroutine

  /* AST_OUTPUT */
  struct AST_STRUCT **output_expressions; // List of expressions to be output
  size_t output_expressions_size;         // Number of output expressions

  /* AST_DEFINITE_LOOP */
  struct AST_STRUCT *loop_variable;   // Variable being iterated
  struct AST_STRUCT *start_expr;      // Start expression (if FOR <var> <- start TO end [STEP step])
  struct AST_STRUCT *end_expr;        // End expression (if FOR <var> <- start TO end [STEP step])
  struct AST_STRUCT *step_expr;       // Step expression (if any)
  struct AST_STRUCT *collection_expr; // Collection to iterate over (if FOR <var> IN collection)
  struct AST_STRUCT **loop_body;      // Body of the loop (list of statements)
  size_t loop_body_size;              // Number of statements in the loop body

  /* AST_INDEFINITE_LOOP */
  struct AST_STRUCT *condition;   // Condition to evaluate for the loop (e.g., "a < 4")
} ast_;

// Function to initialize an AST node of a given type.
ast_ *init_ast(enum ast_type type);

// Function to convert an AST type to a string representation.
const char *ast_type_to_string(enum ast_type type);

#endif
