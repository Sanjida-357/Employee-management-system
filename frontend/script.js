const API_BASE = "http://localhost:8080";

let currentSlipText = "";
let currentSlipHTML = "";
let editingEmployeeId = null;

function showMessage(message, isError = false) {
  const box = document.getElementById("messageBox");
  if (!box) return;

  box.style.display = "block";
  box.textContent = message;
  box.style.background = isError ? "#3b0d0d" : "rgba(30, 64, 175, 0.18)";
  box.style.color = isError ? "#fecaca" : "#dbeafe";
  box.style.border = isError
    ? "1px solid rgba(239, 68, 68, 0.35)"
    : "1px solid rgba(96, 165, 250, 0.22)";
}

function clearMessage() {
  const box = document.getElementById("messageBox");
  if (!box) return;
  box.style.display = "none";
  box.textContent = "";
}

function formatCurrency(value) {
  const num = Number(value) || 0;
  return `₹${num.toFixed(2)}`;
}

function getCurrentDate() {
  const today = new Date();
  return today.toLocaleDateString("en-IN", {
    day: "2-digit",
    month: "long",
    year: "numeric"
  });
}

function getFormData() {
  return {
    empID: parseInt(document.getElementById("empID").value),
    name: document.getElementById("name").value.trim(),
    department: document.getElementById("department").value.trim(),
    employmentType: document.getElementById("employmentType").value,
    daysPresent: parseInt(document.getElementById("daysPresent").value),
    totalDays: parseInt(document.getElementById("totalDays").value),
    leaveTaken: parseInt(document.getElementById("leaveTaken").value),
    baseSalary: parseFloat(document.getElementById("baseSalary").value),
    overtimeHours: parseFloat(document.getElementById("overtimeHours").value),
    bonus: parseFloat(document.getElementById("bonus").value)
  };
}

function validateEmployee(data) {
  if (
    isNaN(data.empID) ||
    !data.name ||
    !data.department ||
    !data.employmentType ||
    isNaN(data.daysPresent) ||
    isNaN(data.totalDays) ||
    isNaN(data.leaveTaken) ||
    isNaN(data.baseSalary) ||
    isNaN(data.overtimeHours) ||
    isNaN(data.bonus)
  ) {
    showMessage("Please fill all fields properly.", true);
    return false;
  }

  const validTypes = ["Permanent", "Part-Time", "Contract"];
  if (!validTypes.includes(data.employmentType)) {
    showMessage("Invalid employment type.", true);
    return false;
  }

  if (data.empID <= 0) {
    showMessage("Employee ID must be positive.", true);
    return false;
  }

  if (data.totalDays <= 0) {
    showMessage("Total working days must be greater than 0.", true);
    return false;
  }

  if (
    data.daysPresent < 0 ||
    data.leaveTaken < 0 ||
    data.baseSalary < 0 ||
    data.overtimeHours < 0 ||
    data.bonus < 0
  ) {
    showMessage("Values cannot be negative.", true);
    return false;
  }

  if (data.daysPresent > data.totalDays) {
    showMessage("Days present cannot be greater than total days.", true);
    return false;
  }

  if (data.leaveTaken > data.totalDays) {
    showMessage("Leave taken cannot be greater than total days.", true);
    return false;
  }

  return true;
}

function renderEmployeesToTable(tableBodyId, employees) {
  const tbody = document.getElementById(tableBodyId);
  if (!tbody) return;

  tbody.innerHTML = "";

  if (!employees || employees.length === 0) {
    tbody.innerHTML = `<tr><td colspan="6">No employees found.</td></tr>`;
    return;
  }

  employees.forEach((emp) => {
    const row = document.createElement("tr");
    row.innerHTML = `
      <td>${emp.empID}</td>
      <td>${emp.name}</td>
      <td>${emp.department}</td>
      <td>${emp.employmentType}</td>
      <td>${formatCurrency(emp.baseSalary)}</td>
      <td>
        <button class="action-btn view-btn" onclick="viewEmployee(${emp.empID})">View</button>
        <button class="action-btn edit-btn" onclick="fillFormForEdit(${emp.empID})">Edit</button>
        <button class="action-btn delete-btn" onclick="deleteEmployee(${emp.empID})">Delete</button>
      </td>
    `;
    tbody.appendChild(row);
  });
}

