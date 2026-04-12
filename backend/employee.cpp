#include <iostream>
#include <string>
using namespace std;

class Employee {
protected:
    int empID;
    string name;

public:
    void setBasicInfo(int id, string empName) {
        empID = id;
        name = empName;
    }

    int getEmpID() { return empID; }
    string getName() { return name; }
};

class Attendance {
protected:
    int daysPresent, totalDays;

public:
    void setAttendance(int present, int total) {
        daysPresent = present;
        totalDays = total;
    }

    double attendancePercentage() {
        if (totalDays == 0) return 0;
        return ((double)daysPresent / totalDays) * 100;
    }
};

class Leave {
protected:
    int leaveTaken;
    int maxLeave = 5;

public:
    void setLeave(int leave) {
        leaveTaken = leave;
    }

    int extraLeave() {
        return (leaveTaken > maxLeave) ? (leaveTaken - maxLeave) : 0;
    }

    double leaveDeduction() {
        return extraLeave() * 500;
    }
};

class Salary : public Employee, public Attendance, public Leave {
private:
    double baseSalary, overtimeHours, bonus;

public:
    void setSalaryDetails(double base, double overtime, double bonusAmount) {
        baseSalary = base;
        overtimeHours = overtime;
        bonus = bonusAmount;
    }

    double overtimePay() {
        return overtimeHours * 200;
    }

    double grossSalary() {
        return baseSalary + overtimePay() + bonus;
    }

    double tax() {
        return grossSalary() * 0.10;
    }

    double insurance() {
        return grossSalary() * 0.05;
    }

    double pension() {
        return grossSalary() * 0.03;
    }

    double netSalary() {
        return grossSalary() - (tax() + insurance() + pension() + leaveDeduction());
    }

    void displaySalarySlip() {
        cout << "\nEmployee ID: " << getEmpID();
        cout << "\nName: " << getName();

        cout << "\n----- Salary Slip -----";
        cout << "\nBase Salary: " << baseSalary;
        cout << "\nOvertime Pay: " << overtimePay();
        cout << "\nBonus: " << bonus;
        cout << "\nGross Salary: " << grossSalary();

        cout << "\n\nDeductions:";
        cout << "\nTax: " << tax();
        cout << "\nInsurance: " << insurance();
        cout << "\nPension: " << pension();
        cout << "\nLeave Deduction: " << leaveDeduction();

        cout << "\n\nNet Salary: " << netSalary();
        cout << "\nAttendance: " << attendancePercentage() << "%";
        cout << "\n----------------------\n";
    }
};

int main() {
    // Temporary test (can remove later when connecting frontend)
    Salary emp;

    emp.setBasicInfo(101, "Rahul");
    emp.setAttendance(20, 22);
    emp.setLeave(6);
    emp.setSalaryDetails(30000, 5, 2000);

    emp.displaySalarySlip();

    return 0;
}