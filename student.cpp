#include "student.hpp"

#include <fstream>
#include <sstream>

const char * db_name = "students.tbl";
const char * db_name_out = "students_out.tbl";

#define zero_memory(chunk) memset(chunk, '\0', sizeof(chunk))

const char * SubjectToString(Subject subject)
{
    switch (subject)
    {
    case Subject::MATHEMATICS: return "Math";
    case Subject::COMPUTER_SCIENCE: return "CSci";
    case Subject::PHYSICS: return "Phys";
    case Subject::CHEMISTRY: return "Chem";
    case Subject::UNKNOWN: break;
    }
    return "";
}


/**
 * Записывает студента в список.
 */
void last_support(Students& students, Student& student)
{
    int m_count = 0;
    float sum = 0.0f;

    for (const auto & p : student.rating)
    {
        m_count += p.second.size();
        for (const int m : p.second)
        {
            sum += m;
        }
    }

    student.avg = (sum / (float)m_count);

    students.push_back(std::move(student));
    student.name.clear();
    student.rating.clear();
}


void readMeta(std::ifstream & ifs, Student& student)
{
    std::stringbuf strbuf;
    // <birthday>;<groupid>
    ifs.get(strbuf, ';');
    student.birthday = strbuf.str();

    if (ifs.peek() == ';')
        ifs.get();

    ifs >> student.group;
}


void readName(std::ifstream & ifs, Student& student)
{
    std::string name, lname;
    ifs >> name >> lname;
    student.name = name + " " + lname;
}


void readSubject(std::ifstream & ifs, Student& student)
{
    uint32_t subi = 0;
    ifs >> subi;
    const Subject sub = (Subject)subi;

    if (sub >= Subject::UNKNOWN)
        return;

    auto& rating = student.rating[sub];

    while (ifs.good())
    {
        int rat = 0;

        ifs >> rat;
        rating.push_back(rat);

        if ('\n' == ifs.peek())
            break;
    }
}

Students GetAllStudents(const char * filename)
{
    if (!filename) 
        filename = db_name;

    std::ifstream ifs(filename);
    Students students;
    Student student;
    bool last_p = false;

    if (!ifs.is_open())
        return students;

    int last_c = '\n';
    while (!ifs.eof())
    {
        int c = ifs.get();

        if (last_c == '\n') 
        {
            switch (c) {
            case 'p':
                readSubject(ifs, student);
                last_p = true;
                break;

            case 's':
                if (last_p) last_support(students, student);
                last_p = false;
                readName(ifs, student);
                break;

            case 'm':
                readMeta(ifs, student);
                last_p = false;
                break;
            default:
                break;
            }
        }

        last_c = c;
    }

    // for last student
    if (last_p) students.push_back(student);

    return students;
}


void SaveStudentsInfo(Students students, const char * filename)
{
    const char * out_name = (filename == nullptr) ? db_name_out : filename;

    std::ofstream ofs(out_name);

    for (const auto & student : students)
    {
        ofs << "s "
            << student.name
            << std::endl;

        ofs << "m "
            << student.birthday.c_str()
            << ";"
            << student.group.c_str()
            << std::endl;

        for (const auto & p : student.rating)
        {
            const auto subject = static_cast<int>(p.first);
            const auto& rating = p.second;

            ofs << "p" << subject;

            for (const auto rat : rating)
            {
                ofs << " " << rat;
            }
            ofs << std::endl;
        }
    }
}


void PrintStudent(const Student & student, FILE * stream)
{
    if (!stream)
        stream = stdout;

    fprintf(stream, "Name: %s\n", student.name.c_str());

    const auto & rating = student.rating;

    for (const auto & sub_info : rating)
    {
        const auto & subject = sub_info.first;
        const auto & marks_list = sub_info.second;

        fprintf(stream, "%s: ", SubjectToString(subject));

        for (const auto & mark : marks_list)
        {
            fprintf(stream, "%d", mark); putc(' ', stream);
        }
        putc('\n', stream);
    }

    fprintf(stream, "Avg: %.2f\n", student.avg);
    putc('\n', stream);
}


void PrintAllStudents(int max_students, FILE* stream)
{
    if (!stream) 
        stream = stdout;

    const auto all_students = GetAllStudents();
    int printed_students = 0;

    for (const auto & student : all_students)
    {
        PrintStudent(student, stream);
        
        if (printed_students++ > max_students) 
            break;
    }
}
