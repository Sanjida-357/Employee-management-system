# Employee Management System

## Overview
The Employee Management System (EMS) is a C++-based project developed using Object-Oriented Programming concepts. It manages employee records, attendance, leave, payroll, and salary slip generation through a backend server and a web-based frontend dashboard.

The system demonstrates integration of OOP, file handling, and frontend-backend communication in a complete application.

---

## Features
- Add employee records
- View all employees
- Search employee by ID
- Update employee details
- Delete employee records
- Track attendance and leave
- Calculate salary based on employee type
- Handle overtime, bonus, tax, insurance, pension, and leave deduction
- Generate salary slips
- Store employee data in JSON file
- Download salary slips

---

## Payroll Logic

### Employee Types and Deductions

| Type        | Tax | Insurance | Pension | Overtime Rate |
|------------|-----|----------|--------|---------------|
| Permanent  | 10% | 5%       | 8%     | 300 per hour  |
| Part-Time  | 5%  | 0%       | 0%     | 200 per hour  |
| Contract   | 8%  | 0%       | 0%     | 200 per hour  |

---

### Leave Policy
- One paid leave is allowed per month
- Additional absences are treated as unpaid

Formula:
Absent Days = Total Days - Days Present
Unpaid Days = Absent Days - Paid Leave
Leave Deduction = (Base Salary / Total Days) × Unpaid Days

---

### Salary Calculation
Gross Salary = Base Salary + Bonus + Overtime Pay
Net Salary = Gross Salary - (Tax + Insurance + Pension + Leave Deduction)


---

## OOP Concepts Used
- Classes and Objects
- Inheritance
- Polymorphism
- Abstract Classes
- Function Overloading
- Templates
- Encapsulation
- Exception Handling
- Dynamic Memory Allocation

---

## Employee Types
- Permanent Employee
- Part-Time Employee
- Contract Employee

---

## Technologies Used
- C++
- cpp-httplib
- nlohmann/json
- HTML
- CSS
- JavaScript

---

## Project Structure
```text
Employee-management-system/
│
├── backend/
│   ├── server.cpp
│   ├── employee.cpp
│   ├── feature.cpp
│   ├── httplib.h
│   ├── json.hpp
│   └── employees.json
│
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── script.js
│
├── README.md
└── .gitignore