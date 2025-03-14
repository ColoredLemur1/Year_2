# COMP2611-Artificial Intelligence-Coursework#2 - Descision Trees

import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score
from sklearn.metrics import recall_score
from sklearn.tree import export_text
import warnings
import os

# STUDENT NAME: Javier Duarte Macias
# STUDENT EMAIL:  sc23jdm@leeds.ac.uk
    
def print_tree_structure(model, header_list):
    tree_rules = export_text(model, feature_names=header_list[:-1])
    print(tree_rules)
    
# Task 1 [8 marks]: 
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

# Task 2[8 marks]: 
def filter_data(data):
    filtered_data=[None]*1
    # Insert your code here for task 2
    filtered_data = data[~np.isin(data, -99).any(axis=1)]
    return filtered_data

# Task 3 [8 marks]: 
def statistics_data(data):
    coefficient_of_variation=None
    # Insert your code here for task 3
    std_per_column = np.std(data[:,:-1],axis=0)
    mean_per_column = np.mean(data[:,:-1],axis=0)

    coefficient_of_variation = std_per_column/mean_per_column
    return coefficient_of_variation

# Task 4 [8 marks]: 
def split_data(data, test_size=0.3, random_state=1):
    x_train, x_test, y_train, y_test=None, None, None, None
    np.random.seed(1)
    
    # Insert your code here for task 4
    X= data[:, :-1]
    Y = data[:, -1]
    x_train, x_test, y_train, y_test = train_test_split(X, Y, test_size=test_size, random_state=random_state, stratify=Y)
    return x_train, x_test, y_train, y_test

# Task 5 [8 marks]: 
def train_decision_tree(x_train, y_train,ccp_alpha=0):
    model=None
    # Insert your code here for task 5
    model = DecisionTreeClassifier(ccp_alpha=ccp_alpha, random_state=1)
    model.fit(x_train, y_train)
    return model

# Task 6 [8 marks]: 
def make_predictions(model, X_test):
    y_test_predicted=None
    # Insert your code here for task 6
    y_test_predicted = model.predict(X_test)
    return y_test_predicted

# Task 7 [8 marks]: 
def evaluate_model(model, x, y):
    accuracy, recall=None,None
    # Insert your code here for task 7
    preicted_y = model.predict(x)

    accuracy = accuracy_score(y,preicted_y)

    recall = recall_score(y, preicted_y, average='binary')
    
    return accuracy, recall

# Task 8 [8 marks]: 
def optimal_ccp_alpha(x_train, y_train, x_test, y_test):
    optimal_ccp_alpha=None

    # Insert your code here for task 8
    first_model = train_decision_tree(x_train,y_train,ccp_alpha=0)
    first_accuracy, _ = evaluate_model(first_model,x_test,y_test)

    accuracy_variable = first_accuracy * 0.99
    
    ccp_alpha =0.001
    optimal_ccp_alpha = 0

    while True:
        pruned_model = train_decision_tree(x_train,y_train,ccp_alpha=ccp_alpha)
        pruned_accuracy, _ = evaluate_model(pruned_model,x_test,y_test)

        if pruned_accuracy >= accuracy_variable:
            optimal_ccp_alpha = ccp_alpha
            ccp_alpha += 0.001
        else:
            break
    return optimal_ccp_alpha

# Task 9 [8 marks]: 
def tree_depths(model):
    depth=None
    # Get the depth of the unpruned tree
    # Insert your code here for task 9
    depth = model.get_depth()
    return depth

 # Task 10 [8 marks]: 
def important_feature(x_train, y_train, header_list):
    best_feature = None
    best_feature_model = None
    ccp_alpha = 0
    feature_names = header_list[:-1]
    while ccp_alpha <= 1:
        model = train_decision_tree(x_train, y_train, ccp_alpha=ccp_alpha)
        depth = tree_depths(model)
        if depth == 1:
            rules = export_text(model, feature_names=feature_names)
            first_line = rules.split("\n")[0]
            parts = first_line.split(" ")
            for i, part in enumerate(parts):
                if part in ["<=", ">"]:
                    best_feature = parts[i - 1]
                    return best_feature
        elif depth > 1:
            best_feature_model = model
        ccp_alpha += 0.01
    
    if best_feature_model is not None:
        rules = export_text(best_feature_model, feature_names=feature_names)
        first_line = rules.split("\n")[0]

        parts = first_line.split(" ")
        for i, part in enumerate(parts):
            if part in ["<=", ">"]:
                best_feature = parts[i - 1]
                return best_feature
    return None
    
# Task 11 [10 marks]: 
def optimal_ccp_alpha_single_feature(x_train, y_train, x_test, y_test, header_list):
    good_name=None
    # Insert your code here for task 11
    important = important_feature(x_train,y_train,header_list)
    if important == None:
        return None
    
    index = header_list.index(important)

    single_feature_x_train = x_train[:, index].reshape(-1, 1)
    single_feature_x_test = x_test[:, index].reshape(-1, 1)

    good_name = optimal_ccp_alpha(single_feature_x_train,y_train,single_feature_x_test,y_test)
    return good_name

