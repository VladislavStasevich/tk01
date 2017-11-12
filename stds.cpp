#include <vector>
#include <list>
#include <map>
#include <sys\stat.h>
#include <sys\types.h>
#include <locale>

#define zero_memory(chunk) memset(chunk, '\0', sizeof(chunk))

static const char * db_name = "students.tbl";

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

Students GetAllStudents();
void SaveStudentsInfo(Students);


bool check_database() 
{
    FILE * fp;

    fp = fopen(db_name, "r");
    if (fp) {
        fclose(fp);
        return true;
    }
    return false;
}


void readName(FILE * fd, Student& student) 
{
    char last_name[64]; zero_memory(last_name);
    char initials[64];  zero_memory(initials);
    
    fscanf(fd, "%s %s\n", &last_name, &initials);
    
    student.name += std::string(last_name);
    student.name += " ";
    student.name += std::string(initials);
}


void readMeta(FILE * fd, Student& student)
{
    char metaline[BUFSIZ]; zero_memory(metaline);
    
    fscanf(fd, "%s;%s", &metaline);

    char * sep = strchr(metaline, ';');

    student.birthday = std::string(metaline, sep - metaline);
    student.group = std::string(sep + 1);
}


void readSubject(FILE * fd, Student& student) 
{
    Subject sub = Subject::UNKNOW;
    uint32_t subi = 0;
    fscanf(fd, "%d ", &subi);
    sub = (Subject)subi;
 
    if (sub >= Subject::UNKNOW)
        return;
    
    auto& rating = student.rating[sub];

    while (!feof(fd))
    {
        int rat = 0;
        const int endl = fscanf(fd, "%d ", &rat);
        if (!endl) break;
        rating.push_back(rat);
    }
}


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


void SaveStudentsInfo(Students) 
{

}


int main(int argc, char ** argv) 
{
    setlocale(LC_ALL, "");

    auto stud = GetAllStudents();

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
    }
    
    return 0;
}

