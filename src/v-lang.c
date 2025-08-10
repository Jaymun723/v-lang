#include "ast/program.h"
#include "emitter.h"
#include "lexer.h"
#include "wasm/module.h"
#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE 0x123

const char *argp_program_version = "v-lang 0.0.0";
const char *argp_program_bug_address = "<jaymun723@yahoo.com>";
static char doc[] = "The V-Lang compiler.";
static char args_doc[] = "INPUT.v";

/* Options: only -o FILE for output, plus -v alias for version */
static struct argp_option options[] = {
    {"output", 'o', "FILE", 0,
     "Write output to FILE (default: derive from INPUT)", 0},
    {"version", 'v', 0, 0, "Print program version", -1},
    {"help", 'h', 0, 0, "Give this help list", -1},
    {"usage", USAGE, 0, 0, "Give a short usage message", 0},
    {0}};

struct arguments {
  char *input;
  char *output;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
  case 'o':
    arguments->output = arg;
    break;
  case 'v':
    /* Short alias for built-in --version */
    puts(argp_program_version);
    exit(0);
    break;
  case ARGP_KEY_ARG:
    if (state->arg_num >= 1) {
      argp_usage(state); /* too many args */
    }
    arguments->input = arg;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1) {
      argp_usage(state); /* missing input */
    }
    break;
  case 'h':
    argp_state_help(state, state->out_stream, ARGP_HELP_STD_HELP);
    break;
  case USAGE:
    // argp_state_help(state, state->out_stream,
    //                 ARGP_HELP_USAGE | ARGP_HELP_EXIT_OK);
    argp_usage(state);
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/* argp struct with all unused fields zeroed */
static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

/* helper: derive output filename from input (replace .v with .wasm) */
static char *derive_output(const char *input) {
  if (!input)
    return NULL;
  size_t len = strlen(input);
  const char *dot = strrchr(input, '.');
  size_t base_len = dot ? (size_t)(dot - input) : len;
  size_t out_len = base_len + strlen(".wasm") + 1;
  char *out = malloc(out_len);
  if (!out)
    return NULL;
  memcpy(out, input, base_len);
  strcpy(out + base_len, ".wasm");
  return out;
}

int main(int argc, char **argv) {
  struct arguments arguments = {NULL, NULL};

  argp_parse(&argp, argc, argv, ARGP_NO_HELP, 0, &arguments);

  /* If no explicit output given, derive one */
  char *derived_output = NULL;
  if (!arguments.output) {
    derived_output = derive_output(arguments.input);
    if (!derived_output) {
      fprintf(stderr, "Failed to allocate output filename\n");
      return 2;
    }
    arguments.output = derived_output;
  }

  /* Example: just print settings */
  // printf("Input : %s\n", arguments.input);
  // printf("Output: %s\n", arguments.output);

  FILE *inputFile = fopen(arguments.input, "rb");
  TokenList *tkl = tokenizeFromFile(inputFile);
  fclose(inputFile);
  if (removeSpaces(tkl)) {
    fprintf(stderr, "Syntax error: your file should not start with a space.\n");
    freeTkl(tkl, true);
    return 1;
  }
  WasmModule *module = createWasmModule();
  AstProgram *program = parseAstProgram(tkl);
  freeTkl(tkl, true);

  if (program == NULL) {
    freeWasmModule(module);
    return 1;
  }

  emit(module, program);

  FILE *outputFile = fopen(arguments.output, "wb");
  writeModule(outputFile, module);

  fclose(outputFile);
  freeAstProgram(program);
  freeWasmModule(module);

  free(derived_output);
  return 0;
}