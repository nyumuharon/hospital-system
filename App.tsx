import React, { useState, useEffect } from 'react';
import { AuthPage } from './pages/AuthPage';
import { Sidebar } from './components/Sidebar';
import { DoctorDashboard } from './pages/DoctorDashboard';
import { PharmacyDashboard } from './pages/PharmacyDashboard';
import { EpidemicDashboard } from './pages/EpidemicDashboard';
import { RealTimeDashboard } from './pages/RealTimeDashboard';
import { User, Role, Drug, Prescription, Patient } from './types';
import { INITIAL_INVENTORY } from './services/mockData';

function App() {
  // Auth State
  const [user, setUser] = useState<User | null>(null);

  // UI State
  const [activeTab, setActiveTab] = useState('');

  // Database State (Lifted up to mimic a backend)
  const [inventory, setInventory] = useState<Drug[]>(INITIAL_INVENTORY);
  const [prescriptions, setPrescriptions] = useState<Prescription[]>([]);

  // Initialize Tab based on role when user logs in
  useEffect(() => {
    if (user) {
      if (user.role === Role.DOCTOR) setActiveTab('doctor-dash');
      else if (user.role === Role.PHARMACIST) setActiveTab('realtime-dash');
      else if (user.role === Role.ADMIN) setActiveTab('realtime-dash');
    }
  }, [user]);

  // Actions
  const handleLogin = (selectedUser: User) => {
    setUser(selectedUser);
  };

  const handleLogout = () => {
    setUser(null);
    setActiveTab('');
  };

  const handlePrescribe = (patientId: string, items: { drugId: string, quantity: number }[]) => {
    if (!user) return;

    const newRx: Prescription = {
      id: `rx-${Date.now()}`,
      patientId,
      patientName: "Patient Name Lookup Mock", // In real app, lookup patient
      doctorId: user.id,
      doctorName: user.name,
      date: new Date().toISOString(),
      items: items.map(i => {
        const drug = inventory.find(d => d.id === i.drugId);
        return {
          drugId: i.drugId,
          drugName: drug?.name || 'Unknown',
          quantity: i.quantity
        };
      }),
      status: 'PENDING',
      totalCost: items.reduce((acc, i) => {
        const drug = inventory.find(d => d.id === i.drugId);
        return acc + (drug ? drug.price * i.quantity : 0);
      }, 0)
    };

    // Mock name lookup for UI nicety
    if (patientId === 'p1') newRx.patientName = 'John Doe';
    if (patientId === 'p2') newRx.patientName = 'Jane Smith';
    if (patientId === 'p3') newRx.patientName = 'Robert Brown';
    if (patientId === 'p4') newRx.patientName = 'Emily White';

    setPrescriptions([...prescriptions, newRx]);
  };

  const handleDispense = (prescriptionId: string) => {
    const rx = prescriptions.find(p => p.id === prescriptionId);
    if (!rx) return;

    // Decrement stock
    const updatedInventory = inventory.map(drug => {
      const item = rx.items.find(i => i.drugId === drug.id);
      if (item) {
        return { ...drug, stock: drug.stock - item.quantity };
      }
      return drug;
    });

    setInventory(updatedInventory);

    // Update Rx Status
    setPrescriptions(prev => prev.map(p =>
      p.id === prescriptionId ? { ...p, status: 'DISPENSED' } : p
    ));
  };

  if (!user) {
    return <AuthPage onLogin={handleLogin} />;
  }

  return (
    <div className="flex min-h-screen bg-slate-50 text-slate-900 font-sans">
      <Sidebar
        user={user}
        activeTab={activeTab}
        setActiveTab={setActiveTab}
        onLogout={handleLogout}
      />

      <main className="flex-1 ml-64 p-8 h-screen overflow-hidden">
        {activeTab === 'doctor-dash' && user.role === Role.DOCTOR && (
          <DoctorDashboard onPrescribe={handlePrescribe} />
        )}
        {activeTab === 'pharmacy-dash' && user.role === Role.PHARMACIST && (
          <PharmacyDashboard
            prescriptions={prescriptions}
            inventory={inventory}
            onDispense={handleDispense}
          />
        )}
        {activeTab === 'realtime-dash' && (
          <RealTimeDashboard />
        )}
        {activeTab === 'inventory' && user.role === Role.PHARMACIST && (
          <div className="p-6 bg-white rounded-xl shadow-sm h-full overflow-auto">
            <h2 className="text-2xl font-bold mb-4">Live Inventory</h2>
            <table className="w-full text-left text-sm">
              <thead className="bg-slate-100 text-slate-500">
                <tr>
                  <th className="p-3">Drug Name</th>
                  <th className="p-3">Current Stock</th>
                  <th className="p-3">Threshold</th>
                  <th className="p-3">Status</th>
                </tr>
              </thead>
              <tbody>
                {inventory.map(drug => (
                  <tr key={drug.id} className="border-b border-slate-50">
                    <td className="p-3">{drug.name}</td>
                    <td className="p-3 font-mono">{drug.stock}</td>
                    <td className="p-3 text-slate-400">{drug.minThreshold}</td>
                    <td className="p-3">
                      {drug.stock < drug.minThreshold ? (
                        <span className="text-red-600 font-bold bg-red-50 px-2 py-1 rounded text-xs">Low Stock</span>
                      ) : (
                        <span className="text-green-600 bg-green-50 px-2 py-1 rounded text-xs">OK</span>
                      )}
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        )}
        {activeTab === 'epidemic-ai' && user.role === Role.ADMIN && (
          <EpidemicDashboard />
        )}

        {/* Placeholder views for menu items not fully implemented in this demo */}
        {(activeTab === 'patient-history' || activeTab === 'admin-reports' || activeTab === 'settings') && (
          <div className="flex items-center justify-center h-full text-slate-400 bg-white rounded-xl border border-dashed border-slate-300">
            <div className="text-center">
              <p className="text-lg font-medium mb-2">Module Under Construction</p>
              <p className="text-sm">This feature is part of the extended system specification.</p>
            </div>
          </div>
        )}
      </main>
    </div>
  );
}

export default App;