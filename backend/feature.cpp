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

    int getPaidLeave() const { return paidLeave; }
    int getUnpaidLeave() const { return unpaidLeave; }

    double calculateLeaveDeduction(double dailySalary) const {
        return unpaidLeave * dailySalary;
    }
};

// ================= PAYROLL CLASS =================
class Payroll : public Attendance, public Leave {
private:
    Employee* emp;
    double bonus;
    double overtimeRate;

public:
    Payroll(Employee* e) {
        emp = e;
        bonus = 0;
        overtimeRate = 200;
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
        out << "Employee ID: " << emp->getEmpID() << "\n";
        out << "Name: " << emp->getName() << "\n";
        out << "Department: " << emp->getDepartment() << "\n";
        out << "Type: " << emp->getEmployeeType() << "\n";
        out << "Days Present: " << daysPresent << "\n";
        out << "Total Days: " << totalDays << "\n";
        out << "Attendance %: " << getAttendancePercentage() << "\n";
        out << "Paid Leave: " << paidLeave << "\n";
        out << "Unpaid Leave: " << unpaidLeave << "\n";
        out << "Overtime Hours: " << overtimeHours << "\n";
        out << "Bonus: " << bonus << "\n";
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

// ================= EMPLOYEE RECORD =================
struct EmployeeRecord {
    int empID;
    string name;
    string department;
    string employeeType;
    double baseSalary;
    int daysPresent;
    int totalDays;
    int paidLeave;
    int unpaidLeave;
    double overtimeHours;
    double bonus;
};

inline string recordToCSV(const EmployeeRecord& r) {
    ostringstream out;
    out << r.empID << ","
        << r.name << ","
        << r.department << ","
        << r.employeeType << ","
        << r.baseSalary << ","
        << r.daysPresent << ","
        << r.totalDays << ","
        << r.paidLeave << ","
        << r.unpaidLeave << ","
        << r.overtimeHours << ","
        << r.bonus;
    return out.str();
}

inline EmployeeRecord csvToRecord(const string& line) {
    stringstream ss(line);
    string token;
    EmployeeRecord r;

    getline(ss, token, ','); r.empID = stoi(token);
    getline(ss, r.name, ',');
    getline(ss, r.department, ',');
    getline(ss, r.employeeType, ',');
    getline(ss, token, ','); r.baseSalary = stod(token);
    getline(ss, token, ','); r.daysPresent = stoi(token);
    getline(ss, token, ','); r.totalDays = stoi(token);
    getline(ss, token, ','); r.paidLeave = stoi(token);
    getline(ss, token, ','); r.unpaidLeave = stoi(token);
    getline(ss, token, ','); r.overtimeHours = stod(token);
    getline(ss, token, ','); r.bonus = stod(token);

    return r;
}

inline EmployeeRecord payrollToRecord(const Payroll& p) {
    Employee* e = p.getEmployee();
    EmployeeRecord r;
    r.empID = e->getEmpID();
    r.name = e->getName();
    r.department = e->getDepartment();
    r.employeeType = e->getEmployeeType();
    r.baseSalary = e->getBaseSalary();
    r.daysPresent = p.getDaysPresent();
    r.totalDays = p.getTotalDays();
    r.paidLeave = p.getPaidLeave();
    r.unpaidLeave = p.getUnpaidLeave();
    r.overtimeHours = p.getOvertimeHours();
    r.bonus = p.getBonus();
    return r;
}

// ================= FILE HANDLING =================
inline vector<EmployeeRecord> loadEmployeeRecords(const string& filename = "employees.txt") {
    vector<EmployeeRecord> records;
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

inline void writeAllEmployeeRecords(const vector<EmployeeRecord>& records, const string& filename = "employees.txt") {
    ofstream file(filename, ios::trunc);

    if (!file) {
        throw InvalidInputException("File could not be opened.");
    }

    for (const auto& r : records) {
        file << recordToCSV(r) << endl;
    }

    file.close();
}

inline bool saveEmployeeToFile(const Payroll& p, const string& filename = "employees.txt") {
    EmployeeRecord newRecord = payrollToRecord(p);
    vector<EmployeeRecord> records = loadEmployeeRecords(filename);

    bool updatedExisting = false;
    for (auto& r : records) {
        if (r.empID == newRecord.empID) {
            r = newRecord;
            updatedExisting = true;
            break;
        }
    }

    if (!updatedExisting) {
        records.push_back(newRecord);
    }

    writeAllEmployeeRecords(records, filename);
    return updatedExisting;
}

inline vector<string> loadEmployeesFromFile(const string& filename = "employees.txt") {
    vector<string> lines;
    vector<EmployeeRecord> records = loadEmployeeRecords(filename);

    for (const auto& r : records) {
        ostringstream out;
        out << "ID: " << r.empID
            << " | Name: " << r.name
            << " | Dept: " << r.department
            << " | Type: " << r.employeeType
            << " | Salary: " << r.baseSalary
            << " | Present: " << r.daysPresent << "/" << r.totalDays
            << " | Paid Leave: " << r.paidLeave
            << " | Unpaid Leave: " << r.unpaidLeave
            << " | Overtime: " << r.overtimeHours
            << " | Bonus: " << r.bonus;
        lines.push_back(out.str());
    }

    return lines;
}

inline bool findEmployeeByID(int id, EmployeeRecord& found, const string& filename = "employees.txt") {
    vector<EmployeeRecord> records = loadEmployeeRecords(filename);

    for (const auto& r : records) {
        if (r.empID == id) {
            found = r;
            return true;
        }
    }

    return false;
}

inline bool updateEmployeeByID(int id, const EmployeeRecord& updatedRecord, const string& filename = "employees.txt") {
    vector<EmployeeRecord> records = loadEmployeeRecords(filename);

    for (auto& r : records) {
        if (r.empID == id) {
            r = updatedRecord;
            writeAllEmployeeRecords(records, filename);
            return true;
        }
    }

    return false;
}

inline bool deleteEmployeeByID(int id, const string& filename = "employees.txt") {
    vector<EmployeeRecord> records = loadEmployeeRecords(filename);
    vector<EmployeeRecord> filtered;
    bool found = false;

    for (const auto& r : records) {
        if (r.empID == id) {
            found = true;
        } else {
            filtered.push_back(r);
        }
    }

    if (found) {
        writeAllEmployeeRecords(filtered, filename);
    }

    return found;
}

// ================= SALARY SLIP FILE =================
inline void saveSlipToFile(const string& slipText, const string& filename = "salary_slips.txt") {
    ofstream file(filename, ios::app);

    if (!file) {
        throw InvalidInputException("Salary slip file could not be opened.");
    }

    file << "====================================\n";
    file << slipText << "\n";
    file << "====================================\n";

    file.close();
}

#endif