# ------------------------------------------------------------
# Naive Bayes Classifier (Spam Detection)
#
# In Artificial Intelligence and Machine Learning, the Naive Bayes
# algorithm is a probabilistic classifier based on Bayes' theorem.
# It assumes that features (like words in an email) are independent
# given the class label (spam or not spam).
#
# For spam detection:
#   - We calculate the probability of an email being spam
#     given the words it contains.
#   - Even though independence assumption is "naive"
#     (words in reality are not independent), it works well.
#   - The class with the highest probability is chosen.
#
# Applications: Spam filters, document categorization, sentiment analysis.
# ------------------------------------------------------------

import os
from collections import defaultdict
from typing import List, Tuple, Dict

# Training dataset
training_data: List[Tuple[str, str]] = [
    ("buy cheap now", "spam"),
    ("limited offer buy", "spam"),
    ("cheap deal discount", "spam"),
    ("meeting schedule today", "ham"),
    ("project deadline extended", "ham"),
    ("let us meet tomorrow", "ham"),
]

# Preprocess and count word frequencies per class
word_counts: Dict[str, Dict[str, int]] = {"spam": defaultdict(int), "ham": defaultdict(int)}
class_counts: Dict[str, int] = {"spam": 0, "ham": 0}
vocab: set = set()

for text, label in training_data:
    class_counts[label] += 1
    for word in text.split():
        word_counts[label][word] += 1
        vocab.add(word)

def predict(text: str) -> str:
    """Predict if the given email text is spam or ham."""
    words: List[str] = text.split()
    results: Dict[str, float] = {}
    for label in ["spam", "ham"]:
        # Prior probability 
        prob: float = class_counts[label] / sum(class_counts.values())
        # Likelihood with Laplace smoothing
        for word in words:
            prob *= (word_counts[label][word] + 1) / (sum(word_counts[label].values()) + len(vocab))
        results[label] = prob
    return max(results, key=results.get)

def main() -> None:
    test_emails: List[str] = ["cheap offer today", "project meeting tomorrow"]
    for email in test_emails:
        print("Test email:", email)
        print("Prediction:", predict(email))
        print("-" * 40)

    print("\nLab:", os.path.basename(__file__))
    print("Surab Parajuli\tSection: A\nRoll: 34\tSymbol: 80010139")

if __name__ == "__main__":
    main()
