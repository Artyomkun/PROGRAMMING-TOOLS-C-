#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <cstddef>

// ------------------------------------------------------------
// Data Structures
// ------------------------------------------------------------

struct Date 
{
    int day;
    int month;
    int year;
};

struct Student 
{
    std::string lastName;
    std::string firstName;
    std::string patronymic;
    Date birthDate;
    int marks[5];
    
    int calculate_total_score() const;
    void print_info() const;
    bool operator<(const Student& other) const;
    bool operator>(const Student& other) const;
};

// ------------------------------------------------------------
// Constants
// ------------------------------------------------------------

const size_t SUBJECT_COUNT = 5;
const std::string INPUT_FILENAME = "input.txt";
const std::string OUTPUT_FILENAME = "output.txt";

// ------------------------------------------------------------
// Student Structure Methods
// ------------------------------------------------------------

int Student::calculate_total_score() const 
{
    int total = 0;
    
    for (size_t i = 0; i < SUBJECT_COUNT; ++i) 
    {
        total += marks[i];
    }
    
    return total;
}

void Student::print_info() const 
{
    std::cout << lastName << " " << firstName << " " << patronymic << " - ";
    std::cout << "Birth: " << birthDate.year << ", ";
    std::cout << "Total score: " << calculate_total_score();
    
    std::cout << " [Marks: ";
    for (size_t i = 0; i < SUBJECT_COUNT; ++i) 
    {
        std::cout << marks[i];
        (i < SUBJECT_COUNT - 1) ? std::cout << " " : std::cout << "]";
    }
    
    std::cout << std::endl;
}

bool Student::operator<(const Student& other) const 
{
    return calculate_total_score() < other.calculate_total_score();
}

bool Student::operator>(const Student& other) const 
{
    return calculate_total_score() > other.calculate_total_score();
}

// ------------------------------------------------------------
// Generic Selection Sort Functions
// ------------------------------------------------------------

// Template for array of any primitive type
template <typename T>
void selection_sort(T* array, const size_t size, const bool ascending = true)
{
    if (size == 0) return;
    
    for (size_t i = 0; i < size - 1; ++i) 
    {
        size_t extreme_index = i;
        
        for (size_t j = i + 1; j < size; ++j) 
        {
            if (ascending) 
            {
                if (array[j] < array[extreme_index]) 
                {
                    extreme_index = j;
                }
            }
            else 
            {
                if (array[j] > array[extreme_index]) 
                {
                    extreme_index = j;
                }
            }
        }
        
        if (extreme_index != i) 
        {
            T temp = array[i];
            array[i] = array[extreme_index];
            array[extreme_index] = temp;
        }
    }
}

// Overload for array of Students
void selection_sort(Student* array, const size_t size, const bool ascending = true)
{
    if (size == 0) return;
    
    for (size_t i = 0; i < size - 1; ++i) 
    {
        size_t extreme_index = i;
        
        for (size_t j = i + 1; j < size; ++j) 
        {
            if (ascending) 
            {
                if (array[j] < array[extreme_index]) 
                {
                    extreme_index = j;
                }
            }
            else 
            {
                if (array[j] > array[extreme_index]) 
                {
                    extreme_index = j;
                }
            }
        }
        
        if (extreme_index != i) 
        {
            Student temp = array[i];
            array[i] = array[extreme_index];
            array[extreme_index] = temp;
        }
    }
}

// Template for vector of any primitive type
template <typename T>
void selection_sort(std::vector<T>& vec, const bool ascending = true)
{
    if (vec.empty()) return;
    
    const size_t size = vec.size();
    
    for (size_t i = 0; i < size - 1; ++i) 
    {
        size_t extreme_index = i;
        
        for (size_t j = i + 1; j < size; ++j) 
        {
            if (ascending) 
            {
                if (vec[j] < vec[extreme_index]) 
                {
                    extreme_index = j;
                }
            }
            else 
            {
                if (vec[j] > vec[extreme_index]) 
                {
                    extreme_index = j;
                }
            }
        }
        
        if (extreme_index != i) 
        {
            T temp = vec[i];
            vec[i] = vec[extreme_index];
            vec[extreme_index] = temp;
        }
    }
}

