#include <stdio.h>
#include "./utils/unittests.c"

int main() {
    char msg[4000];

    unittestp_make_return_message(msg, __func__, "%d: %s", 6, "went.");
    printf("%s\n", msg);
}
