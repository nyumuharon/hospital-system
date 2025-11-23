import React, { useState } from 'react';
import { Prescription, Drug } from '../types';
import { PackageCheck, AlertTriangle, History } from 'lucide-react';

interface PharmacyDashboardProps {
  prescriptions: Prescription[];
  inventory: Drug[];
  onDispense: (prescriptionId: string) => void;
}

export const PharmacyDashboard: React.FC<PharmacyDashboardProps> = ({ prescriptions, inventory, onDispense }) => {
  const [selectedRx, setSelectedRx] = useState<Prescription | null>(null);

  const pendingPrescriptions = prescriptions.filter(p => p.status === 'PENDING');
  const completedPrescriptions = prescriptions.filter(p => p.status === 'DISPENSED');

  const getDrugStock = (id: string) => inventory.find(d => d.id === id)?.stock || 0;

  // Check if all items in the selected Rx are in stock
  const canDispense = selectedRx?.items.every(item => getDrugStock(item.drugId) >= item.quantity);

  return (
    <div className="h-full flex flex-col gap-6">
      <div className="flex justify-between items-end">
        <div>
          <h2 className="text-2xl font-bold text-slate-800">Pharmacy Dispatch</h2>
          <p className="text-slate-500">Review and dispense validated prescriptions.</p>
        </div>
        <div className="flex gap-2">
          <div className="bg-orange-50 text-orange-700 px-4 py-2 rounded-lg border border-orange-100 text-sm font-medium flex items-center gap-2">
            <AlertTriangle size={16} />
            {inventory.filter(d => d.stock < d.minThreshold).length} Low Stock Alerts
          </div>
        </div>
      </div>

      <div className="grid grid-cols-12 gap-6 flex-1 overflow-hidden">
        {/* Prescription Queue */}
        <div className="col-span-4 bg-white rounded-xl shadow-sm border border-slate-200 flex flex-col overflow-hidden">
          <div className="p-4 bg-slate-50 border-b border-slate-100">
            <h3 className="font-bold text-slate-700">Queue ({pendingPrescriptions.length})</h3>
          </div>
          <div className="flex-1 overflow-y-auto p-2 space-y-2">
            {pendingPrescriptions.length === 0 && (
               <div className="text-center p-6 text-slate-400 text-sm">No pending prescriptions.</div>
            )}
            {pendingPrescriptions.map(rx => (
              <button
                key={rx.id}
                onClick={() => setSelectedRx(rx)}
                className={`w-full text-left p-4 rounded-lg border transition-all ${
                  selectedRx?.id === rx.id 
                  ? 'bg-blue-50 border-blue-200 shadow-sm' 
                  : 'bg-white border-slate-100 hover:bg-slate-50'
                }`}
              >
                <div className="flex justify-between items-start mb-1">
                  <span className="font-semibold text-slate-800">{rx.patientName}</span>
                  <span className="text-xs text-slate-500">{new Date(rx.date).toLocaleTimeString()}</span>
                </div>
                <div className="text-xs text-slate-500 mb-2">Dr. {rx.doctorName}</div>
                <div className="flex gap-1 flex-wrap">
                   {rx.items.map((i, idx) => (
                     <span key={idx} className="text-xs bg-slate-100 px-2 py-1 rounded text-slate-600">
                       {i.drugName} x{i.quantity}
                     </span>
                   ))}
                </div>
              </button>
            ))}
          </div>
          <div className="p-2 border-t border-slate-100 bg-slate-50">
             <div className="text-xs font-bold text-slate-500 uppercase px-2 mb-2">Recent Dispensed</div>
             <div className="max-h-32 overflow-y-auto space-y-1">
               {completedPrescriptions.slice(0, 3).map(rx => (
                 <div key={rx.id} className="px-3 py-2 flex justify-between items-center text-xs text-slate-400">
                   <span>{rx.patientName}</span>
                   <span className="flex items-center gap-1"><PackageCheck size={12} /> Done</span>
                 </div>
               ))}
             </div>
          </div>
        </div>

        {/* Detail View */}
        <div className="col-span-8 bg-white rounded-xl shadow-sm border border-slate-200 flex flex-col">
           {selectedRx ? (
             <>
               <div className="p-6 border-b border-slate-100 flex justify-between items-center">
                 <div>
                   <h3 className="text-xl font-bold text-slate-800">Prescription Details</h3>
                   <p className="text-sm text-slate-500">ID: {selectedRx.id}</p>
                 </div>
                 <div className="text-right">
                   <p className="text-sm font-bold text-slate-800">Total Cost</p>
                   <p className="text-2xl font-bold text-teal-600">${selectedRx.totalCost.toFixed(2)}</p>
                 </div>
               </div>

               <div className="p-6 flex-1 overflow-y-auto">
                 <table className="w-full text-sm">
                   <thead className="bg-slate-50 text-slate-500 font-medium">
                     <tr>
                       <th className="px-4 py-3 text-left">Medication</th>
                       <th className="px-4 py-3 text-center">Qty Required</th>
                       <th className="px-4 py-3 text-center">Current Stock</th>
                       <th className="px-4 py-3 text-right">Status</th>
                     </tr>
                   </thead>
                   <tbody className="divide-y divide-slate-100">
                     {selectedRx.items.map((item, idx) => {
                       const stock = getDrugStock(item.drugId);
                       const hasStock = stock >= item.quantity;
                       return (
                         <tr key={idx}>
                           <td className="px-4 py-3 font-medium text-slate-800">{item.drugName}</td>
                           <td className="px-4 py-3 text-center">{item.quantity}</td>
                           <td className="px-4 py-3 text-center">
                             <span className={stock < 10 ? 'text-amber-600 font-bold' : 'text-slate-600'}>
                               {stock}
                             </span>
                           </td>
                           <td className="px-4 py-3 text-right">
                             {hasStock ? (
                               <span className="text-green-600 font-medium text-xs px-2 py-1 bg-green-50 rounded-full">Available</span>
                             ) : (
                               <span className="text-red-600 font-medium text-xs px-2 py-1 bg-red-50 rounded-full">Insufficient</span>
                             )}
                           </td>
                         </tr>
                       );
                     })}
                   </tbody>
                 </table>

                 <div className="mt-8 bg-slate-50 p-4 rounded-lg border border-slate-200">
                   <h4 className="font-bold text-slate-700 mb-2 text-sm">Pharmacist Verification</h4>
                   <label className="flex items-center gap-2 text-sm text-slate-600 cursor-pointer">
                     <input type="checkbox" className="rounded text-teal-600 focus:ring-teal-500" />
                     I certify that I have reviewed the patient history and drug interactions.
                   </label>
                 </div>
               </div>

               <div className="p-6 border-t border-slate-100 flex justify-end gap-3">
                 <button 
                    onClick={() => setSelectedRx(null)}
                    className="px-4 py-2 border border-slate-300 rounded-lg text-slate-600 hover:bg-slate-50 transition-colors"
                 >
                   Cancel
                 </button>
                 <button
                   onClick={() => {
                     onDispense(selectedRx.id);
                     setSelectedRx(null);
                   }}
                   disabled={!canDispense}
                   className={`px-6 py-2 rounded-lg font-semibold text-white transition-all shadow-md flex items-center gap-2 ${
                     canDispense 
                     ? 'bg-teal-600 hover:bg-teal-700' 
                     : 'bg-slate-300 cursor-not-allowed'
                   }`}
                 >
                   <PackageCheck size={18} />
                   Confirm & Dispense
                 </button>
               </div>
             </>
           ) : (
             <div className="h-full flex items-center justify-center text-slate-400 flex-col">
               <History size={48} className="mb-4 opacity-20" />
               <p>Select a prescription from the queue.</p>
             </div>
           )}
        </div>
      </div>
    </div>
  );
};