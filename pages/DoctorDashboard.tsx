import React, { useState, useMemo } from 'react';
import { MOCK_PATIENTS, INITIAL_INVENTORY } from '../services/mockData';
import { Patient, Drug } from '../types';
import { Search, Plus, AlertCircle, CheckCircle2 } from 'lucide-react';

interface DoctorDashboardProps {
  onPrescribe: (patientId: string, items: {drugId: string, quantity: number}[]) => void;
}

export const DoctorDashboard: React.FC<DoctorDashboardProps> = ({ onPrescribe }) => {
  const [selectedPatient, setSelectedPatient] = useState<Patient | null>(null);
  const [searchTerm, setSearchTerm] = useState('');
  const [prescriptionItems, setPrescriptionItems] = useState<{drug: Drug, quantity: number}[]>([]);
  const [showSuccess, setShowSuccess] = useState(false);

  const filteredPatients = useMemo(() => {
    return MOCK_PATIENTS.filter(p => p.name.toLowerCase().includes(searchTerm.toLowerCase()));
  }, [searchTerm]);

  const handleAddDrug = (drugId: string) => {
    const drug = INITIAL_INVENTORY.find(d => d.id === drugId);
    if (!drug) return;
    
    if (prescriptionItems.find(item => item.drug.id === drugId)) return;

    setPrescriptionItems([...prescriptionItems, { drug, quantity: 1 }]);
  };

  const updateQuantity = (drugId: string, qty: number) => {
    setPrescriptionItems(items => items.map(item => 
      item.drug.id === drugId ? { ...item, quantity: Math.max(1, qty) } : item
    ));
  };

  const removeDrug = (drugId: string) => {
    setPrescriptionItems(items => items.filter(item => item.drug.id !== drugId));
  };

  const handleSubmit = () => {
    if (!selectedPatient || prescriptionItems.length === 0) return;
    
    onPrescribe(
      selectedPatient.id, 
      prescriptionItems.map(i => ({ drugId: i.drug.id, quantity: i.quantity }))
    );
    
    setShowSuccess(true);
    setTimeout(() => {
      setShowSuccess(false);
      setPrescriptionItems([]);
      setSelectedPatient(null);
    }, 2000);
  };

  return (
    <div className="h-full flex flex-col gap-6">
      <div className="flex justify-between items-end">
        <div>
          <h2 className="text-2xl font-bold text-slate-800">Consultation & Prescription</h2>
          <p className="text-slate-500">Manage patient visits and issue electronic prescriptions.</p>
        </div>
      </div>

      <div className="grid grid-cols-12 gap-6 flex-1 overflow-hidden">
        {/* Patient Selection Column */}
        <div className="col-span-4 bg-white rounded-xl shadow-sm border border-slate-200 flex flex-col overflow-hidden">
          <div className="p-4 border-b border-slate-100 bg-slate-50">
            <div className="relative">
              <Search className="absolute left-3 top-1/2 -translate-y-1/2 text-slate-400" size={18} />
              <input 
                type="text" 
                placeholder="Search patients..." 
                className="w-full pl-10 pr-4 py-2 border border-slate-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-teal-500 text-sm"
                value={searchTerm}
                onChange={(e) => setSearchTerm(e.target.value)}
              />
            </div>
          </div>
          <div className="flex-1 overflow-y-auto p-2 space-y-1">
            {filteredPatients.map(patient => (
              <button
                key={patient.id}
                onClick={() => setSelectedPatient(patient)}
                className={`w-full text-left p-3 rounded-lg transition-colors flex items-center justify-between ${
                  selectedPatient?.id === patient.id ? 'bg-teal-50 border border-teal-200' : 'hover:bg-slate-50'
                }`}
              >
                <div>
                  <p className="font-semibold text-slate-800">{patient.name}</p>
                  <p className="text-xs text-slate-500">Age: {patient.age} • {patient.gender}</p>
                </div>
                {selectedPatient?.id === patient.id && <div className="w-2 h-2 bg-teal-500 rounded-full" />}
              </button>
            ))}
          </div>
        </div>

        {/* Clinical Area */}
        <div className="col-span-8 flex flex-col gap-6 overflow-y-auto pr-2">
          {selectedPatient ? (
            <>
              {/* Patient Info Card */}
              <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-200">
                 <div className="flex justify-between items-start mb-4">
                   <h3 className="text-lg font-bold text-slate-800">Patient History: {selectedPatient.name}</h3>
                   <span className="text-xs bg-slate-100 text-slate-600 px-2 py-1 rounded">Last Visit: {selectedPatient.lastVisit}</span>
                 </div>
                 <div className="grid grid-cols-2 gap-4">
                   <div className="bg-red-50 p-3 rounded-lg border border-red-100">
                     <p className="text-xs font-bold text-red-700 uppercase mb-1">Allergies</p>
                     <p className="text-sm text-red-900">{selectedPatient.allergies.length > 0 ? selectedPatient.allergies.join(', ') : 'None known'}</p>
                   </div>
                   <div className="bg-blue-50 p-3 rounded-lg border border-blue-100">
                     <p className="text-xs font-bold text-blue-700 uppercase mb-1">Medical Conditions</p>
                     <p className="text-sm text-blue-900">{selectedPatient.history.length > 0 ? selectedPatient.history.join(', ') : 'None recorded'}</p>
                   </div>
                 </div>
              </div>

              {/* Prescribing Interface */}
              <div className="bg-white rounded-xl shadow-sm border border-slate-200 flex-1 flex flex-col">
                <div className="p-4 border-b border-slate-100 flex justify-between items-center">
                  <h3 className="font-bold text-slate-800">Create Prescription</h3>
                  <select 
                    className="text-sm border-slate-300 border rounded-lg p-2 focus:ring-teal-500 focus:border-teal-500"
                    onChange={(e) => handleAddDrug(e.target.value)}
                    value=""
                  >
                    <option value="" disabled>+ Add Medication</option>
                    {INITIAL_INVENTORY.map(drug => (
                      <option 
                        key={drug.id} 
                        value={drug.id} 
                        disabled={drug.stock === 0}
                        className={drug.stock < drug.minThreshold ? 'text-red-500' : ''}
                      >
                        {drug.name} {drug.stock === 0 ? '(Out of Stock)' : `(${drug.stock} avail)`}
                      </option>
                    ))}
                  </select>
                </div>

                <div className="flex-1 p-4 overflow-y-auto">
                  {prescriptionItems.length === 0 ? (
                    <div className="h-full flex flex-col items-center justify-center text-slate-400">
                      <Plus size={48} className="mb-2 opacity-20" />
                      <p>No medications added yet.</p>
                    </div>
                  ) : (
                    <table className="w-full text-sm text-left">
                      <thead className="text-xs text-slate-500 uppercase bg-slate-50">
                        <tr>
                          <th className="px-4 py-2">Drug Name</th>
                          <th className="px-4 py-2">Stock Status</th>
                          <th className="px-4 py-2">Quantity</th>
                          <th className="px-4 py-2">Action</th>
                        </tr>
                      </thead>
                      <tbody>
                        {prescriptionItems.map((item) => (
                          <tr key={item.drug.id} className="border-b border-slate-100">
                            <td className="px-4 py-3 font-medium">{item.drug.name}</td>
                            <td className="px-4 py-3">
                              {item.drug.stock < item.drug.minThreshold ? (
                                <span className="text-amber-600 flex items-center gap-1 text-xs">
                                  <AlertCircle size={12} /> Low ({item.drug.stock})
                                </span>
                              ) : (
                                <span className="text-green-600 text-xs">Available ({item.drug.stock})</span>
                              )}
                            </td>
                            <td className="px-4 py-3">
                              <input 
                                type="number" 
                                min="1" 
                                max={item.drug.stock} 
                                value={item.quantity}
                                onChange={(e) => updateQuantity(item.drug.id, parseInt(e.target.value))}
                                className="w-20 p-1 border border-slate-300 rounded"
                              />
                            </td>
                            <td className="px-4 py-3">
                              <button 
                                onClick={() => removeDrug(item.drug.id)}
                                className="text-red-500 hover:text-red-700 text-xs hover:underline"
                              >
                                Remove
                              </button>
                            </td>
                          </tr>
                        ))}
                      </tbody>
                    </table>
                  )}
                </div>

                <div className="p-4 bg-slate-50 border-t border-slate-200 flex justify-between items-center">
                   <div className="text-sm text-slate-500">
                     Total Items: <span className="font-bold text-slate-800">{prescriptionItems.length}</span>
                   </div>
                   <button
                     onClick={handleSubmit}
                     disabled={prescriptionItems.length === 0}
                     className={`px-6 py-2 rounded-lg font-semibold shadow-sm transition-all flex items-center gap-2 ${
                       prescriptionItems.length === 0 
                       ? 'bg-slate-300 text-slate-500 cursor-not-allowed' 
                       : 'bg-teal-600 text-white hover:bg-teal-700 hover:shadow-md'
                     }`}
                   >
                     {showSuccess ? (
                       <>
                         <CheckCircle2 size={18} /> Sent!
                       </>
                     ) : (
                       "Submit Prescription"
                     )}
                   </button>
                </div>
              </div>
            </>
          ) : (
             <div className="h-full flex items-center justify-center bg-white rounded-xl border border-slate-200 border-dashed">
               <div className="text-center text-slate-400">
                 <Search size={48} className="mx-auto mb-4 opacity-20" />
                 <p className="text-lg font-medium">Select a patient to begin consultation</p>
               </div>
             </div>
          )}
        </div>
      </div>
    </div>
  );
};