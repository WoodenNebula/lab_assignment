# ------------------------------------------------------------
# Medical Expert System (Disease Diagnosis)
#
# In Artificial Intelligence, an Expert System is a program that
# mimics human decision-making by using a knowledge base of rules
# and an inference engine to draw conclusions.
#
# For medical diagnosis:
#   - The knowledge base stores rules mapping symptoms to diseases.
#   - The inference engine checks which disease best matches
#     the observed symptoms.
#   - Such systems assist doctors in decision-making and can
#     provide preliminary diagnosis.
#
# Applications: Medical diagnosis, troubleshooting, recommendation systems.
# ------------------------------------------------------------

import os
from typing import List, Dict

# Knowledge base: Disease -> List of symptoms
knowledge_base: Dict[str, List[str]] = {
    "Flu": ["fever", "cough", "sore throat", "fatigue"],
    "Cold": ["cough", "sneezing", "runny nose"],
    "Migraine": ["headache", "nausea", "sensitivity to light"],
    "Chickenpox": ["fever", "rash", "fatigue", "itching"],
}

def diagnose(symptoms: List[str]) -> str:
    """Diagnose disease based on symptoms using simple rule matching."""
    max_match = 0
    probable_disease = "Unknown"
    for disease, disease_symptoms in knowledge_base.items():
        match_count = len(set(symptoms) & set(disease_symptoms))
        if match_count > max_match:
            max_match = match_count
            probable_disease = disease
    return probable_disease

def main() -> None:
    test_cases: List[List[str]] = [
        ["fever", "cough", "fatigue"],      # Should likely match Flu
        ["headache", "nausea", "fatigue"],  # Should likely match Migraine
    ]

    for i, symptoms in enumerate(test_cases, 1):
        print(f"Test Case {i}: Symptoms -> {symptoms}")
        print("Diagnosis:", diagnose(symptoms))
        print("-" * 40)

    # Footer
    print("\nLab:", os.path.basename(__file__))
    print("Surab Parajuli\tSection: A\nRoll: 34\tSymbol: 80010139")

if __name__ == "__main__":
    main()
