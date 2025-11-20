export enum Role {
  DOCTOR = 'DOCTOR',
  PHARMACIST = 'PHARMACIST',
  ADMIN = 'ADMIN',
}

export interface User {
  id: string;
  name: string;
  role: Role;
  avatar: string;
}

export interface Drug {
  id: string;
  name: string;
  stock: number;
  expiryDate: string;
  price: number;
  minThreshold: number;
}

export interface Patient {
  id: string;
  name: string;
  age: number;
  gender: 'Male' | 'Female';
  history: string[];
  allergies: string[];
  lastVisit: string;
}

export interface Prescription {
  id: string;
  patientId: string;
  patientName: string;
  doctorId: string;
  doctorName: string;
  date: string;
  items: { drugId: string; drugName: string; quantity: number }[];
  status: 'PENDING' | 'DISPENSED';
  totalCost: number;
}

export interface SymptomRecord {
  patientId: string;
  symptoms: string[];
  diagnosis: string; // Preliminary diagnosis
  timestamp: string;
  location: string;
}

export interface EpidemicReport {
  riskLevel: 'LOW' | 'MODERATE' | 'HIGH' | 'CRITICAL';
  detectedOutbreaks: string[];
  analysis: string;
  recommendations: string[];
  timestamp: string;
}