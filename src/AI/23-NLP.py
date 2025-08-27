# ------------------------------------------------------------
# NLP Tasks Demonstration using NLTK
#
# Natural Language Processing (NLP) is a branch of AI that deals with
# interaction between computers and human language. Common tasks:
#   - Sentence tokenization: Splitting text into sentences
#   - Word tokenization: Splitting sentences into words
#   - Stop words removal: Removing common words like 'the', 'is'
#   - Stemming: Reducing words to their root form
#   - POS tagging: Assigning parts of speech (noun, verb, etc.)
#
# NLTK is a Python library that provides tools for these NLP tasks.
# Applications: Text preprocessing, sentiment analysis, chatbots, information retrieval.
# ------------------------------------------------------------

import os
from typing import List
import nltk
from nltk.tokenize import sent_tokenize, word_tokenize
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer

def main() -> None:
    nltk.download('wordnet')
    nltk.download('punkt_tab')
    nltk.download('stopwords')
    nltk.download('averaged_perceptron_tagger_eng')
    
    text = ("Natural Language Processing (NLP) is a field of Artificial Intelligence. "
            "It enables machines to understand and interpret human language.")

    print("Original Text:")
    print(text)
    print("-" * 40)

    # Sentence Tokenization
    sentences: List[str] = sent_tokenize(text)
    print("Sentence Tokenization:")
    print(sentences)
    print("-" * 40)

    # Word Tokenization
    words: List[str] = word_tokenize(text)
    print("Word Tokenization:")
    print(words)
    print("-" * 40)

    # Stop words filtering
    stop_words = set(stopwords.words('english'))
    filtered_words: List[str] = [w for w in words if w.lower() not in stop_words and w.isalpha()]
    print("After Stop Words Removal:")
    print(filtered_words)
    print("-" * 40)

    # Word Stemming
    stemmer = PorterStemmer()
    stemmed_words: List[str] = [stemmer.stem(w) for w in filtered_words]
    print("After Stemming:")
    print(stemmed_words)
    print("-" * 40)

    # POS Tagging
    pos_tags = nltk.pos_tag(filtered_words)
    print("POS Tagging:")
    print(pos_tags)
    print("-" * 40)

    print("\nLab:", os.path.basename(__file__))
    print("Surab Parajuli\tSection: A\nRoll: 34\tSymbol: 80010139")

if __name__ == "__main__":
    main()
