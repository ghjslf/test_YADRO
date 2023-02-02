#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "utils.hpp"


struct Coordinate {
    std::string index;
    std::string header;
};

int calculate(int arg1, int arg2, const std::string &op) {
    if (op == "+") {
        return arg1 + arg2;
    }
    if (op == "-") {
        return arg1 - arg2;
    }   
    if (op == "*") {
        return arg1 * arg2;
    }
    if (op == "/") {
        if (arg2 == 0) {
            throw std::invalid_argument("Zero division error");
        }
        return arg1 / arg2;
    }
    throw std::invalid_argument("Unknown operator");
}

std::string lookup_table(const std::vector<std::vector<std::string>> &table, Coordinate cell) {
    auto index = cell.index;
    auto header = cell.header;

    auto record_iterator = std::find_if(begin(table), end(table), [index] (auto row) { return row[0] == index; });
    if (record_iterator == end(table)) {
        throw std::invalid_argument("No such row");
    }

    std::vector<std::string> record = *record_iterator;

    auto header_iterator = std::find(table[0].begin(), table[0].end(), header);
    if (header_iterator == end(table[0])) {
        throw std::invalid_argument("No such column");
    }

    return record[std::distance(table[0].begin(), header_iterator)];
}

Coordinate parse_argument(std::string argument) {
    // Implementation is based on: 
    // 1) only digits used for indices 
    // 2) names do not contain digits
    Coordinate cell;

    size_t position = 0;
    while (!isdigit(argument[position])) {
        cell.header.push_back(argument[position]);
        position++;
    }
    while (isdigit(argument[position])) {
        cell.index.push_back(argument[position]);
        position++;
    }
    if (argument.size() != position || cell.index == "" || cell.header == "") {
        throw std::invalid_argument("Invalid expression argument");
    }

    return cell;
}

std::string expression_handle(const std::vector<std::vector<std::string>> &table, std::string &expression) {
    std::string first_argument;
    std::string second_argument;
    std::string operation;

    size_t operation_position = expression.find_first_of("+-*/");
    if (operation_position == std::string::npos) {
        throw std::invalid_argument("Invalid expression");
    }

    // Parse first argument
    for (size_t position = 1; position < operation_position; position++) {
        first_argument.push_back(expression[position]);
    }

    auto first_value = lookup_table(table, parse_argument(first_argument));
    if (first_value == "") {
        throw std::invalid_argument("Empty cell. Please fill table");
    }
    if (!is_number(first_value)) {
        throw std::invalid_argument("Cell contains not number value");
    }

    // Parse operation
    operation = expression[operation_position];

    // Parse second argument
    for (size_t position = operation_position + 1; position < expression.size(); position++) {
        second_argument.push_back(expression[position]);
    }

    auto second_value = lookup_table(table, parse_argument(second_argument));
    if (second_value == "") {
        throw std::invalid_argument("Empty cell. Please fill table");
    }
    if (!is_number(second_value)) {
        throw std::invalid_argument("Cell contains not number value");
    }

    // Change cell value if current value in passed through arguments cells is a number
    // If there is a formula program execution will be interrupted with error
    return convert_to_string(calculate(convert_to_integer(first_value), convert_to_integer(second_value), operation));
}

void format_table(std::vector<std::vector<std::string>> &table) {
    if (table.size() != table[0].size()) {
        throw std::invalid_argument("Wrong table format");
    }
    for (size_t record_number = 1; record_number < table.size(); record_number++) {
        if (table[0].size() != table[record_number].size()) {
            throw std::invalid_argument("Wrong table format");
        }
        for (size_t cell_number = 1; cell_number < table[record_number].size(); cell_number++) {

            if (table[record_number][cell_number][0] == '=') {
                table[record_number][cell_number] = expression_handle(table, table[record_number][cell_number]);
            }
            else {
                if (table[record_number][cell_number] == "") {
                    throw std::invalid_argument("Empty cell. Please fill table");
                }
                if (!is_number(table[record_number][cell_number])) {
                    throw std::invalid_argument("Cell contains not number value");
                }
            }
        }
    }
}

void print_table(const std::vector<std::vector<std::string>> &table) {
    for (size_t record_number = 0; record_number < table.size(); record_number++) {
        for (size_t cell_number = 0; cell_number < table[record_number].size() - 1; cell_number++) {
            std::cout << table[record_number][cell_number] << ',';
        }
        std::cout << table[record_number][table[record_number].size() - 1] << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        throw std::invalid_argument("File path not passed :(");
    }

    std::string path = argv[1];
    std::fstream input_file_stream(path);
    if (!input_file_stream.is_open()) {
        throw std::invalid_argument("Can't open file :(");
    }

    std::vector<std::vector<std::string>> table;

    std::string line;
    // Get records
    while (std::getline(input_file_stream, line)) {
        table.push_back(split(line, ','));
    }

    try {
        format_table(table);
    }
    catch (std::invalid_argument const &ex){
        std::cout << ex.what() << std::endl;
        return -1;
    }

    print_table(table);
    
    return 0;
}