// Overload for vector of Students
void selection_sort(std::vector<Student>& vec, const bool ascending = true)
{
    if (vec.empty()) return;
    
    const size_t size = vec.size();
    
    for (size_t i = 0; i < size - 1; ++i) 
    {
        size_t extreme_index = i;
        
        for (size_t j = i + 1; j < size; ++j) 
        {
            if (ascending) 
            {
                if (vec[j] < vec[extreme_index]) 
                {
                    extreme_index = j;
                }
            }
            else 
            {
                if (vec[j] > vec[extreme_index]) 
                {
                    extreme_index = j;
                }
            }
        }
        
        if (extreme_index != i) 
        {
            Student temp = vec[i];
            vec[i] = vec[extreme_index];
            vec[extreme_index] = temp;
        }
    }
}

// ------------------------------------------------------------
// Input/Output Functions
// ------------------------------------------------------------

std::pair<std::string, std::vector<Student>> read_from_file(const std::string& filename)
{
    std::ifstream input_file(filename);
    std::string group_number;
    std::vector<Student> students;
    
    if (!input_file.is_open()) 
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return std::make_pair(group_number, students);
    }

    std::getline(input_file, group_number);

    Student current_student;
    
    while (input_file >> current_student.lastName 
                      >> current_student.firstName 
                      >> current_student.patronymic
                      >> current_student.birthDate.day 
                      >> current_student.birthDate.month 
                      >> current_student.birthDate.year) 
    {
        for (size_t i = 0; i < SUBJECT_COUNT; ++i) 
        {
            input_file >> current_student.marks[i];
        }
        
        students.push_back(current_student);
    }

    input_file.close();
    return std::make_pair(group_number, students);
}

void write_to_file(const std::string& filename, 
                   const std::string& group_number, 
                   const std::vector<Student>& students)
{
    std::ofstream output_file(filename);
    
    if (!output_file.is_open()) 
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    output_file << "Group: " << group_number << "\n\n";
    output_file << "Students sorted by total score (ascending):\n";
    output_file << "============================================\n\n";
    
    for (const auto& student : students) 
    {
        output_file << student.lastName << " " 
                    << student.firstName << " " 
                    << student.patronymic << "\n";
        output_file << "Birth date: " 
                    << student.birthDate.day << "." 
                    << student.birthDate.month << "." 
                    << student.birthDate.year << "\n";
        output_file << "Marks: ";
        
        for (size_t i = 0; i < SUBJECT_COUNT; ++i) 
        {
            output_file << student.marks[i];
            (i < SUBJECT_COUNT - 1) ? output_file << " " : output_file << "\n";
        }
        
        output_file << "Total score: " << student.calculate_total_score() << "\n";
        output_file << "----------------------------------------\n";
    }

    output_file.close();
}

// ------------------------------------------------------------
// Display Functions
// ------------------------------------------------------------

template <typename T>
void display_array(const T* array, const size_t size, const std::string& title)
{
    std::cout << "\n" << title << "\n";
    std::cout << std::string(title.length(), '-') << "\n";
    
    for (size_t i = 0; i < size; ++i) 
    {
        std::cout << array[i] << " ";
    }
    std::cout << "\n";
}

void display_students(const std::vector<Student>& students, const std::string& title)
{
    std::cout << "\n" << title << "\n";
    std::cout << std::string(title.length(), '-') << "\n";
    
    for (size_t i = 0; i < students.size(); ++i) 
    {
        std::cout << i + 1 << ". ";
        students[i].print_info();
    }
}

void display_program_header()
{
    std::cout << "================================================\n";
    std::cout << "    Universal Sorter with Functional Polymorphism\n";
    std::cout << "================================================\n\n";
}

void display_instructions()
{
    std::cout << "This program demonstrates functional polymorphism with:\n";
    std::cout << "1. Sorting arrays of primitive types (int, double, etc.)\n";
    std::cout << "2. Sorting arrays of Student objects\n";
    std::cout << "3. Sorting vectors of primitive types\n";
    std::cout << "4. Sorting vectors of Student objects\n\n";
    
    std::cout << "File operations:\n";
    std::cout << "- Reads student data from '" << INPUT_FILENAME << "'\n";
    std::cout << "- Sorts students by total score (selection sort)\n";
    std::cout << "- Writes sorted data to '" << OUTPUT_FILENAME << "'\n";
    std::cout << "================================================\n\n";
}

// ------------------------------------------------------------
// Demonstration Functions
// ------------------------------------------------------------

