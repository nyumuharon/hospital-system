import { Drug, Patient, Role, User, SymptomRecord } from '../types';

// Mock Users
export const MOCK_USERS: User[] = [
  { id: 'u1', name: 'Dr. Sarah Bennett', role: Role.DOCTOR, avatar: 'https://picsum.photos/seed/doc1/200/200' },
  { id: 'u2', name: 'James Pillman', role: Role.PHARMACIST, avatar: 'https://picsum.photos/seed/pharm1/200/200' },
  { id: 'u3', name: 'Admin Overseer', role: Role.ADMIN, avatar: 'https://picsum.photos/seed/admin1/200/200' },
];

// Mock Inventory
export const INITIAL_INVENTORY: Drug[] = [
  { id: 'd1', name: 'Amoxicillin 500mg', stock: 150, expiryDate: '2025-12-01', price: 15.50, minThreshold: 50 },
  { id: 'd2', name: 'Ibuprofen 400mg', stock: 42, expiryDate: '2024-06-15', price: 8.00, minThreshold: 100 }, // Low stock
  { id: 'd3', name: 'Lisinopril 10mg', stock: 200, expiryDate: '2025-08-20', price: 12.25, minThreshold: 30 },
  { id: 'd4', name: 'Metformin 500mg', stock: 80, expiryDate: '2025-01-10', price: 10.00, minThreshold: 40 },
  { id: 'd5', name: 'Azithromycin 250mg', stock: 10, expiryDate: '2024-05-01', price: 25.00, minThreshold: 20 }, // Very low
  { id: 'd6', name: 'Tamiflu 75mg', stock: 300, expiryDate: '2026-01-01', price: 45.00, minThreshold: 50 },
];

// Mock Patients
export const MOCK_PATIENTS: Patient[] = [
  { id: 'p1', name: 'John Doe', age: 34, gender: 'Male', history: ['Hypertension'], allergies: ['Penicillin'], lastVisit: '2024-01-15' },
  { id: 'p2', name: 'Jane Smith', age: 28, gender: 'Female', history: ['Asthma'], allergies: [], lastVisit: '2024-02-10' },
  { id: 'p3', name: 'Robert Brown', age: 55, gender: 'Male', history: ['Diabetes T2'], allergies: ['Sulfa'], lastVisit: '2024-03-01' },
  { id: 'p4', name: 'Emily White', age: 7, gender: 'Female', history: [], allergies: [], lastVisit: '2024-03-05' },
];

// Recent Symptom Records for AI Analysis (Simulating a database of intake forms)
export const RECENT_SYMPTOMS: SymptomRecord[] = [
  { patientId: 'p1', symptoms: ['High Fever', 'Cough', 'Fatigue'], diagnosis: 'Viral Infection', timestamp: '2024-03-10', location: 'North Wing' },
  { patientId: 'p2', symptoms: ['High Fever', 'Rash', 'Joint Pain'], diagnosis: 'Undiagnosed', timestamp: '2024-03-11', location: 'North Wing' },
  { patientId: 'p3', symptoms: ['Fever', 'Red Eyes', 'Runny Nose'], diagnosis: 'Flu', timestamp: '2024-03-11', location: 'East Wing' },
  { patientId: 'p4', symptoms: ['High Fever', 'Rash', 'Muscle Pain'], diagnosis: 'Undiagnosed', timestamp: '2024-03-12', location: 'North Wing' },
  { patientId: 'p5', symptoms: ['Cough', 'Shortness of breath'], diagnosis: 'Bronchitis', timestamp: '2024-03-12', location: 'West Wing' },
  { patientId: 'p6', symptoms: ['High Fever', 'Rash', 'Bleeding gums'], diagnosis: 'Undiagnosed', timestamp: '2024-03-13', location: 'North Wing' },
];