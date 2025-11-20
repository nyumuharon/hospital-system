import React, { useState, useEffect } from 'react';
import { analyzeEpidemicRisks } from '../services/geminiService';
import { RECENT_SYMPTOMS } from '../services/mockData';
import { EpidemicReport } from '../types';
import { 
  Activity, 
  AlertOctagon, 
  BrainCircuit, 
  MapPin, 
  RefreshCw, 
  ThermometerSun 
} from 'lucide-react';
import {
  BarChart,
  Bar,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
  PieChart,
  Pie,
  Cell
} from 'recharts';

export const EpidemicDashboard: React.FC = () => {
  const [loading, setLoading] = useState(false);
  const [report, setReport] = useState<EpidemicReport | null>(null);

  const generateReport = async () => {
    setLoading(true);
    try {
      const result = await analyzeEpidemicRisks(RECENT_SYMPTOMS);
      setReport(result);
    } catch (e) {
      console.error(e);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    // Initial Load
    generateReport();
  }, []);

  // Stats for Charts (derived from mock data)
  const symptomsCount = RECENT_SYMPTOMS.reduce((acc, curr) => {
    curr.symptoms.forEach(s => {
      acc[s] = (acc[s] || 0) + 1;
    });
    return acc;
  }, {} as Record<string, number>);

  const chartData = Object.keys(symptomsCount).map(key => ({
    name: key,
    count: symptomsCount[key]
  })).sort((a,b) => b.count - a.count).slice(0, 5);

  const COLORS = ['#0088FE', '#00C49F', '#FFBB28', '#FF8042', '#8884d8'];

  return (
    <div className="h-full flex flex-col gap-6 overflow-y-auto">
      <div className="flex justify-between items-center">
        <div>
          <h2 className="text-2xl font-bold text-slate-800 flex items-center gap-2">
            <BrainCircuit className="text-purple-600" />
            AI Epidemic Detection Center
          </h2>
          <p className="text-slate-500">Real-time symptom cluster analysis using Gemini AI.</p>
        </div>
        <button
          onClick={generateReport}
          disabled={loading}
          className="px-4 py-2 bg-purple-600 text-white rounded-lg shadow-sm hover:bg-purple-700 disabled:opacity-50 flex items-center gap-2 transition-all"
        >
          <RefreshCw size={18} className={loading ? "animate-spin" : ""} />
          {loading ? "Analyzing..." : "Refresh Analysis"}
        </button>
      </div>

      {/* Top Cards */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-200">
          <div className="flex items-center gap-3 mb-2">
            <div className="p-2 bg-blue-100 text-blue-600 rounded-lg">
              <Activity size={24} />
            </div>
            <h3 className="font-semibold text-slate-700">Total Cases Analyzed</h3>
          </div>
          <p className="text-3xl font-bold text-slate-900">{RECENT_SYMPTOMS.length}</p>
          <p className="text-xs text-slate-400 mt-1">Last 7 Days</p>
        </div>

        <div className={`p-6 rounded-xl shadow-sm border ${
          report?.riskLevel === 'CRITICAL' || report?.riskLevel === 'HIGH' 
            ? 'bg-red-50 border-red-200' 
            : report?.riskLevel === 'MODERATE' 
            ? 'bg-amber-50 border-amber-200' 
            : 'bg-green-50 border-green-200'
        }`}>
          <div className="flex items-center gap-3 mb-2">
            <div className={`p-2 rounded-lg ${
               report?.riskLevel === 'CRITICAL' ? 'bg-red-200 text-red-700' : 'bg-green-200 text-green-700'
            }`}>
              <AlertOctagon size={24} />
            </div>
            <h3 className="font-semibold text-slate-800">Current Risk Level</h3>
          </div>
          {loading ? (
            <div className="h-8 w-32 bg-slate-200 animate-pulse rounded" />
          ) : (
            <p className={`text-3xl font-bold ${
               report?.riskLevel === 'CRITICAL' ? 'text-red-700' : 'text-slate-900'
            }`}>{report?.riskLevel || "Analyzing..."}</p>
          )}
        </div>

        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-200">
          <div className="flex items-center gap-3 mb-2">
            <div className="p-2 bg-orange-100 text-orange-600 rounded-lg">
              <ThermometerSun size={24} />
            </div>
            <h3 className="font-semibold text-slate-700">External Factors</h3>
          </div>
          <p className="text-sm text-slate-600">
            <span className="font-bold">Climate:</span> High Humidity, Rain
          </p>
          <p className="text-sm text-slate-600">
            <span className="font-bold">Alerts:</span> Vector-borne vectors up
          </p>
        </div>
      </div>

      {/* Main Content Grid */}
      <div className="grid grid-cols-1 lg:grid-cols-3 gap-6 min-h-[400px]">
        
        {/* Left: Charts */}
        <div className="lg:col-span-1 bg-white p-6 rounded-xl shadow-sm border border-slate-200 flex flex-col">
          <h3 className="font-bold text-slate-800 mb-4">Symptom Distribution</h3>
          <div className="flex-1 min-h-[250px]">
             <ResponsiveContainer width="100%" height="100%">
               <PieChart>
                 <Pie
                   data={chartData}
                   cx="50%"
                   cy="50%"
                   innerRadius={60}
                   outerRadius={80}
                   paddingAngle={5}
                   dataKey="count"
                 >
                   {chartData.map((entry, index) => (
                     <Cell key={`cell-${index}`} fill={COLORS[index % COLORS.length]} />
                   ))}
                 </Pie>
                 <Tooltip />
               </PieChart>
             </ResponsiveContainer>
          </div>
          <div className="mt-4 space-y-2">
            {chartData.map((entry, idx) => (
              <div key={entry.name} className="flex justify-between items-center text-sm">
                <span className="flex items-center gap-2">
                  <span className="w-3 h-3 rounded-full" style={{ backgroundColor: COLORS[idx % COLORS.length] }} />
                  {entry.name}
                </span>
                <span className="font-bold text-slate-600">{entry.count}</span>
              </div>
            ))}
          </div>
        </div>

        {/* Right: AI Report */}
        <div className="lg:col-span-2 bg-white p-6 rounded-xl shadow-sm border border-slate-200 relative overflow-hidden">
           {/* Decorative background */}
           <div className="absolute top-0 right-0 -mt-10 -mr-10 w-40 h-40 bg-purple-50 rounded-full blur-3xl pointer-events-none" />

           <div className="relative z-10">
             <h3 className="font-bold text-slate-800 mb-6 flex items-center justify-between">
               <span>Intelligent Analysis Report</span>
               <span className="text-xs font-normal text-slate-400 bg-slate-100 px-2 py-1 rounded">
                 Model: Gemini 2.5 Flash
               </span>
             </h3>

             {loading ? (
               <div className="space-y-4 animate-pulse">
                 <div className="h-4 bg-slate-100 rounded w-3/4"></div>
                 <div className="h-4 bg-slate-100 rounded w-full"></div>
                 <div className="h-4 bg-slate-100 rounded w-5/6"></div>
                 <div className="h-32 bg-slate-50 rounded w-full mt-4"></div>
               </div>
             ) : report ? (
               <div className="space-y-6">
                 <div>
                   <h4 className="text-sm font-bold text-slate-500 uppercase tracking-wide mb-2">Detected Anomalies</h4>
                   <div className="flex gap-2 flex-wrap">
                     {report.detectedOutbreaks.map((outbreak, idx) => (
                       <span key={idx} className="px-3 py-1 bg-red-100 text-red-700 rounded-full text-sm font-medium border border-red-200">
                         {outbreak}
                       </span>
                     ))}
                   </div>
                 </div>

                 <div>
                   <h4 className="text-sm font-bold text-slate-500 uppercase tracking-wide mb-2">AI Analysis</h4>
                   <div className="p-4 bg-slate-50 rounded-lg border border-slate-200 text-slate-700 text-sm leading-relaxed">
                     {report.analysis}
                   </div>
                 </div>

                 <div>
                    <h4 className="text-sm font-bold text-slate-500 uppercase tracking-wide mb-2">Strategic Recommendations</h4>
                    <ul className="space-y-2">
                      {report.recommendations.map((rec, idx) => (
                        <li key={idx} className="flex items-start gap-3 text-sm text-slate-700">
                          <div className="mt-1 min-w-[20px] h-5 w-5 rounded-full bg-purple-100 text-purple-600 flex items-center justify-center text-xs font-bold">
                            {idx + 1}
                          </div>
                          {rec}
                        </li>
                      ))}
                    </ul>
                 </div>
               </div>
             ) : (
               <div className="text-center text-slate-400 py-10">
                 Unable to load report. Please check API configuration.
               </div>
             )}
           </div>
        </div>
      </div>

      {/* Location Context (Mock Map Area) */}
      <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-200">
        <h3 className="font-bold text-slate-800 mb-4 flex items-center gap-2">
          <MapPin size={18} /> Geospatial Clustering
        </h3>
        <div className="h-48 bg-slate-100 rounded-lg flex items-center justify-center border-2 border-dashed border-slate-300">
          <div className="text-center">
            <MapPin className="mx-auto text-slate-300 mb-2" size={32} />
            <p className="text-slate-400 text-sm">Geospatial data visualization integration point.</p>
            <p className="text-xs text-slate-400">Cluster ID: North Wing (High Concentration)</p>
          </div>
        </div>
      </div>
    </div>
  );
};