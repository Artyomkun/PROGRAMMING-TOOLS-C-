#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <windows.h>  

class Edition {
    public:
        Edition(const std::string& title, const std::string& author_surname)
            : title(title), author_surname(author_surname) {
        }

        virtual ~Edition() = default;

        virtual void print_info() const = 0;

        bool matches_criteria(const std::string& search_surname) const {
            return author_surname == search_surname;
        }

        std::string get_title() const { 
            return title; 
        }

        std::string get_author_surname() const { 
            return author_surname; 
        }

    protected:
        std::string title;
        std::string author_surname;
};

class Book : public Edition {
    public:
        Book(const std::string& title, const std::string& author_surname,
            int year, const std::string& publisher)
            : Edition(title, author_surname), year(year), publisher(publisher) {
        }

        void print_info() const override {
            std::cout << "Книга: \"" << title << "\"\n";
            std::cout << "Автор: " << author_surname << "\n";
            std::cout << "Год издания: " << year << "\n";
            std::cout << "Издательство: " << publisher << "\n";
            std::cout << "------------------------\n";
        }

    private:
        int year;
        std::string publisher;
};

class Article : public Edition {
    public:
        Article(const std::string& title, const std::string& author_surname,
            const std::string& journal, int journal_number, int year)
            : Edition(title, author_surname), journal(journal),
            journal_number(journal_number), year(year) {
        }

        void print_info() const override {
            std::cout << "Статья: \"" << title << "\"\n";
            std::cout << "Автор: " << author_surname << "\n";
            std::cout << "Журнал: \"" << journal << "\" №" << journal_number << "\n";
            std::cout << "Год издания: " << year << "\n";
            std::cout << "------------------------\n";
        }

    private:
        std::string journal;
        int journal_number;
        int year;
};

class OnlineResource : public Edition {
    public:
        OnlineResource(const std::string& title, const std::string& author_surname,
                                const std::string& link, const std::string& annotation)
            : Edition(title, author_surname), link(link), annotation(annotation) {
        }

        void print_info() const override {
            std::cout << "Онлайн-ресурс: \"" << title << "\"\n";
            std::cout << "Автор: " << author_surname << "\n";
            std::cout << "Ссылка: " << link << "\n";
            std::cout << "Аннотация: " << annotation << "\n";
            std::cout << "------------------------\n";
        }

    private:
        std::string link;
        std::string annotation;
};

void print_catalog(const std::vector<std::shared_ptr<Edition>>& catalog) {
    std::cout << "\n=== ПОЛНЫЙ КАТАЛОГ ИЗДАНИЙ ===\n";
    if (catalog.empty()) {
        std::cout << "Каталог пуст.\n";
        return;
    }

    for (size_t i = 0; i < catalog.size(); ++i) {
        std::cout << "Издание #" << i + 1 << ":\n";
        catalog[i]->print_info();
    }
}

std::vector<std::shared_ptr<Edition>> search_by_author(
    const std::vector<std::shared_ptr<Edition>>& catalog,
    const std::string& surname) {

    std::vector<std::shared_ptr<Edition>> results;

    for (const auto& edition : catalog) {
        if (edition->matches_criteria(surname)) {
            results.push_back(edition);
        }
    }

    return results;
}

void print_search_results(const std::vector<std::shared_ptr<Edition>>& results,
    const std::string& surname) {
    std::cout << "\n=== РЕЗУЛЬТАТЫ ПОИСКА ПО АВТОРУ: " << surname << " ===\n";

    if (results.empty()) {
        std::cout << "Изданий данного автора не найдено.\n";
        return;
    }

    std::cout << "Найдено " << results.size() << " изданий:\n\n";
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "Результат #" << i + 1 << ":\n";
        results[i]->print_info();
    }
}

std::vector<std::shared_ptr<Edition>> create_sample_catalog() {
    std::vector<std::shared_ptr<Edition>> catalog;

    catalog.push_back(std::make_shared<Book>(
        "Программирование на C++", "Страуструп", 2013, "Диалектика"));

    catalog.push_back(std::make_shared<Book>(
        "Совершенный код", "Макконнелл", 2019, "БХВ"));

    catalog.push_back(std::make_shared<Article>(
        "Искусственный интеллект в медицине", "Петров",
        "Научные открытия", 5, 2022));

    catalog.push_back(std::make_shared<Article>(
        "Квантовые вычисления", "Иванов",
        "Компьютерные технологии", 12, 2023));

    catalog.push_back(std::make_shared<OnlineResource>(
        "Введение в машинное обучение", "Сидоров",
        "https://example.com/ml-intro",
        "Базовые концепции машинного обучения"));

    catalog.push_back(std::make_shared<Book>(
        "Язык программирования C++", "Страуструп", 2020, "Диалектика"));

    return catalog;
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    auto catalog = create_sample_catalog();

    print_catalog(catalog);

    std::cout << "\n=== ПОИСК ИЗДАНИЙ ПО АВТОРУ ===\n";

    std::string search_surname;
    std::cout << "Введите фамилию автора для поиска: ";
    std::getline(std::cin, search_surname);

    auto search_results = search_by_author(catalog, search_surname);

    print_search_results(search_results, search_surname);

    std::cout << "\n=== ДЕМОНСТРАЦИЯ ПОИСКА ДЛЯ АВТОРА 'СТРАУСТРУП' ===\n";
    auto demo_results = search_by_author(catalog, "Страуструп");
    print_search_results(demo_results, "Страуструп");

    return 0;
}