async function saveEmployee() {
  const data = getFormData();
  if (!validateEmployee(data)) return;

  try {
    const res = await fetch(`${API_BASE}/employees`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(data)
    });

    const result = await res.json();

    if (!res.ok) {
      throw new Error(result.message || "Failed to save employee");
    }

    showMessage(result.message || "Employee saved successfully.");
    clearForm();
    loadEmployees();
  } catch (err) {
    showMessage(err.message || "Failed to save employee.", true);
  }
}

async function loadEmployees() {
  try {
    const res = await fetch(`${API_BASE}/employees`);
    const employees = await res.json();

    renderEmployeesToTable("employeeTableBody", employees);
    renderEmployeesToTable("employeeTableBodyEmployees", employees);
  } catch (err) {
    showMessage("Could not load employees.", true);
  }
}

async function searchEmployee() {
  const id = document.getElementById("searchID").value;

  if (!id) {
    showMessage("Enter employee ID to search.", true);
    return;
  }

  try {
    const res = await fetch(`${API_BASE}/employees/${id}`);
    const result = await res.json();

    if (!res.ok) {
      throw new Error(result.message || "Employee not found");
    }

    renderEmployeesToTable("employeeTableBody", [result]);
    renderEmployeesToTable("employeeTableBodyEmployees", [result]);

    showMessage("Employee found successfully.");
  } catch (err) {
    showMessage(err.message || "Employee not found.", true);
  }
}

function copySearchAndRun(sourceInputId) {
  const sourceInput = document.getElementById(sourceInputId);
  const mainSearch = document.getElementById("searchID");

  if (!sourceInput || !mainSearch) return;

  mainSearch.value = sourceInput.value;
  searchEmployee();
}

function fillForm(emp) {
  document.getElementById("empID").value = emp.empID;
  document.getElementById("name").value = emp.name;
  document.getElementById("department").value = emp.department;
  document.getElementById("employmentType").value = emp.employmentType;
  document.getElementById("daysPresent").value = emp.daysPresent;
  document.getElementById("totalDays").value = emp.totalDays;
  document.getElementById("leaveTaken").value = emp.leaveTaken;
  document.getElementById("baseSalary").value = emp.baseSalary;
  document.getElementById("overtimeHours").value = emp.overtimeHours;
  document.getElementById("bonus").value = emp.bonus;
}

async function viewEmployee(id) {
  try {
    const res = await fetch(`${API_BASE}/employees/${id}`);
    const emp = await res.json();

    if (!res.ok) {
      throw new Error(emp.message || "Employee not found");
    }

    fillForm(emp);
    editingEmployeeId = null;
    showMessage("Employee details loaded into the form.");
    activateMenuSection("dashboard");
  } catch (err) {
    showMessage(err.message || "Could not load employee.", true);
  }
}

async function fillFormForEdit(id) {
  try {
    const res = await fetch(`${API_BASE}/employees/${id}`);
    const emp = await res.json();

    if (!res.ok) {
      throw new Error(emp.message || "Employee not found");
    }

    fillForm(emp);
    editingEmployeeId = id;

    showMessage(`Editing employee ID ${id}. Update after making changes.`);
    activateMenuSection("dashboard");
  } catch (err) {
    showMessage(err.message || "Could not load employee.", true);
  }
}

async function updateEmployee() {
  const data = getFormData();
  if (!validateEmployee(data)) return;

  if (editingEmployeeId === null) {
    showMessage("Please click Edit on an employee first.", true);
    return;
  }

  try {
    const res = await fetch(`${API_BASE}/employees/${editingEmployeeId}`, {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(data)
    });

    const result = await res.json();

    if (!res.ok) {
      throw new Error(result.message || "Failed to update employee");
    }

    showMessage(result.message || "Employee updated successfully.");
    clearForm();
    loadEmployees();
  } catch (err) {
    showMessage(err.message || "Failed to update employee.", true);
  }
}

