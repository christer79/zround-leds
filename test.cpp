#include <stdio.h>

int main()
{
    char *buffer = "\$LAP 1 1 2 ALOsdalkmdlka 1.7 9.9#";

    char type[4];
    int race_number, position, laps, extra;
    char nick[15];
    float lap_time, best_lap;
    int total_read = sscanf(buffer, "$LAP %d %d %d %[A-Za-z] %f %f#", &race_number, &position, &laps, &nick, &lap_time, &best_lap);

    printf("Value of is: %d\n", race_number);
    printf("Value of is: %d\n", position);
    printf("Value of is: %d\n", laps);
    printf("Value of is: %s\n", nick);
    printf("Value of is: %f\n", lap_time);
    printf("Value of is: %f\n", best_lap);

    printf("\nTotal items read: %d", total_read);
    return 0;
}