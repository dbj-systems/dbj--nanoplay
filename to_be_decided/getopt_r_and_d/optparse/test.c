#define OPTPARSE_IMPLEMENTATION
#define OPTPARSE_API static
#include "optparse.h"

#ifdef _DEBUG
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHATTY 

#define COMPARE_TO_GETOPT
#ifdef COMPARE_TO_GETOPT
// windows has no native getopt.h
// #include <getopt.h>
#include "../chunde_getopt/getopt.h"

#endif // COMPARE_TO_GETOPT

static void print_argv(char **argv)
{
#ifdef CHATTY
    while (*argv)
        printf("%s ", *argv++);
    printf("\n");
#endif
}

#ifdef COMPARE_TO_GETOPT
static void try_getopt(int argc, char **argv)
{
    int opt;

    print_argv(argv);
    while ((opt = getopt(argc, argv, "abc:d::")) != -1) {
        printf("%c (%d) = '%s'\n", opt, optind, optarg);
    }
#ifdef CHATTY
    printf("optind = %d\n", optind);
    for (; optind < argc; optind++)
        printf("argument: %s\n", argv[optind]);
#endif // CHATTY
}
#endif



static void try_optparse(char **argv)
{
    int opt = 0 ;
    char *arg = 0;
    struct optparse options;

    print_argv(argv);
    optparse_init(&options, argv);
    while ((opt = optparse(&options, "abc:d::")) != -1) {
        if (opt == '?')
            printf("%s: %s\n", argv[0], options.errmsg);
        printf("Parsed '%c' (%d) = '%s'\n", opt, options.optind, options.optarg_ );
    }
#ifdef CHATTY
    printf("optind = %d\n", options.optind);
    while ((arg = optparse_arg(&options)))
        printf("argument: '%s'\n", arg);
#endif // CHATTY

}

static void try_optparse_long(char **argv)
{
    char *arg;
    int opt, longindex;
    struct optparse options;
    struct optparse_long longopts[] = {
        {"amend", 'a', OPTPARSE_NONE},
        {"brief", 'b', OPTPARSE_NONE},
        {"color", 'c', OPTPARSE_REQUIRED},
        {"delay", 'd', OPTPARSE_OPTIONAL},
        {0, 0, 0}
    };

    print_argv(argv);
    optparse_init(&options, argv);
    while ((opt = optparse_long(&options, longopts, &longindex)) != -1) {
        if (opt == '?')
            printf("%s: %s\n", argv[0], options.errmsg);
        printf("%c (%d, %d) = '%s'\n",
               opt, options.optind, longindex, options.optarg_ );
    }
    printf("optind = %d\n", options.optind);
    while ((arg = optparse_arg(&options)))
        printf("argument: %s\n", arg);
}

int test_optparse (int argc, char * argv[argc] )
{
    char * simulated_argv[] = {
        _strdup("TEST") ,
        _strdup("--amend") ,
        _strdup("--b") ,
        _strdup("--color") ,
        _strdup("red") ,
        _strdup("--delay=22") ,
        _strdup("subcommand") ,
        _strdup("example,txt") ,
        _strdup("--amend") ,
        (char*)0
    };

    int simulated_argc = (int)(sizeof(simulated_argv) / sizeof(argv[0])) -1 ;
    // actual cli not used

    __try {
        //#ifdef _DEBUG
        //    __debugbreak();
        //#endif

#ifdef COMPARE_TO_GETOPT
        printf("\nGETOPT\n");
        try_getopt(simulated_argc, simulated_argv);
#endif
        // memcpy(argv_copy, argv, size);
        printf("\nOPTPARSE\n");
        try_optparse(simulated_argv);

#if 0
        printf("\nOPTPARSE LONG\n");
        try_optparse_long(long_argv);
#endif
        for (int k = 0; k < simulated_argc; k++)
            free(simulated_argv[k]);

    }
    __except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ?
        EXCEPTION_EXECUTE_HANDLER :
        EXCEPTION_CONTINUE_SEARCH)
    {
        puts("__except");
    }

    return 0;
}
