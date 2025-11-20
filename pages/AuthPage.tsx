import React from 'react';
import { MOCK_USERS } from '../services/mockData';
import { User, Role } from '../types';
import { ShieldCheck, UserCircle } from 'lucide-react';

interface AuthPageProps {
  onLogin: (user: User) => void;
}

export const AuthPage: React.FC<AuthPageProps> = ({ onLogin }) => {
  return (
    <div className="min-h-screen bg-slate-50 flex items-center justify-center p-4">
      <div className="max-w-4xl w-full bg-white rounded-2xl shadow-2xl overflow-hidden flex flex-col md:flex-row">
        
        {/* Left Side - Branding */}
        <div className="md:w-1/2 bg-slate-900 p-12 flex flex-col justify-between text-white relative overflow-hidden">
          <div className="absolute top-0 left-0 w-full h-full opacity-10 pointer-events-none">
             <svg className="h-full w-full" viewBox="0 0 100 100" preserveAspectRatio="none">
               <path d="M0 100 C 20 0 50 0 100 100 Z" fill="white" />
             </svg>
          </div>
          
          <div className="z-10">
            <div className="w-12 h-12 bg-teal-500 rounded-xl flex items-center justify-center mb-6">
              <ShieldCheck size={32} />
            </div>
            <h1 className="text-4xl font-bold mb-4">MediFlow Intel</h1>
            <p className="text-slate-400 text-lg leading-relaxed">
              Intelligent Hospital Workflow System with integrated AI Epidemic Detection and Analytics.
            </p>
          </div>
          
          <div className="text-sm text-slate-500 z-10">
            © 2024 MediFlow Systems. Secure Access Only.
          </div>
        </div>

        {/* Right Side - Login Roles */}
        <div className="md:w-1/2 p-12 flex flex-col justify-center bg-slate-50">
          <h2 className="text-2xl font-bold text-slate-800 mb-2">Welcome Back</h2>
          <p className="text-slate-500 mb-8">Please select your role to simulate login.</p>

          <div className="space-y-4">
            {MOCK_USERS.map((user) => (
              <button
                key={user.id}
                onClick={() => onLogin(user)}
                className="w-full flex items-center gap-4 p-4 bg-white border border-slate-200 rounded-xl hover:border-teal-500 hover:shadow-md transition-all group text-left"
              >
                <div className="relative">
                  <img 
                    src={user.avatar} 
                    alt={user.name} 
                    className="w-12 h-12 rounded-full object-cover group-hover:scale-105 transition-transform"
                  />
                  <div className={`absolute -bottom-1 -right-1 w-5 h-5 rounded-full border-2 border-white flex items-center justify-center ${
                    user.role === Role.ADMIN ? 'bg-purple-500' : user.role === Role.DOCTOR ? 'bg-blue-500' : 'bg-green-500'
                  }`}>
                    <UserCircle size={12} className="text-white" />
                  </div>
                </div>
                <div>
                  <h3 className="font-semibold text-slate-800 group-hover:text-teal-600 transition-colors">{user.name}</h3>
                  <span className="text-xs font-medium text-slate-400 uppercase tracking-wide">{user.role}</span>
                </div>
              </button>
            ))}
          </div>

          <div className="mt-8 text-center">
             <p className="text-xs text-slate-400">
               Note: This is a prototype demo. No real passwords required.
             </p>
          </div>
        </div>
      </div>
    </div>
  );
};