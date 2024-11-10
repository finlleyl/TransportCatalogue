#ifndef GEO_H
#define GEO_H

#include <cmath>

struct Coordinates {
    double latitude;
    double longitude;
};

inline double ComputeDistance(Coordinates from, Coordinates to) {
    static constexpr double kEarthRadius = 6371000.0;
    using namespace std;
    double lat_rad1 = from.latitude * M_PI / 180.0;
    double lat_rad2 = to.latitude * M_PI / 180.0;
    double delta_lat = (to.latitude - from.latitude) * M_PI / 180.0;
    double delta_lon = (to.longitude - from.longitude) * M_PI / 180.0;

    double a = sin(delta_lat / 2) * sin(delta_lat / 2) +
               cos(lat_rad1) * cos(lat_rad2) *
               sin(delta_lon / 2) * sin(delta_lon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return kEarthRadius * c;
}

#endif // GEO_H