void demonstrate_primitive_array_sorting()
{
    std::cout << "\n=== Demonstrating Array Sorting (Primitive Types) ===\n";
    
    int int_array[] = {64, 25, 12, 22, 11};
    const size_t int_size = sizeof(int_array) / sizeof(int_array[0]);
    
    std::cout << "Original int array: ";
    for (size_t i = 0; i < int_size; ++i) 
    {
        std::cout << int_array[i] << " ";
    }
    
    selection_sort(int_array, int_size, true);
    
    std::cout << "\nSorted (ascending): ";
    for (size_t i = 0; i < int_size; ++i) 
    {
        std::cout << int_array[i] << " ";
    }
    
    double double_array[] = {3.14, 1.41, 2.71, 1.73};
    const size_t double_size = sizeof(double_array) / sizeof(double_array[0]);
    
    std::cout << "\n\nOriginal double array: ";
    for (size_t i = 0; i < double_size; ++i) 
    {
        std::cout << double_array[i] << " ";
    }
    
    selection_sort(double_array, double_size, false);
    
    std::cout << "\nSorted (descending): ";
    for (size_t i = 0; i < double_size; ++i) 
    {
        std::cout << double_array[i] << " ";
    }
    std::cout << "\n";
}

void demonstrate_primitive_vector_sorting()
{
    std::cout << "\n=== Demonstrating Vector Sorting (Primitive Types) ===\n";
    
    std::vector<int> int_vec = {100, 20, 300, 40, 50};
    
    std::cout << "Original int vector: ";
    for (const auto& val : int_vec) 
    {
        std::cout << val << " ";
    }
    
    selection_sort(int_vec, true);
    
    std::cout << "\nSorted (ascending): ";
    for (const auto& val : int_vec) 
    {
        std::cout << val << " ";
    }
    
    std::vector<char> char_vec = {'z', 'a', 'm', 'b', 'c'};
    
    std::cout << "\n\nOriginal char vector: ";
    for (const auto& val : char_vec) 
    {
        std::cout << val << " ";
    }
    
    selection_sort(char_vec, true);
    
    std::cout << "\nSorted (ascending): ";
    for (const auto& val : char_vec) 
    {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

// ------------------------------------------------------------
// Main Program
// ------------------------------------------------------------

int main() 
{
    display_program_header();
    display_instructions();
    
    std::cout << "Press Enter to start...";
    std::cin.get();
    
    // Demonstrate primitive type sorting
    demonstrate_primitive_array_sorting();
    demonstrate_primitive_vector_sorting();
    
    // Student data processing
    std::cout << "\n\n=== Processing Student Data ===\n";
    std::cout << "Reading from '" << INPUT_FILENAME << "'...\n";
    
    const std::pair<std::string, std::vector<Student>> file_data = read_from_file(INPUT_FILENAME);
    const std::string group_number = file_data.first;
    std::vector<Student> students = file_data.second;
    
    if (students.empty()) 
    {
        std::cout << "\nError: No student data found.\n";
        std::cout << "Please create '" << INPUT_FILENAME << "' with the following format:\n";
        std::cout << "----------------------------------------\n";
        std::cout << "GroupNumber\n";
        std::cout << "LastName FirstName Patronymic Day Month Year Mark1 Mark2 Mark3 Mark4 Mark5\n";
        std::cout << "----------------------------------------\n";
        return 1;
    }
    
    std::cout << "Loaded " << students.size() << " students from group: " << group_number << "\n";
    
    display_students(students, "Original Student List");
    
    // Sort using our polymorphic function
    std::cout << "\nSorting students by total score (ascending)...\n";
    selection_sort(students, true);
    
    display_students(students, "Sorted Student List");
    
    // Write to file
    std::cout << "\nWriting results to '" << OUTPUT_FILENAME << "'...\n";
    write_to_file(OUTPUT_FILENAME, group_number, students);
    std::cout << "Results successfully saved.\n";
    
    // Demonstrate array of Students sorting
    std::cout << "\n=== Demonstrating Student Array Sorting ===\n";
    if (students.size() >= 3) 
    {
        Student student_array[3];
        for (size_t i = 0; i < 3; ++i) 
        {
            student_array[i] = students[i];
        }
        
        std::cout << "Student array (first 3 students):\n";
        for (size_t i = 0; i < 3; ++i) 
        {
            std::cout << "  ";
            student_array[i].print_info();
        }
        
        selection_sort(student_array, 3, false);
        
        std::cout << "\nSorted (descending by total score):\n";
        for (size_t i = 0; i < 3; ++i) 
        {
            std::cout << "  ";
            student_array[i].print_info();
        }
    }
    
    std::cout << "\n================================================\n";
    std::cout << "Program completed successfully!\n";
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}