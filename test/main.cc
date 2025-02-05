#include <jsonMaster/jsonMaster.h>
#include <iostream>

int main() {
    std::cout << "Hello, World!\n";
    myjson::JsonMaster jsonFile("data.json");

    // Записываем значения, если их еще нет
    jsonFile.writeIfNotExists("name", std::string("John"));
    jsonFile.writeIfNotExists("age", 30);
    jsonFile.writeIfNotExists("height", 1.75f);
    jsonFile.writeIfNotExists("is_student", true);

    // Пытаемся записать значение с существующим ключом (оно не перезапишется)
    jsonFile.writeIfNotExists("name", std::string("Alice"));

    // Извлекаем значения по ключу
    auto name = jsonFile.getValue<std::string>("name");
    if (name.has_value()) {
        std::cout << "Name: " << name.value() << std::endl;
    } else {
        std::cout << "Name not found!" << std::endl;
    }

    auto age = jsonFile.getValue<int>("age");
    if (age.has_value()) {
        std::cout << "Age: " << age.value() << std::endl;
    } else {
        std::cout << "Age not found!" << std::endl;
    }

    auto height = jsonFile.getValue<float>("height");
    if (height.has_value()) {
        std::cout << "Height: " << height.value() << std::endl;
    } else {
        std::cout << "Height not found!" << std::endl;
    }

    auto isStudent = jsonFile.getValue<bool>("is_student");
    if (isStudent.has_value()) {
        std::cout << "Is student: " << std::boolalpha << isStudent.value() << std::endl;
    } else {
        std::cout << "Is student not found!" << std::endl;
    }

    return 0;
}
