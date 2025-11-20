import React from 'react';
import { Role, User } from '../types';
import { 
  Activity, 
  Pill, 
  FileText, 
  LayoutDashboard, 
  LogOut, 
  Settings,
  AlertTriangle
} from 'lucide-react';

interface SidebarProps {
  user: User;
  activeTab: string;
  setActiveTab: (tab: string) => void;
  onLogout: () => void;
}

export const Sidebar: React.FC<SidebarProps> = ({ user, activeTab, setActiveTab, onLogout }) => {
  
  const getNavItems = () => {
    switch (user.role) {
      case Role.DOCTOR:
        return [
          { id: 'doctor-dash', label: 'Consultation', icon: LayoutDashboard },
          { id: 'patient-history', label: 'Patient History', icon: FileText },
        ];
      case Role.PHARMACIST:
        return [
          { id: 'pharmacy-dash', label: 'Dispense', icon: Pill },
          { id: 'inventory', label: 'Inventory', icon: LayoutDashboard },
        ];
      case Role.ADMIN:
        return [
          { id: 'epidemic-ai', label: 'AI Epidemic Center', icon: AlertTriangle },
          { id: 'admin-reports', label: 'Reports & Analytics', icon: Activity },
          { id: 'settings', label: 'System Config', icon: Settings },
        ];
      default:
        return [];
    }
  };

  const navItems = getNavItems();

  return (
    <div className="w-64 bg-slate-900 text-slate-300 flex flex-col h-screen fixed left-0 top-0 shadow-xl z-50 transition-all duration-300">
      <div className="p-6 border-b border-slate-700 flex items-center gap-3">
        <div className="w-8 h-8 bg-teal-500 rounded-lg flex items-center justify-center text-white font-bold">
          M
        </div>
        <h1 className="text-xl font-bold text-white tracking-tight">MediFlow Intel</h1>
      </div>

      <div className="flex-1 py-6 px-4 space-y-1">
        <div className="text-xs font-semibold text-slate-500 uppercase tracking-wider mb-4 px-2">
          {user.role} Portal
        </div>
        {navItems.map((item) => {
          const Icon = item.icon;
          const isActive = activeTab === item.id;
          return (
            <button
              key={item.id}
              onClick={() => setActiveTab(item.id)}
              className={`w-full flex items-center gap-3 px-4 py-3 rounded-lg transition-colors duration-200 ${
                isActive 
                  ? 'bg-teal-600 text-white shadow-md' 
                  : 'hover:bg-slate-800 hover:text-white'
              }`}
            >
              <Icon size={20} />
              <span className="font-medium">{item.label}</span>
            </button>
          );
        })}
      </div>

      <div className="p-4 border-t border-slate-700 bg-slate-900">
        <div className="flex items-center gap-3 mb-4 px-2">
          <img src={user.avatar} alt="User" className="w-10 h-10 rounded-full border-2 border-slate-600" />
          <div className="flex-1 overflow-hidden">
            <p className="text-sm font-medium text-white truncate">{user.name}</p>
            <p className="text-xs text-slate-500 truncate capitalize">{user.role.toLowerCase()}</p>
          </div>
        </div>
        <button 
          onClick={onLogout}
          className="w-full flex items-center justify-center gap-2 p-2 text-red-400 hover:bg-red-500/10 rounded-lg transition-colors"
        >
          <LogOut size={18} />
          <span>Sign Out</span>
        </button>
      </div>
    </div>
  );
};