#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <list>

/**
 * Идентификаторы предметов
 */
enum class Subject : unsigned char {
    PHYSICS = 0u,
    MATHEMATICS = 1u,
    COMPUTER_SCIENCE = 2u,
    CHEMISTRY = 3u,
    UNKNOWN = 4u
};


const char * SubjectToString(Subject subject);


/**
 * Структура хранящаяя информацию о студенте
 */
struct Student {
    using Rating = std::unordered_map<Subject, std::vector<int>>;

    std::string name;
    std::string birthday;
    std::string group;
    Rating rating;
    float avg = 0.0f;

    Student() = default;
};

/**
 * Список студентов
 */
using Students = std::list<Student>;

/**
 * Считывает всех студентов из файла.
 * @return Список студентов
 */
Students GetAllStudents(const char * filename = nullptr);

/**
 * Записывает список студентов в файл.
 * @param students список студентов.
 * @param [filename] имя файла.
 */
void SaveStudentsInfo(Students students, 
                      const char * filename = nullptr);

/**
 * Записывает студентов в файл или на экран.
 * @param [max_students] максимальное кол-во студентов 
 *        которые будут записаны.
 * @param [stream] файл или поток в который будет записана
 *        информация о студентах.
 */
void PrintAllStudents(int max_students = 3, 
                      FILE* stream = nullptr);

/**
* Записывает студентоа в файл или на экран.
* @param student студент.
* @param [stream] файл или поток в который будет записана
*        информация о студентах.
*/
void PrintStudent(const Student & student, 
                  FILE * stream = nullptr);

#endif // STUDENT_HPP