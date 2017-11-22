#include "student.hpp"

#include <cstring>
#include <cstdlib>


void usage()
{
    puts(R"help(Usage: sdb -<param>
    - a    append new student
    - u    update student info
    - r    remove student from database
    - l    list all students
    - h    prints this text)help");

#ifdef _WIN32
    // only windows has `pause` feature.
    system("PAUSE");
#endif

    exit(EXIT_SUCCESS);
}


void dbg_load_save()
{
    auto stds = GetAllStudents();
    SaveStudentsInfo(stds);
}


int main(int argc, char ** argv)
{
    int stds = 0;

    setlocale(LC_ALL, "");

    if (argc == 1)
        usage();

    argv++;
    argc--;

    switch ((*argv)[1])
    {
    case 'a':

        break;
    case 'u':

        break;
    case 'r':

        break;
    case 'l':
        if (argc > 1) {
            sscanf(*(++argv), "%d", &stds);
            if (stds > 0xFFFF) stds = 10;
        }
        PrintAllStudents(stds);
        break;
    case 'd':
        dbg_load_save();
        break;
    case 'h':
    default:
        usage();
        break;
    }

    return EXIT_SUCCESS;
}

