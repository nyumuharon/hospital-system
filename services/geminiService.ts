import { GoogleGenAI, Type } from "@google/genai";
import { EpidemicReport, SymptomRecord } from "../types";

// Lazy initialization - only create AI client when actually needed
let ai: GoogleGenAI | null = null;

const getAI = () => {
  if (!ai) {
    const apiKey = process.env.API_KEY || process.env.GEMINI_API_KEY;
    if (!apiKey) {
      throw new Error("Gemini API key not configured");
    }
    ai = new GoogleGenAI({ apiKey });
  }
  return ai;
};

export const analyzeEpidemicRisks = async (
  symptomData: SymptomRecord[]
): Promise<EpidemicReport> => {

  const model = 'gemini-2.5-flash';

  const prompt = `
    You are an advanced AI Epidemiology Assistant for a hospital. 
    Analyze the following list of recent patient intake records (symptoms, preliminary diagnosis, location, time).
    
    External Environmental Context (Simulated):
    - Season: Late Summer
    - Recent heavy rainfall in the region.
    - High mosquito activity reported by public health APIs.

    Patient Data:
    ${JSON.stringify(symptomData, null, 2)}

    Your Task:
    1. Detect if there is a potential outbreak based on symptom clusters (e.g., Dengue, Measles, COVID-19, Flu).
    2. Assign a Risk Level (LOW, MODERATE, HIGH, CRITICAL).
    3. Provide specific analysis of why you think this is a pattern.
    4. Suggest immediate recommendations for hospital management.

    Return the response in strict JSON format matching the schema provided.
  `;

  try {
    const aiInstance = getAI();
    const response = await aiInstance.models.generateContent({
      model: model,
      contents: prompt,
      config: {
        responseMimeType: "application/json",
        responseSchema: {
          type: Type.OBJECT,
          properties: {
            riskLevel: { type: Type.STRING, enum: ["LOW", "MODERATE", "HIGH", "CRITICAL"] },
            detectedOutbreaks: {
              type: Type.ARRAY,
              items: { type: Type.STRING }
            },
            analysis: { type: Type.STRING },
            recommendations: {
              type: Type.ARRAY,
              items: { type: Type.STRING }
            },
            timestamp: { type: Type.STRING }
          },
          required: ["riskLevel", "detectedOutbreaks", "analysis", "recommendations"]
        }
      }
    });

    const text = response.text;
    if (!text) throw new Error("No response from AI");

    return JSON.parse(text) as EpidemicReport;

  } catch (error) {
    console.error("Gemini Analysis Failed:", error);
    return {
      riskLevel: "LOW",
      detectedOutbreaks: ["Analysis Failed"],
      analysis: "Could not connect to AI service to perform analysis.",
      recommendations: ["Check internet connection", "Verify API Key"],
      timestamp: new Date().toISOString()
    };
  }
};