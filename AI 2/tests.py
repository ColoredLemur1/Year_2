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

def train_decision_tree(x_train, y_train,ccp_alpha=0):
    model=None
    # Insert your code here for task 5
    model = DecisionTreeClassifier(ccp_alpha=ccp_alpha, random_state=1)
    model.fit(x_train, y_train)
    return model

def make_predictions(model, X_test):
    y_test_predicted=None
    # Insert your code here for task 6
    y_test_predicted = model.predict(X_test)
    return y_test_predicted

def evaluate_model(model, x, y):
    accuracy, recall=None,None
    # Insert your code here for task 7
    preicted_y = model.predict(x)

    accuracy = accuracy_score(y,preicted_y)

    recall = recall_score(y, preicted_y, average='binary')

    return accuracy, recall

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

def tree_depths(model):
    depth=None
    # Get the depth of the unpruned tree
    # Insert your code here for task 9
    depth = model.get_depth()
    return depth

def important_feature(x_train, y_train, header_list):
    best_feature = None
    best_feature_model = None
    ccp_alpha = 0
    feature_names = header_list[:-1]
    print (feature_names)
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

def optimal_depth_two_features(x_train, y_train, x_test, y_test, header_list):
    optimal_depth=None
    # Insert your code here for task 12
    first_important_feature = important_feature(x_train, y_train, header_list)
    if first_important_feature is None:
        return None
    
    other_features = [feature for feature in header_list if feature != first_important_feature]
    other_features.append("anything")
    second_important_feature = important_feature(x_train, y_train, other_features)
    print(second_important_feature)
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


file_path = "Year_2/AI 2/DT.csv"
#Testing
row_num, data, header_list = load_data(file_path)
filtered_data = filter_data(data)

np.set_printoptions(threshold=np.inf)
x_train, x_test, y_train, y_test = split_data(filtered_data)

most_important_feature = important_feature(x_train, y_train, header_list)
print(f"Most Important Feature: {most_important_feature}")

optimal_two = optimal_depth_two_features(x_train, y_train, x_test, y_test, header_list)
print(f"optimal {optimal_two}")
#coefficient_of_variation = np.divide(std_per_column, mean_per_column, out=np.full_like(std_per_column, np.inf), where=mean_per_column!=0)



