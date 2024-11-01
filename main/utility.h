#include <vector>

template <typename T>
void appendArrays(std::vector<T>& targetArray, const std::vector<T>& arrayToAppend) {
    targetArray.insert(targetArray.end(), arrayToAppend.begin(), arrayToAppend.end());
}

template <typename T>
std::vector<T> getUserInput() {
    static_assert(std::is_integral<T>::value, "getUserInput requires an integral type");

    std::string input;
    std::cin >> input;

    std::vector<T> userArray;
    for (char ch : input) {
        if (std::isdigit(ch)) {
            userArray.push_back(static_cast<T>(ch - '0'));
        } else {
            std::cerr << "Warning: Non-digit character ignored: " << ch << std::endl;
        }
    }
    return userArray;
}
