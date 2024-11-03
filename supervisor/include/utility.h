/**
 * @file utility.h
 * @brief Utility functions for the project.
 */

#include <vector>

/**
 * @brief Append one array to another.
 * @tparam T Type of the elements in the arrays.
 * @param targetArray The array to which elements will be appended.
 * @param arrayToAppend The array whose elements will be appended.
 */
template <typename T>
void appendArrays(std::vector<T> &targetArray, const std::vector<T> &arrayToAppend)
{
    targetArray.insert(targetArray.end(), arrayToAppend.begin(), arrayToAppend.end());
}

/**
 * @brief Get user input as an array of integers.
 * @tparam T Type of the elements in the array. Must be an integral type.
 * @return The array of integers entered by the user.
 */
template <typename T>
std::vector<T> getUserInput()
{
    static_assert(std::is_integral<T>::value, "getUserInput requires an integral type");

    std::string input;
    std::cin >> input;

    std::vector<T> userArray;
    for (char ch : input)
    {
        if (std::isdigit(ch))
        {
            userArray.push_back(static_cast<T>(ch - '0'));
        }
        else
        {
            std::cerr << "Warning: Non-digit character ignored: " << ch << std::endl;
        }
    }
    return userArray;
}

/**
 * @brief Add a value to the last element of a vector and handle carry.
 * @tparam T Type of the elements in the vector. Must be an integral type.
 * @param vec The vector to which the value will be added.
 * @param addValue The value to add.
 */
template <typename T>
void addToVector(std::vector<uint8_t> &vec, T addValue)
{
    static_assert(std::is_integral<T>::value, "Template parameter must be an integer type.");
    int carry = static_cast<int>(addValue);
    int arraySize = vec.size() - 1;
    vec[arraySize] += carry;
    for (int i = arraySize; i > 0; --i)
    {
        while (vec[i] >= 10)
        {
            vec[i] -= 10;
            vec[i - 1]++;
        }
    }
}