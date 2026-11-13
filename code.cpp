#include <algorithm>
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
#ifdef __linux__
#include <signal.h>
#endif

class Check {
  public:
    size_t occour(const std::string &text, char c) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!std::isalpha(uc))
            return 0;
        std::size_t pos = 0;
        unsigned char t = std::tolower(uc);
        return std::count_if(text.begin(), text.end(), [t](char ch) { return std::isalpha(static_cast<unsigned char>(ch)) && std::tolower(static_cast<unsigned char>(ch)) == t; });
        return pos;
    }

    Check() = default;

    void fill() { fill_groups(); }

    std::size_t countOccour(std::string_view text) {
        std::size_t index = 0;
        std::size_t total = 0;
        std::size_t count = 0;
        std::set<std::size_t> s;
        std::array<std::size_t, 256> counts{0};

        for (std::size_t spot = 0; spot < text.length(); ++spot) {
            unsigned char c = static_cast<unsigned char>(text[spot]);
            if (std::isalpha(c)) {
                counts[c]++;
            }
        }

        for (std::size_t i = 0; i < 256; ++i) {
            if (counts[i] > 0) {
                s.insert(counts[i]);
            }
        }
        for (const auto &i : s) {
            total += i;
        }
        return total;
    }

    bool testPattern(std::string_view text) {

        std::array<char, 256> letters{};
        std::string matched;
        if (text.empty() || groups.empty() || table.empty())
            return false;
        std::size_t m_t = std::min(groups.size() - 1, table.size());
        std::size_t target_len = countOccour(text);
        target_len = std::min({target_len, text.length(), m_t});
        if (target_len < 6)
            return false;
        if (target_len > text.length())
            target_len = text.length();

        matched.reserve(target_len);

        auto search = [&](auto &&self, size_t start, size_t pos) -> bool {
            if (pos == target_len) {

                if (groups[pos] == groups[pos - 1])
                    return false;
                std::size_t last = occour(matched, matched.back());
                unsigned char cx = static_cast<unsigned char>(matched.back());
                char o = 0;
                for (long i = static_cast<long>(matched.size()) - 1; i >= 0; --i) {
                    unsigned char ci = static_cast<unsigned char>(matched[i]);
                    if (std::tolower(ci) != std::tolower(cx)) {
                        o = matched[i];
                        break;
                    }
                }
                if (o == 0)
                    return false;
                std::size_t p = occour(matched, o);
                return (last == p + 1 && p >= 2);
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
            std::cout << "Pattern Found: " << matched << '\n';
            std::cout << "Actual:Expected:Character\n";
            for (size_t i = 0; i < matched.length(); ++i) {
                unsigned char ch = static_cast<unsigned char>(matched[i]);
                int key = std::isupper(ch) ? 1 : 0;
                std::cout << key << ":" << table[i] << ":" << matched[i] << '\n';
            }

            total_matches++;
            auto found_ = patterns_found.find(matched);
            if (found_ == patterns_found.end()) {
                patterns_found[matched] = 1;
                return true;
            } else if (found && found_ != patterns_found.end()) {
                patterns_found[matched]++;
                std::cout << "\nMatched: again " << matched << ":" << patterns_found[matched] << std::endl;
                return false;
            }
        }
        return false;
    }

    void fill_groups() {
        groups.clear();
        table.clear();
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

    size_t total() const { return total_matches; }

  protected:
    std::vector<std::size_t> groups;
    std::vector<std::size_t> table;
    std::unordered_map<std::string, std::size_t> patterns_found;
    std::size_t total_matches = 0;
};

#ifdef __linux__

volatile sig_atomic_t quit = 0;

void handle_signal(int) { quit = 1; }

#endif

std::string trim(const std::string &text) {
    std::string t;
    for (const auto &i : text) {
        if (i != '\n')
            t += i;
    }
    return t;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Error incorrect arguments..\nUse: " << argv[0] << " textfile attempts chunk_size.\n";
        return EXIT_FAILURE;
    }

    int epoch_timeout = std::stoi(argv[2]);
    int buffer_size = std::stoi(argv[3]);
    if (epoch_timeout < 0 || buffer_size < 6) {
        std::cerr << "Error invalid input or buffer size.\n";
        return EXIT_FAILURE;
    }
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
    if (value.length() <= static_cast<std::size_t>(buffer_size)) {
        std::cerr << "Error file smaller than buffer_size.\n";
        return EXIT_FAILURE;
    }
    std::size_t epoch = static_cast<std::size_t>(epoch_timeout);
    std::size_t in = 0;
    std::uniform_int_distribution<int> dist(0, value.length() - static_cast<std::size_t>(buffer_size) - 1);

#ifdef __linux__
    signal(SIGINT, handle_signal);
#endif
    while (1) {
        std::string shuf = value;
        std::shuffle(shuf.begin(), shuf.end(), eng);
        int pos = dist(eng);
        std::string buffer = shuf.substr(pos, buffer_size);
        std::string pattern;
        if (chk.testPattern(buffer)) {
            std::cout << "True.\n";
        }
        if (epoch > 0) {
            ++in;
            if (in >= epoch)
                break;
        }
#ifdef __linux__
        if (quit == 1) {
            std::cout << "\n";
            break;
        }
#endif
    }
    std::cout << "Ran for: " << in << " iterations, found: " << chk.total() << std::endl;
    return EXIT_SUCCESS;
}
