
#include "input_reader.hpp"

#include <sstream>

void ReadInput(std::istream &input, TransportCatalogue &catalogue) {
    std::cout << "Enter the number of inputs: ";
    int n;
    if (!(input >> n)) {
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

void RemoveUnwantedCharacters(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(),
                             [](char c) { return c == ',' || c == ':'; }),
              str.end());
}

Stop parseStop(std::istream& input) {
    std::string line;
    std::getline(input, line);

    RemoveUnwantedCharacters(line);

    std::istringstream line_stream(line);

    std::string stop_keyword, stop_name;
    double latitude, longitude;

    line_stream >> stop_keyword;
    std::getline(line_stream, stop_name, ' ');
    stop_name.erase(stop_name.find_last_not_of(' ') + 1);

    line_stream >> latitude >> longitude;

    std::unordered_map<std::string, int> distances;
    std::string distance_part;
    while (line_stream >> distance_part) {
        auto m_pos = distance_part.find('m');
        if (m_pos == std::string::npos) {
            throw std::runtime_error("Invalid distance format");
        }

        int distance = std::stoi(distance_part.substr(0, m_pos));

        std::string to_stop;
        line_stream >> to_stop >> to_stop;
        distances[to_stop] = distance;
    }

    return Stop{stop_name, latitude, longitude};
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