async function deleteEmployee(id) {
  const confirmed = confirm(`Are you sure you want to delete employee ID ${id}?`);
  if (!confirmed) return;

  try {
    const res = await fetch(`${API_BASE}/employees/${id}`, {
      method: "DELETE"
    });

    const result = await res.json();

    if (!res.ok) {
      throw new Error(result.message || "Failed to delete employee");
    }

    if (editingEmployeeId === id) {
      clearForm();
    }

    showMessage(result.message || "Employee deleted successfully.");
    loadEmployees();
  } catch (err) {
    showMessage(err.message || "Failed to delete employee.", true);
  }
}

function buildSlipText(data, result) {
  return `
EMPLOYEE SALARY SLIP

Date: ${getCurrentDate()}
Employee ID: ${data.empID}
Employee Name: ${data.name}
Department: ${data.department}
Employment Type: ${data.employmentType}

Attendance Percentage: ${result.attendancePercentage.toFixed(2)}%
Days Present: ${data.daysPresent}
Total Days: ${data.totalDays}
Leave Taken: ${data.leaveTaken}

Basic Pay: ${formatCurrency(result.basicPay)}
Bonus: ${formatCurrency(data.bonus)}
Overtime Pay: ${formatCurrency(result.overtimePay)}
Gross Salary: ${formatCurrency(result.grossSalary)}

Tax Deduction: ${formatCurrency(result.tax)}
Insurance Deduction: ${formatCurrency(result.insurance)}
Pension Deduction: ${formatCurrency(result.pension)}
Leave Deduction: ${formatCurrency(result.leaveDeduction)}

Net Salary: ${formatCurrency(result.netSalary)}
  `.trim();
}

function buildSlipHTML(data, result) {
  return `
    <div class="payslip">
      <div class="payslip-header">
        <div class="payslip-company">
          <h2>Employee Salary Slip</h2>
          <p>Employee Management System</p>
        </div>

        <div class="payslip-meta">
          <div><strong>Date:</strong> ${getCurrentDate()}</div>
          <div><strong>Employee ID:</strong> ${data.empID}</div>
          <div><strong>Department:</strong> ${data.department}</div>
          <div><strong>Type:</strong> ${data.employmentType}</div>
        </div>
      </div>

      <div class="payslip-body">
        <div class="payslip-grid">
          <div class="payslip-section">
            <h3>Employee Details</h3>
            <div class="payslip-row">
              <span class="payslip-label">Employee Name</span>
              <span class="payslip-value">${data.name}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Department</span>
              <span class="payslip-value">${data.department}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Employment Type</span>
              <span class="payslip-value">${data.employmentType}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Basic Pay</span>
              <span class="payslip-value">${formatCurrency(result.basicPay)}</span>
            </div>
          </div>

          <div class="payslip-section">
            <h3>Attendance & Leave</h3>
            <div class="payslip-row">
              <span class="payslip-label">Days Present</span>
              <span class="payslip-value">${data.daysPresent}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Total Working Days</span>
              <span class="payslip-value">${data.totalDays}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Leave Taken</span>
              <span class="payslip-value">${data.leaveTaken}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Attendance Percentage</span>
              <span class="payslip-value">${result.attendancePercentage.toFixed(2)}%</span>
            </div>
          </div>
        </div>

        <div class="payslip-summary">
          <div class="summary-box earnings-box">
            <h3>Earnings</h3>
            <div class="payslip-row">
              <span class="payslip-label">Basic Pay</span>
              <span class="payslip-value">${formatCurrency(result.basicPay)}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Bonus</span>
              <span class="payslip-value">${formatCurrency(data.bonus)}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Overtime Pay</span>
              <span class="payslip-value">${formatCurrency(result.overtimePay)}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Gross Salary</span>
              <span class="payslip-value">${formatCurrency(result.grossSalary)}</span>
            </div>
          </div>

          <div class="summary-box deductions-box">
            <h3>Deductions</h3>
            <div class="payslip-row">
              <span class="payslip-label">Tax</span>
              <span class="payslip-value">${formatCurrency(result.tax)}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Insurance</span>
              <span class="payslip-value">${formatCurrency(result.insurance)}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Pension</span>
              <span class="payslip-value">${formatCurrency(result.pension)}</span>
            </div>
            <div class="payslip-row">
              <span class="payslip-label">Leave Deduction</span>
              <span class="payslip-value">${formatCurrency(result.leaveDeduction)}</span>
            </div>
          </div>
        </div>

        <div class="net-pay-box">
          <span>NET SALARY</span>
          <strong>${formatCurrency(result.netSalary)}</strong>
        </div>

        <div class="payslip-footer">
          This is a system-generated salary slip.
        </div>
      </div>
    </div>
  `;
}

