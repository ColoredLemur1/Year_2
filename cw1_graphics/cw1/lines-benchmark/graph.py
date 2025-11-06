import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

FILE_NAME = 'line_length_vs_performance.csv'
X_COLUMN_NAME = 'Line Length (pixels)'
Y_COLUMN_NAME = 'CPU Time (ns)'

def create_linear_comparison_graph(file_path):
    """
    Reads line length data, plots the actual benchmark, and adds a perfect 
    O(n) linear line starting at (0,0) for comparison.
    """
    try:
        df = pd.read_csv(file_path)
    except (FileNotFoundError, pd.errors.EmptyDataError) as e:
        print(f"Error reading file: {e}")
        return
    
    if X_COLUMN_NAME not in df.columns or Y_COLUMN_NAME not in df.columns:
        print(f"Error: CSV must contain columns '{X_COLUMN_NAME}' and '{Y_COLUMN_NAME}'.")
        return

    df = df.sort_values(by=X_COLUMN_NAME, ascending=True)

    plt.figure(figsize=(12, 8))
    
    # 1. Plot Actual Data
    plt.scatter(df[X_COLUMN_NAME], df[Y_COLUMN_NAME], color='#D93A00', s=100, edgecolors='black', label='CPU times')
    plt.plot(df[X_COLUMN_NAME], df[Y_COLUMN_NAME], color='gray', linestyle='--', alpha=0.5)

    # 2. Calculate and Plot Ideal Linear Line
    
    # Get the coordinates for the largest data point (X_max, Y_max)
    X_max = df[X_COLUMN_NAME].max()
    Y_max = df[Y_COLUMN_NAME].max()
    
    # Define the coordinates for the Ideal O(n) line: starts at (0, 0) and ends at (X_max, Y_max)
    ideal_x = [0, X_max]
    ideal_y = [0, Y_max]

    # Plot the Ideal Linear Line
    plt.plot(ideal_x, ideal_y, 
             color='forestgreen', 
             linestyle='-', 
             linewidth=2, 
             alpha=0.8,
             label='perfect O(n) line') 

    # 3. Apply Styling
    plt.title('Line Length benchmark vs linear O(n)', fontsize=18, fontweight='bold')
    plt.xlabel(X_COLUMN_NAME, fontsize=14)
    plt.ylabel(Y_COLUMN_NAME, fontsize=14)
    
    plt.grid(axis='both', linestyle=':', alpha=0.7)
    
    plt.gca().spines['top'].set_visible(False)
    plt.gca().spines['right'].set_visible(False)

    # Format Y-axis to show full numbers with commas instead of scientific notation
    def format_with_commas(value, pos):
        return f'{int(value):,}'
    
    plt.gca().yaxis.set_major_formatter(FuncFormatter(format_with_commas))

    # Add data labels
    for index, row in df.iterrows():
        plt.text(row[X_COLUMN_NAME], 
                 row[Y_COLUMN_NAME] * 1.015,
                 f'{int(row[Y_COLUMN_NAME]):,}', 
                 ha='center', va='bottom', fontsize=10)

    # Add a legend
    plt.legend(frameon=True, fontsize=12)

    plt.xticks(rotation=45, ha='right', fontsize=12)
    plt.tight_layout()

    output_filename = 'line_length.png'
    plt.savefig(output_filename, dpi=300)
    
    print(f"\nSuccess! Comparison graph saved as: {output_filename}")

if __name__ == "__main__":
    create_linear_comparison_graph(FILE_NAME)