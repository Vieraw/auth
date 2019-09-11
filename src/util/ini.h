#ifndef INI_H
#define INI_H

#include <string>
#include <fstream>
#include <list>
#include <map>
#include <algorithm>

namespace util {
    class ini_error : public std::exception {
        std::string error;
    public:

        explicit ini_error(std::string &&s) : std::exception(), error(s) {}

        ~ini_error() override = default;

        const char *what() const noexcept override { return error.c_str(); }
    };

    template<typename T>
    inline void ltrim(std::basic_string<T> &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
    }

    template<typename T>
    inline void rtrim(std::basic_string<T> &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
    }

    template<typename T>
    class ini {
    public:

        ini() = default;

        explicit ini(const std::string &filename) {
            std::ifstream file{filename};
            if (!file.is_open()) {
                throw ini_error("Ini read error: unable to open file: " + filename);
            }
            read(file);
            file.close();
        }

        explicit ini(const std::ifstream &file) {
            read(file);
        }

        typedef std::basic_string<T> String;
        typedef std::map<String, String> Section;
        typedef std::map<String, Section> Sections;

        static const T section_start = static_cast<T>('[');
        static const T section_end = static_cast<T>(']');
        static const T assign = static_cast<T>('=');
        static const T comment = static_cast<T>(';');

        void write(std::basic_ostream<T> &os) const {
            for (auto const &sec : sections) {
                os << section_start << sec.first << section_end << std::endl;
                for (auto const &val : sec.second) {
                    os << val.first << assign << val.second << std::endl;
                }
            }
        }

        void read(std::basic_istream<T> &is) {
            String line;
            String section;
            while (!is.eof()) {
                std::getline(is, line);
                ltrim(line);
                rtrim(line);
                const size_t length = line.length();
                if (length > 0) {
                    const size_t pos = line.find_first_of(assign);
                    const T &front = line.front();
                    if (front == comment) {
                        continue;
                    }
                    if (front == section_start) {
                        if (line.back() == section_end) {
                            section = line.substr(1, length - 2);
                        } else {
                            errors.push_back(line);
                        }
                    } else if (pos != 0 && pos != String::npos) {
                        String variable{line.substr(0, pos)};
                        String value{line.substr(pos + 1, length)};
                        rtrim(variable);
                        ltrim(value);
                        auto &sec = sections[section];
                        if (sec.find(variable) == sec.end()) {
                            sec.insert(std::make_pair(variable, value));
                        } else {
                            errors.push_back(line);
                        }
                    } else {
                        errors.push_back(line);
                    }
                }
            }
        }

        Section &operator[](const String &key) {
            return sections[key];
        }

        void clear() {
            sections.clear();
            errors.clear();
        }

    private:
        Sections sections;
        std::list<String> errors;

    };
}
#endif // INI_H
