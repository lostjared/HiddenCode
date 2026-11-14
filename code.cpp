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
#include "argz.hpp"

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
        std::size_t total = 0;
        for (std::size_t spot = 0; spot < text.length(); ++spot) {
            unsigned char ch = static_cast<unsigned char>(text[spot]);
            if (std::isalpha(ch)) {
                total++;
            }
        }
        return total;
    }

    bool testPattern(std::string_view text) {
        if (text.empty() || groups.empty() || table.empty())
            return false;

        std::size_t m_t = std::min(groups.size() - 1, table.size());
        std::size_t max_len = countOccour(text);
        max_len = std::min({max_len, text.length(), m_t});

        std::vector<std::size_t> valid_lengths;
        std::size_t t_len = 0;
        std::size_t step = 1;
        while (t_len + step <= max_len) {
            t_len += step;
            if (t_len >= 6) {
                valid_lengths.push_back(t_len);
            }
            step++;
        }

        for (auto it = valid_lengths.rbegin(); it != valid_lengths.rend(); ++it) {
            std::size_t target_len = *it;

            std::array<char, 256> letters{};
            std::string matched;
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

            if (search(search, 0, 0)) {
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
                } else {
                    patterns_found[matched]++;
                    return false;
                }
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
    size_t words() const { return patterns_found.size(); }

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

std::string reverseString(std::string_view rev) {
    std::string value;
    for (auto it = rev.rbegin(); it != rev.rend(); ++it) {
        value += *it;
    }
    return value;
}

int main(int argc, char **argv) {

    Argz<std::string> parser(argc, argv);
    parser.addOptionDoubleValue(256, "input", "file").addOptionDoubleValue(257, "keyword", "keyword").addOptionDoubleValue(258, "iter", "iterations").addOptionDoubleValue(259, "chunk", "chunk size").addOptionDouble(260, "rev", "reverse");

    Argument<std::string> arg;
    std::string input_file, keyword_var;
    std::string iter, chunk;
    bool reverse = false;
    int arg_value = 0;
    try {
        while ((arg_value = parser.proc(arg)) != -1) {
            switch (arg_value) {
            case 256:
                input_file = arg.arg_value;
                break;
            case 257:
                keyword_var = arg.arg_value;
                break;
            case 258:
                iter = arg.arg_value;
                break;
            case 259:
                chunk = arg.arg_value;
                break;
            case 260:
                reverse = true;
                break;
            }
        }

    } catch (const ArgException<std::string> &e) {
        std::cerr << e.text() << std::endl;
        return EXIT_FAILURE;
    }

    if (!keyword_var.empty()) {
        Check chk;
        chk.fill();
        if (reverse)
            keyword_var = reverseString(keyword_var);
        if (chk.testPattern(keyword_var)) {
            std::cout << "True: Contains Pattern.\n";
        } else {
            std::cout << "False: Does not contain Pattern.\n";
        }
        return EXIT_SUCCESS;
    } else if (input_file.empty() || (!input_file.empty() && (chunk.empty() || iter.empty()))) {
        std::cerr << "Error incorrect arguments..\nUse: " << argv[0] << " --input textfile --iter attempts --chunk chunk_size.\n";
        return EXIT_FAILURE;
    }

    int epoch_timeout = std::stoi(iter);
    int buffer_size = std::stoi(chunk);
    if (epoch_timeout < 0 || buffer_size < 6) {
        std::cerr << "Error invalid input or buffer size.\n";
        return EXIT_FAILURE;
    }
    Check chk;
    chk.fill();
    std::fstream file;
    file.open(input_file, std::ios::in);
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
    if (reverse)
        value = reverseString(value);

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
    std::cout << "Ran for: " << in << " iterations, found: " << chk.total() << " unique: " << chk.words() << std::endl;
    return EXIT_SUCCESS;
}
