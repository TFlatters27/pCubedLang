#include "include/scope.h"
#include <stdio.h>
#include <string.h>

// Assuming a list of built-in method names
const char *builtin_methods[] = {
    "LEN",
    "POSITION",
    "SUBSTRING",
    "SLICE",
    "STRING_TO_INT",
    "STRING_TO_REAL",
    "INT_TO_STRING",
    "REAL_TO_STRING",
    "CHAR_TO_CODE",
    "CODE_TO_CHAR",
    "RANDOM_INT",
    NULL // End of the list marker
};

int is_builtin_method(const char *name)
{
  for (size_t i = 0; builtin_methods[i] != NULL; i++)
  {
    if (strcmp(name, builtin_methods[i]) == 0)
    {
      return 1; // Match found
    }
  }
  return 0; // No match
}

scope_ *init_scope(scope_ *parent_scope, const char *scope_name)
{
  scope_ *scope = calloc(1, sizeof(struct SCOPE_STRUCT));
  if (!scope)
  {
    fprintf(stderr, "Error: Memory allocation failed for scope initialization.\n");
    exit(EXIT_FAILURE);
  }

  if (parent_scope)
  {
    // Initialize lists from parent scope and copy elements
    scope->instantiation_definitions = init_ast_list();
    scope->variable_definitions = init_ast_list();

    // Copy parent instantiation_definitions list
    for (size_t i = 0; parent_scope->instantiation_definitions[i] != NULL; i++)
    {
      add_ast_to_list(&scope->instantiation_definitions, parent_scope->instantiation_definitions[i]);
    }

    // Copy parent variable_definitions list
    for (size_t i = 0; parent_scope->variable_definitions[i] != NULL; i++)
    {
      add_ast_to_list(&scope->variable_definitions, parent_scope->variable_definitions[i]);
    }
    // printf("Created new scope %p from parent scope %p\n", scope, parent_scope);
  }
  else
  {
    // Initialize empty lists
    scope->instantiation_definitions = init_ast_list();
    scope->variable_definitions = init_ast_list();
    // printf("Created new scope %p\n", scope);
  }

  scope->scope_name = scope_name;

  return scope;
}

void set_scope(ast_ *node, scope_ *scope)
{
  if (scope == NULL)
    return;
  if (node == NULL)
  {
    // break condition
    return;
  }

  // Assign the scope to the node
  node->scope = scope;

  // If node has child nodes, propagate the scope to them
  switch (node->type)
  {
  case AST_COMPOUND:
  {
    int i = 0;
    while (node->compound_value[i] != NULL)
    {
      set_scope(node->compound_value[i], scope);
      i++;
    }
  }
  break;
  case AST_ASSIGNMENT:
    if (node->lhs != NULL)
    {
      set_scope(node->lhs, scope);
    }
    if (node->rhs != NULL)
    {
      set_scope(node->rhs, scope);
    }
    break;
  case AST_ARITHMETIC_EXPRESSION:
  case AST_BOOLEAN_EXPRESSION:
    if (node->left != NULL)
    {
      set_scope(node->left, scope);
    }
    if (node->right != NULL)
    {
      set_scope(node->right, scope);
    }
    break;
  case AST_ARRAY_ACCESS:
  {
    int i = 0;
    while (node->index[i] != NULL)
    {
      set_scope(node->index[i], scope);
      i++;
    }
  }
  break;
  case AST_INSTANTIATION:
  {
    int i = 0;
    while (node->arguments[i] != NULL)
    {
      set_scope(node->arguments[i], scope);
      i++;
    }
  }
  break;
  case AST_SUBROUTINE:
  {
    // Set scope for parameters and body
    int i = 0;
    while (node->parameters[i] != NULL)
    {
      set_scope(node->parameters[i], scope);
      i++;
    }
    i = 0;
    while (node->body[i] != NULL)
    {
      set_scope(node->body[i], scope);
      i++;
    }
  }
  break;
  case AST_DEFINITE_LOOP:
  case AST_INDEFINITE_LOOP:
    if (node->loop_variable != NULL)
    {
      set_scope(node->loop_variable, scope);
    }
    if (node->collection_expr != NULL)
    {
      set_scope(node->collection_expr, scope);
    }
    if (node->end_expr != NULL)
    {
      set_scope(node->end_expr, scope);
    }
    if (node->step_expr != NULL)
    {
      set_scope(node->step_expr, scope);
    }
    {
      int i = 0;
      while (node->loop_body[i] != NULL)
      {
        set_scope(node->loop_body[i], scope);
        i++;
      }
    }
    break;
  case AST_SELECTION:
    if (node->if_condition != NULL)
    {
      set_scope(node->if_condition, scope);
    }
    {
      int i = 0;
      while (node->if_body[i] != NULL)
      {
        set_scope(node->if_body[i], scope);
        i++;
      }
    }

    // Set scope for ELSE IF conditions and bodies
    if (node->else_if_conditions != NULL && node->else_if_bodies != NULL)
    {
      int i = 0;
      while (node->else_if_conditions[i] != NULL && node->else_if_bodies[i] != NULL)
      {
        set_scope(node->else_if_conditions[i], scope);

        int j = 0;
        while (node->else_if_bodies[i][j] != NULL)
        {
          set_scope(node->else_if_bodies[i][j], scope);
          j++;
        }
        i++;
      }
    }

    // Set scope for ELSE body
    if (node->else_body != NULL)
    {
      int i = 0;
      while (node->else_body[i] != NULL)
      {
        set_scope(node->else_body[i], scope);
        i++;
      }
    }
    break;
  case AST_OUTPUT:
  {
    int i = 0;
    while (node->output_expressions[i] != NULL)
    {
      set_scope(node->output_expressions[i], scope);
      i++;
    }
  }
  case AST_RETURN:
  {
    set_scope(node->return_value, scope);
  }
  default:
    break;
  }
}

