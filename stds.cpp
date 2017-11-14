#include <vector>
#include <list>
#include <map>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale>

#define zero_memory(chunk) memset(chunk, '\0', sizeof(chunk))

static const char * db_name = "students.tbl";
static const char * db_name_out = "students_out.tbl";

/**
 * Идентификаторы предметов
 */
enum class Subject : unsigned char {
    PHYSICS = 0u,
    MATHEMATICS = 1u,
    COMPUTER_SCIENCE = 2u,
    CHEMISTRY = 3u,
    UNKNOW = 4u
};

const char * SubjectToString(Subject subject)
{
    switch (subject)
    {
    case Subject::MATHEMATICS: return "Math";
    case Subject::COMPUTER_SCIENCE: return "CSci";
    case Subject::PHYSICS: return "Phys";
    case Subject::CHEMISTRY: return "Chem";
    case Subject::UNKNOW: break;
    }
    return "";
}

/**
 * Структура хранящаяя информацию о студенте
 */
struct Student {
    std::string name;
    std::string birthday;
    std::string group;
    std::map<Subject, std::vector<int>> rating;
    float avg = 0.0f;

    Student() = default;
};

/**
 * Список студентов
 */
using Students = std::list<Student>;

/**
 * Проверка существования файла.
 * @return true если файл существует/доступен иначе false
 */
bool check_database();

/**
 * Считывает всех студентов из файла.
 * @return Список студентов
 */
Students GetAllStudents();

/**
 * Записывает список студентов в файл.
 * @param Students список студентов.
 */
void SaveStudentsInfo(Students);

/**
 * Считывает ФИО из фалйа
 */
void readName(FILE * fd, Student& student)
{
    char last_name[64]; zero_memory(last_name);
    char initials[64];  zero_memory(initials);

    fscanf(fd, "%s %s\n", &last_name, &initials);

    student.name += std::string(last_name);
    student.name += " ";
    student.name += std::string(initials);
}

/**
 * Считывает мета информацию из фалйа
 */
void readMeta(FILE * fd, Student& student)
{
    char metaline[BUFSIZ]; zero_memory(metaline);

    // <birthday>;<groupid>
    const char * meta_format = "%s;%s";

    fscanf(fd, meta_format, &metaline);

    char * sep = strchr(metaline, ';');

    student.birthday = std::string(metaline, sep - metaline);
    student.group = std::string(sep + 1);
}

/**
 * Считывает оценки предмета из фалйа
 */
void readSubject(FILE * fd, Student& student)
{
    uint32_t subi = 0;
    fscanf(fd, "%d ", &subi);
    const Subject sub = (Subject)subi;

    if (sub >= Subject::UNKNOW)
        return;

    auto& rating = student.rating[sub];

    while (!feof(fd))
    {
        int rat = 0;
        if(!fscanf(fd, "%d ", &rat))
            break;
        rating.push_back(rat);
    }
}

/**
 * Записывает студента в список.
 */
void last_support(Students& students, Student& student)
{
    int m_count = 0;
    int sum = 0.0f;

    for (const auto & p : student.rating)
    {
        m_count += p.second.size();
        for (const int m : p.second)
        {
            sum += m;
        }
    }

    student.avg = ((float)sum / (float)m_count);

    students.push_back(std::move(student));
    student.name.clear();
    student.rating.clear();
}

Students GetAllStudents()
{
    FILE * fd;
    Students students;
    Student student;
    bool last_p = false;
    char buffer[BUFSIZ];

    fd = fopen(db_name, "r");
    if (!fd) {
        return students;
    }

    while (!feof(fd))
    {
        int c = getc(fd);

        switch (c) {
        case 'p':
            readSubject(fd, student);
            last_p = true;
            break;

        case 's':
            if (last_p) last_support(students, student);
            last_p = false;
            readName(fd, student);
            break;

        case 'm':
            readMeta(fd, student);
            last_p = false;
            break;
        }
    }

    // for last student
    if (last_p) students.push_back(student);

    return students;
}

void SaveStudentsInfo(Students students)
{
    FILE * fp;

    fp = fopen(db_name_out, "w");
    if (!fp) {
        puts("Error has been occured: Database file is locked or access is denided\n");
        exit(EXIT_SUCCESS);
    }

    for (const auto & student : students)
    {
        fprintf(fp, "s %s\n", student.name.c_str());
        fprintf(fp, "m %s;%s\n", student.birthday.c_str(), student.group.c_str());

        for (const auto & p : student.rating)
        {
            const int subject = (int)p.first;
            const auto& rating = p.second;

            fprintf(fp, "p%d", subject);

            for (const auto rat : rating)
            {
                fputc(' ', fp);
                fprintf(fp, "%d", rat);
            }
            fputc('\n', fp);
        }
    }
}

void usage()
{
    puts("Usage: sdb -<param>          \n\
    -a    append new student           \n\
    -u    update student info          \n\
    -r    remove student from database \n\
    -l    list all students            \n\
    -h    prints this text");

#ifdef _WIN32
    // only windows has `pause` feature.
    system("PAUSE");
#endif

    exit(EXIT_SUCCESS);
}


void list_all_students(int max_students = 3)
{
    auto stud = GetAllStudents();
    int i = 0;
    for (auto stde : stud)
    {
        printf("Name: %s\n", stde.name.c_str());

        auto& mp = stde.rating;

        for (auto mm : mp)
        {
            printf("%s: ", SubjectToString(mm.first));

            const auto& rating = mm.second;
            for (const auto &rat : rating)
            {
                printf("%d", rat); putc(' ', stdout);
            }
            putc('\n', stdout);
        }

        printf("Avg: %.2f\n", stde.avg);
        putc('\n', stdout);

        i++;
        if (i > max_students) break;
    }
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
        list_all_students(stds);
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

