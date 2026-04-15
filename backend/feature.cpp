#ifndef FEATURE_CPP
#define FEATURE_CPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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

    double getAttendancePercentage() const {
        return (double(daysPresent) / totalDays) * 100;
    }
};

// ================= LEAVE CLASS =================
class Leave {
protected:
    int paidLeave;
    int unpaidLeave;

public:
    Leave() {
        paidLeave = 0;
        unpaidLeave = 0;
    }

    void setLeave(int paid, int unpaid) {
        if (paid < 0 || unpaid < 0) {
            throw InvalidInputException("Leave cannot be negative.");
        }
        paidLeave = paid;
        unpaidLeave = unpaid;
    }

    double calculateLeaveDeduction(double dailySalary) const {
        return unpaidLeave * dailySalary;
    }
};

#endif
// ================= PAYROLL CLASS =================
class Payroll : public Attendance, public Leave {
private:
    Employee* emp;   // dynamic memory
    double bonus;
    double overtimeRate;

public:
    Payroll(Employee* e) {
        emp = e;
        bonus = 0;
        overtimeRate = 200; // default
    }

    ~Payroll() {
        delete emp; // free memory
    }

    void setBonus(double b) {
        if (b < 0) throw InvalidInputException("Bonus cannot be negative.");
        bonus = b;
    }

    double calculateOvertimePay() const {
        return overtimeHours * overtimeRate;
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
        double daily = emp->getBaseSalary() / maxValue(totalDays, 1);
        return Leave::calculateLeaveDeduction(daily);
    }

    double calculateNetSalary() const {
        double gross = calculateGrossSalary();
        double deductions = calculateTax() + calculateInsurance() + calculatePension() + calculateLeaveDeduction();
        return gross - deductions;
    }

    string generateSlip() const {
        ostringstream out;

        out << "EMPLOYEE SALARY SLIP\n";
        out << "Name: " << emp->getName() << "\n";
        out << "Type: " << emp->getEmployeeType() << "\n";
        out << "Department: " << emp->getDepartment() << "\n";
        out << "-------------------------\n";
        out << "Gross Salary: " << calculateGrossSalary() << "\n";
        out << "Tax: " << calculateTax() << "\n";
        out << "Insurance: " << calculateInsurance() << "\n";
        out << "Pension: " << calculatePension() << "\n";
        out << "Leave Deduction: " << calculateLeaveDeduction() << "\n";
        out << "Net Salary: " << calculateNetSalary() << "\n";

        return out.str();
    }
};
// ================= FILE HANDLING =================
void saveEmployeeToFile(const Payroll& p, const string& filename = "employees.txt") {
    ofstream file(filename, ios::app);

    if (!file) {
        throw InvalidInputException("File could not be opened.");
    }

    file << p.calculateNetSalary() << endl;

    file.close();
}

vector<string> loadEmployeesFromFile(const string& filename = "employees.txt") {
    vector<string> data;
    ifstream file(filename);

    if (!file) {
        return data; // return empty if file doesn't exist
    }

    string line;
    while (getline(file, line)) {
        data.push_back(line);
    }

    file.close();
    return data;
}