scope_ *get_scope(ast_ *node)
{
  if (node == NULL)
  {
    fprintf(stderr, "Error: Invalid node\n");
    return NULL;
  }

  if (node->scope == NULL)
  {
    return NULL;
  }

  return node->scope;
}

ast_ *scope_add_instantiation_definition(scope_ *scope, ast_ *idef)
{
  if (!scope || !idef)
  {
    if (idef->type == AST_RECORD_DEFINITION)
    {
      fprintf(stderr, "Error: Invalid scope or record definition for record '%s'.\n", idef->record_name);
    }
    else if (idef->type == AST_SUBROUTINE)
    {
      fprintf(stderr, "Error: Invalid scope or subroutine definition for subroutine '%s'.\n", idef->subroutine_name);
    }
    else
    {
      fprintf(stderr, "Error: Invalid instantiation definition type.\n");
    }
    exit(EXIT_FAILURE);
  }

  // Check if the record_name or subroutine_name matches a built-in method
  if (idef->type == AST_RECORD_DEFINITION && is_builtin_method(idef->record_name))
  {
    fprintf(stderr, "Error: Record '%s' conflicts with a built-in method name.\n", idef->record_name);
    exit(EXIT_FAILURE);
  }
  else if (idef->type == AST_SUBROUTINE && is_builtin_method(idef->subroutine_name))
  {
    fprintf(stderr, "Error: Subroutine '%s' conflicts with a built-in method name.\n", idef->subroutine_name);
    exit(EXIT_FAILURE);
  }

  // Check for conflicting record or subroutine names
  for (size_t i = 0; scope->instantiation_definitions[i] != NULL; i++)
  {
    ast_ *existing_def = scope->instantiation_definitions[i];

    // Check for record name conflicts
    if (idef->type == AST_RECORD_DEFINITION)
    {
      if (existing_def->type == AST_RECORD_DEFINITION &&
          strcmp(existing_def->record_name, idef->record_name) == 0)
      {
        fprintf(stderr, "Error: Record '%s' already exists in scope `%s`.\n", idef->record_name, scope->scope_name);
        exit(EXIT_FAILURE);
      }
      else if (existing_def->type == AST_SUBROUTINE &&
               strcmp(existing_def->subroutine_name, idef->record_name) == 0 &&
               existing_def->parameter_count == idef->field_count)
      {
        // A record cannot have the same name and field count as an existing subroutine
        fprintf(stderr, "Error: Record '%s' with %d fields conflicts with subroutine '%s' with %d parameters in scope `%s`.\n",
                idef->record_name, idef->field_count, existing_def->subroutine_name, existing_def->parameter_count, scope->scope_name);
        exit(EXIT_FAILURE);
      }
    }

    // Check for subroutine name conflicts
    else if (idef->type == AST_SUBROUTINE)
    {
      if (existing_def->type == AST_RECORD_DEFINITION &&
          strcmp(existing_def->record_name, idef->subroutine_name) == 0 &&
          existing_def->field_count == idef->parameter_count)
      {
        // A subroutine cannot have the same name and parameter count as an existing record
        fprintf(stderr, "Error: Subroutine '%s' with %d parameters conflicts with record '%s' with %d fields in scope `%s`.\n",
                idef->subroutine_name, idef->parameter_count, existing_def->record_name, existing_def->field_count, scope->scope_name);
        exit(EXIT_FAILURE);
      }
      else if (existing_def->type == AST_SUBROUTINE &&
               strcmp(existing_def->subroutine_name, idef->subroutine_name) == 0 &&
               existing_def->parameter_count == idef->parameter_count)
      {
        // A subroutine cannot have the same name and parameter count as an existing subroutine
        fprintf(stderr, "Error: Subroutine '%s' with %d parameters already exists in scope `%s`.\n",
                idef->subroutine_name, idef->parameter_count, scope->scope_name);
        exit(EXIT_FAILURE);
      }
    }
  }

  // Add the record or subroutine definition to the list
  add_ast_to_list(&scope->instantiation_definitions, idef);

  return idef;
}

