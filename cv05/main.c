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

// 1. Vytvořte funkci struct measurement_t load_measurement(), která čte ze
// stdin jednotlivé položky měření a vrací odpovídající měření.
Measurement createMeasurement() {
  Measurement m;
  printf("Enter year: ");
  scanf("%d", &m.date.year);
  printf("Enter month: ");
  scanf("%d", &m.date.month);
  printf("Enter day: ");
  scanf("%d", &m.date.day);
  printf("Enter hours: ");
  scanf("%d", &m.DateTime.hours);
  printf("Enter minutes: ");
  scanf("%d", &m.DateTime.minutes);
  printf("Enter seconds: ");
  scanf("%d", &m.DateTime.seconds);
  printf("Enter temperature: ");
  scanf("%f", &m.temperature);
  m.DateTime.date = m.date;
  return m;
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

// Načtěte 5 měření do pole (ve funkci main) a spočítejte:
// • průměrnou teplotu, maximální teplotu, nejteplejší dopoledne,

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

  Measurement m = createMeasurement();
  printMeasurement(m);

  if (isMeasurementValid(m)) {
    printf("Valid measurement.\n");
  } else {
    printf("Invalid measurement.\n");
  }

  Measurement measurements[5];
  for (int i = 0; i < 5; i++) {
    printf("Enter measurement %d:\n", i + 1);
    measurements[i] = createMeasurement();
    if (!isMeasurementValid(measurements[i])) {
      printf("Invalid measurement. Please enter a valid measurement.\n");
      i--;
    }
  }

  float sum = 0;
  float maxTemp = measurements[0].temperature;
  Measurement hottestMorning = measurements[0];

  for (int i = 0; i < 5; i++) {
    sum += measurements[i].temperature;
    if (measurements[i].temperature > maxTemp) {
      maxTemp = measurements[i].temperature;
    }
    if (measurements[i].DateTime.hours < 12) {
      if (measurements[i].temperature > hottestMorning.temperature) {
        hottestMorning = measurements[i];
      }
    }
  }
  printf("Average temperature: %.2f\n", sum / 5);
  printf("Maximum temperature: %.2f\n", maxTemp);
  printf("Hottest morning: %02d:%02d:%02d\n", hottestMorning.DateTime.hours,
         hottestMorning.DateTime.minutes, hottestMorning.DateTime.seconds);
  return 0;
}
