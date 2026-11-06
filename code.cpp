#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class Check {
  public:
    size_t occour(const std::string &text, char c) {
        std::size_t pos = 0;
        for (std::size_t i = 0; i < text.length(); ++i) {
            if (tolower(text[i]) == tolower(c))
                pos++;
        }
        return pos;
    }

    Check() = default;

    void fill() { fill_groups(); }

    std::size_t countOccour(const std::string &text) {
        std::size_t index = 0;
        std::size_t total = 0;
        std::size_t count = 0;
        std::set<std::size_t> s;
        for (std::size_t spot = 0; spot < text.length(); ++spot) {
            char c = text[spot];
            for (std::size_t place = 0; place < spot + 1; ++place) {
                count = occour(text, c);
                s.insert(count);
            }
        }
        for (auto &i : s) {
            total += i;
        }
        return total;
    }

    bool testPattern(std::string_view text) {

        std::array<char, 256> letters{};
        std::string matched;
        std::string text_input = std::string(text);
        if (text.empty())
            return false;
        std::size_t total = countOccour(text_input);

        auto search = [&](auto &&self, size_t start, size_t pos) -> bool {
            if (pos > 1 && pos == total) {
                return true;
            }

            int group = groups[pos];
            bool new_group = (pos == 0 || groups[pos - 1] != group);

            for (size_t i = start; i < text.length(); ++i) {
                unsigned char ch = static_cast<unsigned char>(text.at(i));
                int key = std::isupper(ch) ? 1 : 0;

                if (key != table[pos] || !std::isalpha(ch))
                    continue;

                char lower = static_cast<char>(std::tolower(ch));

                if (!new_group && letters.at(group) != lower)
                    continue;

                if (new_group) {
                    bool used = false;

                    for (int j = 0; j < group; ++j) {
                        if (letters.at(j) == lower)
                            used = true;
                    }

                    if (used)
                        continue;
                }

                char previous = letters[group];

                if (new_group)
                    letters.at(group) = lower;

                matched += text.at(i);

                if (self(self, i + 1, pos + 1))
                    return true;

                matched.pop_back();
                letters.at(group) = previous;
            }

            return false;
        };
        bool found = search(search, 0, 0);
        if (found) {
            if (matched.length() > 0) {
                std::size_t last = occour(matched, matched[matched.size() - 1]);
                std::string val;
                char cx = matched[matched.size() - 1];
                char o = 0;
                for (long i = matched.size() - 1; i >= 0; --i) {
                    if (std::tolower(matched[i]) != std::tolower(cx)) {
                        o = matched[i];
                        break;
                    }
                }
                std::size_t p = occour(matched, o);
                if (last < p || p <= 1) {
                    return false;
                }
            }
            std::cout << "Pattern Found: " << matched << '\n';
            std::cout << "Actual:Expected:Character\n";
            for (size_t i = 0; i < matched.length(); ++i) {
                unsigned char ch = static_cast<unsigned char>(matched[i]);
                int key = std::isupper(ch) ? 1 : 0;
                std::cout << key << ":" << table[i] << ":" << matched[i] << '\n';
            }
        }
        return found;
    }

    void fill_groups() {
        char index = 'A';
        std::size_t i = 0;
        while (1) {
            for (std::size_t z = 0; z < i + 1; ++z) {
                groups.push_back(index);
            }
            ++index;
            ++i;
            if (index > 'Z') {
                break;
            }
        }
        index = 'a';
        while (1) {
            for (std::size_t z = 0; z < i + 1; ++z) {
                groups.push_back(index);
            }
            ++index;
            if (index > 'z')
                break;
            i++;
        }

        std::size_t off = 0;

        for (std::size_t z = 0; z < groups.size() - 1; ++z) {
            off = !off;
            table.push_back(off);
        }
    }

  protected:
    std::vector<std::size_t> groups;
    std::vector<std::size_t> table;
};

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Error incorrect arguments..\n";
        return EXIT_FAILURE;
    }

    int epoch_timeout = std::stoi(argv[2]);
    int buffer_size = std::stoi(argv[3]);
    Check chk;
    chk.fill();
    std::fstream file;
    file.open(argv[1], std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Error.\n";
        return EXIT_FAILURE;
    }
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::ostringstream stream;
    stream << file.rdbuf();
    std::string value = stream.str();
    std::size_t epoch = epoch_timeout;
    std::size_t found = 0;
    std::size_t in = 0;

    while (1) {
        std::string shuf = value;
        std::shuffle(shuf.begin(), shuf.end(), eng);
        std::string buffer = shuf.substr(0, buffer_size);
        if (chk.testPattern(buffer)) {
            std::cout << "True.\n";
            ++found;
        }
        if (epoch > 0) {
            ++in;
            if (in > epoch)
                break;
        }
    }

    std::cout << "Ran for: " << in << " iterations, found: " << found << std::endl;
    return EXIT_SUCCESS;
}
