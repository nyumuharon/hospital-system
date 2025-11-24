import React, { useState, useEffect } from 'react';
import { Activity, AlertTriangle, DollarSign, Package, RefreshCw } from 'lucide-react';
import { DashboardData } from '../types';
import { BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer } from 'recharts';

export function RealTimeDashboard() {
    const [data, setData] = useState<DashboardData | null>(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState<string | null>(null);
    const [lastUpdated, setLastUpdated] = useState<Date>(new Date());

    const fetchData = async () => {
        try {
            const response = await fetch('http://localhost:8080/api/dashboard');
            if (!response.ok) throw new Error('Failed to fetch data');
            const jsonData = await response.json();
            setData(jsonData);
            setLastUpdated(new Date());
            setError(null);
        } catch (err) {
            setError('Connection to Hospital System failed. Is the C++ backend running?');
        } finally {
            setLoading(false);
        }
    };

    useEffect(() => {
        fetchData();
        const interval = setInterval(fetchData, 3000); // Poll every 3 seconds
        return () => clearInterval(interval);
    }, []);

    if (loading && !data) return <div className="p-8 text-center">Connecting to Hospital System...</div>;

    return (
        <div className="p-8 bg-slate-50 min-h-full overflow-auto">
            <div className="flex justify-between items-center mb-8">
                <div>
                    <h1 className="text-3xl font-bold text-slate-800">Live Pharmacy Command Center</h1>
                    <p className="text-slate-500 flex items-center gap-2 mt-1">
                        <span className="relative flex h-3 w-3">
                            <span className="animate-ping absolute inline-flex h-full w-full rounded-full bg-green-400 opacity-75"></span>
                            <span className="relative inline-flex rounded-full h-3 w-3 bg-green-500"></span>
                        </span>
                        System Online • Last updated: {lastUpdated.toLocaleTimeString()}
                    </p>
                </div>
                <button
                    onClick={fetchData}
                    className="p-2 hover:bg-white rounded-full transition-colors text-slate-500"
                    title="Force Refresh"
                >
                    <RefreshCw size={20} />
                </button>
            </div>

            {error && (
                <div className="bg-red-50 text-red-600 p-4 rounded-lg mb-6 flex items-center gap-3 border border-red-100">
                    <AlertTriangle size={20} />
                    {error}
                </div>
            )}

            {/* Key Metrics */}
            <div className="grid grid-cols-1 md:grid-cols-3 gap-6 mb-8">
                <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
                    <div className="flex justify-between items-start">
                        <div>
                            <p className="text-sm font-medium text-slate-500 mb-1">Daily Revenue</p>
                            <h3 className="text-3xl font-bold text-slate-800">
                                KES {data?.revenue.toLocaleString() ?? '0'}
                            </h3>
                        </div>
                        <div className="p-3 bg-emerald-50 text-emerald-600 rounded-lg">
                            <DollarSign size={24} />
                        </div>
                    </div>
                </div>

                <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
                    <div className="flex justify-between items-start">
                        <div>
                            <p className="text-sm font-medium text-slate-500 mb-1">Low Stock Alerts</p>
                            <h3 className={`text-3xl font-bold ${data?.lowStock ? 'text-red-600' : 'text-slate-800'}`}>
                                {data?.lowStock ?? 0}
                            </h3>
                        </div>
                        <div className={`p-3 rounded-lg ${data?.lowStock ? 'bg-red-50 text-red-600' : 'bg-slate-50 text-slate-600'}`}>
                            <AlertTriangle size={24} />
                        </div>
                    </div>
                </div>

                <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
                    <div className="flex justify-between items-start">
                        <div>
                            <p className="text-sm font-medium text-slate-500 mb-1">Pending Prescriptions</p>
                            <h3 className="text-3xl font-bold text-slate-800">
                                {data?.pendingRx ?? 0}
                            </h3>
                        </div>
                        <div className="p-3 bg-blue-50 text-blue-600 rounded-lg">
                            <Package size={24} />
                        </div>
                    </div>
                </div>
            </div>

            <div className="grid grid-cols-1 lg:grid-cols-2 gap-8">
                {/* Live Transactions Feed */}
                <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
                    <h3 className="text-lg font-bold text-slate-800 mb-4 flex items-center gap-2">
                        <Activity size={20} className="text-blue-500" />
                        Recent Transactions
                    </h3>
                    <div className="space-y-4">
                        {data?.recentTransactions.length === 0 ? (
                            <p className="text-slate-400 text-center py-8">No transactions today</p>
                        ) : (
                            data?.recentTransactions.map((tx) => (
                                <div key={tx.id} className="flex items-center justify-between p-3 hover:bg-slate-50 rounded-lg transition-colors border-b border-slate-50 last:border-0">
                                    <div className="flex items-center gap-3">
                                        <div className="w-10 h-10 rounded-full bg-slate-100 flex items-center justify-center text-slate-500 font-bold text-xs">
                                            #{tx.id}
                                        </div>
                                        <div>
                                            <p className="font-medium text-slate-700">Payment via {tx.method}</p>
                                            <p className="text-xs text-slate-400">{tx.date}</p>
                                        </div>
                                    </div>
                                    <span className="font-bold text-emerald-600">
                                        +KES {tx.amount.toLocaleString()}
                                    </span>
                                </div>
                            ))
                        )}
                    </div>
                </div>

                {/* Placeholder for Chart (requires more data points in real app) */}
                <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100 flex flex-col justify-center items-center text-center">
                    <div className="w-16 h-16 bg-indigo-50 text-indigo-500 rounded-full flex items-center justify-center mb-4">
                        <Activity size={32} />
                    </div>
                    <h3 className="text-lg font-bold text-slate-800 mb-2">Analytics Engine Ready</h3>
                    <p className="text-slate-500 max-w-xs">
                        Real-time charts will appear here as more data is collected by the system.
                    </p>
                </div>
            </div>
        </div>
    );
}
