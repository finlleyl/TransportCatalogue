//
// Created by ilya_ on 05.11.2024.
//

#ifndef INPUT_READER_HPP
#define INPUT_READER_HPP

#include <iostream>
#include <string>
#include "transport_catalogue.hpp"

void ReadInput(std::istream &input, TransportCatalogue &catalogue);

Stop parseStop(std::istream &input);

Bus parseBus(std::istream &input, TransportCatalogue &catalogue);

std::vector<std::string_view> Split(std::string_view str, char delimiter);

void RemoveUnwantedCharacters(std::string& str);


#endif //INPUT_READER_HPP
