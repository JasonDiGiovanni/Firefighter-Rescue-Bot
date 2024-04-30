#ifndef HEADER_FILE
#define HEADER_FILE
//Function headers and macro definitions

double move_forward(oi_t*sensor_data, double distance_mm);
double move_backward(oi_t*sensor_data, double distance_mm);
double turn_right(oi_t*sensor_data, double angle_deg);
double turn_left(oi_t*sensor_data, double angle_deg);

#endif
