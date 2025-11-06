import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker # Import the ticker module

FILE_NAME = 'line_length_vs_performance.csv'
X_COLUMN_NAME = 'Line Length (pixels)'
Y_COLUMN_NAME = 'CPU Time (ns)'

def create_report_graph(file_path):
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
    
    # --- Start Plotting ---
    plt.scatter(df[X_COLUMN_NAME], df[Y_COLUMN_NAME], color='#D93A00', s=100, edgecolors='black', label='Benchmark Point')
    plt.plot(df[X_COLUMN_NAME], df[Y_COLUMN_NAME], color='gray', linestyle='--', alpha=0.5)

    # --- New: X-Axis Ticker Formatting ---
    # Define a custom formatter to display the numbers as full integers
    # We divide by 1 to effectively keep the number but change the display format
    # This keeps the ticks as full numbers (e.g., 10000000)
    formatter = ticker.FuncFormatter(lambda x, pos: f'{int(x):,}') 
    plt.gca().xaxis.set_major_formatter(formatter)
    
    # Optionally, reduce the number of major ticks to prevent crowding
    plt.gca().xaxis.set_major_locator(ticker.MaxNLocator(nbins=6))
    
    # -----------------------------------

    # Apply Professional Styling
    plt.title('System Benchmark Results: Performance Comparison', fontsize=18, fontweight='bold')
    plt.xlabel(X_COLUMN_NAME, fontsize=14)
    plt.ylabel(Y_COLUMN_NAME, fontsize=14)
    
    plt.grid(axis='both', linestyle=':', alpha=0.7)
    
    plt.gca().spines['top'].set_visible(False)
    plt.gca().spines['right'].set_visible(False)

    # Add data labels directly next to the points
    for index, row in df.iterrows():
        plt.text(row[X_COLUMN_NAME], 
                 row[Y_COLUMN_NAME] * 1.05, 
                 f'{row[Y_COLUMN_NAME]:.1f}', 
                 ha='center', va='bottom', fontsize=10)

    # Ensure X-axis labels are readable after reformatting
    plt.xticks(rotation=45, ha='right', fontsize=12)
    plt.tight_layout()

    output_filename = 'professional_scatter_graph_formatted_x.png'
    plt.savefig(output_filename, dpi=300)
    
    print(f"\nSuccess! Graph saved as: {output_filename}")


if __name__ == "__main__":
    create_report_graph(FILE_NAME)