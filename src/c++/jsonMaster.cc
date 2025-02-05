#include <jsonMaster/jsonMaster.h> 


namespace myjson {

void JsonMaster::load() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // Простейший парсинг JSON (без вложенных структур)
    // Пример: {"key1":"value1","key2":42,"key3":3.14,"key4":true}
    size_t pos = 0;
    while ((pos = content.find('"', pos)) != std::string::npos) {
        size_t keyStart = pos + 1;
        pos = content.find('"', keyStart);
        std::string key = content.substr(keyStart, pos - keyStart);

        pos = content.find(':', pos + 1);
        pos = content.find_first_not_of(" \t\n\r", pos + 1);

        if (content[pos] == '"') { // Строка
            size_t valueStart = pos + 1;
            pos = content.find('"', valueStart);
            std::string value = content.substr(valueStart, pos - valueStart);
            data[key] = value;
        } else if (content[pos] == 't' || content[pos] == 'f') { // Булево значение
            if (content.substr(pos, 4) == "true") {
                data[key] = true;
                pos += 4;
            } else if (content.substr(pos, 5) == "false") {
                data[key] = false;
                pos += 5;
            } else {
                throw std::runtime_error("Invalid boolean value");
            }
        } else { // Число (int или float)
            size_t valueStart = pos;
            pos = content.find_first_of(",}\n\r", pos);
            std::string valueStr = content.substr(valueStart, pos - valueStart);

            if (valueStr.find('.') != std::string::npos) {
                data[key] = std::stof(valueStr);
            } else {
                data[key] = std::stoi(valueStr);
            }
            pos--;
        }
        pos++;
    }
}

void JsonMaster::save() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    file << "{\n";
    size_t i = 0;
    for (const auto& [key, value] : data) {
        if (i != 0) file << ",\n";
        file << "    \"" << key << "\": ";

        std::visit([&file](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) {
                file << "\"" << arg << "\"";
            } else if constexpr (std::is_same_v<T, bool>) {
                file << (arg ? "true" : "false");
            } else {
                file << arg;
            }
        }, value);

        i++;
    }
    file << "\n}\n";    
}

JsonMaster::JsonMaster(const std::string& filename) : filename(filename) {
    load();
}

void JsonMaster::writeIfNotExists(const std::string& key, const JsonValue& value) {
    if (data.find(key) == data.end()) {
        data[key] = value;
        save();
    }
}

void JsonMaster::printAll() const {
    for (const auto& [key, value] : data) {
        std::cout << key << ": ";
        std::visit([](auto&& arg) {
            std::cout << arg;
        }, value);
        std::cout << std::endl;
    }
}

}