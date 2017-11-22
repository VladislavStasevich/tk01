# coding=utf-8

from random import randrange

MIN_RATE = 3
MAX_RATE = 10
STUDENT_COUNT = 100


def join_ints(ints):
    stra = ''
    ln = len(ints) - 1
    for x in range(ln):
        stra += "{} ".format(ints[x])
    stra += str(ints[-1])
    return stra


def select_random(collect):
    return collect[randrange(0, len(collect))]


names = (
    'А.В.',
    'Д.А.',
    'Г.Е.',
    'В.Д.',
    'А.В.',
    'К.А.',
    'Д.И.',
    'А.У.',
    'С.В.',
)

fnames = (
    'Стасевич',
    'Пашко',
    'Давыдович',
    'Иванов',
    'Петров'
)


subj = (
    0,  # PHYSICS
    1,  # MATHEMATICS
    2,  # COMPUTER_SCIENCE
    3   # CHEMISTRY
)


def gen_name():
    return select_random(names)


def gen_first_name():
    return select_random(fnames)


def gen_marks(sub):
    co = randrange(MIN_RATE, MAX_RATE)

    lst = []
    for num in range(co):
        lst.append(randrange(MIN_RATE, MAX_RATE))

    return "p{sub} {marks}".format(
        sub=sub,
        marks=join_ints(lst)
    )


def gen_full_name():
    return 's {} {}'.format(gen_first_name(), gen_name())


def gen_meta_info():
    return 'm {dd}.{mm}.{yyyy};{group}'.format(
        dd=randrange(1, 29),
        mm=randrange(1, 12),
        yyyy=randrange(1995, 1997),
        group=randrange(1, 3)
    )


def gen_subjects(file_p):
    for x in subj:
        file_p.write(gen_marks(x) + '\n')


students_db = open("students.tbl", 'w')

for pers in range(STUDENT_COUNT):
    students_db.write(gen_full_name() + '\n')
    students_db.write(gen_meta_info() + '\n')
    gen_subjects(students_db)
