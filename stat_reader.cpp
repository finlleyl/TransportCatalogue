// stat_reader.cpp

#include "stat_reader.hpp"
#include <iomanip>
#include <sstream>
#include "transport_catalogue.hpp"

void ProcessOutputRequests(std::istream& input, const TransportCatalogue& catalogue) {
    int query_count = 0;
    input >> query_count;
    input.ignore();

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(input, line);

        std::istringstream line_stream(line);
        std::string command;
        line_stream >> command;

        if (command == "Bus") {
            OutputBus(line_stream, std::cout, catalogue);
        } else if (command == "Stop") {
            OutputStop(line_stream, std::cout, catalogue);
        }
    }
}

void OutputBus(std::istream &input, std::ostream &output, const TransportCatalogue &catalogue) {
    std::string bus_name;
    input >> bus_name;
    bus_name.erase(0, bus_name.find_first_not_of(' '));

    if (catalogue.findBus(bus_name)) {
        if (const auto route_info = catalogue.getRouteInfo(bus_name); route_info.has_value()) {
            output << "Bus " << bus_name << ": "
                      << route_info.value().stop_count << " stops on route, "
                      << route_info.value().unique_stop_count << " unique stops, "
                      << std::setprecision(6) << route_info.value().route_length << " route length" << std::endl;
        } else {
            output << "Bus " << bus_name << ": no route info available" << std::endl;
        }
    } else {
        output << "Bus " << bus_name << ": not found" << std::endl;
    }
}

void OutputStop(std::istream &input, std::ostream &output, const TransportCatalogue &catalogue) {
    std::string stop_name;
    input >> stop_name;
    stop_name.erase(0, stop_name.find_first_not_of(' '));

    if (const auto stop_info = catalogue.getStopInfo(stop_name); stop_info.has_value()) {
        if (const auto& buses = stop_info->buses; buses.empty()) {
            output << "Stop " << stop_name << ": no buses" << std::endl;
        } else {
            output << "Stop " << stop_name << ": buses";
            for (const Bus* bus : buses) {
                output << " " << bus->name;
            }
            output << std::endl;
        }
    } else {
        output << "Stop " << stop_name << ": not found" << std::endl;
    }
}

