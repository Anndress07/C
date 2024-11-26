#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct {
  char* buffer;
  size_t buffer_lenght;
  ssize_t input_lenght;
} InputBuffer;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;


InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_lenght = 0;
  input_buffer->input_lenght = 0;

  return input_buffer;

}

void print_prompt() { printf("db > "); }

ssize_t getline(char **lineptr, size_t *n, FILE *stream);

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_lenght), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  input_buffer->input_lenght = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;

}

void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);  
}
/*
This function will return either META_COMMAND_SUCCESS or META_COMMAND_UNRECOGNIZED_COMMAND, values defined in the MetaCommandResult enum.
If the value on the input buffer is equal to ".exit", it ceases execution.
*/
MetaCommandResult do_meta_command(InputBuffer * input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}


/* Returns either PREPARE_SUCCESS or PREPARE_UNRECOGNIZED_STATEMENT, defined in PrepareResult enum. 
As input, takes the pointer to the input buffer, and a pointer to the statement struct, which is supposed to hold the SQL statement. 
*/

PrepareResult prepare_statement(InputBuffer* input_buffer,
                                Statement* statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) { 
      // checks if first 6 elements in buffer equal to "insert"
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
      printf("This is where we would do an insert.\n");
      break;
    case (STATEMENT_SELECT):
      printf("This is where we would do a select.\n");
      break;
  }
}

int main(int argc, char* argv[]) {
  InputBuffer*  input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    // commands that start with an "." are metacommands
      if (input_buffer->buffer[0] == '.') {
        switch (do_meta_command(input_buffer)) {
          case (META_COMMAND_SUCCESS):
            continue;
          case (META_COMMAND_UNRECOGNIZED_COMMAND):
            printf("Unrecognized command '%s'.\n", input_buffer->buffer);
            continue;
        }
      }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case (PREPARE_SUCCESS):
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
        continue;
      }


      execute_statement(&statement);
      printf("Executed.\n");

  }
}
