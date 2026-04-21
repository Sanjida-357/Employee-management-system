#ifndef FEATURE_CPP
#define FEATURE_CPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "employee.cpp"

using namespace std;

// ================= ATTENDANCE CLASS =================
class Attendance {
protected:
    int daysPresent;
    int totalDays;
    double overtimeHours;

public:
    Attendance() {
        daysPresent = 0;
        totalDays = 0;
        overtimeHours = 0;
    }

    void setAttendance(int present, int total) {
        if (present < 0 || total <= 0 || present > total) {
            throw InvalidInputException("Invalid attendance data.");
        }
        daysPresent = present;
        totalDays = total;
    }

    void setOvertime(double hours) {
        if (hours < 0) {
            throw InvalidInputException("Overtime cannot be negative.");
        }
        overtimeHours = hours;
    }

    int getDaysPresent() const { return daysPresent; }
    int getTotalDays() const { return totalDays; }
    double getOvertimeHours() const { return overtimeHours; }

    double getAttendancePercentage() const {
        return (static_cast<double>(daysPresent) / totalDays) * 100.0;
    }

    int getAbsentDays() const {
        return totalDays - daysPresent;
    }
};

// ================= LEAVE CLASS =================
class Leave {
protected:
    int paidLeaveUsed;

public:
    Leave() {
        paidLeaveUsed = 0;
    }

    void setLeave(int paid) {
        if (paid < 0) {
            throw InvalidInputException("Leave cannot be negative.");
        }
        paidLeaveUsed = paid;
    }

    int getPaidLeaveUsed() const { return paidLeaveUsed; }

    double calculateLeaveDeduction(int daysPresent, int totalDays, double baseSalary) const {
        const int paidLeaveLimit = 1;

        int absentDays = totalDays - daysPresent;
        int usablePaidLeave = min(paidLeaveUsed, paidLeaveLimit);
        int unpaidDays = absentDays - usablePaidLeave;

        if (unpaidDays <= 0) return 0.0;

        double dailySalary = baseSalary / totalDays;
        return unpaidDays * dailySalary;
    }
};

// ================= PAYROLL CLASS =================
class Payroll : public Attendance, public Leave {
private:
    Employee* emp;
    double bonus;

public:
    Payroll(Employee* e) {
        emp = e;
        bonus = 0;
    }

    ~Payroll() {
        delete emp;
    }

    Employee* getEmployee() const {
        return emp;
    }

    void setBonus(double b) {
        if (b < 0) throw InvalidInputException("Bonus cannot be negative.");
        bonus = b;
    }

    double getBonus() const { return bonus; }

    double getOvertimeRate() const {
        if (emp->getEmployeeType() == "Permanent") {
            return 300.0;
        }
        return 200.0;
    }

    double calculateOvertimePay() const {
        return overtimeHours * getOvertimeRate();
    }

    double calculateGrossSalary() const {
        return emp->calculateBasicPay() + calculateOvertimePay() + bonus;
    }

    double calculateTax() const {
        return calculateGrossSalary() * emp->getTaxRate();
    }

    double calculateInsurance() const {
        return calculateGrossSalary() * emp->getInsuranceRate();
    }

    double calculatePension() const {
        return calculateGrossSalary() * emp->getPensionRate();
    }

    double calculateLeaveDeduction() const {
        return Leave::calculateLeaveDeduction(daysPresent, totalDays, emp->getBaseSalary());
    }

    double calculateNetSalary() const {
        double deductions = calculateTax() + calculateInsurance() + calculatePension() + calculateLeaveDeduction();
        return calculateGrossSalary() - deductions;
    }

    string generateSlip() const {
        ostringstream out;

        out << "EMPLOYEE SALARY SLIP\n";
        out << "Employee ID: " << emp->getEmpID() << "\n";
        out << "Name: " << emp->getName() << "\n";
        out << "Department: " << emp->getDepartment() << "\n";
        out << "Type: " << emp->getEmployeeType() << "\n";
        out << "Days Present: " << daysPresent << "\n";
        out << "Total Days: " << totalDays << "\n";
        out << "Attendance %: " << getAttendancePercentage() << "\n";
        out << "Paid Leave Used: " << paidLeaveUsed << "\n";
        out << "Overtime Hours: " << overtimeHours << "\n";
        out << "Bonus: " << bonus << "\n";
        out << "-------------------------\n";
        out << "Basic Pay: " << emp->calculateBasicPay() << "\n";
        out << "Overtime Pay: " << calculateOvertimePay() << "\n";
        out << "Gross Salary: " << calculateGrossSalary() << "\n";
        out << "Tax: " << calculateTax() << "\n";
        out << "Insurance: " << calculateInsurance() << "\n";
        out << "Pension: " << calculatePension() << "\n";
        out << "Leave Deduction: " << calculateLeaveDeduction() << "\n";
        out << "Net Salary: " << calculateNetSalary() << "\n";

        return out.str();
    }
};

// ================= EMPLOYEE RECORD =================
struct EmployeeRecordFile {
    int empID;
    string name;
    string department;
    string employeeType;
    double baseSalary;
    int daysPresent;
    int totalDays;
    int paidLeaveUsed;
    double overtimeHours;
    double bonus;
};

inline string recordToCSV(const EmployeeRecordFile& r) {
    ostringstream out;
    out << r.empID << ","
        << r.name << ","
        << r.department << ","
        << r.employeeType << ","
        << r.baseSalary << ","
        << r.daysPresent << ","
        << r.totalDays << ","
        << r.paidLeaveUsed << ","
        << r.overtimeHours << ","
        << r.bonus;
    return out.str();
}

inline EmployeeRecordFile csvToRecord(const string& line) {
    stringstream ss(line);
    string token;
    EmployeeRecordFile r;

    getline(ss, token, ','); r.empID = stoi(token);
    getline(ss, r.name, ',');
    getline(ss, r.department, ',');
    getline(ss, r.employeeType, ',');
    getline(ss, token, ','); r.baseSalary = stod(token);
    getline(ss, token, ','); r.daysPresent = stoi(token);
    getline(ss, token, ','); r.totalDays = stoi(token);
    getline(ss, token, ','); r.paidLeaveUsed = stoi(token);
    getline(ss, token, ','); r.overtimeHours = stod(token);
    getline(ss, token, ','); r.bonus = stod(token);

    return r;
}

inline vector<EmployeeRecordFile> loadEmployeeRecords(const string& filename = "employees.txt") {
    vector<EmployeeRecordFile> records;
    ifstream file(filename);

    if (!file) return records;

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            records.push_back(csvToRecord(line));
        }
    }

    file.close();
    return records;
}

inline void writeAllEmployeeRecords(const vector<EmployeeRecordFile>& records, const string& filename = "employees.txt") {
    ofstream file(filename);

    for (const auto& record : records) {
        file << recordToCSV(record) << "\n";
    }

    file.close();
}

#endif