#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <iomanip>

struct RaceResult {
    double time_seconds; 
    double norm_seconds; 
    bool passed; 
    
    RaceResult(double time = 0.0, double norm = 300.0) 
        : time_seconds(time), norm_seconds(norm) {
        passed = (time_seconds <= norm_seconds);
    } 

    bool check_norm() const {
        return time_seconds <= norm_seconds;
    } 

    void display() const {
        std::cout << "Результат: " << time_seconds << " сек. ";
        std::cout << "(Норматив: " << norm_seconds << " сек.) - ";
        std::cout << (passed ? "СДАЛ" : "НЕ СДАЛ");
    }
}; 

struct Student {
    std::string full_name;  
    int course;   
    std::string group;   
    RaceResult race_result;  

    Student(const std::string& name = "", int crs = 1, 
            const std::string& grp = "", double time = 0.0)
                : full_name(name), course(crs), group(grp), race_result(time) {}
    
    bool passed_race() const {
        return race_result.passed;
    }
    
    void display() const {
        std::cout << std::left;
        std::cout << std::setw(25) << full_name;
        std::cout << std::setw(8) << course;
        std::cout << std::setw(10) << group;
        race_result.display();
        std::cout << std::endl;
    }
    
    void write_to_file(std::ofstream& file) const {
        file << full_name << " " << course << " " 
            << group << " " << race_result.time_seconds << std::endl;
    }
};

// ========== СВОБОДНЫЕ ФУНКЦИИ ==========

std::vector<Student> read_students_from_file(const std::string& filename) {
    std::vector<Student> students;
    std::ifstream input_file(filename);
    
    if (!input_file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return students;
    }
    
    std::string name;
    int course;
    std::string group;
    double time;
    
    while (input_file >> name >> course >> group >> time) {
        students.emplace_back(name, course, group, time);
    }
    
    input_file.close();
    return students;
}

std::vector<Student> read_students_from_file(const std::string& filename, double norm) {
    std::vector<Student> students;
    std::ifstream input_file(filename);
    
    if (!input_file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return students;
    }
    
    std::string name;
    int course;
    std::string group;
    double time;
    
    while (input_file >> name >> course >> group >> time) {
        Student student(name, course, group, time);
        student.race_result.norm_seconds = norm;
        student.race_result.passed = student.race_result.check_norm();
        students.push_back(student);
    }
    
    input_file.close();
    return students;
}

void write_students_to_file(const std::string& filename, const std::vector<Student>& students) {
    std::ofstream output_file(filename);
    
    if (!output_file.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
        return;
    }
    
    for (const auto& student : students) {
        student.write_to_file(output_file);
    }
    
    output_file.close();
}

void write_students_to_file(const std::string& filename, const std::vector<Student>& students, bool only_passed) {
    std::ofstream output_file(filename);
    
    if (!output_file.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
        return;
    }
    
    for (const auto& student : students) {
        if (!only_passed || student.passed_race()) {
            student.write_to_file(output_file);
        }
    }
    
    output_file.close();
}

void display_students(const std::vector<Student>& students) {
    if (students.empty()) {
        std::cout << "Список студентов пуст." << std::endl;
        return;
    }
    
    std::cout << "\n=== СПИСОК СТУДЕНТОВ ===" << std::endl;
    std::cout << std::left;
    std::cout << std::setw(25) << "ФИО";
    std::cout << std::setw(8) << "Курс";
    std::cout << std::setw(10) << "Группа";
    std::cout << "Результат забега" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    for (const auto& student : students) {
        student.display();
    }
}

void display_students(const std::vector<Student>& students, bool show_passed) {
    if (students.empty()) {
        std::cout << "Список студентов пуст." << std::endl;
        return;
    }
    
    std::cout << "\n=== СПИСОК СТУДЕНТОВ (" 
                << (show_passed ? "СДАЛИ" : "НЕ СДАЛИ") 
                << ") ===" << std::endl;
    std::cout << std::left;
    std::cout << std::setw(25) << "ФИО";
    std::cout << std::setw(8) << "Курс";
    std::cout << std::setw(10) << "Группа";
    std::cout << "Результат забега" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    size_t count = 0;
    for (const auto& student : students) {
        if (student.passed_race() == show_passed) {
            student.display();
            ++count;
        }
    }
    
    if (count == 0) {
        std::cout << "Нет студентов в этой категории." << std::endl;
    }
}

template<typename FilterFunc>
std::vector<Student> filter_students(const std::vector<Student>& students, FilterFunc filter) {
    std::vector<Student> result;
    for (const auto& student : students) {
        if (filter(student)) {
            result.push_back(student);
        }
    }
    return result;
}

bool passed_race_filter(const Student& student) {
    return student.passed_race();
}

bool failed_race_filter(const Student& student) {
    return !student.passed_race();
}

bool course_filter(const Student& student, int course) {
    return student.course == course;
}

// ========== ГЛАВНАЯ ФУНКЦИЯ ==========

int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    
    std::cout << "==========================================" << std::endl;
    std::cout << "   ОБРАБОТКА РЕЗУЛЬТАТОВ ЗАБЕГА СТУДЕНТОВ" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    const std::string INPUT_FILE = "input.txt";
    const std::string OUTPUT_FILE = "output.txt";
    const double DEFAULT_NORM = 300.0;
    
    std::cout << "\nЧтение данных из файла " << INPUT_FILE << "..." << std::endl;
    std::vector<Student> all_students = read_students_from_file(INPUT_FILE, DEFAULT_NORM);
    
    if (all_students.empty()) {
        std::cout << "Файл пуст или не найден. Используем тестовые данные." << std::endl;
        
        all_students = {
            Student("Иванов_Иван_Иванович", 2, "ИСП-201", 280.5),
            Student("Петров_Петр_Петрович", 1, "ИСП-101", 320.0),
            Student("Сидорова_Анна_Сергеевна", 3, "ИСП-301", 295.0),
            Student("Козлов_Алексей_Владимирович", 2, "ИСП-202", 310.5),
            Student("Никитина_Елена_Дмитриевна", 1, "ИСП-102", 275.0)
        };
        
        write_students_to_file(INPUT_FILE, all_students);
        std::cout << "Создан файл с тестовыми данными." << std::endl;
    }
    
    display_students(all_students);
    
    display_students(all_students, true);
    
    display_students(all_students, false);
    
    std::vector<Student> passed_students = filter_students(all_students, passed_race_filter);
    
    std::cout << "\nЗапись студентов, сдавших норматив, в файл " << OUTPUT_FILE << "..." << std::endl;
    write_students_to_file(OUTPUT_FILE, all_students, true);
    
    std::cout << "\n==========================================" << std::endl;
    std::cout << "   ОТЧЕТ:" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Всего студентов: " << all_students.size() << std::endl;
    std::cout << "Сдали норматив: " << passed_students.size() << std::endl;
    std::cout << "Не сдали норматив: " << all_students.size() - passed_students.size() << std::endl;
    std::cout << "Результаты сохранены в файл: " << OUTPUT_FILE << std::endl;
    std::cout << "==========================================" << std::endl;
    
    #ifdef _WIN32
        system("pause");
    #endif
    
    return 0;
}