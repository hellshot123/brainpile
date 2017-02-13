//fuck you - thanks man

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>

std::string version = "v0.1.0";

class Brainfuck
{
public:
    char data[30000];
    char *d;
    const char *p;

    Brainfuck(const char prog[])
    {
        d = data;
        p = prog;
    }

    void pIncrease()
    {
        d++;
    }

    void pDecreaase()
    {
        d--;
    }

    void bIncrease()
    {
        (*d)++;
    }

    void bDecrease()
    {
        (*d)--;
    }

    void out()
    {
        std::cout << *d;
    }

    void in()
    {
        std::cin >> *d;
    }

    void bracketOpen()
    {
        int bal = 1;
        if (*d == '\0')
        {
            do
            {
                p++;
                if      (*p == '[') bal++;
                else if (*p == ']') bal--;
            }
            while ( bal != 0 );
        }
    }

    void bracketClose()
    {
        int bal = 0;
        do
        {
            if      (*p == '[') bal++;
            else if (*p == ']') bal--;
            p--;
        }
        while ( bal != 0 );
    }

    void evaluate()
    {
        while (*p)
        {
            switch (*p)
            {
            case '>':
                pIncrease();
                break;
            case '<':
                pDecreaase();
                break;
            case '+':
                bIncrease();
                break;
            case '-':
                bDecrease();
                break;
            case '.':
                out();
                break;
            case ',':
                in();
                break;
            case '[':
                bracketOpen();
                break;
            case ']':
                bracketClose();
                break;
            }
            p++;
        }
    }
};

void compile(char* filename)
{
    std::ifstream file;
    int length;
    int opt;
    char* buffer;
    struct stat stats;

    if( stat (filename, &stats) == 0 )
    {
        file.open(filename);
        file.seekg(0, std::ios::end);
        length = file.tellg();
        file.seekg(0, std::ios::beg);
        buffer = new char[length];
        file.read(buffer, length);
        file.close();
    }
    else
    {
        fprintf (stderr, "Cannot open file %s.\n", filename);
        exit(2);
    }

    Brainfuck interpreter = Brainfuck(buffer);
    interpreter.evaluate();
}

void usage()
{
    std::cout << "Brainpiler " << version << " Arthur Connop 2017"  							<< std::endl;
    std::cout << "Usage: brainpile -h | -c <filename>" 													<< std::endl << std::endl;

    std::cout << "    -c           Compile and run a source file."                << std::endl;
    std::cout << "    -h           Print this usage."                             << std::endl << std::endl;
}

int main(int argc, char **argv)
{
    std::ifstream file;
    int length;
    int opt;
    char* filename;
    char* buffer;
    struct stat stats;

    opterr = 0;
    while ((opt = getopt (argc, argv, "hc:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            usage();
            exit(0);

        case 'c':
            compile((char*) optarg);
            break;

        case '?':
            if (optopt == 'c')
                fprintf (stderr, "Option '-%c' requires an argument.\n", optopt);
            else
                fprintf (stderr, "Unknown option '-%c'.\n", optopt);
            exit(1);

        default:
            exit(2);
        }
    }
}