async function generateSalarySlip() {
  const data = getFormData();
  if (!validateEmployee(data)) return;

  try {
    const res = await fetch(`${API_BASE}/calculate`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(data)
    });

    const result = await res.json();

    if (!res.ok) {
      throw new Error(result.message || "Failed to generate salary slip");
    }

    currentSlipText = buildSlipText(data, result);
    currentSlipHTML = buildSlipHTML(data, result);

    document.getElementById("salarySlip").innerHTML = currentSlipHTML;

    showMessage("Salary slip generated successfully.");
    activateMenuSection("slips");
  } catch (err) {
    showMessage(err.message || "Failed to generate salary slip.", true);
  }
}

function saveSalarySlip() {
  if (!currentSlipText) {
    showMessage("Generate a salary slip first.", true);
    return;
  }

  const empID = document.getElementById("empID").value || "employee";
  localStorage.setItem(`salarySlip_${empID}`, currentSlipText);
  localStorage.setItem(`salarySlipHTML_${empID}`, currentSlipHTML);
  showMessage("Salary slip saved successfully.");
}

function downloadSlip() {
  if (!currentSlipText) {
    showMessage("Generate a salary slip first.", true);
    return;
  }

  const empID = document.getElementById("empID").value || "employee";
  const blob = new Blob([currentSlipText], { type: "text/plain" });
  const link = document.createElement("a");

  link.href = URL.createObjectURL(blob);
  link.download = `salary_slip_${empID}.txt`;
  link.click();

  showMessage("Salary slip downloaded successfully.");
}

function clearForm() {
  document.getElementById("empID").value = "";
  document.getElementById("name").value = "";
  document.getElementById("department").value = "";
  document.getElementById("employmentType").value = "Permanent";
  document.getElementById("daysPresent").value = "";
  document.getElementById("totalDays").value = "";
  document.getElementById("leaveTaken").value = "";
  document.getElementById("baseSalary").value = "";
  document.getElementById("overtimeHours").value = "";
  document.getElementById("bonus").value = "";

  editingEmployeeId = null;
  currentSlipText = "";
  currentSlipHTML = "";
}

function setActiveMenu(element) {
  document.querySelectorAll(".menu-item").forEach((item) => {
    item.classList.remove("active");
  });

  if (element) {
    element.classList.add("active");
  }
}

function showOnlySection(sectionId) {
  document.querySelectorAll(".content-section").forEach((section) => {
    section.classList.remove("active-section");
  });

  const target = document.getElementById(sectionId);
  if (target) {
    target.classList.add("active-section");
  }
}

function activateMenuSection(sectionName) {
  const menuItems = document.querySelectorAll(".menu-item");

  if (sectionName === "dashboard") {
    setActiveMenu(menuItems[0]);
    showOnlySection("dashboardSection");
  } else if (sectionName === "employees") {
    setActiveMenu(menuItems[1]);
    showOnlySection("employeesSection");
    loadEmployees();
  } else if (sectionName === "payroll") {
    setActiveMenu(menuItems[2]);
    showOnlySection("payrollSection");
  } else if (sectionName === "slips") {
    setActiveMenu(menuItems[3]);
    showOnlySection("slipsSection");
  }
}

function showSection(section, element) {
  clearMessage();
  setActiveMenu(element);

  if (section === "dashboard") {
    showOnlySection("dashboardSection");
    showMessage("Dashboard view active");
  } else if (section === "employees") {
    showOnlySection("employeesSection");
    loadEmployees();
    showMessage("Employees section active");
  } else if (section === "payroll") {
    showOnlySection("payrollSection");
    showMessage("Payroll section active");
  } else if (section === "slips") {
    showOnlySection("slipsSection");
    showMessage("Salary slips section active");
  }
}

window.onload = function () {
  activateMenuSection("dashboard");
  loadEmployees();
};