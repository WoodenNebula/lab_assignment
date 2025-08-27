#include <iostream>
#include <string>
#include <string_view>

#include "src/commons.h"

enum class EMonth {
  January = 1,
  February,
  March,
  April,
  May,
  June,
  July,
  August,
  September,
  October,
  November,
  December
};

struct SDate {
  int Year;
  EMonth Month;
  int Day;

  friend std::ostream &operator<<(std::ostream &out, const SDate &date) {
    out << date.Year << " " << SDate::EnumToString(date.Month) << " "
        << date.Day;
    return out;
  }

public:
  static std::string_view EnumToString(EMonth month) {
    switch (month) {
    case EMonth::January:
      return "January";
    case EMonth::February:
      return "February";
    case EMonth::March:
      return "March";
    case EMonth::April:
      return "April";
    case EMonth::May:
      return "May";
    case EMonth::June:
      return "June";
    case EMonth::July:
      return "July";
    case EMonth::August:
      return "August";
    case EMonth::September:
      return "September";
    case EMonth::October:
      return "October";
    case EMonth::November:
      return "November";
    case EMonth::December:
      return "December";
    }
    return "";
  }
};

class CCompany {
public:
  CCompany(const std::string &name, const std::string &location)
      : Name(name), Location(location) {}

public:
  std::string Name;
  std::string Location;
};

class CJob {
public:
  CJob(const CCompany &employer, const std::string &pos, int salary)
      : Employer(employer), Position(pos), m_salary(salary) {}

  friend std::ostream &operator<<(std::ostream &out, const CJob &job) {
    out << "Employer: " << job.Employer.Name << "\n"
        << "Work Address: " << job.Employer.Location << "\n"
        << "Position: " << job.Position << "\n"
        << "Salary: " << job.m_salary;

    return out;
  }

public:
  CCompany Employer;
  std::string Position;

protected:
  int m_salary;
};

class CPerson {
public:
  CPerson(const std::string &name, const std::string &country, const SDate &DoB,
          int height, int weight, const CJob &job)
      : Name(name), Country(country), m_DoB(DoB), m_height(height),
        m_weight(weight), Job(job) {}

  void DisplayAllDetails() {
    std::cout << "Name: " << Name << "\n"
              << "DoB: " << m_DoB << "\n"
              << "Height: " << m_height << "\"\n"
              << "Weight: " << m_weight << " kg\n"
              << "Country: " << Country << "\n"
              << "Job: {\n"
              << Job << "\n"
              << "}" << std::endl;
  }

public:
  std::string Name;
  std::string Country;
  CJob Job;

protected:
  SDate m_DoB;
  int m_weight;
  int m_height;
};

int main() {
  CCompany company("ABC company", "Kathmandu");
  CJob job(company, "AI Researcher", 150'000);

  SDate DoB = {1990, EMonth::December, 15};
  CPerson ram("Ram", "Nepal", DoB, 6, 75, job);

  ram.DisplayAllDetails();

  footer();
}
