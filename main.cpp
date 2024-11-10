#include <iostream>
#include "input_reader.hpp"
#include "stat_reader.hpp"
#include "transport_catalogue.hpp"

int main() {
    TransportCatalogue catalogue;

    ReadInput(std::cin, catalogue);

    ProcessOutputRequests(std::cin, catalogue);

    return 0;
}
