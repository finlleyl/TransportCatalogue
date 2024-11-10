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

void OutputBus(std::ostream& output, const TransportCatalogue& catalogue, std::string_view bus_name) {
    const auto info = catalogue.GetRouteInfo(bus_name);
    if (!info) {
        output << "Bus " << bus_name << ": not found\n";
        return;
    }

    output << "Bus " << bus_name << ": "
           << info->stop_count << " stops on route, "
           << info->unique_stop_count << " unique stops, "
           << info->route_length << " route length, "
           << std::setprecision(6) << info->curvature << " curvature\n";
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