# Task 12 [10 marks]: 
def optimal_depth_two_features(x_train, y_train, x_test, y_test, header_list):
    optimal_depth=None
    # Insert your code here for task 12
    first_important_feature = important_feature(x_train, y_train, header_list)
    if first_important_feature is None:
        return None
    
    other_features = [feature for feature in header_list if feature != first_important_feature]

    other_features.append("anything")
    
    second_important_feature = important_feature(x_train, y_train, other_features)
    if second_important_feature is None:
        return None
    
    first_index = header_list.index(first_important_feature)
    second_index = header_list.index(second_important_feature)

    two_x_train = x_train[:, [first_index, second_index]]
    two_x_test = x_test[:, [first_index, second_index]]

    optimal_alpha = optimal_ccp_alpha(two_x_train, y_train, two_x_test, y_test)

    model = train_decision_tree(two_x_train, y_train, ccp_alpha=optimal_alpha)

    optimal_depth = tree_depths(model)

    return optimal_depth    

# Example usage (Main section):
if __name__ == "__main__":
    # Load data
    file_path = "Year_2/AI 2/DT.csv"
    num_rows, data, header_list = load_data(file_path)
    print(f"Data is read. Number of Rows: {num_rows}"); 
    print("-" * 50)

    # Filter data
    data_filtered = filter_data(data)
    num_rows_filtered=data_filtered.shape[0]
    print(f"Data is filtered. Number of Rows: {num_rows_filtered}"); 
    print("-" * 50)

    # Data Statistics
    coefficient_of_variation = statistics_data(data_filtered)
    print("Coefficient of Variation for each feature:")
    for header, coef_var in zip(header_list[:-1], coefficient_of_variation):
        print(f"{header}: {coef_var}")
    print("-" * 50)
    # Split data
    x_train, x_test, y_train, y_test = split_data(data_filtered)
    print(f"Train set size: {len(x_train)}")
    print(f"Test set size: {len(x_test)}")
    print("-" * 50)
    
    # Train initial Decision Tree
    model = train_decision_tree(x_train, y_train)
    print("Initial Decision Tree Structure:")
    print_tree_structure(model, header_list)
    print("-" * 50)
    
    # Evaluate initial model
    acc_test, recall_test = evaluate_model(model, x_test, y_test)
    print(f"Initial Decision Tree - Test Accuracy: {acc_test:.2%}, Recall: {recall_test:.2%}")
    print("-" * 50)
    # Train Pruned Decision Tree
    model_pruned = train_decision_tree(x_train, y_train, ccp_alpha=0.002)
    print("Pruned Decision Tree Structure:")
    print_tree_structure(model_pruned, header_list)
    print("-" * 50)
    # Evaluate pruned model
    acc_test_pruned, recall_test_pruned = evaluate_model(model_pruned, x_test, y_test)
    print(f"Pruned Decision Tree - Test Accuracy: {acc_test_pruned:.2%}, Recall: {recall_test_pruned:.2%}")
    print("-" * 50)
    # Find optimal ccp_alpha
    optimal_alpha = optimal_ccp_alpha(x_train, y_train, x_test, y_test)
    print(f"Optimal ccp_alpha for pruning: {optimal_alpha:.4f}")
    print("-" * 50)
    # Train Pruned and Optimized Decision Tree
    model_optimized = train_decision_tree(x_train, y_train, ccp_alpha=optimal_alpha)
    print("Optimized Decision Tree Structure:")
    print_tree_structure(model_optimized, header_list)
    print("-" * 50)
    
    # Get tree depths
    depth_initial = tree_depths(model)
    depth_pruned = tree_depths(model_pruned)
    depth_optimized = tree_depths(model_optimized)
    print(f"Initial Decision Tree Depth: {depth_initial}")
    print(f"Pruned Decision Tree Depth: {depth_pruned}")
    print(f"Optimized Decision Tree Depth: {depth_optimized}")
    print("-" * 50)
    
    # Feature importance
    important_feature_name = important_feature(x_train, y_train,header_list)
    print(f"Important Feature for Fraudulent Transaction Prediction: {important_feature_name}")
    print("-" * 50)
    
    # Test optimal ccp_alpha with single feature
    optimal_alpha_single = optimal_ccp_alpha_single_feature(x_train, y_train, x_test, y_test, header_list)
    print(f"Optimal ccp_alpha using single most important feature: {optimal_alpha_single:.4f}")
    print("-" * 50)
    
    # Test optimal depth with two features
    optimal_depth_two = optimal_depth_two_features(x_train, y_train, x_test, y_test, header_list)
    print(f"Optimal tree depth using two most important features: {optimal_depth_two}")
    print("-" * 50)        
# References: 
# Here please provide recognition to any source if you have used or got code snippets from
# Please tell the lines that are relavant to that reference.
# For example: 
# Line 80-87 is inspired by a code at https://stackoverflow.com/questions/48414212/how-to-calculate-accuracy-from-decision-trees


