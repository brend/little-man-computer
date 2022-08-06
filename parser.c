#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH     1024
#define PROGRAM_LENGTH  100
#define LABEL_LENGTH    100

#define IS_WHITESPACE(c)   ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define FIND_WHITESPACE 0
#define FIND_NON_WHITESPACE 1

#define INVALID_INST(i) ((int)(i) < 0)

typedef struct {
    INSTRUCTION inst;
    char *operand;
    char *label;
} TOKEN;

typedef struct {
    char *label;
    int mailbox;
} ADDR;

int address(const ADDR *addresses, int addrc, const char *label) {
    for (int i  = 0; i < addrc; ++i) {
        if (strcmp(label, addresses[i].label) == 0) {
            return addresses[i].mailbox;
        }
    }

    return -1;
}

int find_next_whitespace(const char *s, int start, int find_none_whitespace) {
    int n = strlen(s);
    int i;

    for (i = start; i < n; ++i) {
        if (IS_WHITESPACE(s[i]) != find_none_whitespace) {
            return i;
        }
    }

    return i;
}

char *copy_substring(const char *s, int j, int k) {
    size_t length = k - j;
    char *word = (char *) malloc(sizeof(char) * (length + 1));

    strncpy(word, s + j, length);
    word[length + 1] = 0;

    return word;
}

char **split(const char *line) {
    char **words = malloc(sizeof(char *) * 3);
    int word_count = 0;
    int max = strlen(line);

    words[0] = words[1] = words[2] = 0;

    int k, j = 0;
    int ws = IS_WHITESPACE(line[0]) ? FIND_WHITESPACE : FIND_NON_WHITESPACE;

    while ((k = find_next_whitespace(line, j, ws = !ws)) < max) {
        if (word_count >= 3) {
            fprintf(stderr, "*** TOO MANY TOKENS\n");
            exit(ERR_TOO_MANY_TOKENS);
        }

        if (!IS_WHITESPACE(line[j])) {
            char *word = copy_substring(line, j, k);

            words[word_count++] = word;
        }

        j = k;
    }

    if (j != k && !IS_WHITESPACE(line[j])) {
        char *word = copy_substring(line, j, k);

        words[word_count++] = word;
    }

    return words;
}

INSTRUCTION parse_inst(const char *s) {
    if (strcmp(s, "ADD") == 0) {
        return OP_ADD;
    }
    if (strcmp(s, "SUB") == 0) {
        return OP_SUB;
    }
    if (strcmp(s, "STA") == 0) {
        return OP_STA;
    }
    if (strcmp(s, "LDA") == 0) {
        return OP_LDA;
    }
    if (strcmp(s, "BRA") == 0) {
        return OP_BRA;
    }
    if (strcmp(s, "BRZ") == 0) {
        return OP_BRZ;
    }
    if (strcmp(s, "BRP") == 0) {
        return OP_BRP;
    }
    if (strcmp(s, "INP") == 0) {
        return OP_INP;
    }
    if (strcmp(s, "OUT") == 0) {
        return OP_OUT;
    }
    if (strcmp(s, "HLT") == 0) {
        return OP_HLT;
    }
    if (strcmp(s, "DAT") == 0) {
        return OP_DAT;
    }
    
    return -1;
}

TOKEN tokenize_line(const char *line) {
    TOKEN t = {0};
    char **words = split(line);
    int word_count = 0;

    while (words && words[word_count]) {
        ++word_count;
    }

    switch (word_count) {
    case 0:
        return t;
    case 1:
        t.inst = parse_inst(words[0]);
        break;
    case 2:
        t.inst = parse_inst(words[0]);

        if (!INVALID_INST(t.inst)) {
            t.operand = words[1];
        } else {
            t.label = words[0];
            t.inst = parse_inst(words[1]);
        }
        break;
    case 3:
        t.label = words[0];
        t.inst = parse_inst(words[1]);
        t.operand = words[2];
        break;
    }

    if (INVALID_INST(t.inst)) {
        fprintf(stderr, "*** INSTRUCTION EXPECTED\n");
        exit(ERR_UNKNOWN_INSTRUCTION);
    }

    return t;
}

void tokenize(FILE *file, TOKEN tokens[PROGRAM_LENGTH], int *program_length) {
    char line[LINE_LENGTH];
    int i = 0;

    while (fgets(line, LINE_LENGTH, file)) {
        if (i > PROGRAM_LENGTH) {
            fprintf(stderr, "*** PROGRAM TOO LONG");
            exit(ERR_PROGRAM_TOO_LONG);
        }

        tokens[i++] = tokenize_line(line);

        // printf("TOKEN(label: %s, inst: %i, operand: %s)\n", tokens[i-1].label, tokens[i-1].inst, tokens[i-1].operand);
    }

    *program_length = i;
}

void build_addresses(ADDR addresses[100], TOKEN tokens[PROGRAM_LENGTH], int program_length, int *addrc) {
    *addrc = 0;

    for (int k = 0; k < program_length; ++k) {
        if (tokens[k].label) {
            addresses[*addrc].label = tokens[k].label;
            addresses[*addrc].mailbox = k;
            *addrc = *addrc + 1;
        }
    }
}

void compile(
    TOKEN tokens[PROGRAM_LENGTH], 
    int program_length, 
    ADDR addresses[100], 
    int addrc, 
    LMC *lmc
) {
    for (int k = 0; k < program_length; ++k) {
        TOKEN t = tokens[k];

        switch (t.inst) {
        case OP_INP:
            lmc->mailbox[k] = 901;
            break;
        case OP_OUT:
            lmc->mailbox[k] = 902;
            break;
        case OP_HLT:
            lmc->mailbox[k] = 0;
            break;
        case OP_DAT:
            lmc->mailbox[address(addresses, addrc, t.label)] = t.operand ? atoi(t.operand) : 0;
            break;
        default:
            lmc->mailbox[k] = t.inst * 100 + address(addresses, addrc, t.operand);
            break;
            
        }
    }
}

LMC parse(const char *filename) {
    LMC lmc = {0};
    FILE *file = fopen(filename, "r");

    if (!file) {
        fprintf(stderr, "*** COULD NOT OPEN FILE\n");
        exit(ERR_OPEN_FILE);
        return lmc;
    }

    TOKEN tokens[PROGRAM_LENGTH] = {0};
    int program_length = 0;

    tokenize(file, tokens, &program_length);
    fclose(file);
    
    ADDR addresses[100] = {0};
    int addrc = 0;

    build_addresses(addresses, tokens, program_length, &addrc);
    compile(tokens, program_length, addresses, addrc, &lmc);

    return lmc;
}