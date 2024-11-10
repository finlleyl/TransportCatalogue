// stat_reader.cpp

#include "stat_reader.hpp"
#include <iomanip>
#include <sstream>
#include "transport_catalogue.hpp"

void ProcessOutputRequests(std::istream& input, const TransportCatalogue& catalogue) {
    int query_count = 0;
    input >> query_count;
    input.ignore();

    for (int i = 0; i < query_count; ++i) {`
        std::string line;
        std::getline(input, line);

        std::istringstream line_stream(line);
        std::string command;
        line_stream >> command;

        if (command == "Bus") {
            std::string bus_name;
            line_stream >> bus_name;
            bus_name.erase(0, bus_name.find_first_not_of(' '));

            if (catalogue.findBus(bus_name)) {
                auto route_info = catalogue.getRouteInfo(bus_name);
                if (route_info.has_value()) {
                    std::cout << "Bus " << bus_name << ": "
                              << route_info.value().stop_count << " stops on route, "
                              << route_info.value().unique_stop_count << " unique stops, "
                              << std::setprecision(6) << route_info.value().route_length << " route length" << std::endl;
                } else {
                    std::cout << "Bus " << bus_name << ": no route info available" << std::endl;
                }
            } else {
                std::cout << "Bus " << bus_name << ": not found" << std::endl;
            }
        }
    }
}
