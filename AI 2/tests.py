import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score
from sklearn.metrics import recall_score
from sklearn.tree import export_text
import warnings
import os

def load_data(file_path, delimiter=','):
    num_rows, data, header_list=None, None, None
    if not os.path.isfile(file_path):
        warnings.warn(f"Task 1: Warning - CSV file '{file_path}' does not exist.")
        return None, None, None
    # Insert your code here for task 1
    data = pd.read_csv(file_path)
    num_rows = data.shape[0]
    header_list = data.columns.tolist()
    return num_rows, data, header_list

file_path = "Year_2/AI 2/DT.csv"
num_rows, data, header_list = load_data(file_path)
print(f"Data is read. Number of Rows: {num_rows}"); 
print("-" * 50)