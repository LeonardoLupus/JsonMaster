#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <stdexcept>
#include <optional>
#include <variant>
#include <iomanip>

namespace myjson {

    using JsonValue = std::variant<std::string, int, float, bool, double>;

    class JsonMaster {
    private:
        std::string filename;
        std::map<std::string, JsonValue> data;

        // Чтение JSON-файла и загрузка данных в память
        void load();

        // Сохранение данных обратно в JSON-файл с форматированием
        void save();

    public:
        // Конструктор, принимающий имя файла
        JsonMaster(const std::string& filename);

        // Запись значения в файл, если ключ еще не существует
        void writeIfNotExists(const std::string& key, const JsonValue& value);

        // Получение значения по ключу
        template <typename T>
        std::optional<T> getValue(const std::string& key) const {
            auto it = data.find(key);
            if (it != data.end()) {
                try {
                    return std::get<T>(it->second);
                } catch (const std::bad_variant_access&) {
                    throw std::runtime_error("Type mismatch for key: " + key);
                }
            }
            return std::nullopt;
        }

        // Вывод всех данных (для отладки)
        void printAll() const;
    };

} // namespace myjson