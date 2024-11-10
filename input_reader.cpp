
#include "input_reader.hpp"

#include <sstream>

void ReadInput(std::istream &input, TransportCatalogue &catalogue) {
    std::cout << "Enter the number of inputs: ";
    int n;
    if (!(std::cin >> n)) {
        throw std::runtime_error("Invalid input: number of commands must be an integer");
    }
    std::cin.ignore();

    for (int i = 0; i < n; ++i) {
        std::string line;
        if (!std::getline(input, line)) {
            throw std::runtime_error("Invalid input: unexpected end of input");
        }

        std::istringstream line_stream(line);
        std::string command;
        line_stream >> command;

        if (command == "Stop") {
            catalogue.addStop(parseStop(line_stream));
        } else if (command == "Bus") {
            catalogue.addBus(parseBus(line_stream, catalogue));
        }
    }
}

Stop parseStop(std::istream &input) {
    std::string stop_name;
    double latitude, longitude;

    if (!std::getline(input, stop_name, ':')) {
        throw std::runtime_error("Invalid stop format: no name found");
    }

    std::string coordinates;
    if (!std::getline(input, coordinates)) {
        throw std::runtime_error("Invalid stop format: no coordinates found");
    }

    coordinates.erase(0, coordinates.find_first_not_of(' '));

    std::erase(coordinates, ',');

    std::istringstream iss(coordinates);
    if (!(iss >> latitude >> longitude)) {
        throw std::runtime_error("Invalid stop format: incorrect coordinates");
    }

    return {std::move(stop_name), latitude, longitude};
}

std::vector<std::string_view> Split(std::string_view str, char delimiter) {
    std::vector<std::string_view> result;
    size_t pos = 0;
    while (true) {
        size_t delimiter_pos = str.find(delimiter, pos);
        result.push_back(str.substr(pos, delimiter_pos - pos));
        if (delimiter_pos == std::string_view::npos) {
            break;
        }
        pos = delimiter_pos + 1;
    }
    return result;
}

Bus parseBus(std::istream &input, TransportCatalogue& catalogue) {
    std::string bus_name;
    if (!std::getline(input, bus_name, ':')) {
        throw std::runtime_error("Invalid bus format: no bus name found");
    }

    bus_name.erase(0, bus_name.find_first_not_of(' '));

    std::string route_string;
    if (!std::getline(input, route_string)) {
        throw std::runtime_error("Invalid bus format: no route description found");
    }

    route_string.erase(0, route_string.find_first_not_of(' '));

    char delimiter = route_string.find('>') != std::string::npos ? '>' : '-';
    std::vector<std::string_view> stop_names = Split(route_string, delimiter);

    std::vector<const Stop*> stops;
    for (const auto& stop_name : stop_names) {
        const Stop* stop = catalogue.findStop(stop_name);
        if (!stop) {
            Stop new_stop(std::string(stop_name), 0.0, 0.0);
            catalogue.addStop(new_stop);
            stop = catalogue.findStop(stop_name);
        }
        stops.push_back(stop);
    }


    bool is_round_trip = (delimiter == '-');
    return {std::move(bus_name), std::move(stops), is_round_trip};
}
