// stat_reader.h

#ifndef STAT_READER_H
#define STAT_READER_H

#include <iostream>
#include "transport_catalogue.hpp"

void ProcessOutputRequests(std::istream& input, const TransportCatalogue& catalogue);

void OutputBus(std::istream& input, std::ostream& output, const TransportCatalogue& catalogue);

void OutputStop(std::istream& input, std::ostream& output, const TransportCatalogue& catalogue);

#endif // STAT_READER_H
