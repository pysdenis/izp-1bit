// Usage: program reads measurements from standard input (stdin).
// Each measurement should be provided as seven whitespace-separated values:
//   year month day hours minutes seconds temperature
// Example (file):
//   2023 6 15 9 30 0 21.5
// Run with input redirection:
//   ./main < mereni.txt
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int day;
  int month;
  int year;
} Date;

typedef struct {
  Date date;
  int hours;
  int minutes;
  int seconds;
} DateTime;

typedef struct {
  Date date;
  DateTime DateTime;
  float temperature;
} Measurement;

int isDateValid(Date date) {
  if (date.year > 2500) return 0;
  if (date.month < 1 || date.month > 12) return 0;
  if (date.day < 1 || date.day > 31) return 0;
  if (date.month == 2 && date.day > 29) return 0;
  if ((date.month == 4 || date.month == 6 || date.month == 9 ||
       date.month == 11) &&
      date.day > 30)
    return 0;
  return 1;
}

Date compareDates(const Date date1, const Date date2) {
  if (date1.year < date2.year) return date1;
  if (date1.year > date2.year) return date2;
  if (date1.month < date2.month) return date1;
  if (date1.month > date2.month) return date2;
  if (date1.day < date2.day) return date1;
  return date2;
}

DateTime createDateTime(Date date, int hours, int minutes, int seconds) {
  DateTime dt;
  dt.date = date;
  dt.hours = hours;
  dt.minutes = minutes;
  dt.seconds = seconds;
  return dt;
}

int getTimeInYearInSeconds(DateTime dateTime) {
  int seconds = 0;

  seconds += dateTime.seconds;
  seconds += dateTime.minutes * 60;
  seconds += dateTime.hours * 3600;
  seconds += (dateTime.date.day - 1) * 86400;
  for (int month = 1; month < dateTime.date.month; month++) {
    if (month == 2) {
      if ((dateTime.date.year % 4 == 0 && dateTime.date.year % 100 != 0) ||
          (dateTime.date.year % 400 == 0)) {
        seconds += 29 * 86400;
      } else {
        seconds += 28 * 86400;
      }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
      seconds += 30 * 86400;
    } else {
      seconds += 31 * 86400;
    }
  }

  return seconds;
}

int loadMeasurement(Measurement *m) {
  int fields = scanf("%d %d %d %d %d %d %f", &m->date.year, &m->date.month,
                     &m->date.day, &m->DateTime.hours, &m->DateTime.minutes,
                     &m->DateTime.seconds, &m->temperature);
  if (fields != 7) return 0;
  m->DateTime.date = m->date;
  return 1;
}

void printMeasurement(const Measurement m) {
  printf("\n-------------------------------------------------------------\n");
  printf("|\tDate: %02d-%02d-%04d\t|\n", m.date.day, m.date.month, m.date.year);
  printf("|\tTime: %02d:%02d:%02d\t\t|\n", m.DateTime.hours, m.DateTime.minutes,
         m.DateTime.seconds);
  printf("|\tTemperature: %.2f\t|\n", m.temperature);
  printf("\n-------------------------------------------------------------\n");
}

int isMeasurementValid(const Measurement m) {
  if (!isDateValid(m.date)) return 0;
  if (m.DateTime.hours < 0 || m.DateTime.hours > 23) return 0;
  if (m.DateTime.minutes < 0 || m.DateTime.minutes > 59) return 0;
  if (m.DateTime.seconds < 0 || m.DateTime.seconds > 59) return 0;
  return 1;
}

int main(int argc, char *argv[]) {
  //   Date date;
  //   int hours, minutes, seconds;

  //   printf("Enter year: ");
  //   scanf("%d", &date.year);
  //   printf("Enter month: ");
  //   scanf("%d", &date.month);
  //   printf("Enter day: ");
  //   scanf("%d", &date.day);

  //   if (!isDateValid(date)) {
  //     printf("Invalid date. Please enter a valid date.\n");
  //     return 1;
  //   }

  //   printf("Valid date: %02d-%02d-%04d\n", date.day, date.month, date.year);

  //   printf("Enter hours: ");
  //   scanf("%d", &hours);
  //   printf("Enter minutes: ");
  //   scanf("%d", &minutes);
  //   printf("Enter seconds: ");
  //   scanf("%d", &seconds);
  //   DateTime dateTime = createDateTime(date, hours, minutes, seconds);
  //   int totalSeconds = getTimeInYearInSeconds(dateTime);
  //   printf("Total seconds since start of the year: %d\n", totalSeconds);

  // Read first measurement (if any) from stdin. This allows running
  // the program with input redirection: ./main < mereni.txt
  Measurement m;
  if (!loadMeasurement(&m)) {
    fprintf(stderr, "No measurement available on stdin.\n");
    return 1;
  }
  printMeasurement(m);

  if (isMeasurementValid(m)) {
    printf("Valid measurement.\n");
  } else {
    printf("Invalid measurement.\n");
  }

  printf("\n\n\n\n");

  Measurement measurements[5];
  int count = 0;
  while (count < 5) {
    Measurement tmp;
    if (!loadMeasurement(&tmp)) break;
    if (!isMeasurementValid(tmp)) {
      fprintf(stderr, "Invalid measurement encountered; skipping.\n");
      continue;
    }
    measurements[count++] = tmp;
  }

  if (count == 0) {
    fprintf(stderr, "No valid measurements to process.\n");
    return 1;
  }

  float sum = 0;
  float maxTemp = measurements[0].temperature;
  int foundMorning = 0;
  Measurement hottestMorning;

  for (int i = 0; i < count; i++) {
    sum += measurements[i].temperature;
    if (measurements[i].temperature > maxTemp) {
      maxTemp = measurements[i].temperature;
    }
    if (measurements[i].DateTime.hours < 12) {
      if (!foundMorning ||
          measurements[i].temperature > hottestMorning.temperature) {
        hottestMorning = measurements[i];
        foundMorning = 1;
      }
    }
  }
  printf("Average temperature: %.2f\n", sum / count);
  printf("Maximum temperature: %.2f\n", maxTemp);
  if (foundMorning) {
    printf("Hottest morning: %02d:%02d:%02d\n", hottestMorning.DateTime.hours,
           hottestMorning.DateTime.minutes, hottestMorning.DateTime.seconds);
  } else {
    printf("No morning measurements found.\n");
  }

  int pole2d[4][5];

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      pole2d[i][j] = 5;
    }
  }

  FILE *f = fopen("pole.txt", "w");

  if (f == NULL) {
    fprintf(stderr, "Error opening file for writing.\n");
    return 1;
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      fprintf(f, "%d ", pole2d[i][j]);
    }
    fprintf(f, "\n");
  }

  fclose(f);
  return 0;
}