ast_ *scope_get_instantiation_definition(scope_ *scope, const char *iname)
{
  if (!scope || !iname)
  {
    fprintf(stderr, "Error: Invalid scope or instantiation name.\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; scope->instantiation_definitions[i] != NULL; i++)
  {
    ast_ *idef = scope->instantiation_definitions[i];

    if (idef->type == AST_SUBROUTINE && strcmp(idef->subroutine_name, iname) == 0)
    {
      return idef; // Subroutine found
    }
    else if (idef->type == AST_RECORD_DEFINITION && strcmp(idef->record_name, iname) == 0)
    {
      return idef; // Record found
    }
  }
  fprintf(stderr, "Error: No instantiation definition found for '%s' in scope `%s`\n", iname, scope->scope_name);
  exit(EXIT_FAILURE);
}

ast_ *scope_add_variable_definition(scope_ *scope, ast_ *vdef)
{
  if (!scope || !vdef)
  {
    fprintf(stderr, "Error: Invalid scope or variable definition.\n");
    exit(EXIT_FAILURE);
  }

  // Variable name is accessed via vdef->lhs->variable_name
  const char *new_var_name = vdef->lhs->variable_name;

  // Check if user input is required for the new variable
  if (vdef->lhs->userinput == 1)
  {
    printf("%s <- ", new_var_name);

    // Allocate buffer for user input
    char input_buffer[1024]; // Adjust size as needed
    if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL)
    {
      // Remove newline character if present
      size_t len = strlen(input_buffer);
      if (len > 0 && input_buffer[len - 1] == '\n')
      {
        input_buffer[len - 1] = '\0';
      }

      // Store the input as the variable's value (in vdef->rhs or a suitable location)
      vdef->rhs->string_value = strdup(input_buffer); // Assuming rhs stores a string_value
    }
  }

  // Iterate through the existing variable definitions to check for overwriting
  for (size_t i = 0; scope->variable_definitions[i] != NULL; i++)
  {
    if (strcmp(scope->variable_definitions[i]->lhs->variable_name, new_var_name) == 0)
    {
      // Check if the existing variable is a constant
      if (scope->variable_definitions[i]->lhs->constant == 1)
      {
        fprintf(stderr, "Error: Cannot overwrite constant variable '%s'.\n", new_var_name);
        exit(EXIT_FAILURE);
      }

      // Overwrite the existing variable definition
      // printf("Overwriting variable '%s' in scope %p\n", new_var_name, scope);
      scope->variable_definitions[i]->lhs = vdef->lhs;
      scope->variable_definitions[i]->rhs = vdef->rhs;
      return scope->variable_definitions[i]; // Return the overwritten variable
    }
  }

  // If no existing variable was found, add the new definition to the list
  add_ast_to_list(&scope->variable_definitions, vdef);
  // printf("Adding variable %s to scope %p\n", vdef->lhs->variable_name, scope);

  return vdef;
}

ast_ *scope_get_variable_definition(scope_ *scope, const char *vname)
{
  if (!scope || !vname)
  {
    fprintf(stderr, "Error: Invalid scope or variable name.\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; scope->variable_definitions[i] != NULL; i++)
  {
    if (strcmp(scope->variable_definitions[i]->lhs->variable_name, vname) == 0)
    {
      // printf("Found variable %s in scope %p with value %d\n", vname, scope, scope->variable_definitions[i]->rhs->int_value.value);
      return scope->variable_definitions[i]->rhs;
    }
  }

  fprintf(stderr, "Error: Variable definition `%s` not found in scope `%s`.\n", vname, scope->scope_name);
  exit(EXIT_FAILURE); // Variable not found
}
