const API_URL = window.location.protocol === 'file:'
    ? 'http://localhost:8080/api'
    : '/api';

console.log("Using API URL:", API_URL);

async function login() {
    alert("Debug: Login function started. Trying to connect to: " + API_URL);
    const u = document.getElementById('username').value;
    const p = document.getElementById('password').value;

    try {
        console.log("Fetching login...");
        const res = await fetch(`${API_URL}/login`, {
            method: 'POST',
            body: JSON.stringify({ username: u, password: p })
        });
        console.log("Response received");
        const data = await res.json();

        if (data.success) {
            document.getElementById('login-screen').style.display = 'none';
            document.getElementById('dashboard').style.display = 'block';

            // Role-Based Access Control
            const role = data.role;
            const tabInventory = document.getElementById('tab-inventory');
            const tabTriage = document.getElementById('tab-triage');
            const tabReports = document.getElementById('tab-reports');

            // Reset visibility
            tabInventory.style.display = 'none';
            tabTriage.style.display = 'none';
            tabReports.style.display = 'none';

            if (role === 'Admin') {
                tabInventory.style.display = 'inline-block';
                tabTriage.style.display = 'inline-block';
                tabReports.style.display = 'inline-block';
                showTab('inventory');
            } else if (role === 'Doctor') {
                tabInventory.style.display = 'inline-block';
                tabTriage.style.display = 'inline-block';
                showTab('triage'); // Default to Triage for Doctors
            } else if (role === 'Pharmacist') {
                tabInventory.style.display = 'inline-block';
                showTab('inventory');
            } else if (role === 'Billing') {
                tabReports.style.display = 'inline-block';
                showTab('reports');
            }
        } else {
            alert("Login Failed: Invalid Credentials");
        }
    } catch (e) {
        console.error("Login Error:", e);
        alert("Connection error details: " + e.message);
    }
}

function logout() {
    location.reload();
}

function showTab(tabId) {
    document.querySelectorAll('.tab-content').forEach(el => el.style.display = 'none');
    document.getElementById(tabId).style.display = 'block';

    // Update active state on buttons
    document.querySelectorAll('.tabs button').forEach(btn => btn.classList.remove('active'));
    const activeBtn = document.getElementById(`tab-${tabId}`);
    if (activeBtn) activeBtn.classList.add('active');

    if (tabId === 'inventory') loadDrugs();
    if (tabId === 'reports') loadReport();
}

async function loadDrugs() {
    const res = await fetch(`${API_URL}/drugs`);
    const drugs = await res.json();

    const tbody = document.querySelector('#drug-table tbody');
    tbody.innerHTML = '';

    drugs.forEach(d => {
        tbody.innerHTML += `
            <tr>
                <td>${d.name}</td>
                <td>${d.quantity}</td>
                <td>${d.price}</td>
                <td>${d.expiry}</td>
            </tr>
        `;
    });
}

async function loadReport() {
    const res = await fetch(`${API_URL}/report`);
    const data = await res.json();

    document.getElementById('revenue-display').innerText = `KES ${data.revenue}`;
    document.getElementById('users-display').innerText = data.total_users;
}

function calculateBMI() {
    const weight = parseFloat(document.getElementById('weight').value);
    const heightCm = parseFloat(document.getElementById('height').value);

    if (weight > 0 && heightCm > 0) {
        const heightM = heightCm / 100;
        const bmi = (weight / (heightM * heightM)).toFixed(1);

        let status = 'Normal';
        let color = '#2ecc71'; // Green

        if (bmi < 18.5) { status = 'Underweight'; color = '#f1c40f'; }
        else if (bmi >= 25 && bmi < 30) { status = 'Overweight'; color = '#e67e22'; }
        else if (bmi >= 30) { status = 'Obese'; color = '#e74c3c'; }

        const bmiEl = document.getElementById('bmi-value');
        bmiEl.innerText = `${bmi} ${status}`;
        bmiEl.style.backgroundColor = color;
    }
}