#include <cstdlib>
#include <iostream>
#include <cstring>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << "<substring> <filename>\n";
        return EXIT_FAILURE;
    }
    FILE *f = fopen(argv[2], "r");
    if (!f) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    char *s = argv[1];
    size_t s_len = strlen(s);
    int prf[s_len];
    prf[0] = 0;
    for (int i = 1; i < s_len; ++i) {
        int j = prf[i - 1];
        while (j > 0 && s[i] != s[j])
            j = prf[j - 1];
        if (s[i] == s[j]) ++j;
        prf[i] = j;
    }

    int index = 0;
    while (true) {
        char buffer[8192];
        size_t bytes_read = fread(buffer, sizeof(char), std::size(buffer), f);

        if (bytes_read == 0) {
            if (ferror(f)) {
                perror("fread failed");
                fclose(f);
                return EXIT_FAILURE;
            }

            break;
        }
        for (size_t i = 0; i < bytes_read; i++) { // corasick
            while (index > 0 && buffer[i] != s[index]) {
                index = prf[index - 1];
            }
            if (buffer[i] == s[index]) {
                index++;
            }
            if (index == s_len) {
                fclose(f);
                std::cout << "true" << std::endl;
                return EXIT_SUCCESS;
            }
        }
    }
    fclose(f);
    std::cout << "false" << std::endl;
    return EXIT_SUCCESS;
}
