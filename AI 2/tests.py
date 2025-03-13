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
    raw_data = pd.read_csv(file_path)
    data = raw_data.to_numpy()
    num_rows = data.shape[0]
    header_list = raw_data.columns.tolist()
    return num_rows, data, header_list

def filter_data(data):
    filtered_data=[None]*1
    # Insert your code here for task 2
    filtered_data = data[~np.isin(data, -99).any(axis=1)]
    return filtered_data

def statistics_data(data):
    coefficient_of_variation=None
    # Insert your code here for task 3
    std_per_column = np.std(data[:,:-1],axis=0)
    mean_per_column = np.mean(data[:,:-1],axis=0)

    coefficient_of_variation = std_per_column/mean_per_column
    return coefficient_of_variation

def split_data(data, test_size=0.3, random_state=1):
    x_train, x_test, y_train, y_test=None, None, None, None
    np.random.seed(1)
    
    # Insert your code here for task 4
    X= data[:, :-1]
    Y = data[:, -1]
    x_train, x_test, y_train, y_test = train_test_split(X, Y, test_size=test_size, random_state=random_state, stratify=Y)
    return x_train, x_test, y_train, y_test

file_path = "Year_2/AI 2/DT.csv"
#Testing
row_num, data, header_list = load_data(file_path)
filtered_data = filter_data(data)

np.set_printoptions(threshold=np.inf)
#coefficient_of_variation = np.divide(std_per_column, mean_per_column, out=np.full_like(std_per_column, np.inf), where=mean_per_column!=0)



